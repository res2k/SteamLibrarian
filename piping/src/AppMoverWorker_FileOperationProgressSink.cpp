/*
  SteamLibrarian
  Copyright (c) 2015 Frank Richter

  This file is part of SteamLibrarian.

  SteamLibrarian is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  SteamLibrarian is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with SteamLibrarian.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "AppMoverWorker_FileOperationProgressSink.hpp"

#include <QtWin>
#include <QDebug>

#include <atlbase.h>

namespace piping
{
  static HRESULT GetShellItemPath(IShellItem* x, QString& str)
  {
    SFGAOF attr;
    HRESULT hr;
    if (SUCCEEDED((hr = x->GetAttributes(SFGAO_FILESYSTEM, &attr))))
    {
      CComHeapPtr<wchar_t> pathPtr;
      if ((attr & SFGAO_FILESYSTEM) != 0)
      {
        if (SUCCEEDED((hr = x->GetDisplayName(SIGDN_FILESYSPATH, &pathPtr))))
          str = QString::fromWCharArray(pathPtr);
      }
      else
      {
        if (SUCCEEDED((hr = x->GetDisplayName(SIGDN_DESKTOPABSOLUTEPARSING, &pathPtr))))
          str = QString::fromWCharArray(pathPtr);
      }
    }
    return hr;
  }

  template<typename T>
  struct DebugFormatHelper
  {
    typedef const T& result_type;
    static result_type DebugFormat(const T& x)
    {
      return x;
    }
  };

  template<>
  struct DebugFormatHelper < HRESULT >
  {
    typedef QString result_type;
    static result_type DebugFormat(HRESULT hr)
    {
      return QtWin::stringFromHresult(hr);
    }
  };

  template<>
  struct DebugFormatHelper < DWORD >
  {
    typedef QString result_type;
    static result_type DebugFormat(DWORD x)
    {
      return QString::number(x, 16);
    }
  };

  template<>
  struct DebugFormatHelper < LPCWSTR >
  {
    typedef QString result_type;
    static result_type DebugFormat(LPCWSTR x)
    {
      return x ? QString::fromWCharArray(x) : QStringLiteral("(null)");
    }
  };

  template<>
  struct DebugFormatHelper < IShellItem* >
  {
    typedef QString result_type;
    static result_type DebugFormat(IShellItem* x)
    {
      if (!x) return QStringLiteral("(null)");
      HRESULT hr;
      QString str;
      if (SUCCEEDED((hr = GetShellItemPath(x, str))))
      {
        return str;
      }
      return QStringLiteral("[%1]").arg(QtWin::stringFromHresult(hr));
    }
  };

  template<typename T>
  static typename DebugFormatHelper<T>::result_type DebugFormat(const T& x)
  {
    return DebugFormatHelper<T>::DebugFormat(x);
  }

#define A(x)  #x ":" << DebugFormat(x)

  FileOperationProgressSink::FileOperationProgressSink() : m_refCount(0) {}

  HRESULT FileOperationProgressSink::QueryInterface(const IID& iid, void** ppvOut)
  {
    if (iid == __uuidof(IFileOperationProgressSink))
    {
      AddRef();
      *ppvOut = static_cast<IFileOperationProgressSink*> (this);
      return S_OK;
    }
    else if (iid == __uuidof(IUnknown))
    {
      AddRef();
      *ppvOut = static_cast<IUnknown*> (this);
      return S_OK;
    }
    return E_NOINTERFACE;
  }

  ULONG FileOperationProgressSink::AddRef()
  {
    return InterlockedIncrement(&m_refCount);
  }

  ULONG FileOperationProgressSink::Release()
  {
    ULONG rc = InterlockedDecrement(&m_refCount);
    if (rc == 0)
    {
      delete this;
    }
    return rc;
  }

  HRESULT FileOperationProgressSink::StartOperations()
  {
    qDebug() << __FUNCTION__;
    return S_OK;
  }

  HRESULT FileOperationProgressSink::FinishOperations(HRESULT hrResult)
  {
    qDebug() << __FUNCTION__ << A(hrResult);
    return S_OK;
  }

  HRESULT FileOperationProgressSink::PreRenameItem(DWORD dwFlags, IShellItem *psiItem, LPCWSTR pszNewName)
  {
    qDebug() << __FUNCTION__ << A(dwFlags) << A(psiItem) << A(pszNewName);
    return S_OK;
  }

  HRESULT FileOperationProgressSink::PostRenameItem(DWORD dwFlags, IShellItem *psiItem, LPCWSTR pszNewName, HRESULT hrRename, IShellItem *psiNewlyCreated)
  {
    qDebug() << __FUNCTION__ << A(dwFlags) << A(psiItem) << A(pszNewName) << A(hrRename) << A(psiNewlyCreated);
    return S_OK;
  }

  HRESULT FileOperationProgressSink::PreMoveItem(DWORD dwFlags, IShellItem *psiItem, IShellItem *psiDestinationFolder, LPCWSTR pszNewName)
  {
    qDebug() << __FUNCTION__ << A(dwFlags) << A(psiItem) << A(psiDestinationFolder) << A(pszNewName);
    return S_OK;
  }

  HRESULT FileOperationProgressSink::PostMoveItem(DWORD dwFlags, IShellItem *psiItem, IShellItem *psiDestinationFolder, LPCWSTR pszNewName,
    HRESULT hrMove, IShellItem *psiNewlyCreated)
  {
    qDebug() << __FUNCTION__ << A(dwFlags) << A(psiItem) << A(psiDestinationFolder) << A(pszNewName) << A(hrMove) << A(psiNewlyCreated);
    if (SUCCEEDED(hrMove))
    {
      ProcessedItem new_item;
      if (SUCCEEDED(GetShellItemPath(psiItem, new_item.m_source))
        && SUCCEEDED(GetShellItemPath(psiNewlyCreated, new_item.m_dest)))
      {
        m_processedItems.push_back(std::move(new_item));
      }
    }
    return S_OK;
  }

  HRESULT FileOperationProgressSink::PreCopyItem(DWORD dwFlags, IShellItem *psiItem, IShellItem *psiDestinationFolder, LPCWSTR pszNewName)
  {
    qDebug() << __FUNCTION__ << A(dwFlags) << A(psiItem) << A(psiDestinationFolder) << A(pszNewName);
    return S_OK;
  }

  HRESULT FileOperationProgressSink::PostCopyItem(DWORD dwFlags, IShellItem *psiItem, IShellItem *psiDestinationFolder,
    LPCWSTR pszNewName, HRESULT hrCopy, IShellItem *psiNewlyCreated)
  {
    qDebug() << __FUNCTION__ << A(dwFlags) << A(psiItem) << A(psiDestinationFolder) << A(pszNewName) << A(hrCopy) << A(psiNewlyCreated);
    return S_OK;
  }

  HRESULT FileOperationProgressSink::PreDeleteItem(DWORD dwFlags, IShellItem *psiItem)
  {
    qDebug() << __FUNCTION__ << A(dwFlags) << A(psiItem);
    return S_OK;
  }

  HRESULT FileOperationProgressSink::PostDeleteItem(DWORD dwFlags, IShellItem *psiItem, HRESULT hrDelete, IShellItem *psiNewlyCreated)
  {
    qDebug() << __FUNCTION__ << A(dwFlags) << A(psiItem) << A(hrDelete) << A(psiNewlyCreated);
    return S_OK;
  }

  HRESULT FileOperationProgressSink::PreNewItem(DWORD dwFlags, IShellItem *psiDestinationFolder, LPCWSTR pszNewName)
  {
    qDebug() << __FUNCTION__ << A(dwFlags) << A(psiDestinationFolder) << A(pszNewName);
    return S_OK;
  }

  HRESULT FileOperationProgressSink::PostNewItem(DWORD dwFlags, IShellItem *psiDestinationFolder, LPCWSTR pszNewName,
    LPCWSTR pszTemplateName, DWORD dwFileAttributes, HRESULT hrNew, IShellItem *psiNewItem)
  {
    qDebug() << __FUNCTION__ << A(dwFlags) << A(psiDestinationFolder) << A(pszNewName) << A(hrNew) << A(psiNewItem);
    return S_OK;
  }

  HRESULT FileOperationProgressSink::UpdateProgress(UINT iWorkTotal, UINT iWorkSoFar)
  {
    //qDebug() << __FUNCTION__ << A(iWorkTotal) << A(iWorkSoFar);
    return S_OK;
  }

  HRESULT FileOperationProgressSink::ResetTimer()
  {
    qDebug() << __FUNCTION__;
    return S_OK;
  }

  HRESULT FileOperationProgressSink::PauseTimer()
  {
    qDebug() << __FUNCTION__;
    return S_OK;
  }

  HRESULT FileOperationProgressSink::ResumeTimer()
  {
    qDebug() << __FUNCTION__;
    return S_OK;
  }
} // namespace piping