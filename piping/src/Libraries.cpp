#include "piping/Libraries.hpp"

#include "piping/Library.hpp"

#include <QDir>
#include <QFile>
#include <QFileSystemWatcher>
#include <QSet>
#include <QStringBuilder>

#include "vdf.hpp"

#undef foreach

namespace piping
{
  Libraries::Libraries(const QString& steamPath, QObject* parent) : QObject(parent)
  {
    m_masterLibraryPath = steamPath;
    if (QFile::exists(m_masterLibraryPath))
    {
      m_masterLibraryPath = QFileInfo(m_masterLibraryPath).canonicalFilePath();
      // Set up 'master' library
      m_libraries.append(new Library(m_masterLibraryPath, this));
      QString masterSteamapps = m_masterLibraryPath % QStringLiteral("/steamapps");

      // Set up file watcher to react to libraryfolders changes
      QFileSystemWatcher* fsw = new QFileSystemWatcher(this);
      connect(fsw, &QFileSystemWatcher::directoryChanged, this, &Libraries::MasterLibraryFolderChanged);
      fsw->addPath(masterSteamapps);

      m_libraryFolderVdf = masterSteamapps % QStringLiteral("/libraryfolders.vdf");
      RescanLibraryFoldersVDF<false>();
    }
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
    QFileInfo libraryFolderVdf_qfi(m_libraryFolderVdf);
    QFileInfo changed_qfi(path);
    if (libraryFolderVdf_qfi == changed_qfi)
      RescanLibraryFoldersVDF<true>();
  }

  template<bool EmitSignals>
  void Libraries::RescanLibraryFoldersVDF()
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
            QFileInfo lib_dir_qfi(QString::fromStdWString(folder_pair.second.data()));
            newLibs.insert(lib_dir_qfi.canonicalFilePath());
          }
        }
      }
      catch (std::exception& e)
      {
        qCritical("Error parsing %s: %s", qUtf8Printable(m_libraryFolderVdf), qUtf8Printable(QString::fromLocal8Bit(e.what())));
      }

      // Check for removed libraries
      QList<Library*>::iterator currentLibsIt = m_libraries.begin();
      while (currentLibsIt != m_libraries.end())
      {
        Library* lib = *currentLibsIt;
        if (!newLibs.contains(lib->path()))
        {
          if (EmitSignals) emit libraryRemove(lib);
          currentLibsIt = m_libraries.erase(currentLibsIt);
          delete lib;
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
        if (EmitSignals) emit libraryAdd(lib);
      }
    }
  }
} // namespace piping
