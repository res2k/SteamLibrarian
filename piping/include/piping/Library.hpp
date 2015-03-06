/**\file
* Steam library
*/
#ifndef __PIPING_LIBRARY_HPP__
#define __PIPING_LIBRARY_HPP__

#include <QObject>

namespace piping
{
  class App;

  /// Steam library
  class Library : public QObject
  {
    Q_OBJECT
  private:
    /// Library path
    QString m_path;
    /// Apps
    QList<App*> m_apps;
  public:
    Library(const QString& path, QObject* parent = nullptr);

    /// Get library display name
    QString displayName() const;
    /// Get library path
    const QString& path() const;

    /// Get number of apps
    Q_INVOKABLE int count() const;
    /// Get an app
    Q_INVOKABLE piping::App* get(int index) const;

    Q_PROPERTY(QString displayName READ displayName CONSTANT)
    Q_PROPERTY(QString path READ path CONSTANT)
  };
} // namespace piping

#endif // __PIPING_LIBRARY_HPP__
