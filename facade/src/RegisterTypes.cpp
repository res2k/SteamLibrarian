#include "facade/RegisterTypes.hpp"

#include <QtQml>

#include "LibrariesModel.hpp"
#include "sortfilterproxymodel.h"

namespace facade
{
  void RegisterTypes(const char * uri, int versionMajor, int versionMinor)
  {
    qmlRegisterType<LibrariesModel>(uri, versionMajor, versionMinor, "LibrariesModel");
    qmlRegisterType<SortFilterProxyModel>(uri, versionMajor, versionMinor, "SortFilterProxyModel");
  }
} // namespace facade
