#include "AppMoverWorker.hpp"

#include "AppMoverWorker_FileOperationProgressSink.hpp"

#include <QtWin>
#include <QDir>
#include <QFileInfo>
#include <QPair>

#include <Windows.h>
#include <comdef.h>
#include <ShlObj.h>

#include <boost/range/adaptor/reversed.hpp>
#include <boost/scope_exit.hpp>

_COM_SMARTPTR_TYPEDEF(IFileOperation, __uuidof(IFileOperation));
_COM_SMARTPTR_TYPEDEF(IFileOperationProgressSink, __uuidof(IFileOperationProgressSink));
_COM_SMARTPTR_TYPEDEF(IShellItem, __uuidof(IShellItem));

namespace piping
{
  AppMoverWorker::AppMoverWorker(QObject* parent)
    : QObject(parent), m_recurse(true)
  {
  }

  void AppMoverWorker::setRecurse(bool recurse)
  {
    m_recurse = recurse;
  }

  void AppMoverWorker::Move(const QString& sourcePath, const QString& destination)
  {
    m_moveFiles.append(move_files_pair(sourcePath, destination));
  }

  void AppMoverWorker::Perform()
  {
    HRESULT hr = CoInitialize(nullptr);
    if (!SUCCEEDED(hr)) { SignalResult(hr); return; }

    BOOST_SCOPE_EXIT(void) {
      CoUninitialize();
    } BOOST_SCOPE_EXIT_END

    IFileOperationPtr file_op;
    hr = file_op.CreateInstance(CLSID_FileOperation);
    if (!SUCCEEDED(hr)) { SignalResult(hr); return; }

    DWORD opFlags = FOF_NOCONFIRMMKDIR | FOF_NOERRORUI | FOFX_EARLYFAILURE | FOFX_KEEPNEWERFILE;
    opFlags |= m_recurse ? 0 : FOF_NORECURSION;
    hr = file_op->SetOperationFlags(opFlags);
    if (!SUCCEEDED(hr)) { SignalResult(hr); return; }

    FileOperationProgressSink* sink = new FileOperationProgressSink();
    IFileOperationProgressSinkPtr sink_ref(sink);
    DWORD pfopsCookie;
    hr = file_op->Advise(sink_ref, &pfopsCookie);
    if (!SUCCEEDED(hr)) { SignalResult(hr); return; }

    BOOL userCancelled(false);
    // Block so cancelled/finished messages are emitted after undoData
    {
      HRESULT signalHr = S_OK;
      BOOST_SCOPE_EXIT(this_, sink, &signalHr) {
        // Set undo ops from 'sink'
        QList<move_files_pair> moveFilesUndo;
        for (const auto& processedItem : boost::adaptors::reverse(sink->m_processedItems))
        {
          moveFilesUndo.append(move_files_pair(processedItem.m_dest, processedItem.m_source));
        }
        this_->undoData(moveFilesUndo);

        // Signal should arrive after undo data
        if (!SUCCEEDED(signalHr)) this_->SignalResult(signalHr);
      } BOOST_SCOPE_EXIT_END

      Q_FOREACH(const move_files_pair& files_pair, m_moveFiles)
      {
        QString srcPath = QDir::toNativeSeparators(files_pair.first);
        QString destPath = files_pair.second;
        QString destDir, destName;
        {
          QFileInfo fi(destPath);
          destDir = QDir::toNativeSeparators(fi.dir().path());
          destName = fi.fileName();
        }
        // Make sure dest dir exists
        QDir().mkpath(destDir);

        IShellItemPtr srcItem;
        hr = SHCreateItemFromParsingName(reinterpret_cast<const WCHAR*> (srcPath.unicode()),
          nullptr, IID_PPV_ARGS(&srcItem));
        if (!SUCCEEDED(hr)) { signalHr = hr; return; }

        IShellItemPtr destFolder;
        hr = SHCreateItemFromParsingName(reinterpret_cast<const WCHAR*> (destDir.unicode()),
          nullptr, IID_PPV_ARGS(&destFolder));
        if (!SUCCEEDED(hr)) { signalHr = hr; return; }

        hr = file_op->MoveItem(srcItem, destFolder,
          destName.isEmpty() ? nullptr : reinterpret_cast<const WCHAR*> (destName.unicode()),
          nullptr);
        if (!SUCCEEDED(hr)) { signalHr = hr; return; }
      }

      hr = file_op->PerformOperations();
      if (!SUCCEEDED(hr) && (hr != COPYENGINE_E_USER_CANCELLED)) { signalHr = hr; return; }
      if (hr == COPYENGINE_E_USER_CANCELLED)
      {
        userCancelled = true;
      }
      else
      {
        hr = file_op->GetAnyOperationsAborted(&userCancelled);
        if (!SUCCEEDED(hr)) { signalHr = hr; return; }
      }
    }

    if (userCancelled)
    {
      emit cancelled();
    }
    else
    {
      emit finished();
    }
  }

  void AppMoverWorker::SignalResult(HRESULT result)
  {
    emit failed(QtWin::stringFromHresult(result));
  }
} // namespace piping
