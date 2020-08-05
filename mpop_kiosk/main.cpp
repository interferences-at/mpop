#include <QGuiApplication>
#include <QQmlContext>
#include <QQmlApplicationEngine>
#include "rfidreader.h"
#include "oscreceiver.h"
#include "oscsender.h"
#include "screensaver.h"
#include "kioskconfig.h"
#include "languageswitcher.h"


/**
 * @brief Load the kiosk configuration from the environment variables.
 * @param config
 */
void load_kiosk_config_from_env_vars(KioskConfig& config) {
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();

    config.send_osc_dataviz_port = env.value("SEND_OSC_DATAVIZ_PORT", "14444").toUInt();
    config.send_osc_dataviz_host = env.value("SEND_OSC_DATAVIZ_HOST", "127.0.0.1");
    config.service_host = env.value("MPOP_SERVICE_HOST", "0.0.0.0");
    config.service_port_number = env.value("MPOP_SERVICE_PORT_NUMBER", "3333").toUInt();
    config.receive_osc_port = env.value("RECEIVE_OSC_PORT", "15555").toUInt();
    config.is_verbose = KioskConfig::toBoolean(env.value("CONFIG_IS_VERBOSE", "true"));
    config.is_fullscreen = KioskConfig::toBoolean(env.value("CONFIG_IS_FULLSCREEN", "false"));
    config.kiosk_mode = env.value("MPOP_KIOSK_MODE", "central");

    if (config.is_verbose) {
        qDebug() << "send_osc_dataviz_port:" << config.send_osc_dataviz_port;
        qDebug() << "send_osc_dataviz_host:" << config.send_osc_dataviz_host;
        qDebug() << "service_host:" << config.service_host;
        qDebug() << "service_port_number:" << config.service_port_number;
        qDebug() << "receive_osc_port:" << config.receive_osc_port;
        qDebug() << "is_verbose:" << config.is_verbose;
        qDebug() << "is_fullscreen:" << config.is_fullscreen;
        qDebug() << "kiosk_mode:" << config.kiosk_mode;
    }

    if (config.kiosk_mode == "entrance") {
        // ok
    } else if (config.kiosk_mode == "central") {
        // ok
    } else if (config.kiosk_mode == "exit") {
        // ok
    } else {
        QTextStream standardOutput(stdout);
        standardOutput << QObject::tr("Unknown kiosk mode: %s").arg(config.kiosk_mode) << endl;
        qDebug() << "Default to central mode.";
        config.kiosk_mode = "central";
    }
}


/**
 * @brief Main entry point for this software.
 * @param argc Count of command-line arguments.
 * @param argv Values for all command-line arguments.
 * @return 0 when it exits without error.
 */
int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    // Load the configuration option from environment variables:
    KioskConfig kioskConfig;
    load_kiosk_config_from_env_vars(kioskConfig);

    // Allow to override some configuration options with command line arguments:
    int argumentCount = QCoreApplication::arguments().size();
    QStringList argumentList = QCoreApplication::arguments();
    QTextStream standardOutput(stdout);

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

    // Print some useful info:
    standardOutput << QObject::tr("Send OSC to %1:%2").arg(
        kioskConfig.send_osc_dataviz_host).arg(
        kioskConfig.send_osc_dataviz_port) << endl;

    // Create the Screensaver QML type
    qmlRegisterType<Screensaver>("Screensaver", 1, 0, "Screensaver");

    QQmlApplicationEngine engine;

    // Instanciate important business logic elements:
    RFIDReader rfidReader;
    OscReceiver oscReceiver(kioskConfig.receive_osc_port);
    OscSender oscSender(kioskConfig.send_osc_dataviz_host, kioskConfig.send_osc_dataviz_port);
    LanguageSwitcher languageSwitcher;

    // Pass C++ objects to QML.
    engine.rootContext()->setContextProperty("rfidReader", &rfidReader);
    engine.rootContext()->setContextProperty("oscSender", &oscSender);
    engine.rootContext()->setContextProperty("oscReceiver", &oscReceiver);
    engine.rootContext()->setContextProperty("kioskConfig", &kioskConfig);
    engine.rootContext()->setContextProperty("languageSwitcher", &languageSwitcher);

    // Load main QML file
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
