#include "piping/Library.hpp"

#include "piping/App.hpp"

#include <QtConcurrent>
#include <QDir>
#include <QFileInfo>
#include <QFileSystemWatcher>
#include <QSet>
#include <QStringBuilder>
#include <QStringList>

#include "vdf.hpp"

namespace piping
{
  Library::Library(const QString& path, QObject* parent) : QObject(parent), m_path(path)
  {
    m_steamAppsPath = m_path % QStringLiteral("/steamapps");

    // Set up watcher for lib dir
    m_fsw = new QFileSystemWatcher(this);
    connect(m_fsw, &QFileSystemWatcher::directoryChanged, this, &Library::LibraryFolderChanged);
    m_fsw->addPath(m_steamAppsPath);

    // Scan m_steamAppsPath for .acf files
    RescanForACFs();
  }

  Library::~Library()
  {
    Q_FOREACH(QFutureWatcher<acf_parse_result>* watcher, m_activeACFParseWatchers)
    {
      watcher->future().waitForFinished();
    }
  }

  QString Library::displayName() const
  {
    return QDir::toNativeSeparators(m_path);
  }

  const QString& Library::path() const { return m_path;  }

  int Library::count() const { return m_apps.count(); }
  
  App* Library::get(int index) const { return m_apps[index]; }

  void Library::LibraryFolderChanged(const QString& path)
  {
    RescanForACFs();
  }

  void Library::RescanForACFs()
  {
    QDir steamappsDir(m_steamAppsPath);
    steamappsDir.setNameFilters(QStringList(QStringLiteral("*.acf")));
    QFileInfoList acf_entries = steamappsDir.entryInfoList();

    QList<QString> oldACFs = m_appObjMap.keys();

    QSet<QString> seenACFs;
    Q_FOREACH(const QFileInfo& acf_info, acf_entries)
    {
      QString acfBase(acf_info.fileName());
      if (!m_appObjMap.contains(acfBase))
      {
        // New .acf
        NewACF(acfBase);
      }
      seenACFs.insert(acfBase);
    }

    Q_FOREACH(const QString& oldACF, oldACFs)
    {
      if (!seenACFs.contains(oldACF))
      {
        // .acf is gone
        RemoveACF(oldACF);
      }
    }
  }

  void Library::NewACF(const QString& acfName)
  {
    QFutureWatcher<acf_parse_result>* watcher =
      new QFutureWatcher<acf_parse_result>(this);
    connect(watcher, &QFutureWatcher<acf_parse_result>::finished, this, &Library::ParseACFFinished);
    m_activeACFParseWatchers.insert(watcher);

    watcher->setFuture(QtConcurrent::run(this, &Library::ParseACFWorker, acfName));
  }

  void Library::RemoveACF(const QString& acfName)
  {
    int index = m_appObjMap[acfName];
    App* app = m_apps[index];
    emit appRemove(app);

    int lastIndex = m_apps.count()-1;
    if (index != lastIndex)
    {
      QString lastACF = m_apps[lastIndex]->acfName();
      m_apps[index] = m_apps[lastIndex];
      m_appObjMap[lastACF] = index;
    }
    m_apps.takeLast();
    m_appObjMap.remove(app->acfName());
    app->deleteLater();
  }

  Library::acf_parse_result Library::ParseACFWorker(const QString& acfPath)
  {
    QString acf_full_path = m_steamAppsPath % QDir::separator() % acfPath;
    std::shared_ptr<vdf::vdf_ptree> acf;
    try
    {
      acf = std::make_shared<vdf::vdf_ptree> (vdf::ReadVDF(acf_full_path));
    }
    catch (std::exception& e)
    {
      qCritical("Error parsing %s: %s", qUtf8Printable(acf_full_path), qUtf8Printable(QString::fromLocal8Bit(e.what())));
    }
    return std::make_pair(acf, acfPath);
  }

  void Library::ParseACFFinished()
  {
    QFutureWatcher<acf_parse_result>* watcher(static_cast<QFutureWatcher<acf_parse_result>*> (sender()));
    m_activeACFParseWatchers.remove(watcher);
    acf_parse_result vdf = watcher->future().result();
    watcher->deleteLater();

    if (vdf.first)
    {
      App* app = new App(this, vdf.second);
      app->SetACF(std::move (*(vdf.first)));
      int index = m_apps.count();
      m_apps.append(app);
      m_appObjMap[vdf.second] = index;
      emit appAdd(app);
    }
  }
} // namespace piping
