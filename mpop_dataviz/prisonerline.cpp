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

}


PrisonerLine::~PrisonerLine() {

}

void PrisonerLine::animateXYAndRotation(qint64 currentTime, qint64 animationDuration, const QEasingCurve::Type& easing, qreal x, qreal y, qreal rotation) {

    _xAnimation.setDuration(animationDuration);
    _yAnimation.setDuration(animationDuration);
    _rotationAnimation.setDuration(animationDuration);

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

void PrisonerLine::draw(const qint64& elapsed) {
    Q_UNUSED(elapsed);

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


