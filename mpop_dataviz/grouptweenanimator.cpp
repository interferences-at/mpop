#include "grouptweenanimator.h"
#include <QDebug>

static const qreal ZERO_VAL = 0.0;
static const qreal ONE_VAL = 0.0;

GroupTweenAnimator::GroupTweenAnimator()
{
    this->_easingType = QEasingCurve::Linear; // default
    this->_startTime = 0; // default
    this->_endTime = 10000000L; // FIXME default
}


GroupTweenAnimator::~GroupTweenAnimator()
{

}


//void GroupTweenAnimator::addSceneObjects(const QVector<SceneObject*>& sceneObjects) {
//
//    //AnimatedSceneObject* animatedSceneObject
//    _sceneObjects.append(sceneObjects);
//}

void GroupTweenAnimator::addSceneObjectToAnimate(SceneObject::ptr sceneObject, qreal targetX, qreal targetY, qreal targetRotation) {
    AnimatedSceneObjectPtr animatedObj(new AnimatedSceneObject);
    animatedObj->sceneObject = sceneObject;

    animatedObj->xFrom = sceneObject->getX();
    animatedObj->xTo = targetX;

    animatedObj->yFrom = sceneObject->getY();
    animatedObj->yTo = targetY;

    animatedObj->rotationFrom = sceneObject->getRotation();
    animatedObj->rotationTo = targetRotation;

    this->_animatedSceneObjects.append(animatedObj);
}

// static method
qreal GroupTweenAnimator::mapValue(qreal inValue, qreal inFrom, qreal inTo, qreal outFrom, qreal outTo) {
    //if ((inTo - inFrom) <= 0.0) {
        // qDebug() << "mapValue: division by zero!";
        // return 0.0;
    //}

    // processing formula:
    // qreal ret = outFrom + (outTo - outFrom) * ((inValue - inFrom) / (inTo - inFrom));

    // arduino formula:
    qreal ret = (inValue - inFrom) * (outTo - outFrom) / (inTo - inFrom) + outFrom;

    // FIXME check for a division by zero
    return ret;
}

/**
 * @brief Linear interpolation between the original value and a target.
 *
 * Note that the start and final value can be in any range, including negative numbers.
 *
 * @param progressRatio The progress is within the range [0, 1]
 * @param startValue Original position of the object to translate. (one-dimensional)
 * @param targetValue (Destination)
 * @return The interpolated value.
 */
static qreal interpolate_linearly(qreal progressRatio, qreal startValue, qreal targetValue) {
    qreal ret = startValue + ( ( targetValue - startValue ) * progressRatio );
    return ret;
}


void GroupTweenAnimator::updateSceneObjectsPosition(const qint64& currentTime) {
    bool hasSomeTimeLeft = ! this->isDone(currentTime);
    qreal elapsedRatio = this->getElapsedRatio(currentTime);
    //qreal timeLeft = this->getTimeLeft(currentTime);

    for (auto iter = this->_animatedSceneObjects.begin(); iter != _animatedSceneObjects.end(); ++ iter) {
        AnimatedSceneObjectPtr animatedData = (*iter);
        SceneObject::ptr sceneObject = animatedData->sceneObject;

        if (hasSomeTimeLeft) {

            // qDebug() << "elapsedRatio: " << elapsedRatio;
//            qreal newX = GroupTweenAnimator::mapValue(elapsedRatio, ZERO_VAL, ONE_VAL,
//                animatedData->xFrom, animatedData->xTo);
//            qreal newY = GroupTweenAnimator::mapValue(elapsedRatio, ZERO_VAL, ONE_VAL,
//                animatedData->yFrom, animatedData->yTo);
//            qreal newRotation = GroupTweenAnimator::mapValue(elapsedRatio, ZERO_VAL, ONE_VAL,
//                animatedData->rotationFrom, animatedData->rotationTo);

            qreal newX = interpolate_linearly(elapsedRatio, animatedData->xFrom, animatedData->xTo);
            qreal newY = interpolate_linearly(elapsedRatio, animatedData->yFrom, animatedData->yTo);
            qreal newRotation = interpolate_linearly(elapsedRatio, animatedData->rotationFrom, animatedData->rotationTo);

            sceneObject->setX(newX);
            sceneObject->setY(newY);
            sceneObject->setRotation(newRotation);

        } else {
            // time is up. Got to final position.
            sceneObject->setX(animatedData->xTo);
            sceneObject->setY(animatedData->yTo);
            sceneObject->setRotation(animatedData->rotationTo);
            // qDebug() << "This animation is done. You should delete this GroupTweenAnimator";
        }
    }
    //return hasSomeTimeLeft;
}

qreal GroupTweenAnimator::getElapsedRatio(const qint64& currentTime) const {
    if (this->isDone(currentTime)) {
        return 1.0;
    } else {
        qreal elapsed = this->getElapsed(currentTime);
        qreal totalDuration = this->_duration;
        if (totalDuration <= elapsed) {
            return 1.0;
        } else if (totalDuration <= 10) {
            return 1.0;
        } else {
            return elapsed / totalDuration;
        }
//        qreal ret = GroupTweenAnimator::mapValue(
//                    static_cast<qreal>(currentTime),
//                    static_cast<qreal>(this->_startTime),
//                    static_cast<qreal>(this->_endTime),
//                    ZERO_VAL, ONE_VAL);
//        qDebug() <<
//            "mapValue " << ret << " = " <<
//                    static_cast<qreal>(currentTime) << ", " <<
//                    static_cast<qreal>(this->_startTime) << ", " <<
//                    static_cast<qreal>(this->_endTime) << ", " <<
//                    ZERO_VAL << ", " <<
//                    ONE_VAL;
    }
}

qint64 GroupTweenAnimator::getElapsed(const qint64& currentTime) const {
    qint64 ret = currentTime - this->_startTime;
    return ret;
}

void GroupTweenAnimator::start(const qint64& currentTime) {
    this->_startTime = currentTime;
    this->_endTime = currentTime + this->_duration;
}

qint64 GroupTweenAnimator::getTimeLeft(const qint64& currentTime) const {
    qint64 ret = this->_endTime - currentTime;
    return ret;
}

bool GroupTweenAnimator::isDone(const qint64& currentTime) const {
    qint64 timeLeft = this->_endTime - currentTime;
    bool ret = timeLeft <= 0;
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
