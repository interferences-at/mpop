#include "controller.h"
#include "datavizwindow.h"
#include "oscreceiver.h"
#include <QtGui/QGuiApplication>


// Constants:
// TODO: add a command-line argument for how many windows to show.
static const int NUM_WINDOWS = 1;
// static const int NUM_WINDOWS = 4;
static const int WINDOW_WIDTH = 1920;
static const int WINDOW_HEIGHT = 1080;
static const int OSC_RECEIVE_PORT = 31337;
static const bool HIDE_CURSOR = false;


int main(int argc, char* argv[]) {
    QGuiApplication app(argc, argv);
    QSurfaceFormat format;
    format.setSamples(16);
    QVector<DatavizWindow*> windows; // TODO use shared pointers

    for (int i = 0; i < NUM_WINDOWS; i ++) {
        int x = (i % 2) * WINDOW_WIDTH;
        int y = (i / 2) * WINDOW_HEIGHT;
        QPoint windowPosition(x, y);

        DatavizWindow* window = new DatavizWindow();
        if (HIDE_CURSOR) {
            window->setCursor(Qt::BlankCursor);
        }
        window->setFormat(format);
        window->resize(WINDOW_WIDTH, WINDOW_HEIGHT);
        window->setPosition(windowPosition);
        qDebug() << "Window" << i << "of size:" << WINDOW_WIDTH << "x" << WINDOW_HEIGHT << "at position" << windowPosition;
        window->setFlags(Qt::Window | Qt::FramelessWindowHint);
        window->show();

        //window->setAnimating(true);
        windows.append(window);
    }

    OscReceiver oscReceiver(OSC_RECEIVE_PORT);
    Controller controller(&oscReceiver, windows);

    int ret = app.exec();

    // cleanup
    for (int i = 0; i < NUM_WINDOWS; i ++) {
        delete windows[i];
    }
    qDebug() << "Exitting regularly.";
    return ret;
}
