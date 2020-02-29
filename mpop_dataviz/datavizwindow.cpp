#include "datavizwindow.h"
#include "openglwindow.h"
#include <QtGui/QGuiApplication>
#include <QtGui/QMatrix4x4>
#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QScreen>
#include <QtCore/qmath.h>


DatavizWindow::DatavizWindow()
{
    qDebug() << "Create a Window";
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

    glPushMatrix();
    glTranslatef(-0.5, -0.5, 0.0);
    glScalef(1.0, 1.0, 1.0);

    glBegin(GL_QUADS);
    glColor4f(255, 127, 0, 255);
    glVertex2f(0.0, 0.0);
    glColor4f(127, 255, 0, 255);
    glVertex2f(1.0, 0.0);
    glColor4f(0, 127, 255, 255);
    glVertex2f(1.0, 1.0);
    glColor4f(255, 0, 127, 255);
    glVertex2f(0.0, 1.0);
    glEnd();
    glPopMatrix();
}


DatavizWindow::~DatavizWindow()
{
    qDebug() << "~DatavizWindow";
}
