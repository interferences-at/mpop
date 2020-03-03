#include "barchartlayout.h"
#include "prisonerline.h"
#include <QDebug>


BarChartLayout::BarChartLayout()
{

}


BarChartLayout::~BarChartLayout()
{

}


void BarChartLayout::moveObjectsToLayout() {

    // TODO: never ever iterate over size of line vector
    // const int numLines = _sceneObjects.size();
    static const double WIDTH_OF_EACH_COLUMN = 0.3;
    static const double DISTANCE_BETWEEN_BARS = 0.02;
    static const double DISTANCE_BETWEEN_COLUMN = 0.05;
    static const double DISTANCE_BETWEEN_ROW = 0.15;
    static const double ADJUST_FIFTH_X = 0.1;
    static const double OUTSIDE_X = 3.0; // outside of the screen
    static const double OUTSIDE_Y = 3.0; // outside of the screen

    int lineIndex = 0;
    for (int barIndex = 0; barIndex < _barValues.size(); barIndex ++) {
        if (lineIndex >= _prisonerLines.size()) {
            qWarning() << "Out of bound: " << lineIndex;
        }
        quint8 barValue = this->_barValues[barIndex];
        int row = 0;

        for (int lineInBar = 0; lineInBar < barValue; lineInBar ++) {
            PrisonerLine* line = _prisonerLines[lineIndex];
            int moduloFive = lineInBar % 5;

            if (moduloFive < 4) {
                line->setPosition(barIndex * WIDTH_OF_EACH_COLUMN + moduloFive * DISTANCE_BETWEEN_BARS, row * DISTANCE_BETWEEN_ROW);
            } else if (moduloFive == 4) {
                line->setPosition(barIndex * WIDTH_OF_EACH_COLUMN + moduloFive * DISTANCE_BETWEEN_BARS + DISTANCE_BETWEEN_COLUMN - ADJUST_FIFTH_X, row * DISTANCE_BETWEEN_ROW);
                line->setOrientation(60.0);
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
