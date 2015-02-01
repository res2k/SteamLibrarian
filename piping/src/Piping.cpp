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
    void MutexWaitSuccessful ();

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

#include "Piping.moc"

namespace piping
{
  Piping::Piping(QObject* parent) : QObject(parent), m_steamRunning(false), m_runningStateDetector(nullptr)
  {
    FetchInstallPath();
    if (!m_steamInstallPath.isEmpty())
    {
      m_runningStateDetector = new RunningStateDetector(this);
    }
  }

  Piping::~Piping()
  {}

  bool Piping::steamRunning() const
  {
    return m_steamRunning;
  }

  namespace
  {
    /// Helper class for automatic reg key cleanup
    class AutoRegKey
    {
      HKEY key;

      void reset(HKEY new_key = 0)
      {
	if (key) RegCloseKey(key);
	key = new_key;
      }
    public:
      AutoRegKey() : key(0) {}
      ~AutoRegKey() { if (key) RegCloseKey(key); }

      operator HKEY() const { return key; }
      HKEY* operator&()
      {
	reset();
	return &key;
      }
    };
  } // namespace

  void Piping::FetchInstallPath()
  {
    AutoRegKey steam_key;
    if (RegOpenKeyExW(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Valve\\Steam", 0, KEY_READ | KEY_WOW64_32KEY, &steam_key)
        == ERROR_SUCCESS)
    {
      DWORD type(0);
      DWORD cbData(0);
      if ((RegGetValueW(steam_key, nullptr, L"InstallPath", RRF_RT_REG_SZ, &type, nullptr, &cbData)
          == ERROR_SUCCESS) && (type == REG_SZ))
      {
	// Resize QString to the buffer size we need
	m_steamInstallPath.resize((cbData / sizeof(wchar_t)));
	// Fetch data
	if ((RegGetValueW(steam_key, nullptr, L"InstallPath", RRF_RT_REG_SZ, &type, m_steamInstallPath.data(), &cbData)
            != ERROR_SUCCESS) || (type != REG_SZ))
	{
	  m_steamInstallPath.clear();
	  return;
	}
	// Get rid off any excessive null terminators
	m_steamInstallPath.resize(wcslen(reinterpret_cast<const wchar_t*> (m_steamInstallPath.constData())));
      }
    }
  }

    void Piping::SetSteamRunning(bool running)
    {
      m_steamRunning = running;
      emit steamRunningChanged(m_steamRunning);
    }

    //--------------------------------------------------------------------------

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
      case WAIT_OBJECT_0+1:
	// Stop request
	break;
      default:
	break;
      }
    }
} // namespace piping
