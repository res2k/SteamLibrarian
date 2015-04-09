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
    connect(m_fsw, &QFileSystemWatcher::fileChanged, this, &Library::ACFFileChanged);
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

  quint64 Library::freeSpace() const
  {
    QStorageInfo si(m_path);
    return si.bytesAvailable();
  }

  int Library::count() const { return m_apps.count(); }
  
  App* Library::get(int index) const { return m_apps[index]; }

  void Library::LibraryFolderChanged(const QString& path)
  {
    RescanForACFs();
  }

  void Library::ACFFileChanged(const QString& path)
  {
    QFileInfo fi(path);
    QString acfName = fi.fileName();
    ParseACF(acfName);
  }

  void Library::RescanForACFs()
  {
    QDir steamappsDir(m_steamAppsPath);
    steamappsDir.setNameFilters(QStringList(QStringLiteral("*.acf")));
    QFileInfoList acf_entries = steamappsDir.entryInfoList();

    QList<QString> oldACFs = m_acfAppMap.keys();

    QSet<QString> seenACFs;
    Q_FOREACH(const QFileInfo& acf_info, acf_entries)
    {
      QString acfBase(acf_info.fileName());
      if (!m_acfAppMap.contains(acfBase))
      {
        // New .acf
        ParseACF(acfBase);
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

  void Library::ParseACF(const QString& acfName)
  {
    QFutureWatcher<acf_parse_result>* watcher =
      new QFutureWatcher<acf_parse_result>(this);
    connect(watcher, &QFutureWatcher<acf_parse_result>::finished, this, &Library::ParseACFFinished);
    m_activeACFParseWatchers.insert(watcher);

    watcher->setFuture(QtConcurrent::run(this, &Library::ParseACFWorker, acfName));
  }

  void Library::RemoveACF(const QString& acfName)
  {
    App* app = m_acfAppMap.value(acfName, nullptr);
    if (!app) return;

    QString acf_full_path = m_steamAppsPath % QDir::separator() % acfName;
    m_fsw->removePath(acf_full_path);
    app->RemoveACF(acfName);
    if (!app->HaveACFs())
    {
      emit appRemove(app);

      int index = m_appObjMap[app];
      int lastIndex = m_apps.count() - 1;
      if (index != lastIndex)
      {
        App* lastApp = m_apps[lastIndex];
        m_apps[index] = m_apps[lastIndex];
        m_appObjMap[lastApp] = index;
      }
      m_apps.takeLast();
      m_appObjMap.remove(app);
      m_acfAppMap.remove(acfName);
      m_dirAppMap.remove(app->installDir());
      app->deleteLater();
    }
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
      boost::optional<vdf::vdf_ptree> install_dir_child(vdf.first->get_child_optional(L"AppState.installdir"));
      if (install_dir_child)
      {
        QString installDir(QString::fromStdWString(install_dir_child->data()));
        if (m_acfAppMap.contains(vdf.second))
        {
          App* prevApp = m_acfAppMap.value(vdf.second);
          if (prevApp->installDir() != installDir)
          {
            // The installation directory changed, need to remove .acf from app
            RemoveACF(vdf.second);
          }
        }
        App* app = m_dirAppMap.value(installDir, nullptr);
        if (!app)
        {
          app = new App(this, installDir);
          m_dirAppMap[installDir] = app;
          int index = m_apps.count();
          m_apps.append(app);
          m_appObjMap[app] = index;
          emit appAdd(app);
        }
        app->AddACF(vdf.second, std::move(*(vdf.first)));
        m_acfAppMap[vdf.second] = app;
        QString acf_full_path = m_steamAppsPath % QDir::separator() % vdf.second;
        m_fsw->addPath(acf_full_path);
      }
      else
      {
        RemoveACF(vdf.second);
      }
    }
  }
} // namespace piping
