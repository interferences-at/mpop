#include "controller.h"
#include "datavizwindow.h"
#include "oscreceiver.h"
#include "applicationoptions.h"
#include <QtGui/QGuiApplication>
#include <QSharedPointer>


// Constants:
static const QString APPLICATION_VERSION = "0.1.0-SNAPSHOT";
static const QString APPLICATION_NAME = "mpop_dataviz";


/**
 * @brief Main entry point of this application.
 *
 * To specify the command-line arguments:
 * Go in the "Project" part on the left of QtCreator and then in the "Run Settings" tab.
 * There is a Arguments line edit where you can put all you need to pass to your app when launching it.
 * Example arguments:
 *
 * --verbose --show-window-frame --port 31337
 *
 * @param argc Number of command-line arguments.
 * @param argv List of command-line arguments.
 * @return 0 if there is no error, 1 if there is one.
 */
int main(int argc, char* argv[]) {
    QGuiApplication app(argc, argv);

    QCoreApplication::setApplicationName(APPLICATION_NAME);
    QCoreApplication::setApplicationVersion(APPLICATION_VERSION);

    // Parse command-line arguments.
    QCommandLineParser parser;
    parser.setApplicationDescription("OSC-controlled Dataviz for MPOP");
    parser.addHelpOption();
    parser.addVersionOption();

    // boolean options: (flags)
    const QCommandLineOption verboseOption({"V", "verbose"}, "Enable a verbose output."); // bool
    parser.addOption(verboseOption);

    const QCommandLineOption hideCursorOption({"c", "hide-cursor"}, "Hide the mouse cursor."); // bool
    parser.addOption(hideCursorOption);

    const QCommandLineOption showWindowFrameOption({"F", "show-window-frame"}, "Show the window frame."); // bool
    parser.addOption(showWindowFrameOption);

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
    parser.addOption(xWindowPositionOption);

    const QCommandLineOption yWindowPositionOption({"y", "y-position"}, "Window Y position", "y-position", "0");
    parser.addOption(yWindowPositionOption);

    const QCommandLineOption windowIdOffsetOption({"i", "window-id-offset"}, "Window ID offset", "windowIdOffsetOption","0");
    parser.addOption(windowIdOffsetOption);

    // parser.process(app); // parse for --help and --version options.
    // Parse our custom options:
    if (! parser.parse(QCoreApplication::arguments())) {
        QString errorMessage = parser.errorText();
        qDebug() << errorMessage;
        parser.showHelp(1);
    }

    if (parser.isSet("help")) {
        parser.showHelp(0);
    }

    // Store the options values in this struct:
    ApplicationOptions options;

    // bool options:
    options.verbose = parser.isSet(verboseOption);
    options.hide_cursor = parser.isSet(hideCursorOption);
    options.show_window_frame = parser.isSet(showWindowFrameOption);


    // int options:
    // FIXME: check the parsing of int command-line options.
    options.num_windows = parser.value(numWindowsOption).toInt();
    options.window_width = parser.value(widthOption).toInt();
    options.window_height = parser.value(heightOption).toInt();
    options.osc_receive_port = static_cast<quint16>(parser.value(oscReceivePortOption).toInt());
    options.window_height = parser.value(heightOption).toInt();
    options.window_offset_id = parser.value(windowIdOffsetOption).toUInt();

    if (options.verbose) {
//        for (int i = 0; i < argc; ++ i) {
//            qDebug() << argv[i];
//        }
        options.printAll();
    }


    // Create window(s)
    // Note: it's possible to either show a single window (and be able to specify its (x,y) position)
    // or to show more than one window.
    // If you show more than one window, their position will be computed below.
    // Note that all the windows currently are running in the same thread, so it sounds like a better
    // idea to launch four instance of this application, rather than one instance with four windows.
    // Simply use a different OSC receive port for each of them.

    QVector<QSharedPointer<DatavizWindow>> windows;

    for (int i = 0; i < options.num_windows; i ++) {
        int x = options.window_x;
        int y = options.window_y;

        if (options.num_windows > 1) {
            x = (i % 2) * options.window_width;
            y = (i / 2) * options.window_height;
        }
        QSharedPointer<DatavizWindow> window(new DatavizWindow());

        windows.append(window);
        if (options.hide_cursor) {
            window->setCursor(Qt::BlankCursor);
        }
        QSurfaceFormat format;
        format.setSamples(16);
        window->setFormat(format);
        window->resize(options.window_width, options.window_height);
        QPoint windowPosition(x, y);
        window->setPosition(windowPosition);
        window->setWindowId(i); // The index use to be the ID
        window->setOffsetId(options.window_offset_id); // Set window ID offset
        qDebug() << "Window" << i << "of size:" <<
            options.window_width << "x" << options.window_height <<
            "at position" << x << "," << y;
        qDebug() << "Window ID: " << window->getWindowId() << endl;
        if (options.show_window_frame) {
            window->setFlags(Qt::Window);
        } else {
            window->setFlags(Qt::Window | Qt::FramelessWindowHint);
        }
        window->show();
    }

    // Connect the window(s) to the OSC receiver, via a controller
    // that takes care of the logic of the application.
    OscReceiver oscReceiver(options.osc_receive_port);
    Controller controller(&oscReceiver, windows);

    // Run the application.
    int ret = app.exec();

    // Once done, clear the memory and exit.
    windows.clear();
    qDebug() << "Exitting.";
    return ret;
}
