/**\file
 * Libraries container
 */
#ifndef __PIPING_LIBRARIES_HPP__
#define __PIPING_LIBRARIES_HPP__

#include <QList>
#include <QObject>

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
  public:
    Libraries();
    Libraries(const QString& steamPath, QObject* parent = nullptr);

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
    /// Rescan libraryfolders.vdf
    template<bool EmitSignals> void RescanLibraryFoldersVDF();
  };
} // namespace piping

#endif // __PIPING_LIBRARIES_HPP__
