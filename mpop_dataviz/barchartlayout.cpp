#include "barchartlayout.h"
#include "prisonerline.h"
#include <QDebug>
//#include <QEasingCurve>


BarChartLayout::BarChartLayout()
{

}


BarChartLayout::~BarChartLayout()
{

}


void BarChartLayout::moveObjectsToLayout() {
    // TODO: never ever iterate over size of line vector
    // const int numLines = _sceneObjects.size();
    static const qreal WIDTH_OF_EACH_COLUMN = 0.3;
    static const qreal DISTANCE_BETWEEN_BARS = 0.02;
    static const qreal DISTANCE_BETWEEN_COLUMN = 0.05;
    static const qreal DISTANCE_BETWEEN_ROW = 0.15;
    static const qreal ADJUST_FIFTH_X = 0.1;
    static const qreal OUTSIDE_X = 3.0; // outside of the screen
    static const qreal OUTSIDE_Y = 3.0; // outside of the screen
    static const QEasingCurve easing(QEasingCurve::InOutQuad);
    static const int animationMS = 4000; // ms

    int lineIndex = 0;
    for (int barIndex = 0; barIndex < _barValues.size(); barIndex ++) {
        if (lineIndex >= _prisonerLines.size()) {
            qWarning() << "Out of bound: " << lineIndex;
        }
        int barValue = this->_barValues[barIndex];
        int row = 0;

        // Each bar in the bar chart is a column
        // We group lines by groups of 5
        for (int lineInBar = 0; lineInBar < barValue; lineInBar ++) {
            PrisonerLine* line = _prisonerLines[lineIndex];
            int moduloFive = lineInBar % 5;

            if (moduloFive < 4) {
                qreal x = barIndex * WIDTH_OF_EACH_COLUMN + moduloFive * DISTANCE_BETWEEN_BARS;
                qreal y = row * DISTANCE_BETWEEN_ROW;
                qreal rotation = 0.0;

                // line->setPosition(x, y);
                line->animateXYAndRotation(animationMS, easing, x, y, rotation);
            } else if (moduloFive == 4) {
                qreal x = barIndex * WIDTH_OF_EACH_COLUMN + moduloFive * DISTANCE_BETWEEN_BARS + DISTANCE_BETWEEN_COLUMN - ADJUST_FIFTH_X;
                qreal y = row * DISTANCE_BETWEEN_ROW;
                qreal rotation = 60.0;

                //line->setPosition(x, y);
                //line->setOrientation(60.0);
                line->animateXYAndRotation(animationMS, easing, x, y, rotation);
                row += 1;
            }

            ++ lineIndex;
        }
    }

    // Move leftover out of the screen, if needed
    if (lineIndex < _prisonerLines.size()) {
        for ( ; lineIndex < _prisonerLines.size(); lineIndex ++) {
            PrisonerLine* line = _prisonerLines[lineIndex];
            line->setPosition(OUTSIDE_X, OUTSIDE_Y);
        }
    }
}


void BarChartLayout::setBars(const QList<int>& values) {
    this->_barValues = values;
}
