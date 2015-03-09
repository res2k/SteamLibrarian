/**\file
* Steam library
*/
#ifndef __PIPING_LIBRARY_HPP__
#define __PIPING_LIBRARY_HPP__

#include <QFutureWatcher>
#include <QHash>
#include <QObject>

#include <memory>

#include "vdf_fwd.hpp"

class QFileSystemWatcher;

namespace piping
{
  class App;

  /// Steam library
  class Library : public QObject
  {
    Q_OBJECT
  private:
    /// Library path
    QString m_path;
    /// Library 'steamapps' path
    QString m_steamAppsPath;
    /// Apps
    QList<App*> m_apps;
    /// Library directory watcher
    QFileSystemWatcher* m_fsw;
    /// Map from .acf name to m_apps index
    QHash<QString, int> m_appObjMap;
    typedef std::pair<std::shared_ptr<vdf::vdf_ptree>, QString> acf_parse_result;
    /// Active watchers for .acf parsing
    QSet<QFutureWatcher<acf_parse_result>*> m_activeACFParseWatchers;
  public:
    Library(const QString& path, QObject* parent = nullptr);
    ~Library();

    /// Get library display name
    QString displayName() const;
    /// Get library path
    const QString& path() const;

    /// Get number of apps
    Q_INVOKABLE int count() const;
    /// Get an app
    Q_INVOKABLE piping::App* get(int index) const;

    Q_PROPERTY(QString displayName READ displayName CONSTANT)
    Q_PROPERTY(QString path READ path CONSTANT)
  signals:
    /// Emitted when a new app was added
    void appAdd(piping::App* app);
    /// Emitted when an app is about to be removed
    void appRemove(piping::App* app);
  protected:
    /// The library folder contents changed
    void LibraryFolderChanged(const QString& path);
    /// Look for .acf files
    void RescanForACFs();

    /// Scan a new .acf
    void NewACF(const QString& acfName);
    /// Remove an .acf + associated app object
    void RemoveACF(const QString& acfName);

    /// Worker method for concurrent .acf parsing
    acf_parse_result ParseACFWorker(const QString& acfName);
    /// Signalled if an ACF worker finished
    void ParseACFFinished();
  };
} // namespace piping

#endif // __PIPING_LIBRARY_HPP__
