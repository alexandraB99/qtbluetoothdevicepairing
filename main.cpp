#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtQml>

#include "bluetooth.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    Bluetooth bluetooth;
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);

    bluetooth.startScanning();

    engine.rootContext()->setContextProperty("Bluetooth", &bluetooth);
    engine.load(url);

    return app.exec();
}
