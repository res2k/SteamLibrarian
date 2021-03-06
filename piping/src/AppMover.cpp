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

#include "piping/AppMover.hpp"

#include "piping/App.hpp"
#include "piping/Library.hpp"

#include <QDir>
#include <QStringBuilder>
#include <QStringList>

#include "AppMoverWorker.hpp"

namespace piping
{
  AppMover::AppMover(QList<QPair<QString, QString>> files, QObject* parent)
    : QObject(parent), m_files(files), m_app(nullptr), m_destLib(false), m_recurse(false)
  {
  }

  AppMover::AppMover(App* app, Library* destination, QObject* parent)
    : QObject(parent), m_app(app), m_destLib(destination), m_recurse(true)
  {
  }

  AppMover::~AppMover()
  {
    DiscardThread();
  }

  void AppMover::perform()
  {
    if (m_thread) return;

    qRegisterMetaType<QList<QPair<QString, QString>>>("QList<QPair<QString,QString> >");

    // The action will happen on another thread.
    m_thread = new QThread(this);
    m_thread->setObjectName(QStringLiteral("AppMover thread"));
    AppMoverWorker* worker = new AppMoverWorker;
    worker->moveToThread(m_thread);
    worker->setRecurse(m_recurse);
    connect(m_thread, &QThread::finished, worker, &QObject::deleteLater);
    connect(worker, &AppMoverWorker::undoData, this, &AppMover::workerUndoData);
    connect(worker, &AppMoverWorker::finished, this, &AppMover::workerSucceeded);
    connect(worker, &AppMoverWorker::cancelled, this, &AppMover::workerCancelled);
    connect(worker, &AppMoverWorker::failed, this, &AppMover::workerFailed);

    m_thread->start();

    if (m_files.isEmpty())
    {
      // Generate list of files to move
      QStringList appFiles(m_app->GetAppFiles());
      QString srcLibPath(m_app->library()->path() % QDir::separator() % QStringLiteral("steamapps"));
      QString destLibPath(m_destLib->path() % QDir::separator() % QStringLiteral("steamapps"));
      Q_FOREACH(const QString& file, appFiles)
      {
        QString srcPath = srcLibPath % QDir::separator() % file;
        QString destPath = destLibPath % QDir::separator() % file;
        QMetaObject::invokeMethod(worker, "Move", Q_ARG(QString, srcPath), Q_ARG(QString, destPath));
      }
    }
    else if (m_app)
    {
      typedef QPair<QString, QString> QString_pair;
      Q_FOREACH(const QString_pair& files, m_files)
      {
        QMetaObject::invokeMethod(worker, "Move", Q_ARG(QString, files.first), Q_ARG(QString, files.second));
      }
    }
    QMetaObject::invokeMethod(worker, "Perform");
  }

  const QString& AppMover::errorString() const
  {
    return m_errorString;
  }

  QObject* AppMover::getUndoMover()
  {
    if (m_undoData.empty()) return nullptr;
    return new AppMover(m_undoData);
  }

  void AppMover::SetErrorString(const QString& errorString)
  {
    m_errorString = errorString;
    emit errorStringChanged();
  }

  void AppMover::DiscardThread()
  {
    if (!m_thread) return;
    m_thread->quit();
    m_thread->wait();
    m_thread = nullptr;
  }

  void AppMover::workerUndoData(QList<QPair<QString, QString>> undoData)
  {
    m_undoData = undoData;
  }

  void AppMover::workerSucceeded()
  {
    emit finished(Success);
  }

  void AppMover::workerCancelled()
  {
    emit finished(UserCancelled);
  }

  void AppMover::workerFailed(const QString& message)
  {
    SetErrorString(message);
    emit finished(Failure);
  }

} // namespace piping
