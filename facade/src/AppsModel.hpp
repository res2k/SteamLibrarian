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
* Item model for Steam applications list
*/
#ifndef __FACADE_APPSMODEL_HPP__
#define __FACADE_APPSMODEL_HPP__

#include "piping/Libraries.hpp"

#include <QAbstractListModel>
#include <QPointer>

#include <set>

#include "RedBlackTree.hpp"

namespace piping
{
  class App;
} // namespace piping
  
namespace facade
{
  class AppsModel : public QAbstractListModel
  {
    Q_OBJECT
  private:
    /// Libraries object we're associated with
    QPointer<piping::Libraries> m_libraries;
    typedef RedBlackTree<piping::App*> app_tree_type;
    /// Library objects
    RedBlackTree<piping::Library*, app_tree_type> m_libs;
  public:
    enum LibraryRoles
    {
      AppNameRole = Qt::UserRole + 1,
      LibRole,
      SizeOnDiskRole,
      SizeOnDiskStrRole
    };
    AppsModel(QObject* parent = nullptr);

    /// Set the libraries object we're attached to
    void setLibraries(piping::Libraries* libraries);
    /// Get the libraries object we're attached to
    piping::Libraries* libraries();
    Q_PROPERTY(piping::Libraries* libraries READ libraries WRITE setLibraries)

    int count() const;
    Q_PROPERTY(int count READ count NOTIFY countChanged)
    Q_INVOKABLE piping::App* get(int index) const;

    /**\name QAbstractItemModel implementation
     * @{ */
    QHash<int, QByteArray> roleNames() const;
    int rowCount(const QModelIndex& index) const;
    QVariant data(const QModelIndex& index, int role) const;
    /** @} */
  private:
    void AddLibrary(piping::Library* lib);
  signals:
    void countChanged();
  private slots:
    void libraryAdded(piping::Library* lib);
    void libraryRemoved(piping::Library* lib);
    void appAdded(piping::App* app);
    void appRemoved(piping::App* app);
    void appDataChanged();
  };
} // namespace facade

#endif // __FACADE_APPSMODEL_HPP__
