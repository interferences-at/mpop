#pragma once


#include "prisonerline.h"
#include "sceneobject.h"
#include <QVector>
#include <QSharedPointer>

/**
 * @brief Animates a group of scene objects.
 */
class GroupTweenAnimator
{
private:
    struct AnimatedSceneObject {
        SceneObject* sceneObject;
        qreal xFrom;
        qreal xTo;
        qreal yFrom;
        qreal yTo;
        qreal rotationFrom;
        qreal rotationTo;
        // TODO add z and scale
    };

    typedef QSharedPointer<AnimatedSceneObject> AnimatedSceneObjectPtr;

public:
    GroupTweenAnimator();
    virtual ~GroupTweenAnimator();
    /**
     * @brief Adds scene objects.
     * @param sceneObjects Scene objects to animate.
     */
    //void addSceneObjects(const QVector<SceneObject*>& sceneObjects);

    void addSceneObjectToAnimate(SceneObject* sceneObject, qreal targetX, qreal targetY, qreal targetRotation);
    /**
     * @brief Move the objects to the position they should be at now.
     * @return True if there is still some time left to the animation.
     */
    bool updateSceneObjectsPosition(const qint64& currentTime);

    void start(const qint64& currentTime);

    bool isDone(const qint64& currentTime) const;

    void setEasingType(QEasingCurve::Type easingType);

    void setDuration(qint64 durationMS);

    qint64 getEndTime() const;

    /**
     * Re-maps a number from one range to another.
     *
     * Numbers outside the range are not clamped to 0 and 1, because
     * out-of-range values are often intentional and useful.
     *
     * @param inValue The incoming value to be converted
     * @param inFrom Lower bound of the value's current range
     * @param inTo Upper bound of the value's current range
     * @param outFrom Lower bound of the value's target range
     * @param outTo Upper bound of the value's target range
     */
    static qreal mapValue(qreal inValue, qreal inFrom, qreal inTo, qreal outFrom, qreal outTo);

private:
    // QVector<SceneObject*> _sceneObjects;
    QVector<AnimatedSceneObjectPtr> _animatedSceneObjects;
private:
    qint64 _duration;
    qint64 _startTime;
    qint64 _endTime;
    QEasingCurve::Type _easingType;

    qreal getElapsedRatio(const qint64& currentTime) const;
    qint64 getTimeLeft(const qint64& currentTime) const;
    qint64 getElapsed(const qint64& currentTime) const;
};
