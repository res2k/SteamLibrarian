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
    /// App to move
    App* m_app;
    /// Destination library
    Library* m_destLib;
    /// Thread used to do the actual work
    QPointer<QThread> m_thread;
    /// Error string
    QString m_errorString;
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

    /// Worker ran successfully
    void workerSucceeded();
    /// Worker ran, but action was cancelled by user
    void workerCancelled();
    /// Worker failed
    void workerFailed(const QString& message);
  };
} // namespace piping

#endif // __PIPING_APPMOVER_HPP__
