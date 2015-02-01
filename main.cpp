#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "piping/Piping.hpp"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QQmlApplicationEngine engine;
    QQmlContext* context = engine.rootContext();

    piping::Piping steamPiping;
    context->setContextProperty("piping", &steamPiping);

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
