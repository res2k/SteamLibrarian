#include "facade/RegisterTypes.hpp"

#include <QtQml>

#include "sortfilterproxymodel.h"

namespace facade
{
        void RegisterTypes(const char * uri, int versionMajor, int versionMinor)
	{
	  qmlRegisterType<SortFilterProxyModel>(uri, versionMajor, versionMinor, "SortFilterProxyModel");
	}
} // namespace facade
