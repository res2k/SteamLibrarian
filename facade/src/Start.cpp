#include "facade/Start.hpp"

#include <QQmlApplicationEngine>

#include "StandardImageProvider.hpp"

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
    engine.addImageProvider(QStringLiteral("standard"), new StandardImageProvider);
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
  }
} // namespace facade
