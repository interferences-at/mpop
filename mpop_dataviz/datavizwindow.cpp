#include "datavizwindow.h"
#include "prisonerline.h"
#include <QtGui/QGuiApplication>
#include <QtGui/QMatrix4x4>
#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QScreen>
#include <QtCore/qmath.h>

// Static window ID variable
uint DatavizWindow::windowId = -1;

DatavizWindow::DatavizWindow() {
    qDebug() << "Create a Window";
    _elapsedTimer.start();

    static const int NUM_LINES = 1000;
    QVector<PrisonerLine::ptr> prisonerLines;
    for (int i = 0; i < NUM_LINES; i ++) {
        PrisonerLine::ptr line = PrisonerLine::ptr::create();
        _sceneObjects.push_back(qSharedPointerDynamicCast<SceneObject>(line));
        prisonerLines.push_back(line);
    }

    _barChartLayout.addPrisonerLines(prisonerLines); // Only a subsets of all sceneobjects - only the lines

    // The initial example values:
    QList<int> bars;
    bars.push_back(10);
    bars.push_back(20);
    bars.push_back(70);
    showBarChartBars(bars);

    // Set default window ID to +1
    windowId++;
}


qint64 DatavizWindow::elapsed() const {
    return this->_elapsedTimer.elapsed();
}


void DatavizWindow::showBarChartBars(const QList<int>& bars) {
    _barChartLayout.setBars(bars);
    _barChartLayout.moveObjectsToLayout(this->elapsed()); // Important: do it after you called setBars
}


void DatavizWindow::initializeGL() {
    // setSwapInterval(1);
    glClearColor(0, 0, 0, 0);
}


void DatavizWindow::resizeGL(int w, int h) {
    const qreal retinaScale = this->devicePixelRatio();

    // Note: Avoid issuing OpenGL commands from this function as
    // there may not be a context current when it is invoked.
    // If it cannot be avoided, call makeCurrent().
    this->makeCurrent();

    glViewport(0, 0,
               w * static_cast<GLsizei>(retinaScale),
               h * static_cast<GLsizei>(retinaScale));
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);

    // Note: Here, the center is (0, 0), and not the top-left corner, like it often is.
    GLdouble ratio = (static_cast<GLdouble>(w)) / static_cast<GLdouble>(h);
    GLdouble left = - ratio;
    GLdouble right = ratio;
    GLdouble top = 1.0;
    GLdouble bottom = -1.0;

    glLoadIdentity();
    glOrtho(left, right, bottom, top, -1, 1);

    // Instead, we could eventually use a pespective view, here:
    // gluPerspective(60.0f, ratio, 1.0f, 100.0f);
}


void DatavizWindow::paintGL() {
    // We must clear the background here
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // TODO: enable only one layout at a time, perhaps.
    _barChartLayout.updateObjectPosition(this->elapsed());

    for (auto iter = _sceneObjects.begin(); iter != _sceneObjects.end(); ++ iter) {
        SceneObject::ptr obj = (*iter);
        if (obj->getVisible()) {
            // FIXME: we should take care of the Z-sorting of the scene objects.
            obj->draw(this->elapsed());
        }
    }
    // swapBuffers();
    this->update(); // ask for a new render next time the screen refreshes.
}


DatavizWindow::~DatavizWindow()
{
    qDebug() << "~DatavizWindow";
    for (auto iter = _sceneObjects.begin(); iter != _sceneObjects.end(); ++ iter) {
        iter->clear();
    }
}

/*
void DatavizWindow::setAnimating(bool animating) {
    _is_animating = animating;
    if (animating) {
        // renderLater();
    }
}
*/
