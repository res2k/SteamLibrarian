#include "piping/RegisterTypes.hpp"

#include <QtQml>

#include "piping/App.hpp"
#include "piping/AppMover.hpp"
#include "piping/Libraries.hpp"
#include "piping/Library.hpp"
#include "piping/Piping.hpp"

namespace piping
{
  static QObject* CreatePiping(QQmlEngine *engine, QJSEngine *scriptEngine)
  {
    return new Piping;
  }

  void RegisterTypes(const char * uri, int versionMajor, int versionMinor)
  {
    qmlRegisterSingletonType<Piping>(uri, versionMajor, versionMinor, "Piping", &CreatePiping);
    qmlRegisterUncreatableType<App>(uri, versionMajor, versionMinor, "App",
      QStringLiteral("For access from Library only"));
    qmlRegisterUncreatableType<Libraries>(uri, versionMajor, versionMinor, "Libraries",
      QStringLiteral("For access from 'Piping' only"));
    qmlRegisterUncreatableType<Library>(uri, versionMajor, versionMinor, "Library",
      QStringLiteral("For access from Libraries only"));
    qmlRegisterUncreatableType<AppMover>(uri, versionMajor, versionMinor, "AppMover",
      QStringLiteral("For access from App only"));
  }
} // namespace piping
