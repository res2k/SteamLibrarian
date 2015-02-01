/**\file
 * Piping - interface to Steam.
 */
#ifndef __PIPING_PIPING_HPP__
#define __PIPING_PIPING_HPP__

#include <QObject>
#include <QPointer>
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

    /// Object that performs the current pending action
    QPointer<QObject> m_pendingActionActor;
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

    /**\name Steam control
     * @{ */
  public:
    /**
     * Returns whether an action is currently pending.
     * \remarks An action is only "pending" as far as we're concerned with it,
     *   "pending" turning off doesn't give any guarantee that the action
     *   actually completed.
     */
    bool actionPending() const;

    Q_PROPERTY(bool actionPending READ actionPending NOTIFY actionPendingChanged)
  signals:
    void actionPendingChanged(bool newState);
  public slots:
    /// Start Steam.
    void steamStart();
    /// Shut down steam
    void steamShutdown();
    /** @} */
  private:
    /// Fetch Steam install path
    void FetchInstallPath();
    /// Set steam running state (called by RunningStateDetector)
    Q_INVOKABLE void SetSteamRunning(bool running);
    /// Set 'pending action' actor
    void SetPendingActionActor(QObject* actor);
    /// Return Steam executable path
    QString SteamExecutablePath() const;
    /// Launch Steam process with additional command line args
    void LaunchSteamProcess(const QStringList& arguments);
  private slots:
    void actionActorDestroyed();
  };
} // namespace piping

#endif // __PIPING_PIPING_HPP__
