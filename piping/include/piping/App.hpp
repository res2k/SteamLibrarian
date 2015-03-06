/**\file
 * An app in a Steam library
 */
#ifndef __PIPING_APP_HPP__
#define __PIPING_APP_HPP__

#include <QObject>

namespace piping
{
  class Library;

  /// Steam library
  class App : public QObject
  {
    Q_OBJECT
  private:
    /// App name
    QString m_name;
  public:
    App(Library* lib, const QString& name);

    /// Get library this app is in
    Library* library() const;
    /// Get app name
    const QString& name() const;

    Q_PROPERTY(Library* library READ library CONSTANT)
    Q_PROPERTY(QString name READ name CONSTANT)
  };
} // namespace piping

#endif // __PIPING_APP_HPP__
