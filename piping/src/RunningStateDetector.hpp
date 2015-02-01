/**\file
 * Definition of Piping::RunningStateDetector
 */
#ifndef __PIPING_RUNNINGSTATEDETECTOR_HPP__
#define __PIPING_RUNNINGSTATEDETECTOR_HPP__

#include "piping/Piping.hpp"

#include <QThread>
#include <QTimer>

#include <Windows.h>

namespace piping
{
  class Piping::RunningStateDetector : public QObject
  {
    Q_OBJECT
  private:
    /// The SteamInstanceGlobal (if present)
    HANDLE m_steamMutex;
    class WaitMutexThread;
    /// Thread to wait for m_steamMutex
    WaitMutexThread* m_waitMutexThread;

    /// Timer to periodically check whether m_steamMutex appeared
    QTimer m_steamMutexCheckTimer;
  public:
    RunningStateDetector(Piping* owner);
    ~RunningStateDetector();
  private:
    /// Set a new 'steam running' state in the owner.
    void SetSteamRunning(bool running);

    /// Thread to wait for m_steamMutex
    class WaitMutexThread : public QThread
    {
      RunningStateDetector* parent;
      HANDLE stopEvent;
    public:
      WaitMutexThread(RunningStateDetector* parent);
      ~WaitMutexThread();

      void TriggerStop();

      void run();
    };
    /// Function called by wait thread if m_steamMutex signalled
    void MutexWaitSuccessful();

    /// Try to open the mutex and, if successful, register wait callback
    bool TryOpenMutex();
    /// Stop waiting for the mutex
    Q_INVOKABLE void StopMutexWait();
    /// Start timer for periodic mutex check
    Q_INVOKABLE void StartMutexTimer();
    private slots:
    /// Called by the mutex check timer
    void checkTimerTimeout();
  };
} // namespace piping

#endif // __PIPING_RUNNINGSTATEDETECTOR_HPP__
