#pragma once

#include "layout.h"
#include "grouptweenanimator.h"
#include "barchartlayout.h"


class ScreensaverLayout : public Layout
{
public:
    ScreensaverLayout();
    virtual ~ScreensaverLayout();

    void moveObjectsToLayout(qint64 currentTime) override;
    void updateBarsPosition(qint64 currentTime) override;

    void showSceneObject(qint64 currentTime) override;

    QSharedPointer<QVector<PrisonerLine::ptr>> getClosestBars(const QPointF &pos);

    void setParam(const QString& paramName, float value);

private:
    std::uniform_real_distribution<qreal> randomX, randomY, randomRadius;
    std::uniform_real_distribution<qreal> randomFrequency, randomRatioRotation;

    QSharedPointer<GroupTweenAnimator> _groupTweenAnimator;

    qreal _speedRatio = 1;
};

