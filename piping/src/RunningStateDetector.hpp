/*
  SteamLibrarian
  Copyright (c) 2015 Frank Richter

  This file is part of SteamLibrarian.

  SteamLibrarian is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  SteamLibrarian is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with SteamLibrarian.  If not, see <http://www.gnu.org/licenses/>.
*/

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
    /// Called by the mutex check timer
    void checkTimerTimeout();
    /// Called when application state changes
    void applicationStateChanged(Qt::ApplicationState state);
  };
} // namespace piping

#endif // __PIPING_RUNNINGSTATEDETECTOR_HPP__
