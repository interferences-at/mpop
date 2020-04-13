#include "controller.h"
#include "datavizwindow.h"
#include "oscreceiver.h"
#include "applicationoptions.h"
#include <QtGui/QGuiApplication>
#include <QSharedPointer>


// Constants:
// TODO: add a command-line argument for how many windows to show.
static const int NUM_WINDOWS = 1;
// static const int NUM_WINDOWS = 4;
static const int DEFAULT_NUM_WINDOWS = 1;
static const int MAX_NUM_WINDOWS = 4;
static const int WINDOW_WIDTH = 1920;
static const int WINDOW_HEIGHT = 1080;
static const int OSC_RECEIVE_PORT = 31337;
static const bool HIDE_CURSOR = false;
static const QString APPLICATION_VERSION = "0.1.0-SNAPSHOT";
static const QString APPLICATION_NAME = "mpop_dataviz";


int main(int argc, char* argv[]) {
    QGuiApplication app(argc, argv);

    QCoreApplication::setApplicationName(APPLICATION_NAME);
    QCoreApplication::setApplicationVersion(APPLICATION_VERSION);

    QCommandLineParser parser;
    parser.setApplicationDescription("OSC-controlled Dataviz for MPOP");
    parser.addHelpOption();
    parser.addVersionOption();

    // boolean options: (flags)
    const QCommandLineOption verboseOption({"v", "verbose"}, "Enable a verbose output.", "verbose"); // bool
    parser.addOption(verboseOption);

    const QCommandLineOption hideCursorOption({"c", "hide-cursor"}, "Hide the mouse cursor.", "hide-cursor"); // bool
    parser.addOption(hideCursorOption);

    // int options:
    const QCommandLineOption widthOption({"w", "width"}, "Window width", "width", "1920");
    parser.addOption(widthOption);

    const QCommandLineOption heightOption({"H", "height"}, "Window height", "height", "1080");
    parser.addOption(heightOption);

    const QCommandLineOption numWindowsOption({"n", "windows"}, "Number of windows", "windows", "1");
    parser.addOption(numWindowsOption);

    const QCommandLineOption oscReceivePortOption({"p", "port"}, "OSC receive port number", "port", "31337");
    parser.addOption(oscReceivePortOption);

    const QCommandLineOption xWindowPositionOption({"x", "x-position"}, "Window X position", "x-position", "0");
    parser.addOption(widthOption);

    const QCommandLineOption yWindowPositionOption({"y", "y-position"}, "Window Y position", "y-position", "0");
    parser.addOption(heightOption);

    parser.process(app);
    if (! parser.parse(QCoreApplication::arguments())) {
        QString errorMessage = parser.errorText();
        qDebug() << errorMessage;
        parser.showHelp(1);
    }

    if (parser.isSet("help")) {
        parser.showHelp(0);
    }

    ApplicationOptions appOptions;

    appOptions.num_windows = DEFAULT_NUM_WINDOWS;

    // bool options:
    appOptions.verbose = parser.isSet(verboseOption);
    appOptions.hide_cursor = parser.isSet(hideCursorOption);

    // int options:
    // FIXME: check the parsing of int command-line options.
    appOptions.num_windows = parser.value(numWindowsOption).toInt();
    appOptions.window_width = parser.value(widthOption).toInt();
    appOptions.window_height = parser.value(heightOption).toInt();
    appOptions.osc_receive_port = static_cast<quint16>(parser.value(oscReceivePortOption).toInt());
    appOptions.window_height = parser.value(heightOption).toInt();


    QSurfaceFormat format;
    format.setSamples(16);
    QVector<QSharedPointer<DatavizWindow>> windows;

    for (int i = 0; i < appOptions.num_windows; i ++) {
        int x = (i % 2) * appOptions.window_width;
        int y = (i / 2) * appOptions.window_height;
         // TODO: use appOptions.window_x and window_y
        QPoint windowPosition(x, y);

        QSharedPointer<DatavizWindow> window(new DatavizWindow());
        if (appOptions.hide_cursor) {
            window->setCursor(Qt::BlankCursor);
        }
        window->setFormat(format);
        window->resize(appOptions.window_width, appOptions.window_height);
        window->setPosition(windowPosition);
        qDebug() << "Window" << i << "of size:" <<
            appOptions.window_width << "x" << appOptions.window_height <<
            "at position" << x << "," << y;
        // TODO: add frameless window option
        window->setFlags(Qt::Window | Qt::FramelessWindowHint);
        window->show();

        windows.append(window);
    }

    OscReceiver oscReceiver(appOptions.osc_receive_port);
    Controller controller(&oscReceiver, windows);

    int ret = app.exec();

    windows.clear();

    qDebug() << "Exitting regularly.";
    return ret;
}
