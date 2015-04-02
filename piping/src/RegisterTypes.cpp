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
    qmlRegisterType<App>();
    qmlRegisterType<Libraries>();
    qmlRegisterType<Library>();
    qmlRegisterUncreatableType<AppMover>(uri, versionMajor, versionMinor, "AppMover",
      QStringLiteral("For access from App only"));
  }
} // namespace piping
