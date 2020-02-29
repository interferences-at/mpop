#include "openglwindow.h"
#include "datavizwindow.h"
#include <QtGui/QGuiApplication>
#include <QtGui/QMatrix4x4>
#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QScreen>
#include <QtCore/qmath.h>
#include <QSharedPointer>


// Constants:
static const int NUM_WINDOWS = 4;
static const int WINDOW_WIDTH = 1920;
static const int WINDOW_HEIGHT = 1080;


int main(int argc, char* argv[]) {
    QGuiApplication app(argc, argv);
    QSurfaceFormat format;
    format.setSamples(16);
    QList<DatavizWindow*> windows;

    for (int i = 0; i < NUM_WINDOWS; i ++) {
        int x = (i % 2) * WINDOW_WIDTH;
        int y = (i / 2) * WINDOW_HEIGHT;
        QPoint windowPosition(x, y);

        DatavizWindow* window = new DatavizWindow();
        window->setCursor(Qt::BlankCursor);
        window->setFormat(format);
        window->resize(WINDOW_WIDTH, WINDOW_HEIGHT);
        window->setPosition(windowPosition);
        qDebug() << "Window" << i << "of size:" << WINDOW_WIDTH << "x" << WINDOW_HEIGHT << "at position" << windowPosition;
        window->setFlags(Qt::Window | Qt::FramelessWindowHint);
        window->show();

        window->setAnimating(true);
        windows.append(window);
    }

    int ret = app.exec();

    // cleanup
    for (int i = 0; i < NUM_WINDOWS; i ++) {
        delete windows[i];
    }
    return ret;
}


