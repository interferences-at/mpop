#pragma once

#include "sceneobject.h"
#include <QPoint>
#include <QPropertyAnimation>
#include <QEasingCurve>

// TODO: rename to Stick (it's easier to read/write/say)
class PrisonerLine : public SceneObject
{
    //Q_OBJECT
    // properties

public:
    explicit PrisonerLine(); // QObject* parent = nullptr);
    ~PrisonerLine() override;
    virtual void draw(const qint64& elapsed) override;

    //void animateXYAndRotation(qint64 currentTime, qint64 animationDuration, const QEasingCurve::Type& easing, qreal x, qreal y, qreal rotation);

private:

//    QPropertyAnimation _xAnimation;
//    QPropertyAnimation _yAnimation;
//    QPropertyAnimation _zAnimation;
//    QPropertyAnimation _rotationAnimation;
//    QPropertyAnimation _scaleAnimation;
};
