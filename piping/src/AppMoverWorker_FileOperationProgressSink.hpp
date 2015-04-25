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

/**\file
 * IFileOperationProgressSink implementation for AppMoverWorker
 */
#ifndef __PIPING_APPMOVERWORKER_FILEOPERATIONSPROGRESSSINK_HPP__
#define __PIPING_APPMOVERWORKER_FILEOPERATIONSPROGRESSSINK_HPP__

#include <QString>

#include <Shobjidl.h>

#include <vector>

namespace piping
{
  // IFileOperationProgressSink implementation
  class FileOperationProgressSink : public IFileOperationProgressSink
  {
    ULONG m_refCount;
  public:
    FileOperationProgressSink();

    struct ProcessedItem
    {
      QString m_source;
      QString m_dest;
    };
    std::vector<ProcessedItem> m_processedItems;

    HRESULT STDMETHODCALLTYPE QueryInterface(const IID& iid, void** ppvOut);
    ULONG STDMETHODCALLTYPE AddRef();
    ULONG STDMETHODCALLTYPE Release();

    HRESULT STDMETHODCALLTYPE StartOperations();
    HRESULT STDMETHODCALLTYPE FinishOperations(HRESULT hrResult);

    HRESULT STDMETHODCALLTYPE PreRenameItem(DWORD dwFlags, IShellItem *psiItem, LPCWSTR pszNewName);
    HRESULT STDMETHODCALLTYPE PostRenameItem(DWORD dwFlags, IShellItem *psiItem, LPCWSTR pszNewName, HRESULT hrRename, IShellItem *psiNewlyCreated);
    HRESULT STDMETHODCALLTYPE PreMoveItem(DWORD dwFlags, IShellItem *psiItem, IShellItem *psiDestinationFolder, LPCWSTR pszNewName);
    HRESULT STDMETHODCALLTYPE PostMoveItem(DWORD dwFlags, IShellItem *psiItem, IShellItem *psiDestinationFolder, LPCWSTR pszNewName,
      HRESULT hrMove, IShellItem *psiNewlyCreated);
    HRESULT STDMETHODCALLTYPE PreCopyItem(DWORD dwFlags, IShellItem *psiItem, IShellItem *psiDestinationFolder, LPCWSTR pszNewName);
    HRESULT STDMETHODCALLTYPE PostCopyItem(DWORD dwFlags, IShellItem *psiItem, IShellItem *psiDestinationFolder,
      LPCWSTR pszNewName, HRESULT hrCopy, IShellItem *psiNewlyCreated);
    HRESULT STDMETHODCALLTYPE PreDeleteItem(DWORD dwFlags, IShellItem *psiItem);
    HRESULT STDMETHODCALLTYPE PostDeleteItem(DWORD dwFlags, IShellItem *psiItem, HRESULT hrDelete, IShellItem *psiNewlyCreated);
    HRESULT STDMETHODCALLTYPE PreNewItem(DWORD dwFlags, IShellItem *psiDestinationFolder, LPCWSTR pszNewName);
    HRESULT STDMETHODCALLTYPE PostNewItem(DWORD dwFlags, IShellItem *psiDestinationFolder, LPCWSTR pszNewName,
      LPCWSTR pszTemplateName, DWORD dwFileAttributes, HRESULT hrNew, IShellItem *psiNewItem);

    HRESULT STDMETHODCALLTYPE UpdateProgress(UINT iWorkTotal, UINT iWorkSoFar);

    HRESULT STDMETHODCALLTYPE ResetTimer();
    HRESULT STDMETHODCALLTYPE PauseTimer();
    HRESULT STDMETHODCALLTYPE ResumeTimer();
  };
} // namespace piping

#endif // __PIPING_APPMOVERWORKER_FILEOPERATIONSPROGRESSSINK_HPP__
