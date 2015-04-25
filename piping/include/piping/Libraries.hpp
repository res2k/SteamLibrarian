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
 * Libraries container
 */
#ifndef __PIPING_LIBRARIES_HPP__
#define __PIPING_LIBRARIES_HPP__

#include <QFuture>
#include <QFutureWatcher>
#include <QList>
#include <QObject>

#include <memory>

class QFileSystemWatcher;

namespace piping
{
  class Library;

  /// Libraries container
  class Libraries : public QObject
  {
    Q_OBJECT
  private:
    /// Known libraries
    QList<Library*> m_libraries;
    /// "Master" library path
    QString m_masterLibraryPath;
    /// libraryfolders.vdf path
    QString m_libraryFolderVdf;
    /// File system watcher used to detect changes to libraryfolders.vdf
    QFileSystemWatcher* m_fsw;

    typedef QSet<QString> RescanResultType;
    /// Future for current pending libraries rescan
    std::shared_ptr<QFuture<RescanResultType> > pendingRescan;
    /// Watcher for rescan future
    QFutureWatcher<RescanResultType> rescanWatcher;
  public:
    Libraries();
    Libraries(const QString& steamPath, QObject* parent = nullptr);
    ~Libraries();

    /// Get number of known libraries
    Q_INVOKABLE int count() const;
    /// Get a library
    Q_INVOKABLE piping::Library* get(int index) const;

  signals:
    /// Emitted when a new library was added
    void libraryAdd(piping::Library* lib);
    /// Emitted when a library is about to be removed
    void libraryRemove(piping::Library* lib);
  private:
    /// Signalled if the master library folder watcher detected a change
    void MasterLibraryFolderChanged(const QString& path);
    /// Signalled if the master library folder watcher detected a change
    void LibraryFoldersVDFChanged(const QString& path);
    /// Rescan libraryfolders.vdf
    void RescanLibraryFoldersVDF();
    /// Worker method for rescanning the library folder
    RescanResultType RescanLibraryFoldersVDFWorker();
    /// Method to deal with libraries scan result
    void LibraryFoldersScanResult();
  };
} // namespace piping

#endif // __PIPING_LIBRARIES_HPP__
