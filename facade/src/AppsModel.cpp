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

#include "AppsModel.hpp"

#include "piping/App.hpp"
#include "piping/Library.hpp"

#include "FormatByteAmount.hpp"

namespace facade
{
  AppsModel::AppsModel(QObject* parent) : QAbstractListModel(parent)
  {
  }

  void AppsModel::setLibraries(piping::Libraries* libraries)
  {
    if (m_libraries == libraries) return;

    beginResetModel();
    if (m_libraries)
      m_libraries->disconnect(this);
    m_libraries = libraries;
    for (const auto& connectedLib : m_libs)
    {
      connectedLib.first->disconnect(this);
    }
    m_libs.clear();

    if (m_libraries)
    {
      connect(m_libraries, &piping::Libraries::libraryAdd, this, &AppsModel::libraryAdded);
      connect(m_libraries, &piping::Libraries::libraryRemove, this, &AppsModel::libraryRemoved);

      for (int i = 0; i < m_libraries->count(); i++)
      {
        AddLibrary(m_libraries->get(i));
      }
    }
    endResetModel();
    emit countChanged();
  }

  piping::Libraries* AppsModel::libraries()
  {
    return m_libraries;
  }

  int AppsModel::count() const
  {
    return static_cast<int> (m_libs.totalUserCount());
  }

  Q_INVOKABLE piping::App* AppsModel::get(int index) const
  {
    for (auto connectedLib : m_libs)
    {
      int first = static_cast<int> (m_libs.queryUserCount(connectedLib.first));
      int last = first + static_cast<int> (connectedLib.second.size());
      if ((index >= first) && (index < last))
      {
        return *(connectedLib.second.queryBySortedIndex(index - first));
      }
    }
    return nullptr;
  }

  QHash<int, QByteArray> AppsModel::roleNames() const
  {
    QHash<int, QByteArray> roles(QAbstractListModel::roleNames());
    roles[AppNameRole] = "app";
    roles[LibRole] = "library";
    roles[SizeOnDiskRole] = "sizeOnDisk";
    roles[SizeOnDiskStrRole] = "sizeOnDiskStr";
    return roles;
  }

  int AppsModel::rowCount(const QModelIndex& index) const
  {
    return static_cast<int> (m_libs.totalUserCount());
  }

  QVariant AppsModel::data(const QModelIndex& index, int role) const
  {
    piping::App* app = get(index.row());
    if (app)
    {
      switch (role)
      {
      case Qt::DisplayRole:
      case AppNameRole:
        return app->name();
      case LibRole:
        return app->library()->displayName();
      case SizeOnDiskRole:
        return app->sizeOnDisk() + app->downloadingSize();
      case SizeOnDiskStrRole:
        {
          QString sizeStr = FormatByteAmount(app->sizeOnDisk(), roundUp);
          quint64 downloadSize = app->downloadingSize();
          if (downloadSize > 0)
            sizeStr = QStringLiteral("%1 (+ %2)").arg(sizeStr, FormatByteAmount(downloadSize, roundUp));
          return sizeStr;
        }
      default:
        break;
      }
    }

    return QVariant();
  }

  void AppsModel::AddLibrary(piping::Library* lib)
  {
    m_libs.insert(std::move(lib));
    app_tree_type& app_tree = *(m_libs.queryValue(lib));

    connect(lib, &piping::Library::appAdd, this, &AppsModel::appAdded);
    connect(lib, &piping::Library::appRemove, this, &AppsModel::appRemoved);
    for (int i = 0; i < lib->count(); i++)
    {
      app_tree.insert(lib->get(i));
    }
    m_libs.setUserCount(lib, app_tree.size());
  }

  void AppsModel::libraryAdded(piping::Library* lib)
  {
    int newIndex = static_cast<int> (m_libs.querySortedIndex(lib));
    int count = lib->count();
    if (count > 0)
    {
      beginInsertRows(QModelIndex(), newIndex, newIndex + count - 1);
    }
    AddLibrary(lib);
    if (count > 0)
    {
      endInsertRows();
      emit countChanged();
    }
  }

  void AppsModel::libraryRemoved(piping::Library* lib)
  {
    int index = static_cast<int> (m_libs.querySortedIndex(lib));
    int count = lib->count();
    if (count > 0)
    {
      beginRemoveRows(QModelIndex(), index, index + count - 1);
    }
    m_libs.remove(std::move(lib));
    if (count > 0)
    {
      endRemoveRows();
      emit countChanged();
    }
  }

  void AppsModel::appAdded(piping::App* app)
  {
    piping::Library* lib = app->library();
    app_tree_type* app_tree = m_libs.queryValue(lib);
    if (!app_tree) return;

    int newIndex = static_cast<int> (m_libs.queryUserCount(lib) + app_tree->querySortedIndex(app));
    beginInsertRows(QModelIndex(), newIndex, newIndex);
    app_tree->insert(std::move(app));
    m_libs.setUserCount(lib, app_tree->size());
    endInsertRows();
    emit countChanged();

    connect(app, &piping::App::dataChanged, this, &AppsModel::appDataChanged);
  }

  void AppsModel::appRemoved(piping::App* app)
  {
    piping::Library* lib = app->library();
    app_tree_type* app_tree = m_libs.queryValue(lib);
    if (!app_tree) return;

    int index = static_cast<int> (m_libs.queryUserCount(lib) + app_tree->querySortedIndex(app));
    beginRemoveRows(QModelIndex(), index, index);
    app_tree->remove(app);
    m_libs.setUserCount(lib, app_tree->size());
    endRemoveRows();
    emit countChanged();
  }

  void AppsModel::appDataChanged()
  {
    piping::App* app = qobject_cast<piping::App*> (sender());
    if (!app) return;
    piping::Library* lib = app->library();
    app_tree_type* app_tree = m_libs.queryValue(lib);

    int index = static_cast<int> (m_libs.queryUserCount(lib) + app_tree->querySortedIndex(app));
    emit dataChanged(createIndex(index, 0), createIndex(index, 0));
  }
} // namespace facade