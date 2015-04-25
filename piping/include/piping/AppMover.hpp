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
 * Object to encapsulate moving an app to another library
 */
#ifndef __PIPING_APPMOVER_HPP__
#define __PIPING_APPMOVER_HPP__

#include <QObject>
#include <QPointer>
#include <QThread>

namespace piping
{
  class App;
  class Library;

  class AppMover : public QObject
  {
    Q_OBJECT
  private:
    /// Files to move
    QList<QPair<QString, QString>> m_files;
    /// App to move
    App* m_app;
    /// Destination library
    Library* m_destLib;
    /// Whether to enable recursion in the worker
    bool m_recurse;
    /// Thread used to do the actual work
    QPointer<QThread> m_thread;
    /// Error string
    QString m_errorString;
    /// Undo data from worker
    QList<QPair<QString, QString>> m_undoData;

    AppMover(QList<QPair<QString, QString>> files, QObject* parent = nullptr);
  public:
    AppMover(App* app, Library* destination, QObject* parent = nullptr);
    ~AppMover();

    /// Perform the moving action.
    Q_INVOKABLE void perform();
    /// Get last action error string
    const QString& errorString() const;
    /// Get a mover that (tries to) undo the action
    Q_INVOKABLE QObject* getUndoMover();

    Q_PROPERTY(QString errorString READ errorString NOTIFY errorStringChanged)

    /// Action result
    enum Result
    {
      /// Success
      Success,
      /// User cancelled
      UserCancelled,
      /// Other error
      Failure
    };
    Q_ENUMS(Result)
  signals:
    /// Action ended
    void finished(Result result);
    /// Error string changed
    void errorStringChanged();
  private:
    void SetErrorString(const QString& errorString);
    /// Discard the thread
    void DiscardThread();

    /// Undo data from worker
    void workerUndoData(QList<QPair<QString, QString>> undoData);
    /// Worker ran successfully
    void workerSucceeded();
    /// Worker ran, but action was cancelled by user
    void workerCancelled();
    /// Worker failed
    void workerFailed(const QString& message);
  };
} // namespace piping

#endif // __PIPING_APPMOVER_HPP__
