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
