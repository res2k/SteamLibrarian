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
  private:
    typedef QPair<QString, QString> move_files_pair;
    /// List of files to move
    QList<move_files_pair> m_moveFiles;
  public:
    AppMoverWorker(QObject* parent = nullptr);

    /// Move a file or directory
    Q_INVOKABLE void Move(const QString& sourcePath, const QString& destination);

    /// Perform actions
    Q_INVOKABLE void Perform();
  private:
    void SignalResult(HRESULT result);
  signals:
    /// Finished successfully.
    void finished();
    /// User cancelled.
    void cancelled();
    /// A problem occured.
    void failed(const QString& message);
  };
} // namespace piping

#endif // __PIPING_APPMOVERWORKER_HPP__
