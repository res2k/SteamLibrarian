#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "piping/RegisterSingleton.hpp"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    piping::RegisterSingleton("SteamLibrarian", 1, 0);

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
