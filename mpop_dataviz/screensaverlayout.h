#pragma once

#include <QRandomGenerator>
#include <random>
#include <algorithm>

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

    QVector<PrisonerLine::ptr> getLayoutClosestBars(const QPointF &pos);

private:
    QSharedPointer<GroupTweenAnimator> _groupTweenAnimator;
};

