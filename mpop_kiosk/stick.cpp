#include "stick.h"

Stick::Stick() :
    _x(0.0), _y(0.0),
    _width(0.01), _height(0.1)
{

}

void Stick::draw()
{
    glPushMatrix();

    glTranslated(_x, _y, 0.5);
    glRotated(_rotation, 0.0, 0.0, 1.0);

    glPushMatrix();

    glScaled(_width, _height, 1.0);

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

    glFlush();
}

void Stick::setSize(qreal width, qreal height)
{
    _width = width;
    _height = height;
}

void Stick::setColor(const QString &colorCode)
{
    _color = QColor(colorCode);
    setColor(_color.red(), _color.green(), _color.blue(), _color.alpha());
}

void Stick::setColor(int red, int green, int blue, int alpha)
{
    _color = QColor(red, green, blue, alpha);
    _red = _color.redF();
    _green = _color.greenF();
    _blue = _color.blueF();
    _alpha = _color.alphaF();
}


