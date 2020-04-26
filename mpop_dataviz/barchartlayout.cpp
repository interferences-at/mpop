#include "barchartlayout.h"
#include "prisonerline.h"
#include <QDebug>
//#include <QEasingCurve>


BarChartLayout::BarChartLayout()
{
    this->_groupTweenAnimator.reset(new GroupTweenAnimator());
}


BarChartLayout::~BarChartLayout()
{

}

void BarChartLayout::updateObjectPosition(qint64 currentTime) {
    if (! this->_groupTweenAnimator.isNull()) {
        this->_groupTweenAnimator->updateSceneObjectsPosition(currentTime);
        if (this->_groupTweenAnimator->isDone(currentTime)) {
            //qDebug() << "Delete the group tween animator.";
            this->_groupTweenAnimator.clear(); // should delete the instance.
        }
    }
}


void BarChartLayout::moveObjectsToLayout(qint64 currentTime) {
    // TODO: never ever iterate over size of line vector
    // const int numLines = _sceneObjects.size();
    static const qreal WIDTH_OF_EACH_COLUMN = 0.3;
    static const qreal DISTANCE_BETWEEN_BARS = 0.02;
    static const qreal DISTANCE_BETWEEN_COLUMN = 0.05;
    static const qreal DISTANCE_BETWEEN_ROW = 0.15;
    static const qreal ADJUST_FIFTH_X = 0.1;
    static const qreal OUTSIDE_X = 3.0; // outside of the screen
    static const qreal OUTSIDE_Y = 3.0; // outside of the screen
    //static const QEasingCurve easing(QEasingCurve::InOutQuad);
    static const QEasingCurve::Type easingCurveType = QEasingCurve::InOutQuad;
    static const qint64 animationSeconds = 1;
    static const qint64 animationMS = animationSeconds * 1000; // ms

    this->_groupTweenAnimator.reset(new GroupTweenAnimator());
    this->_groupTweenAnimator->setDuration(animationMS);
    this->_groupTweenAnimator->setEasingType(easingCurveType);

    int lineIndex = 0;
    for (int barIndex = 0; barIndex < _barValues.size(); barIndex ++) {
        if (lineIndex >= _prisonerLines.size()) {
            qWarning() << "Out of bound: " << lineIndex;
            break; // exitting this loop
        }
        int barValue = this->_barValues[barIndex];
        int row = 0;

        // Each bar in the bar chart is a column
        // We group lines by groups of 5
        for (int lineInBar = 0; lineInBar < barValue; lineInBar ++) {
            if (lineIndex >= _prisonerLines.size()) {
                qWarning() << "Out of bound: " << lineIndex;
                break; // exitting this loop
            }
            PrisonerLine::ptr line = _prisonerLines[lineIndex];
            SceneObject::ptr sceneObject = qSharedPointerDynamicCast<SceneObject>(line);
            int moduloFive = lineInBar % 5;

            if (moduloFive < 4) {
                qreal x = barIndex * WIDTH_OF_EACH_COLUMN + moduloFive * DISTANCE_BETWEEN_BARS;
                qreal y = row * DISTANCE_BETWEEN_ROW;
                qreal rotation = 0.0;

                this->_groupTweenAnimator->addSceneObjectToAnimate(sceneObject, x, y, rotation);

                //line->setPosition(x, y);
                //line->animateXYAndRotation(currentTime, animationMS, easingCurveType, x, y, rotation);
            } else if (moduloFive == 4) {
                qreal x = barIndex * WIDTH_OF_EACH_COLUMN + moduloFive * DISTANCE_BETWEEN_BARS + DISTANCE_BETWEEN_COLUMN - ADJUST_FIFTH_X;
                qreal y = row * DISTANCE_BETWEEN_ROW;
                qreal rotation = 60.0;

                //line->setPosition(x, y);
                //line->setOrientation(60.0);
                //line->animateXYAndRotation(currentTime, animationMS, easingCurveType, x, y, rotation);
                this->_groupTweenAnimator->addSceneObjectToAnimate(sceneObject, x, y, rotation);

                row += 1;
            }

            // Will process the next line on the next iteration:
            ++ lineIndex;
        }
    }

    // Move leftover out of the screen, if needed
    if (lineIndex < _prisonerLines.size()) {
        for ( ; lineIndex < _prisonerLines.size(); lineIndex ++) {
            PrisonerLine::ptr line = _prisonerLines[lineIndex];
            SceneObject::ptr sceneObject = qSharedPointerDynamicCast<SceneObject>(line);
            // line->setPosition(OUTSIDE_X, OUTSIDE_Y);
            qreal x = OUTSIDE_X;
            qreal y = OUTSIDE_Y;
            qreal rotation = sceneObject->getRotation();
            this->_groupTweenAnimator->addSceneObjectToAnimate(sceneObject, x, y, rotation);
        }
    }

    this->_groupTweenAnimator->start(currentTime);
}


void BarChartLayout::setBars(const QList<int>& values) {
    this->_barValues = values;
}
