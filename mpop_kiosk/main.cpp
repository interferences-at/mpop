#include <QGuiApplication>
#include <QQmlContext>
#include <QQmlApplicationEngine>
#include <QFontDatabase>
#include <QApplication>
#include "rfidreader.h"
#include "oscsender.h"
#include "screensaver.h"
#include "kioskconfig.h"

// Constants:
static const QString APPLICATION_VERSION = "0.1.0";
static const QString APPLICATION_NAME = "mpop_kiosk";

/**
 * @brief Load the kiosk configuration from the environment variables.
 */
void load_kiosk_config_from_env_vars(KioskConfig& config) {
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();

    config.send_osc_dataviz_port = env.value("SEND_OSC_DATAVIZ_PORT", "31337").toUInt();
    config.send_osc_dataviz_host = env.value("SEND_OSC_DATAVIZ_HOST", "127.0.0.1");
    config.service_host = env.value("MPOP_SERVICE_HOST", "0.0.0.0");
    config.service_port_number = env.value("MPOP_SERVICE_PORT_NUMBER", "3333").toUInt();
    config.receive_osc_port = env.value("RECEIVE_OSC_PORT", "15555").toUInt();
    config.is_verbose = KioskConfig::toBoolean(env.value("CONFIG_IS_VERBOSE", "true"));
    config.is_fullscreen = KioskConfig::toBoolean(env.value("CONFIG_IS_FULLSCREEN", "false"));
    config.kiosk_mode = env.value("MPOP_KIOSK_MODE", "central");

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
 * @brief Load configuration options from command line options.
 */
void load_kiosk_config_from_command_line(KioskConfig& config) {
    // Parse command-line arguments.
    QCommandLineParser parser;
    parser.setApplicationDescription("Kiosk for MPOP");
    parser.addHelpOption();
    parser.addVersionOption();

    // boolean options: (flags)
    const QCommandLineOption verboseOption({"V", "verbose"}, "Enable a verbose output."); // bool
    parser.addOption(verboseOption);

    const QCommandLineOption showCursorOption({"c", "show-cursor"}, "Show the mouse cursor."); // bool
    parser.addOption(showCursorOption);

    const QCommandLineOption fullscreenOption({"f", "fullscreen"}, "Fullscreen window"); // bool
    parser.addOption(fullscreenOption);

    // const QCommandLineOption showWindowFrameOption({"F", "show-window-frame"}, "Show the window frame."); // bool
    // parser.addOption(showWindowFrameOption);

    // int options:
    const QCommandLineOption sendOscDatavizPort({"d", "dataviz-port"}, "Send OSC Dataviz port number", "send-osc-dataviz-port");
    parser.addOption(sendOscDatavizPort);

    const QCommandLineOption sendOscDatavizHost({"D", "dataviz-host"}, "Send OSC Dataviz host", "send-osc-dataviz-host");
    parser.addOption(sendOscDatavizHost);

    const QCommandLineOption serviceHostOption({"S", "service-host"}, "Service host", "service-host");
    parser.addOption(serviceHostOption);

    const QCommandLineOption servicePortOption({"s", "service-port"}, "Service port", "service-port");
    parser.addOption(servicePortOption);

    // String options:
    const QCommandLineOption kioskModeOption({"m", "kiosk-mode"}, "Kiosk mode (entry,central,exit)", "kiosk-mode");
    parser.addOption(kioskModeOption);

    // parser.process(app); // parse for --help and --version options.
    // Parse our custom options:
    if (! parser.parse(QApplication::arguments())) {
        QString errorMessage = parser.errorText();
        qDebug() << errorMessage;
        parser.showHelp(1);
    }

    if (parser.isSet("help")) {
        parser.showHelp(0);
    }
    if (parser.isSet(verboseOption)) {
        config.is_verbose = true;
    }
    if (parser.isSet(showCursorOption)) {
        config.show_cursor = true;
    }
    if (parser.isSet(sendOscDatavizPort)) {
        config.send_osc_dataviz_port = parser.value(sendOscDatavizPort).toInt();;
    }
    if (parser.isSet(sendOscDatavizHost)) {
        config.send_osc_dataviz_host = parser.value(sendOscDatavizHost);
    }
    if (parser.isSet(serviceHostOption)) {
        config.service_host = parser.value(serviceHostOption);
    }
    if (parser.isSet(servicePortOption)) {
        config.service_port_number = parser.value(servicePortOption).toInt();
    }
    if (parser.isSet(fullscreenOption)) {
        config.is_fullscreen = true;
    }
    if (parser.isSet(kioskModeOption)) {
        config.kiosk_mode = parser.value(kioskModeOption);
    }
}


/**
 * @brief Print the options, if verbose.
 */
void print_kiosk_options_if_verbose(const KioskConfig& config) {
    if (config.is_verbose) {
        qDebug() << "dataviz_port:" << config.send_osc_dataviz_port;
        qDebug() << "dataviz_host:" << config.send_osc_dataviz_host;
        qDebug() << "service_host:" << config.service_host;
        qDebug() << "service_port_number:" << config.service_port_number;
        qDebug() << "receive_osc_port:" << config.receive_osc_port;
        qDebug() << "is_verbose:" << config.is_verbose;
        qDebug() << "is_fullscreen:" << config.is_fullscreen;
        qDebug() << "kiosk_mode:" << config.kiosk_mode;
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
    QApplication::setApplicationName(APPLICATION_NAME);
    QApplication::setApplicationVersion(APPLICATION_VERSION);
    QGuiApplication app(argc, argv);

    // Load the configuration option from environment variables and/or command-line options:
    KioskConfig kioskConfig;
    load_kiosk_config_from_env_vars(kioskConfig); // Load env vars first
    load_kiosk_config_from_command_line(kioskConfig); // Override with command-line options, if set
    print_kiosk_options_if_verbose(kioskConfig);

    // Load fonts
    QDir fontDir("static/fonts");

    // Check if folder exists
    if (!fontDir.exists()) {
        qDebug() << "----------\nERROR :: Fonts folder doesn't exist.\nMake sure you've included them appropriately in the deployment folder.\n----------";
    }

    // Add fonts to application database
    for (auto file : fontDir.entryList(QDir::Files)) {
        if (QFontDatabase::addApplicationFont("static/fonts/" + file) == -1)
            qDebug() << "Failed to load font:" << file;
    }

    QTextStream standardOutput(stdout);

    // Print some useful info:
    standardOutput << QObject::tr("Send OSC to %1:%2").arg(
        kioskConfig.send_osc_dataviz_host).arg(
        kioskConfig.send_osc_dataviz_port) << endl;

    // Create the Screensaver QML type
    qmlRegisterType<Screensaver>("Screensaver", 1, 0, "Screensaver");
    // Create a QML type for barchart answers
    qmlRegisterType<AnswersView>("AnswersView", 1, 0, "AnswersView");

    QQmlApplicationEngine engine;

    // Instanciate important business logic elements:
    RFIDReader rfidReader;
    OscSender oscSender(kioskConfig.send_osc_dataviz_host, kioskConfig.send_osc_dataviz_port);

    // Pass C++ objects to QML.
    engine.rootContext()->setContextProperty("rfidReader", &rfidReader);
    engine.rootContext()->setContextProperty("oscSender", &oscSender);
    engine.rootContext()->setContextProperty("kioskConfig", &kioskConfig);

    // Load main QML file
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty()) {
        return -1;
    }

    return app.exec();
}
