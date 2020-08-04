#include <QGuiApplication>
#include <QQmlContext>
#include <QQmlApplicationEngine>
#include "rfidreader.h"
#include "oscreceiver.h"
#include "oscsender.h"
#include "screensaver.h"
#include "kioskconfig.h"


bool toBoolean(const QString& value) {
    bool ret = false;
    if (value.toLower() == "true" || value.toInt() == 1) {
        ret = true;
    }
    // I think we could also simply do:
    // return QVariant(value).toBool();
    return ret;
}


void load_kiosk_config_from_env_vars(KioskConfig& config) {
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();

    config.send_osc_dataviz_port = env.value("SEND_OSC_DATAVIZ_PORT", "14444").toUInt();
    config.send_osc_dataviz_host = env.value("SEND_OSC_DATAVIZ_HOST", "127.0.0.1");
    config.service_host = env.value("MPOP_SERVICE_HOST", "0.0.0.0");
    config.service_port_number = env.value("MPOP_SERVICE_PORT_NUMBER", "3333").toUInt();
    config.receive_osc_port = env.value("RECEIVE_OSC_PORT", "15555").toUInt();
    config.is_verbose = toBoolean(env.value("CONFIG_IS_VERBOSE", "true"));

    if (config.is_verbose) {
        qDebug() << "send_osc_dataviz_port:" << config.send_osc_dataviz_port;
        qDebug() << "send_osc_dataviz_host:" << config.send_osc_dataviz_host;
        qDebug() << "service_host:" << config.service_host;
        qDebug() << "service_port_number:" << config.service_port_number;
        qDebug() << "receive_osc_port:" << config.receive_osc_port;
        qDebug() << "is_verbose:" << config.is_verbose;
    }
}


int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    int argumentCount = QCoreApplication::arguments().size();
    QStringList argumentList = QCoreApplication::arguments();
    QTextStream standardOutput(stdout);

    KioskConfig kioskConfig;

    if (argumentCount > 1) {
        kioskConfig.send_osc_dataviz_host = argumentList.at(1);
    }
    if (argumentCount > 2) {
        kioskConfig.send_osc_dataviz_port = static_cast<quint16>(argumentList.at(2).toInt());
    }
    if (argumentCount > 3) {
        kioskConfig.receive_osc_port = static_cast<quint16>(argumentList.at(3).toInt());
    }
    if (argumentCount == 1) {
        standardOutput << QObject::tr("To specify an OSC send host and port: %1 <sendHost> <sendPort> <receivePort").arg(
                              argumentList.first()) << endl;
    }

    standardOutput << QObject::tr("Send OSC to %1:%2").arg(
        kioskConfig.send_osc_dataviz_host).arg(
        kioskConfig.send_osc_dataviz_port) << endl;

    // Create Screensaver QML type
    qmlRegisterType<Screensaver>("Screensaver", 1, 0, "Screensaver");

    QQmlApplicationEngine engine;

    RFIDReader rfidReader;
    OscReceiver oscReceiver(kioskConfig.receive_osc_port);
    OscSender oscSender(kioskConfig.send_osc_dataviz_host, kioskConfig.send_osc_dataviz_port);

    // Pass C++ objects to QML.
    engine.rootContext()->setContextProperty("rfidReader", &rfidReader);
    engine.rootContext()->setContextProperty("oscSender", &oscSender);
    engine.rootContext()->setContextProperty("oscReceiver", &oscReceiver);
    engine.rootContext()->setContextProperty("kioskConfig", &kioskConfig);

    // Load main QML file
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
