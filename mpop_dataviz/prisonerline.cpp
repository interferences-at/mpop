#include "prisonerline.h"
//#include <QtMath>
#include <QDebug>

PrisonerLine::PrisonerLine(QObject* parent) : SceneObject(parent),
    _xAnimation(this, "x"),
    _yAnimation(this, "y"),
    _zAnimation(this, "z"),
    _rotationAnimation(this, "rotation"),
    _scaleAnimation(this, "scale")
{
    _x = 0.0;
    _y = 0.0;
    _z = 0.0;
    //_angleDegrees = 0.0;
    _scale = 1.0;
    _rotation = 0.0;
}


PrisonerLine::~PrisonerLine() {

}

void PrisonerLine::animateXYAndRotation(int msec, const QEasingCurve& easing, qreal x, qreal y, qreal rotation) {

    _xAnimation.setDuration(msec);
    _yAnimation.setDuration(msec);
    _rotationAnimation.setDuration(msec);

    _xAnimation.setStartValue(_x);
    _yAnimation.setStartValue(_y);
    _rotationAnimation.setStartValue(_rotation);

    _xAnimation.setEndValue(x);
    _yAnimation.setEndValue(y);
    _rotationAnimation.setEndValue(rotation);

    _xAnimation.setEasingCurve(easing);
    _yAnimation.setEasingCurve(easing);
    _rotationAnimation.setEasingCurve(easing);

    _xAnimation.start();
    _yAnimation.start();
    _rotationAnimation.start();
}

void PrisonerLine::draw(const QElapsedTimer& timer) {
    Q_UNUSED(timer);

    glPushMatrix();

    //qDebug() << "Draw line at " << _x << _y << "with angle" << _angleDegrees;
    glTranslated(_x, _y, 0.0);
    glRotated(_rotation, 0.0, 0.0, 1.0);

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



void PrisonerLine::setPosition(qreal x, qreal y) {
    setX(x);
    setY(y);
}

// FIXME: deprecated
void PrisonerLine::setOrientation(qreal degrees) {
    //_angleDegrees = degrees;
    setRotation(degrees);
}

void PrisonerLine::setX(qreal value) {
    _x = value;
    emit xChanged(value);
}

void PrisonerLine::setY(qreal value) {
    _y = value;
    emit yChanged(value);
}

void PrisonerLine::setZ(qreal value) {
    _z = value;
    emit zChanged(value);
}

void PrisonerLine::setRotation(qreal value) {
    _rotation = value;
    emit rotationChanged(value);
}

void PrisonerLine::setScale(qreal value) {
    _scale = value;
    emit scaleChanged(value);
}
