#include "barchartlayout.h"
#include "line.h"

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

    for (int barIndex = 0; barIndex < _barValues.size(); barIndex ++) {
        quint8 barValue = this->_barValues[barIndex];
        int row = 0;

        for (int lineInBar = 0; lineInBar < barValue; lineInBar ++) {
            Line* line = dynamic_cast<Line*>(_sceneObjects[lineIndex]); // FIXME: Perhaps layouts should deal with line objects directly.
            int moduloFive = lineInBar % 5;

            if (moduloFive < 4) {
                line->setPosition(lineIndex * 0.1, row * 0.1);
            } else if (moduloFive == 4) {
                line->setPosition(0.2, row * 0.1);
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
