#include "facade/RegisterTypes.hpp"

#include <QtQml>

#include "AppsModel.hpp"
#include "LibrariesModel.hpp"
#include "sortfilterproxymodel.h"
#include "UI.hpp"

namespace facade
{
  static QObject* CreateUI(QQmlEngine *engine, QJSEngine *scriptEngine)
  {
    return new UI;
  }

  void RegisterTypes(const char * uri, int versionMajor, int versionMinor)
  {
    qmlRegisterSingletonType<UI>(uri, versionMajor, versionMinor, "UI", &CreateUI);
    qmlRegisterType<AppsModel>(uri, versionMajor, versionMinor, "AppsModel");
    qmlRegisterType<LibrariesModel>(uri, versionMajor, versionMinor, "LibrariesModel");
    qmlRegisterType<SortFilterProxyModel>(uri, versionMajor, versionMinor, "SortFilterProxyModel");
  }
} // namespace facade
