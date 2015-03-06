#include <QApplication>
#include <QQmlApplicationEngine>

#include "facade/RegisterTypes.hpp"
#include "facade/Start.hpp"
#include "piping/RegisterTypes.hpp"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    piping::RegisterTypes("SteamLibrarian", 1, 0);
    facade::RegisterTypes("SteamLibrarian", 1, 0);

    QQmlApplicationEngine engine;
    facade::Start(engine);

    return app.exec();
}
