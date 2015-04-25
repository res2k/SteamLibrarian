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

#include "piping/Piping.hpp"

#include "piping/Libraries.hpp"

#include <QtConcurrent>
#include <QDir>
#include <QFutureWatcher>
#include <QProcess>
#include <QStringBuilder>
#include <QStringList>

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
    m_libraries = new Libraries(m_steamInstallPath, this);
  }

  Piping::~Piping()
  {}

  bool Piping::steamRunning() const
  {
    return m_steamRunning;
  }

  bool Piping::actionPending() const
  {
    return !m_pendingActionActor.isNull();
  }

  void Piping::steamStart()
  {
    QFutureWatcher<void>* startWatcher(new QFutureWatcher<void>);
    connect(startWatcher, &QFutureWatcher<void>::finished, startWatcher, &QFutureWatcher<void>::deleteLater);
    SetPendingActionActor(startWatcher);
    // Need to start detached b/c Steam will be, well, running
    startWatcher->setFuture(QtConcurrent::run(&QProcess::startDetached, SteamExecutablePath(), QStringList()));
  }

  void Piping::steamShutdown()
  {
    LaunchSteamProcess(QStringList(QStringLiteral("-shutdown")));
  }

  Libraries* Piping::libraries() const
  {
    return m_libraries;
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

  void Piping::SetPendingActionActor(QObject* actor)
  {
    m_pendingActionActor = actor;
    connect(actor, &QObject::destroyed, this, &Piping::actionActorDestroyed);
    emit actionPendingChanged(actionPending());
  }

  QString Piping::SteamExecutablePath() const
  {
    return m_steamInstallPath % QDir::separator() % QStringLiteral("steam.exe");
  }

  void Piping::LaunchSteamProcess(const QStringList& arguments)
  {
    if (m_pendingActionActor) return;

    QProcess* steamProcess = new QProcess;
    steamProcess->setProgram(SteamExecutablePath());
    steamProcess->setArguments(arguments);
    // Destroy process object as soon as it somehow finished
    connect(steamProcess, (void(QProcess::*)(int))&QProcess::finished, steamProcess, &QProcess::deleteLater);
    connect(steamProcess, (void(QProcess::*)(QProcess::ProcessError))&QProcess::error, steamProcess, &QProcess::deleteLater);
    SetPendingActionActor(steamProcess);
    steamProcess->start();

    // TODO: Handle errors...
  }

  void Piping::actionActorDestroyed()
  {
    emit actionPendingChanged(actionPending());
  }
} // namespace piping
