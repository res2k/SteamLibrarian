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

#include "piping/Libraries.hpp"

#include "piping/Library.hpp"

#include <QtConcurrent>
#include <QDir>
#include <QFile>
#include <QFileSystemWatcher>
#include <QSet>
#include <QStringBuilder>

#include <boost/algorithm/cxx11/all_of.hpp>

#include "vdf.hpp"

#undef foreach

namespace piping
{
  Libraries::Libraries(const QString& steamPath, QObject* parent) : QObject(parent)
  {
    // Set up file watcher to react to libraryfolders changes
    m_fsw = new QFileSystemWatcher(this);
    connect(m_fsw, &QFileSystemWatcher::directoryChanged, this, &Libraries::MasterLibraryFolderChanged);
    connect(m_fsw, &QFileSystemWatcher::fileChanged, this, &Libraries::LibraryFoldersVDFChanged);

    m_masterLibraryPath = steamPath;
    connect(&rescanWatcher, &QFutureWatcher<RescanResultType>::finished, this, &Libraries::LibraryFoldersScanResult);
    if (QFile::exists(m_masterLibraryPath))
    {
      m_masterLibraryPath = QFileInfo(m_masterLibraryPath).canonicalFilePath();
      // Set up 'master' library
      m_libraries.append(new Library(m_masterLibraryPath, this));
      QString masterSteamapps = m_masterLibraryPath % QStringLiteral("/steamapps");

      m_fsw->addPath(masterSteamapps);

      m_libraryFolderVdf = masterSteamapps % QStringLiteral("/libraryfolders.vdf");
      if (QFile::exists(m_libraryFolderVdf))
      {
        m_fsw->addPath(m_libraryFolderVdf);
      }
      RescanLibraryFoldersVDF();
    }
  }

  Libraries::~Libraries()
  {
    if (pendingRescan) pendingRescan->waitForFinished();
  }

  int Libraries::count() const
  {
    return m_libraries.count();
  }

  Library* Libraries::get(int index) const
  {
    return m_libraries[index];
  }

  void Libraries::MasterLibraryFolderChanged(const QString& path)
  {
    if (QFile::exists(m_libraryFolderVdf))
    {
      m_fsw->addPath(m_libraryFolderVdf);
    }
    RescanLibraryFoldersVDF();
  }

  void Libraries::LibraryFoldersVDFChanged(const QString& path)
  {
    QFileInfo libraryFolderVdf_qfi(m_libraryFolderVdf);
    QFileInfo changed_qfi(path);
    if (libraryFolderVdf_qfi == changed_qfi)
      RescanLibraryFoldersVDF();
  }

  void Libraries::RescanLibraryFoldersVDF()
  {
    pendingRescan = std::make_shared<QFuture<RescanResultType> >(QtConcurrent::run(this, &Libraries::RescanLibraryFoldersVDFWorker));
    rescanWatcher.setFuture(*pendingRescan);
  }

  struct is_digit
  {
    bool operator()(wchar_t ch) const
    {
      return (ch >= '0') && (ch <= '9');
    }
  };

  Libraries::RescanResultType Libraries::RescanLibraryFoldersVDFWorker()
  {
    QSet<QString> newLibs;
    // Always keep 'master library'
    if (QFile::exists(m_masterLibraryPath))
    {
      newLibs.insert(m_masterLibraryPath);
    }

    if (QFile::exists(m_libraryFolderVdf))
    {
      // Parse libraryfolders.vdf
      try
      {
        vdf::vdf_ptree libraryfolders_vdf(vdf::ReadVDF(m_libraryFolderVdf));

        boost::optional<vdf::vdf_ptree&> folders_key = libraryfolders_vdf.get_child_optional(L"LibraryFolders");
        if (folders_key)
        {
          for(const vdf::vdf_ptree::const_iterator::value_type& folder_pair : *folders_key)
          {
            if (boost::algorithm::all_of(folder_pair.first, is_digit()))
            {
              QFileInfo lib_dir_qfi(QString::fromStdWString(folder_pair.second.data()));
              if (!lib_dir_qfi.exists()) continue;
              newLibs.insert(lib_dir_qfi.canonicalFilePath());
            }
          }
        }
      }
      catch (std::exception& e)
      {
        qCritical("Error parsing %s: %s", qUtf8Printable(m_libraryFolderVdf), qUtf8Printable(QString::fromLocal8Bit(e.what())));
      }
    }
    return std::move(newLibs);
  }

  void Libraries::LibraryFoldersScanResult()
  {
    QSet<QString> newLibs(std::move(pendingRescan->result()));

    // Check for removed libraries
    QList<Library*>::iterator currentLibsIt = m_libraries.begin();
    while (currentLibsIt != m_libraries.end())
    {
      Library* lib = *currentLibsIt;
      if (!newLibs.contains(lib->path()))
      {
        emit libraryRemove(lib);
        currentLibsIt = m_libraries.erase(currentLibsIt);
        lib->deleteLater();
      }
      else
      {
        newLibs.remove(lib->path());
        ++currentLibsIt;
      }
    }
    // Add new libraries
    Q_FOREACH(const QString& newLibPath, newLibs)
    {
      Library* lib = new Library(newLibPath, this);
      m_libraries.append(lib);
      emit libraryAdd(lib);
    }
  }
} // namespace piping
