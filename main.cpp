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

#include <QtQml>
#include <QApplication>
#include <QFont>
#include <QQmlApplicationEngine>
#include <QScreen>

#include "facade/RegisterTypes.hpp"
#include "facade/Start.hpp"
#include "piping/RegisterTypes.hpp"

#include <Windows.h>
#include <Uxtheme.h>
#include <vssym32.h>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    LOGFONTW lfw;
    if (SUCCEEDED(GetThemeSysFont(NULL, TMT_MSGBOXFONT, &lfw)))
    {
      QFont font;
      font.setFamily(QString::fromWCharArray(lfw.lfFaceName));
      font.setPointSize((qAbs(lfw.lfHeight) * 72.0) / app.screens()[0]->logicalDotsPerInchY());
      app.setFont(font);
    }

    piping::RegisterTypes("SteamLibrarian", 1, 0);
    facade::RegisterTypes("SteamLibrarian", 1, 0);
    qmlProtectModule("SteamLibrarian", 1);

    QQmlApplicationEngine engine;
    facade::Start(engine);

    return app.exec();
}
