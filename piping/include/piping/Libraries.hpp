/**\file
 * Libraries container
 */
#ifndef __PIPING_LIBRARIES_HPP__
#define __PIPING_LIBRARIES_HPP__

#include <QList>
#include <QObject>

namespace piping
{
  class Library;

  /// Libraries container
  class Libraries : public QObject
  {
    Q_OBJECT
  private:
    /// Known libraries
    QList<Library*> m_libraries;
  public:
    Libraries();
    Libraries(const QString& steamPath, QObject* parent = nullptr);

    /// Get number of known libraries
    Q_INVOKABLE int count() const;
    /// Get a library
    Q_INVOKABLE piping::Library* get(int index) const;

  signals:
    /// Emitted when a new library was added
    void libraryAdd(piping::Library* lib);
    /// Emitted when a library is about to be removed
    void libraryRemove(piping::Library* lib);
  };
} // namespace piping

#endif // __PIPING_LIBRARIES_HPP__
