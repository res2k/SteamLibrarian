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
