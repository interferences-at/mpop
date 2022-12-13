#include "prisonerline.h"
//#include <QtMath>
#include <QDebug>
#include <QtGui/QOpenGLFunctions>

PrisonerLine::PrisonerLine() : SceneObject()
{

}


PrisonerLine::~PrisonerLine() {

}


void PrisonerLine::draw(const qint64& elapsed) {
    Q_UNUSED(elapsed);

    glPushMatrix();

    //qDebug() << "Draw line at " << _x << _y << "with angle" << _angleDegrees;
    glTranslated(_x, _y, 0.0);
    glRotated(_rotation, 0.0, 0.0, 1.0);

    glPushMatrix();

    glScaled(_width, _height, 1.0); // FIXME

    glTranslatef(-0.5, -0.5, 0.0);

    glBegin(GL_QUADS);
    glColor4f(_red, _green, _blue, _alpha);
    glVertex2f(0.0, 0.0);
    glVertex2f(1.0, 0.0);
    glVertex2f(1.0, 1.0);
    glVertex2f(0.0, 1.0);
    glEnd();

    glPopMatrix();

    glPopMatrix();
}

void PrisonerLine::setColor(const QString &colorCode) {
    _color = QColor(colorCode);
    // Convert Hex to RGBA
    setColor(_color.red(), _color.green(), _color.blue(), _color.alpha());
}

void PrisonerLine::setColor(int red, int green, int blue, int alpha) {
    _color = QColor(red, green, blue, alpha);
    // Convert RGBA to float color (0.0 -> 1.0)
    _red = _color.redF();
    _green = _color.greenF();
    _blue = _color.blueF();
    _alpha = _color.alphaF();
}

void PrisonerLine::setSize(qreal width, qreal height) {
    if (_width != width)
        _width = width;

    if (_height != height)
        _height = height;
}



