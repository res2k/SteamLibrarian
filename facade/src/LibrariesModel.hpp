/**\file
 * Item model for Steam libraries list
 */
#ifndef __FACADE_LIBRARIESMODEL_HPP__
#define __FACADE_LIBRARIESMODEL_HPP__

#include "piping/Libraries.hpp"

#include <QAbstractListModel>
#include <QPointer>

#include <set>

#include "RedBlackTree.hpp"

namespace facade
{
  class LibrariesModel : public QAbstractListModel
  {
    Q_OBJECT
  private:
    /// Libraries object we're associated with
    QPointer<piping::Libraries> m_libraries;
    /// Library objects
    RedBlackTree<piping::Library*> m_libs;
  public:
    enum LibraryRoles
    {
      LibRole = Qt::UserRole+1,
      NameRole,
      AvailableSpaceRole
    };
    LibrariesModel(QObject* parent = nullptr);

    /// Set the libraries object we're attached to
    void setLibraries(piping::Libraries* libraries);
    /// Get the libraries object we're attached to
    piping::Libraries* libraries();
    Q_PROPERTY(piping::Libraries* libraries READ libraries WRITE setLibraries)

    int count() const;
    Q_INVOKABLE int find(piping::Library* lib) const;
    Q_INVOKABLE piping::Library* get(int index) const;
    Q_PROPERTY(int count READ count NOTIFY countChanged)

    /**\name QAbstractItemModel implementation
     * @{ */
    QHash<int, QByteArray> roleNames() const;
    int rowCount(const QModelIndex& index) const;
    QVariant data(const QModelIndex& index, int role) const;
    /** @} */
  signals:
    void countChanged();
  private slots:
    void libraryAdded(piping::Library* lib);
    void libraryRemoved(piping::Library* lib);
  };

} // namespace facade

#endif // __FACADE_LIBRARIESMODEL_HPP__
