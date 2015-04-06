#include "facade/RegisterTypes.hpp"

#include <QtQml>

#include "AppsModel.hpp"
#include "LibrariesModel.hpp"
#include "sortfilterproxymodel.h"

namespace facade
{
  void RegisterTypes(const char * uri, int versionMajor, int versionMinor)
  {
    qmlRegisterType<AppsModel>(uri, versionMajor, versionMinor, "AppsModel");
    qmlRegisterType<LibrariesModel>(uri, versionMajor, versionMinor, "LibrariesModel");
    qmlRegisterType<SortFilterProxyModel>(uri, versionMajor, versionMinor, "SortFilterProxyModel");
  }
} // namespace facade
