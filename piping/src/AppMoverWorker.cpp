#include "AppMoverWorker.hpp"

#include <QtWin>
#include <QDir>
#include <QFileInfo>
#include <QPair>

#include <Windows.h>
#include <comdef.h>
#include <ShlObj.h>

#include <boost/scope_exit.hpp>

_COM_SMARTPTR_TYPEDEF(IFileOperation, __uuidof(IFileOperation));
_COM_SMARTPTR_TYPEDEF(IShellItem, __uuidof(IShellItem));

namespace piping
{
  AppMoverWorker::AppMoverWorker(QObject* parent)
    : QObject(parent)
  {
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

    hr = file_op->SetOperationFlags(FOF_NOCONFIRMMKDIR | FOF_NOERRORUI | FOFX_EARLYFAILURE | FOFX_KEEPNEWERFILE);
    if (!SUCCEEDED(hr)) { SignalResult(hr); return; }

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
      if (!SUCCEEDED(hr)) { SignalResult(hr); return; }

      IShellItemPtr destFolder;
      hr = SHCreateItemFromParsingName(reinterpret_cast<const WCHAR*> (destDir.unicode()),
        nullptr, IID_PPV_ARGS(&destFolder));
      if (!SUCCEEDED(hr)) { SignalResult(hr); return; }

      hr = file_op->MoveItem(srcItem, destFolder,
        destName.isEmpty() ? nullptr : reinterpret_cast<const WCHAR*> (destName.unicode()),
        nullptr);
      if (!SUCCEEDED(hr)) { SignalResult(hr); return; }
    }

    hr = file_op->PerformOperations();
    if (!SUCCEEDED(hr)) { SignalResult(hr); return; }

    BOOL userCancelled(false);
    hr = file_op->GetAnyOperationsAborted(&userCancelled);
    if (!SUCCEEDED(hr)) { SignalResult(hr); return; }
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
