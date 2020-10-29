#include "controller.h"
#include "datavizwindow.h"
#include "oscreceiver.h"
#include "applicationoptions.h"
#include <QtGui/QGuiApplication>
#include <QSharedPointer>
#include <QWidget>
#include <QApplication>
#include <QHBoxLayout>
#include <QUdpSocket>
#include <QMessageBox>
#include <QScreen>

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
    QApplication app(argc, argv);

    QApplication::setApplicationName(APPLICATION_NAME);
    QApplication::setApplicationVersion(APPLICATION_VERSION);

    // Parse command-line arguments.
    QCommandLineParser parser;
    parser.setApplicationDescription("OSC-controlled Dataviz for MPOP");
    parser.addHelpOption();
    parser.addVersionOption();

    // boolean options: (flags)
    const QCommandLineOption verboseOption({"V", "verbose"}, "Enable a verbose output."); // bool
    parser.addOption(verboseOption);

    const QCommandLineOption showCursorOption({"s", "show-cursor"}, "Show the mouse cursor."); // bool
    parser.addOption(showCursorOption);

    const QCommandLineOption showWindowFrameOption({"F", "show-window-frame"}, "Show the window frame."); // bool
    parser.addOption(showWindowFrameOption);

    // int options:
    const QCommandLineOption widthOption({"w", "width"}, "Window width", "width", "1920");
    parser.addOption(widthOption);

    const QCommandLineOption screenOption({"S", "screen"}, "Display index", "screen", "-1");
    parser.addOption(screenOption);

    const QCommandLineOption heightOption({"H", "height"}, "Window height", "height", "1080");
    parser.addOption(heightOption);

    const QCommandLineOption oscReceivePortOption({"p", "port"}, "OSC receive port number", "port", "31337");
    parser.addOption(oscReceivePortOption);

    const QCommandLineOption xWindowPositionOption({"x", "x-position"}, "Window X position", "x-position", "0");
    parser.addOption(xWindowPositionOption);

    const QCommandLineOption yWindowPositionOption({"y", "y-position"}, "Window Y position", "y-position", "0");
    parser.addOption(yWindowPositionOption);

    const QCommandLineOption alignRightOption({"r", "align-right"}, "Align the contents of the window to the right"); // bool
    parser.addOption(alignRightOption);

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

    // Store the options values in this struct:
    ApplicationOptions options;

    // bool options:
    options.verbose = parser.isSet(verboseOption);
    options.show_cursor = parser.isSet(showCursorOption);
    options.show_window_frame = parser.isSet(showWindowFrameOption);
    options.align_right = parser.isSet(alignRightOption);

    // int options:
    // FIXME: check the parsing of int command-line options.
    options.window_width = parser.value(widthOption).toInt();
    options.window_height = parser.value(heightOption).toInt();
    options.osc_receive_port = static_cast<quint16>(parser.value(oscReceivePortOption).toInt());
    options.window_x = parser.value(xWindowPositionOption).toInt();
    options.window_y = parser.value(yWindowPositionOption).toInt();
    options.screen = parser.value(screenOption).toInt();

    if (options.verbose) {
        //        for (int i = 0; i < argc; ++ i) {
        //            qDebug() << argv[i];
        //        }
        options.printAll();
    }

    // Create window
    // We use one (1) instance of mpop_dataviz for each window.
    // Make sure to use a different OSC port for each.

    QSharedPointer<DatavizWindow> window(new DatavizWindow());
    QSurfaceFormat format;
    format.setSamples(16);
    window->setFormat(format);

    static const int SPACING_WIDTH = 660;
    // The window container allows to embed our Qt OpenGL window into a QWidget.
    // Then, we put that in our windowLayout
    // and we put that windowLayout into the mainWindow - which is the actual window.
    QWidget *windowContainer = QWidget::createWindowContainer(window.data());
    windowContainer->setFixedSize(options.contents_width,
                                  options.window_height);
    windowContainer->setFocusPolicy(Qt::StrongFocus);
    // Create a layout and set margin
    QHBoxLayout *windowLayout = new QHBoxLayout;
    // Important! remove all the content margins
    windowLayout->setContentsMargins(0, 0, 0, 0);
    // Align inner window to the left side
    // windowLayout->setAlignment(options.align_right ? Qt::AlignRight : Qt::AlignLeft);

    // Add dataviz widget to layout
    if (options.align_right) {
        windowLayout->addSpacing(SPACING_WIDTH);
    }
    windowLayout->addWidget(windowContainer);

    if (! options.align_right) {
        windowLayout->addSpacing(SPACING_WIDTH);
    }

    // Create mainWindow and keep everything inside
    QWidget* mainWindow = new QWidget;
    mainWindow->setLayout(windowLayout);
    mainWindow->setFixedSize(options.window_width,
                             options.window_height);

    mainWindow->move(options.window_x, options.window_y);

    // Set background color palette
    QPalette palette;
    palette.setColor(QPalette::Background, Qt::black);
    mainWindow->setAutoFillBackground(true);
    mainWindow->setPalette(palette);

    if (options.screen != -1) {
        QList<QScreen*> screens;
        screens = qApp->screens();
        int numScreens = screens.size();
        qDebug() << "There are " << numScreens << " screens and we attempt to use the number " << options.screen;
        if (numScreens >= options.screen) {
            QScreen* screen = qApp->screens()[options.screen];
            mainWindow->windowHandle()->setScreen(screen);
            mainWindow->showFullScreen();
        } else {
            qDebug() << "Invalid screen index!";
        }
    } else {
        mainWindow->move(options.window_x, options.window_y);
    }

    if (options.show_cursor == false) {
        window->setCursor(Qt::BlankCursor);
    }
    static const int WINDOW_ID = 0;
    //window->setWindowId(WINDOW_ID); // Deprecated
    //window->setOffsetId(WINDOW_ID); // Deprecated
    qDebug() << "Window" << WINDOW_ID << "of size:" <<
                options.window_width << "x" << options.window_height <<
                "at position" << options.window_x << "," << options.window_y;
    //qDebug() << "Window ID: " << window->getWindowId();
    if (options.show_window_frame) {
        mainWindow->setWindowFlags(mainWindow->windowFlags() | Qt::Window);
    } else {
        mainWindow->setWindowFlags(mainWindow->windowFlags() | Qt::Window | Qt::FramelessWindowHint);
    }
    QObject::connect(window.data(), &DatavizWindow::closed, mainWindow, &QWidget::close);
    // Show main window
    mainWindow->show();
    // Take a little time to check port availability
    QUdpSocket *socket = new QUdpSocket(window.data());
    if (!socket->bind(options.osc_receive_port)) {
        qDebug() << "The OSC port" << options.osc_receive_port << "is already in use!";
        qDebug() << "Please make sure no other instance is running";

        QMessageBox::critical(mainWindow, QObject::tr("OSC Server Error"),
                              QObject::tr("The OSC port %1 is already in use!\n"
                                          "Please make sure no other instance is running").arg(options.osc_receive_port),
                              QMessageBox::Close);

        // Brutal way of leave program
        // Maybe mainWindow.close()
        return 1;
    } else {
        // Get out
        socket->disconnectFromHost();
        delete socket;
    }

    // Connect the window(s) to the OSC receiver, via a controller
    // that takes care of the logic of the application.
    OscReceiver oscReceiver(options.osc_receive_port);

    // XXX Deprecated: we used to have a vector of windows.
    // We still use it here.
    QVector<QSharedPointer<DatavizWindow>> windows;
    windows.append(window);
    Controller controller(&oscReceiver, windows);

    // Run the application.
    int ret = app.exec();

    windows.clear();
    qDebug() << "Exitting.";
    return ret;
}
