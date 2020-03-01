#include "line.h"
//#include <QtMath>
#include <QDebug>

Line::Line()
{
    _x = 0.0;
    _y = 0.0;
    _angleDegrees = 0.0;
}

Line::~Line() {

}


void Line::draw(const QElapsedTimer& timer) {
    Q_UNUSED(timer);

    glPushMatrix();

    //qDebug() << "Draw line at " << _x << _y << "with angle" << _angleDegrees;
    glTranslated(_x, _y, 0.0);
    glRotated(this->_angleDegrees, 0.0, 0.0, 1.0);

    glPushMatrix();

    glScaled(0.01, 0.1, 1.0); // FIXME

    glTranslatef(-0.5, -0.5, 0.0);

    glBegin(GL_QUADS);
    glColor4f(255, 255, 255, 255);
    glVertex2f(0.0, 0.0);
    glVertex2f(1.0, 0.0);
    glVertex2f(1.0, 1.0);
    glVertex2f(0.0, 1.0);
    glEnd();

    glPopMatrix();

    glPopMatrix();
}



void Line::setPosition(double x, double y) {
    _x = x;
    _y = y;
}

void Line::setOrientation(qreal degrees) {
    _angleDegrees = degrees;
}
