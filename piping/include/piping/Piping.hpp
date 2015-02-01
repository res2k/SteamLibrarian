/**\file
 * Piping - interface to Steam.
 */
#ifndef __PIPING_PIPING_HPP__
#define __PIPING_PIPING_HPP__

#include <QObject>
#include <QString>

namespace piping
{
  class Piping : public QObject
  {
    Q_OBJECT
  private:
    /// Steam install path (or empty, if not found)
    QString m_steamInstallPath;
    /// Steam running state
    bool m_steamRunning;

    class RunningStateDetector;
    /// Helper to detect the Steam running state
    RunningStateDetector* m_runningStateDetector;
  public:
    Piping(QObject* parent = nullptr);
    ~Piping();

    /**\name Steam availability & state
     * @{ */
  public:
    /// Steam install path (or an empty string, if not found)
    QString steamInstallPath() const { return m_steamInstallPath; }
    /// Whether Steam is running
    bool steamRunning() const;

    Q_PROPERTY(QString steamInstallPath READ steamInstallPath CONSTANT)
    Q_PROPERTY(bool steamRunning READ steamRunning NOTIFY steamRunningChanged)
  signals:
    void steamRunningChanged(bool newState);
    /** @} */
  private:
    /// Fetch Steam install path
    void FetchInstallPath();
    /// Set steam running state (called by RunningStateDetector)
    Q_INVOKABLE void SetSteamRunning(bool running);
  };
} // namespace piping

#endif // __PIPING_PIPING_HPP__
