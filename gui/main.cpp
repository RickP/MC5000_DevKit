#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QFontDatabase>
#include "serialcommunication.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    if (QFontDatabase::addApplicationFont(":/font/Roboto-Regular.ttf") == -1) {
        qWarning() << "Failed to load Roboto-Regular.ttf";
    }

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);

    SerialCommunication* serial = new SerialCommunication();

    engine.rootContext()->setContextProperty("serial", serial);

    engine.load(url);

    return app.exec();
}
