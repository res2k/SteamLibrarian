#include "piping/App.hpp"

#include "piping/AppMover.hpp"
#include "piping/Library.hpp"

#include <QDir>
#include <QStringBuilder>

#include "vdf.hpp"

#include <boost/lexical_cast/try_lexical_convert.hpp>

namespace piping
{
  App::App(Library* lib, const QString& installDir) : QObject(lib), m_installDir(installDir) {}

  App::~App()
  {}

  bool App::ACFAppIDLower(const acf_name_data_pair& a, const acf_name_data_pair& b)
  {
    boost::optional<vdf::vdf_ptree> id_child_1(a.second->get_child_optional(L"AppState.appid"));
    boost::optional<vdf::vdf_ptree> id_child_2(b.second->get_child_optional(L"AppState.appid"));
    long id_1 = 0, id_2 = 0;
    if (id_child_1) id_1 = QString::fromStdWString(id_child_1->data()).toLong();
    if (id_child_2) id_2 = QString::fromStdWString(id_child_2->data()).toLong();
    return id_1 < id_2;
  }

  bool App::ACFNameCompare(const acf_name_data_pair& pair, const QString& name)
  {
    return pair.first.compare(name, Qt::CaseInsensitive) == 0;
  }

  void App::AddACF(const QString& acfName, vdf::vdf_ptree&& acfData)
  {
    std::unique_ptr<vdf::vdf_ptree> tree_ptr(new vdf::vdf_ptree(acfData));
    acf_name_data_pair newPair = std::make_pair(acfName, std::move(tree_ptr));
    auto insertPos = std::lower_bound(m_acfData.begin(), m_acfData.end(), newPair, &ACFAppIDLower);
    m_acfData.insert(insertPos, std::move(newPair));
    emit nameChanged();
    emit sizeOnDiskChanged();
  }

  void App::RemoveACF(const QString& acfName)
  {
    auto acfIt = std::find_if(m_acfData.begin(), m_acfData.end(), std::bind(ACFNameCompare, std::placeholders::_1, acfName));
    if (acfIt != m_acfData.end())
    {
      m_acfData.erase(acfIt);
      emit nameChanged();
      emit sizeOnDiskChanged();
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
        boost::optional<vdf::vdf_ptree> name_child(acfPair.second->get_child_optional(L"AppState.name"));
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
      boost::optional<vdf::vdf_ptree> name_child(acfPair.second->get_child_optional(L"AppState.SizeOnDisk"));
      if (name_child)
      {
        quint64 acf_size;
        if (boost::conversion::try_lexical_convert(name_child->data(), acf_size))
          size += acf_size;
      }
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
      boost::optional<vdf::vdf_ptree> installdir_child(acfPair.second->get_child_optional(L"AppState.installdir"));
      if (!installdir_child) continue;
      installdirs.insert(QString::fromStdWString(installdir_child->data()));
    }

    QStringList files;
    Q_FOREACH(const QString& installdir, installdirs)
    {
      files.append(QStringLiteral("common") % QDir::separator() % installdir);
    }
    for (const auto& acfPair : m_acfData)
    {
      // Add .acf files
      files.append(acfPair.first);

      // TODO: Add files from downloading/
    }

    return std::move(files);
  }
} // namespace piping
