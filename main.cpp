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

    QQmlApplicationEngine engine;
    facade::Start(engine);

    return app.exec();
}
