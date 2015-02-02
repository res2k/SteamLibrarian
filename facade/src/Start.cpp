#include "facade/Start.hpp"

#include <QQmlApplicationEngine>

static inline void InitFacadeResources()
{
  Q_INIT_RESOURCE(facade);
}

namespace facade
{
  /// Start the UI.
  void Start(QQmlApplicationEngine& engine)
  {
    InitFacadeResources();
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
  }
} // namespace facade
