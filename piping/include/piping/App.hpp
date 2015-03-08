/**\file
 * An app in a Steam library
 */
#ifndef __PIPING_APP_HPP__
#define __PIPING_APP_HPP__

#include <QObject>

#include "vdf_fwd.hpp"

namespace piping
{
  class Library;

  /// Steam library
  class App : public QObject
  {
    Q_OBJECT
  private:
    /// App .acf name
    QString m_acf;
    /// .acf data
    std::unique_ptr<vdf::vdf_ptree> m_acfData;
  public:
    App(Library* lib, const QString& acf);
    ~App();

    /// Set the .acf data
    void SetACF(vdf::vdf_ptree&& acfData);

    /// Get library this app is in
    piping::Library* library() const;
    /// Get .acf name
    const QString& acfName() const;
    /// Get app name
    QString name() const;

    Q_PROPERTY(piping::Library* library READ library CONSTANT)
    Q_PROPERTY(QString name READ name CONSTANT)
  };
} // namespace piping

#endif // __PIPING_APP_HPP__
