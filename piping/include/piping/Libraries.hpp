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
