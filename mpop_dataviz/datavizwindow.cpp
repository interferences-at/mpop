#include "datavizwindow.h"
#include "openglwindow.h"
#include "prisonerline.h"
#include <QtGui/QGuiApplication>
#include <QtGui/QMatrix4x4>
#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QScreen>
#include <QtCore/qmath.h>



DatavizWindow::DatavizWindow()
{
    qDebug() << "Create a Window";
    _elapsedTimer.start();

    static const int NUM_LINES = 100;
    QVector<PrisonerLine*> prisonerLines;
    for (int i = 0; i < NUM_LINES; i ++) {
        PrisonerLine* line = new PrisonerLine();
        _sceneObjects.push_back(dynamic_cast<SceneObject*>(line));
        prisonerLines.push_back(line);
    }

    _barChartLayout.addPrisonerLines(prisonerLines); // Only a subsets of all sceneobjects - only the lines


    QVector<quint8> bars;
    bars.push_back(10);
    bars.push_back(20);
    bars.push_back(70);
    _barChartLayout.setBars(bars);

    _barChartLayout.moveObjectsToLayout(); // Important: do it after you called setBars
}


void DatavizWindow::initialize() {

}

void DatavizWindow::render() {
    const qreal retinaScale = devicePixelRatio();
    glViewport(0, 0,
               width() * static_cast<GLsizei>(retinaScale),
               height() * static_cast<GLsizei>(retinaScale));
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);

    GLdouble ratio = (static_cast<GLdouble>(width())) / static_cast<GLdouble>(height());
    GLdouble left = - ratio;
    GLdouble right = ratio;
    GLdouble top = 1.0;
    GLdouble bottom = -1.0;

    glLoadIdentity();
    glOrtho(left, right, bottom, top, -1, 1);

    // Instead, we could use a pespective view, here:
    // gluPerspective(60.0f, ratio, 1.0f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    for (auto iter = _sceneObjects.begin(); iter != _sceneObjects.end(); ++ iter) {
        SceneObject* obj = (*iter);
        if (obj->getVisible()) {
            obj->draw(_elapsedTimer);
        }
    }
}


DatavizWindow::~DatavizWindow()
{
    qDebug() << "~DatavizWindow";
    for (auto iter = _sceneObjects.begin(); iter != _sceneObjects.end(); ++ iter) {
        delete (*iter);
    }
}
