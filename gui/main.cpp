#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QFontDatabase>
#include "serialcommunication.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QApplication app(argc, argv);

    if (QFontDatabase::addApplicationFont(":/font/Roboto-Regular.ttf") == -1) {
        qWarning() << "Failed to load Roboto-Regular.ttf";
    }

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));

    SerialCommunication* serial = new SerialCommunication();

    engine.rootContext()->setContextProperty("serial", serial);

    engine.rootContext()->setContextProperty("manualUrl", "https://rickp.github.io/MC5000_DevKit/");

    engine.load(url);

    return app.exec();
}
