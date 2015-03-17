#include "piping/AppMover.hpp"

namespace piping
{
  AppMover::AppMover(App* app, Library* destination, QObject* parent)
    : QObject(parent)
  {
    m_errorString = QStringLiteral("not implemented");
  }

  void AppMover::perform()
  {
    emit finished(Failure);
  }

  const QString& AppMover::errorString() const
  {
    return m_errorString;
  }

  QObject* AppMover::getUndoMover()
  {
    return nullptr;
  }

  void AppMover::setErrorString(const QString& errorString)
  {
    m_errorString = errorString;
    emit errorStringChanged();
  }

} // namespace piping
