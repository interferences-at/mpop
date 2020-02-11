#include <QGuiApplication>
#include <QQmlContext>
#include <QQmlApplicationEngine>
#include "rfidreader.h"

int main(int argc, char *argv[])
{
    // qputenv("QT_IM_MODULE", QByteArray("qtvirtualkeyboard"));

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    RFIDReader rfidReader;
    engine.rootContext()->setContextProperty("rfidReader", &rfidReader);

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
