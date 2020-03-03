#include "barchartlayout.h"
#include "prisonerline.h"

BarChartLayout::BarChartLayout()
{

}

BarChartLayout::~BarChartLayout()
{

}


void BarChartLayout::moveObjectsToLayout() {
    int lineIndex = 0;
    // TODO: never ever iterate over size of line vector
    // const int numLines = _sceneObjects.size();
    static const double WIDTH_OF_EACH_COLUMN = 0.3;
    static const double DISTANCE_BETWEEN_BARS = 0.02;
    static const double DISTANCE_BETWEEN_COLUMN = 0.05;
    static const double DISTANCE_BETWEEN_ROW = 0.15;
    static const double ADJUST_FIFTH_X = 0.1;

    for (int barIndex = 0; barIndex < _barValues.size(); barIndex ++) {
        quint8 barValue = this->_barValues[barIndex];
        int row = 0;

        for (int lineInBar = 0; lineInBar < barValue; lineInBar ++) {
            PrisonerLine* line = dynamic_cast<PrisonerLine*>(_prisonerLines[lineIndex]); // FIXME: Perhaps layouts should deal with line objects directly.
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
}


void BarChartLayout::setBars(const QVector<quint8>& values) {
    this->_barValues = values;
}
