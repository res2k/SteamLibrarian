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

#include "piping/App.hpp"

#include "piping/AppMover.hpp"
#include "piping/Library.hpp"

#include <QtConcurrent>
#include <QDir>
#include <QStringBuilder>

#include "vdf.hpp"

#include <boost/lexical_cast/try_lexical_convert.hpp>

#include <queue>

namespace piping
{
  struct App::ACFData
  {
    QString m_appid;
    vdf::vdf_ptree m_tree;
    QFuture<void> m_downloadScanFuture;
    boost::atomic<quint64> m_downloadSize;

    ACFData(vdf::vdf_ptree&& tree) : m_tree(std::move(tree)), m_downloadSize(0) {}
    ACFData(ACFData&& other) : m_tree(std::move(other.m_tree)), m_downloadSize(0)  {}
    ACFData(const ACFData&) = delete;
    ~ACFData()
    {
      m_downloadScanFuture.waitForFinished();
    }
  };

  App::App(Library* lib, const QString& installDir)
    : QObject(lib), m_installDir(installDir), m_dataChangePending(false)
  {}

  App::~App()
  {}

  bool App::ACFAppIDLower(const acf_name_data_pair& a, const acf_name_data_pair& b)
  {
    long id_1 = 0, id_2 = 0;
    id_1 = a.second->m_appid.toLong();
    id_2 = b.second->m_appid.toLong();
    return id_1 < id_2;
  }

  bool App::ACFNameCompare(const acf_name_data_pair& pair, const QString& name)
  {
    return pair.first.compare(name, Qt::CaseInsensitive) == 0;
  }

  void App::AddACF(const QString& acfName, vdf::vdf_ptree&& acfData)
  {
    {
      auto acfIt = std::find_if(m_acfData.begin(), m_acfData.end(), std::bind(ACFNameCompare, std::placeholders::_1, acfName));
      if (acfIt != m_acfData.end())
      {
        m_acfData.erase(acfIt);
      }
    }

    acf_data_vec::iterator inserted;
    {
      std::unique_ptr<ACFData> data_ptr(new ACFData(std::move(acfData)));
      {
        boost::optional<vdf::vdf_ptree> id_child(data_ptr->m_tree.get_child_optional(L"AppState.appid"));
        if (id_child)
          data_ptr->m_appid = QString::fromStdWString(id_child->data());
      }
      acf_name_data_pair newPair = std::make_pair(acfName, std::move(data_ptr));
      auto insertPos = std::lower_bound(m_acfData.begin(), m_acfData.end(), newPair, &ACFAppIDLower);
      inserted = m_acfData.insert(insertPos, std::move(newPair));
    }
    emit dataChanged();

    // Check for 'downloading' files
    if (!inserted->second->m_appid.isEmpty())
    {
      // TODO: These may very well change while Steam is running -> need to watch FS
      QStringList downloadFiles;

      QString steamappsPath(library()->path() % QDir::separator() % QStringLiteral("steamapps"));
      QString downloadingAppRel(QStringLiteral("downloading") % QDir::separator() % inserted->second->m_appid);
      QString downloadingApp(steamappsPath % QDir::separator() % downloadingAppRel);
      if (QFileInfo::exists(downloadingApp))
      {
        inserted->second->m_downloadScanFuture =
          QtConcurrent::run(this, &App::ScanDownloadFiles, downloadingApp, inserted->second.get());
      }
    }
  }

  void App::RemoveACF(const QString& acfName)
  {
    auto acfIt = std::find_if(m_acfData.begin(), m_acfData.end(), std::bind(ACFNameCompare, std::placeholders::_1, acfName));
    if (acfIt != m_acfData.end())
    {
      m_acfData.erase(acfIt);
      emit dataChanged();
    }
  }

  bool App::HaveACFs() const
  {
    return !m_acfData.empty();
  }

  Library* App::library() const
  {
    return static_cast<Library*> (parent());
  }

  const QString& App::installDir() const
  {
    return m_installDir;
  }

  QString App::name() const
  {
    QString assembledName;
    if (!m_acfData.empty())
    {
      for(const acf_name_data_pair& acfPair : m_acfData)
      {
        QString partStr;
        boost::optional<vdf::vdf_ptree> name_child(acfPair.second->m_tree.get_child_optional(L"AppState.name"));
        if (name_child)
          partStr = QString::fromStdWString(name_child->data());
        else
          partStr = acfPair.first;
        if (!assembledName.isEmpty())
          assembledName.append(QStringLiteral(", "));
        assembledName.append(partStr);
      }
    }
    return assembledName.isEmpty() ? QStringLiteral("???") : assembledName;
  }

