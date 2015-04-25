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
    /// Map from .acf name to App* object
    QHash<QString, App*> m_acfAppMap;
    /// Map from installdir to App* object
    QHash<QString, App*> m_dirAppMap;
    /// Map from App* object name to m_apps index
    QHash<App*, int> m_appObjMap;
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
    /// Get free space available on library disk
    quint64 freeSpace() const;

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
    /// An .acf file changed
    void ACFFileChanged(const QString& path);
    /// Look for .acf files
    void RescanForACFs();

    /// Scan a new or changed .acf
    void ParseACF(const QString& acfName);
    /// Remove an .acf + associated app object
    void RemoveACF(const QString& acfName);

    /// Worker method for concurrent .acf parsing
    acf_parse_result ParseACFWorker(const QString& acfName);
    /// Signalled if an ACF worker finished
    void ParseACFFinished();
  };
} // namespace piping

#endif // __PIPING_LIBRARY_HPP__
