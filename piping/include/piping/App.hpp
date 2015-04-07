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
    /// App install dir name
    QString m_installDir;
    typedef std::pair<QString, std::unique_ptr<vdf::vdf_ptree>> acf_name_data_pair;
    /// .acf data
    std::vector<acf_name_data_pair> m_acfData;
    /// Comparison function for ACF data pairs
    static bool ACFAppIDLower(const acf_name_data_pair& a, const acf_name_data_pair& b);
    /// Comparison function for ACF data pair
    static bool ACFNameCompare(const acf_name_data_pair& pair, const QString& name);
  public:
    App(Library* lib, const QString& installDir);
    ~App();

    /// Add .acf data
    void AddACF(const QString& acfName, vdf::vdf_ptree&& acfData);
    /// Remove some .acf data
    void RemoveACF(const QString& acfName);
    /// Returns whether we have more than 0 ACFs.
    bool HaveACFs() const;

    /// Get library this app is in
    piping::Library* library() const;
    /// Get app install dir name
    const QString& installDir() const;
    /// Get app name
    QString name() const;
    /// Get disk space used by app
    quint64 sizeOnDisk() const;

    /// Query an object to perform a move to another library.
    Q_INVOKABLE QObject* queryMover(piping::Library* destination);

    /**
     * Get a list of file and directory names, relative to the library directory,
     * with files associated with this app
     */
    QStringList GetAppFiles() const;

    Q_PROPERTY(piping::Library* library READ library CONSTANT)
    Q_PROPERTY(QString name READ name NOTIFY dataChanged)
    Q_PROPERTY(quint64 sizeOnDisk READ sizeOnDisk NOTIFY dataChanged)
  signals:
    /// Some application data changed
    void dataChanged();
  };
} // namespace piping

#endif // __PIPING_APP_HPP__
