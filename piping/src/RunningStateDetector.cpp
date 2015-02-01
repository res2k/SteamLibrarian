#include "RunningStateDetector.hpp"

namespace piping
{
  Piping::RunningStateDetector::RunningStateDetector(Piping* owner)
	  : QObject(owner), m_steamMutex(NULL), m_waitMutexThread(nullptr)
  {
    m_steamMutexCheckTimer.setTimerType(Qt::VeryCoarseTimer);
    m_steamMutexCheckTimer.setInterval(2000); // TODO: Might want different interval in background vs foreground
    connect(&m_steamMutexCheckTimer, &QTimer::timeout, this, &RunningStateDetector::checkTimerTimeout);

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
