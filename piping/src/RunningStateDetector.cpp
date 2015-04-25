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

#include "RunningStateDetector.hpp"

#include <QApplication>

namespace piping
{
  static const int GetCheckTimerInterval(Qt::ApplicationState appState)
  {
    return appState == Qt::ApplicationActive ? 500 : 3000;
  }

  Piping::RunningStateDetector::RunningStateDetector(Piping* owner)
	  : QObject(owner), m_steamMutex(NULL), m_waitMutexThread(nullptr)
  {
    m_steamMutexCheckTimer.setTimerType(Qt::VeryCoarseTimer);
    m_steamMutexCheckTimer.setInterval(GetCheckTimerInterval(QApplication::applicationState()));
    connect(&m_steamMutexCheckTimer, &QTimer::timeout, this, &RunningStateDetector::checkTimerTimeout);
    connect(qApp, &QApplication::applicationStateChanged, this, &RunningStateDetector::applicationStateChanged);

    if (!TryOpenMutex())
      StartMutexTimer();
  }

  Piping::RunningStateDetector::~RunningStateDetector()
  {
    StopMutexWait();
  }

  void Piping::RunningStateDetector::SetSteamRunning(bool running)
  {
    QMetaObject::invokeMethod(parent(), "SetSteamRunning", Q_ARG(bool, running));
  }

  void Piping::RunningStateDetector::MutexWaitSuccessful()
  {
    CloseHandle(m_steamMutex);
    m_steamMutex = NULL;
    QMetaObject::invokeMethod(this, "StopMutexWait");
    SetSteamRunning(false);
    QMetaObject::invokeMethod(this, "StartMutexTimer");
  }

  bool Piping::RunningStateDetector::TryOpenMutex()
  {
    m_steamMutex = OpenMutexW(SYNCHRONIZE, false, L"Global\\SteamInstanceGlobal");
    if (m_steamMutex != NULL)
    {
      SetSteamRunning(true);
      m_waitMutexThread = new WaitMutexThread(this);
      m_waitMutexThread->start();
      return true;
    }
    return false;
  }

  void Piping::RunningStateDetector::StopMutexWait()
  {
    if (m_waitMutexThread)
    {
      m_waitMutexThread->TriggerStop();
      m_waitMutexThread->wait();
      m_waitMutexThread->deleteLater();
      m_waitMutexThread = 0;
    }
    if (m_steamMutex != NULL)
    {
      CloseHandle(m_steamMutex);
      m_steamMutex = NULL;
    }
  }

  void Piping::RunningStateDetector::StartMutexTimer()
  {
    m_steamMutexCheckTimer.start();
  }

  void Piping::RunningStateDetector::checkTimerTimeout()
  {
    if (TryOpenMutex())
    {
      m_steamMutexCheckTimer.stop();
    }
  }

  void Piping::RunningStateDetector::applicationStateChanged(Qt::ApplicationState state)
  {
    m_steamMutexCheckTimer.setInterval(GetCheckTimerInterval(state));
  }

  //--------------------------------------------------------------------------

  Piping::RunningStateDetector::WaitMutexThread::WaitMutexThread(RunningStateDetector* parent)
      : parent(parent)
  {
    stopEvent = CreateEvent(nullptr, true, false, nullptr);
  }

  Piping::RunningStateDetector::WaitMutexThread::~WaitMutexThread()
  {
    CloseHandle(stopEvent);
  }

  void Piping::RunningStateDetector::WaitMutexThread::TriggerStop()
  {
    SetEvent(stopEvent);
  }

  void Piping::RunningStateDetector::WaitMutexThread::run()
  {
    HANDLE steamMutex = parent->m_steamMutex;
    HANDLE handles[2] = { steamMutex, stopEvent };
    DWORD result = WaitForMultipleObjects(sizeof(handles) / sizeof(handles[0]), handles, false, INFINITE);
    switch (result)
    {
    case WAIT_OBJECT_0:
      {
	// Mutex signalled -> Steam exited
	ReleaseMutex(steamMutex);
	parent->MutexWaitSuccessful();
      }
      break;
    case WAIT_OBJECT_0 + 1:
      // Stop request
      break;
    default:
      break;
    }
  }
} // namespace piping
