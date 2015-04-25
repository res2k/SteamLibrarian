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
 * Worker object performing AppMover actions
 */
#ifndef __PIPING_APPMOVERWORKER_HPP__
#define __PIPING_APPMOVERWORKER_HPP__

#include <QObject>

#include <Windows.h>

namespace piping
{
  class AppMoverWorker : public QObject
  {
    Q_OBJECT
  public:
    typedef QPair<QString, QString> move_files_pair;
  private:
    /// Whether to use recursion
    bool m_recurse;
    /// List of files to move
    QList<move_files_pair> m_moveFiles;
  public:
    AppMoverWorker(QObject* parent = nullptr);

    /// Set whether to use recursion.
    void setRecurse(bool recurse);
    /// Move a file or directory
    Q_INVOKABLE void Move(const QString& sourcePath, const QString& destination);

    /// Perform actions
    Q_INVOKABLE void Perform();

    /// Get undo list.
    QList<move_files_pair> GetUndoList() const;
  private:
    void SignalResult(HRESULT result);
  signals:
    /// Provides undo data.
    void undoData(QList<QPair<QString, QString>> undoData);
    /// Finished successfully.
    void finished();
    /// User cancelled.
    void cancelled();
    /// A problem occured.
    void failed(const QString& message);
  };
} // namespace piping

#endif // __PIPING_APPMOVERWORKER_HPP__
