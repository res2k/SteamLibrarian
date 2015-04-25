/*
  SteamLibrarian
  Copyright (c) 2015 Frank Richter

  This file is part of SteamLibrarian.

  SteamLibrarian is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  SteamLibrarian is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with SteamLibrarian.  If not, see <http://www.gnu.org/licenses/>.
*/

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