  quint64 App::sizeOnDisk() const
  {
    quint64 size = 0;
    for (const acf_name_data_pair& acfPair : m_acfData)
    {
      boost::optional<vdf::vdf_ptree> name_child(acfPair.second->m_tree.get_child_optional(L"AppState.SizeOnDisk"));
      if (name_child)
      {
        quint64 acf_size;
        if (boost::conversion::try_lexical_convert(name_child->data(), acf_size))
          size += acf_size;
      }
    }
    return size;
  }

  quint64 App::downloadingSize() const
  {
    quint64 size = 0;
    for (const acf_name_data_pair& acfPair : m_acfData)
    {
      size += acfPair.second->m_downloadSize.load(boost::memory_order_acquire);
    }
    return size;
  }

  QObject* App::queryMover(piping::Library* destination)
  {
    return new AppMover(this, destination);
  }

  QStringList App::GetAppFiles() const
  {
    // Unique install dirs
    QSet<QString> installdirs;
    for (const auto& acfPair : m_acfData)
    {
      // Add data files
      boost::optional<vdf::vdf_ptree> installdir_child(acfPair.second->m_tree.get_child_optional(L"AppState.installdir"));
      if (!installdir_child) continue;
      installdirs.insert(QString::fromStdWString(installdir_child->data()));
    }

    QString steamappsPath(library()->path() % QDir::separator() % QStringLiteral("steamapps"));
    QStringList files;
    Q_FOREACH(const QString& installdir, installdirs)
    {
      QString dir_rel(QStringLiteral("common") % QDir::separator() % installdir);
      /* Directory might not exist if e.g. preloading. */
      if (!QFileInfo::exists(steamappsPath % QDir::separator() % dir_rel)) continue;
      files.append(dir_rel);
    }
    for (const auto& acfPair : m_acfData)
    {
      // Add .acf files
      files.append(acfPair.first);

      // Add files from downloading/
      QString downloadingAppRel(QStringLiteral("downloading") % QDir::separator() % acfPair.second->m_appid);
      QString downloadingApp(steamappsPath % QDir::separator() % downloadingAppRel);
      if (QFileInfo::exists(downloadingApp))
      {
        files.append(downloadingAppRel);
      }

      QString downloadingPath(steamappsPath % QDir::separator() % QStringLiteral("downloading"));
      QDir downloadingDir(downloadingPath);
      downloadingDir.setNameFilters(QStringList(QStringLiteral("state_") % acfPair.second->m_appid % QStringLiteral("_*.patch")));
      QFileInfoList stateEntries = downloadingDir.entryInfoList();
      for (const auto& stateEntry : stateEntries)
      {
        files.append(QStringLiteral("downloading") % QDir::separator() % stateEntry.fileName());
      }
    }

    return std::move(files);
  }

  void App::ScanDownloadFiles(const QString& downloadingRoot, ACFData* acfData)
  {
    acfData->m_downloadSize.store(0, boost::memory_order_relaxed);

    std::queue<QString> scanQueue;
    scanQueue.push(downloadingRoot);
    while (!scanQueue.empty())
    {
      QString entry(scanQueue.front());
      scanQueue.pop();
      QFileInfo fi(entry);
      if (fi.isDir())
      {
        QDir entryDir(entry);
        QFileInfoList entries = entryDir.entryInfoList();
        for (const auto& entry : entries)
        {
          if ((entry.fileName() == QStringLiteral("."))
            || (entry.fileName() == QStringLiteral("..")))
          {
            continue;
          }
          scanQueue.push(entry.absoluteFilePath());
        }
      }
      else
      {
        acfData->m_downloadSize.fetch_add(fi.size(), boost::memory_order_acq_rel);
        CoalesceDataChanged();
      }
    }
  }

  void App::CoalesceDataChanged()
  {
    if (!m_dataChangePending.load(boost::memory_order_acquire))
    {
      m_dataChangePending.store(true, boost::memory_order_release);
      QMetaObject::invokeMethod(this, "TriggerDataChanged", Qt::QueuedConnection);
    }
  }

  void App::TriggerDataChanged()
  {
    emit dataChanged();
    m_dataChangePending.store(false, boost::memory_order_release);
  }
} // namespace piping
