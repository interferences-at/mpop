#include "grouptweenanimator.h"

GroupTweenAnimator::GroupTweenAnimator()
{

}


GroupTweenAnimator::~GroupTweenAnimator()
{

}


void GroupTweenAnimator::addSceneObjects(const QVector<SceneObject*>& sceneObjects) {
    _sceneObjects.append(sceneObjects);
}


qreal GroupTweenAnimator::mapValue(qreal inValue, qreal inFrom, qreal inTo, qreal outFrom, qreal outTo) {
    qreal ret = outFrom + (outTo - outFrom) * ((inValue - inFrom) / (inTo - inFrom));
    // FIXME check for a division by zero
    return ret;
}


bool GroupTweenAnimator::updateSceneObjects(const qint64& currentTime) {
    bool hasSomeTimeLeft = ! this->isDone(currentTime);
    if (hasSomeTimeLeft) {

    }

    return hasSomeTimeLeft;
}

void GroupTweenAnimator::start(const qint64& currentTime) {
    this->_startTime = currentTime;
    this->_endTime = currentTime + this->_duration;
}

bool GroupTweenAnimator::isDone(const qint64& currentTime) const {
    qint64 timeLeft = this->_endTime - currentTime;
    bool ret = timeLeft < 0;
    return ret;
}

void GroupTweenAnimator::setEasingType(QEasingCurve::Type easingType) {
    this->_easingType = easingType;
}

void GroupTweenAnimator::setDuration(qint64 durationMS) {
    this->_duration = durationMS;
}


qint64 GroupTweenAnimator::getEndTime() const {
    return this->_endTime;
}
