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

/**
 * @brief Update the position of all objects.
 * @param currentTime
 */
void BarChartLayout::updateBarsPosition(qint64 currentTime) {
    if (! this->_groupTweenAnimator.isNull()) {
        this->_groupTweenAnimator->updateSceneObjectsPosition(currentTime);
        if (this->_groupTweenAnimator->isDone(currentTime)) {
            //qDebug() << "Delete the group tween animator.";
            this->_groupTweenAnimator.clear(); // should delete the instance.
        }
    }
}

void BarChartLayout::showSceneObject(qint64 currentTime)
{
    for (auto line : *_barObjects) {
        if (line->getVisible()) {
            line->draw(currentTime);
        }
    }

    for (auto line : _horizontalLines) {
        line->draw(currentTime);
    }
}

void BarChartLayout::setStartPosition(const QPointF &pos)
{
    if (_x != pos.x())
        _x = pos.x();
    if (_y != pos.y())
        _y = pos.y();
}


void BarChartLayout::moveObjectsToLayout(qint64 currentTime) {
    // TODO: never ever iterate over size of line vector
    const qreal DISTANCE_BETWEEN_BARS = _barsHeight / 3.7;
    const qreal WIDTH_OF_EACH_COLUMN = DISTANCE_BETWEEN_BARS * 3;
    const qreal DISTANCE_BETWEEN_COLUMN = DISTANCE_BETWEEN_BARS / 2;
    const qreal DISTANCE_BETWEEN_ROW = _spaceBetweenRows;
    //static const QEasingCurve easing(QEasingCurve::InOutQuad);
    static const QEasingCurve::Type easingCurveType = QEasingCurve::InOutQuad;
    static const qint64 animationSeconds = 1;
    static const qint64 animationMS = animationSeconds * 1000; // m

    this->_groupTweenAnimator.reset(new GroupTweenAnimator());
    this->_groupTweenAnimator->setDuration(animationMS);
    this->_groupTweenAnimator->setEasingType(easingCurveType);

    _horizontalLines = QVector<PrisonerLine::ptr>();

    int lineIndex = 0;

    int biggestRow = *std::max_element(_rowsValues.begin(), _rowsValues.end());
    qreal columnSize = biggestRow / 5;

    qreal offsetX = ((WIDTH_OF_EACH_COLUMN * columnSize) + ((DISTANCE_BETWEEN_BARS * 2) +
                       DISTANCE_BETWEEN_COLUMN) * (int(columnSize) - 1)) / 2;
    qreal offsetY = ((_barsHeight * (_rowsValues.size() - 1)) +
                     (DISTANCE_BETWEEN_ROW * (_rowsValues.size() - 1))) / 2;

    for (int rowIndex = 0; rowIndex < _rowsValues.size(); rowIndex++) {

        int columnIndex = 0;

        // Each bar in the bar chart is a column
        // We group lines by groups of 5
        for (int barIndex = 0; barIndex < _rowsValues.at(rowIndex); barIndex++) {
            if (lineIndex >= _barObjects->size()) {
                // qDebug() << "bar out of bound: " << lineIndex << " (not a problem)";
                break; // exitting this loop
            }
            PrisonerLine::ptr line = _barObjects->at(lineIndex);
            SceneObject::ptr sceneObject = qSharedPointerDynamicCast<SceneObject>(line);
            int moduloFive = barIndex % 5;

            qreal columnOffset = columnIndex * DISTANCE_BETWEEN_COLUMN;
            qreal x = barIndex * DISTANCE_BETWEEN_BARS + columnOffset;
            qreal y = -rowIndex * (DISTANCE_BETWEEN_ROW + (_barsHeight));
            qreal rotation = 0.0;

            if (moduloFive == 4) {
                x = ((barIndex - 2) * DISTANCE_BETWEEN_BARS) - (DISTANCE_BETWEEN_BARS / 2) + columnOffset;
                rotation = -73;
                // Increase the size of the fifth bar to fit to user story proportion
                line->setSize(_barsWidth, _barsHeight * 1.25);

                columnIndex += 1;
            }

            // Will process the next line on the next iteration:
            ++ lineIndex;

            if (_centered) {
                x -= offsetX;
                y += offsetY;
            } else {
                x += _x;
                y -= _y;
            }

            _groupTweenAnimator->addSceneObjectToAnimate(sceneObject, x, y, rotation);
        }

        if (_rowsValues.at(rowIndex) < 1) {
            for (int i = 0; i < 3; i++) {
                PrisonerLine::ptr line = PrisonerLine::ptr::create();

                line->setSize(_barsHeight, _barsWidth);
                line->setColor("#FFFFFF");

                qreal x = i * (DISTANCE_BETWEEN_BARS + _barsHeight);
                qreal y = -rowIndex * (DISTANCE_BETWEEN_ROW + (_barsHeight));
                x += (_x + (_barsHeight / 2 - _barsWidth));
                y -= _y;

                _groupTweenAnimator->addSceneObjectToAnimate(line, x, y, 0);
                _horizontalLines.append(line);
            }

        }
    }

    this->_groupTweenAnimator->start(currentTime);

    // When everything is done keep track of previous bars data
    _previousBars = *_barObjects;
}


void BarChartLayout::setRows(const QList<int>& values) {
    this->_rowsValues = values;
}
