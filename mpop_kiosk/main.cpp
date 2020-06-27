#include <QGuiApplication>
#include <QQmlContext>
#include <QQmlApplicationEngine>
#include "rfidreader.h"
#include "oscreceiver.h"
#include "oscsender.h"

int main(int argc, char *argv[])
{
    // qputenv("QT_IM_MODULE", QByteArray("qtvirtualkeyboard"));

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    int argumentCount = QCoreApplication::arguments().size();
    QStringList argumentList = QCoreApplication::arguments();
    QTextStream standardOutput(stdout);

    QString sendOscHost = "127.0.0.1";
    quint16 sendOscPort = 14444;
    // TODO: remove OSC receiver. We don't use it.
    quint16 receiveOscPort = 14444;

    if (argumentCount > 1) {
        sendOscHost = argumentList.at(1);
    }
    if (argumentCount > 2) {
        sendOscPort = static_cast<quint16>(argumentList.at(2).toInt());
    }
    if (argumentCount > 3) {
        receiveOscPort = static_cast<quint16>(argumentList.at(3).toInt());
    }
    if (argumentCount == 1) {
        standardOutput << QObject::tr("To specify an OSC send host and port: %1 <sendHost> <sendPort> <receivePort").arg(
                              argumentList.first()) << endl;
    }

    standardOutput << QObject::tr("Receive OSC on port %1").arg(receiveOscPort) << endl;
    standardOutput << QObject::tr("Send OSC to %1:%2").arg(sendOscHost).arg(sendOscPort) << endl;

    QQmlApplicationEngine engine;

    RFIDReader rfidReader;
    OscReceiver oscReceiver(receiveOscPort);
    OscSender oscSender(sendOscHost, sendOscPort);

    // Pass C++ objects to QML.
    engine.rootContext()->setContextProperty("rfidReader", &rfidReader);
    engine.rootContext()->setContextProperty("oscSender", &oscSender);
    engine.rootContext()->setContextProperty("oscReceiver", &oscReceiver);
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
