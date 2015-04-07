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
