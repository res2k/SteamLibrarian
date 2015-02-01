#include "piping/Piping.hpp"

#include <Windows.h>

#include "RunningStateDetector.hpp"

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
} // namespace piping
