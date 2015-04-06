#include "LibrariesModel.hpp"

#include "piping/Library.hpp"

#include "FormatByteAmount.hpp"

namespace facade
{
  LibrariesModel::LibrariesModel(QObject* parent) : QAbstractListModel(parent)
  {
    connect(this, &QAbstractListModel::rowsInserted, this, &LibrariesModel::countChanged);
    connect(this, &QAbstractListModel::rowsRemoved, this, &LibrariesModel::countChanged);
  }

  void LibrariesModel::setLibraries(piping::Libraries* libraries)
  {
    if (m_libraries == libraries) return;

    beginResetModel();
    if (m_libraries) m_libraries->disconnect(this);
    m_libraries = libraries;
    m_libs.clear();
    if (m_libraries)
    {
      connect(m_libraries, &piping::Libraries::libraryAdd, this, &LibrariesModel::libraryAdded);
      connect(m_libraries, &piping::Libraries::libraryRemove, this, &LibrariesModel::libraryRemoved);

      for (int i = 0; i < m_libraries->count(); i++)
      {
        m_libs.insert(m_libraries->get(i));
      }
      emit countChanged();
    }
    endResetModel();
  }

  piping::Libraries* LibrariesModel::libraries()
  {
    return m_libraries;
  }

  int LibrariesModel::count() const
  {
    return static_cast<int> (m_libs.size());
  }

  int LibrariesModel::find(piping::Library* lib) const
  {
    return static_cast<int> (m_libs.querySortedIndex(lib));
  }

  piping::Library* LibrariesModel::get(int index) const
  {
    return *(m_libs.queryBySortedIndex(index));
  }

  QHash<int, QByteArray> LibrariesModel::roleNames() const
  {
    QHash<int, QByteArray> roles(QAbstractListModel::roleNames());
    roles[LibRole] = "lib";
    roles[NameRole] = "name";
    roles[AvailableSpaceRole] = "available";
    return roles;
  }

  int LibrariesModel::rowCount(const QModelIndex& index) const
  {
    return static_cast<int> (m_libs.size());
  }

  QVariant LibrariesModel::data(const QModelIndex& index, int role) const
  {
    piping::Library* lib = *(m_libs.queryBySortedIndex(index.row()));
    switch (role)
    {
    case Qt::DisplayRole:
    case NameRole:
      return lib->displayName();
    case LibRole:
      return QVariant::fromValue(lib);
    case AvailableSpaceRole:
      return FormatByteAmount(lib->freeSpace(), roundDown);
    default:
      break;
    }

    return QVariant();
  }

  void LibrariesModel::libraryAdded(piping::Library* lib)
  {
    int newIndex = static_cast<int> (m_libs.querySortedIndex(lib));
    beginInsertRows(QModelIndex(), newIndex, newIndex);
    m_libs.insert(std::move (lib));
    endInsertRows();
    emit countChanged();
  }

  void LibrariesModel::libraryRemoved(piping::Library* lib)
  {
    int index = static_cast<int> (m_libs.querySortedIndex(lib));
    beginRemoveRows(QModelIndex(), index, index);
    m_libs.remove(std::move(lib));
    endRemoveRows();
    emit countChanged();
  }
} // namespace facade
