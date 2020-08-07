#include "allanswerslayout.h"

bool AllAnswersLayout::_doneMoving = false;

AllAnswersLayout::AllAnswersLayout()
{
    _groupTweenAnimator.reset(new GroupTweenAnimator());

//    for (int i = 0; i < 25; i++) {
//       int random100 = (rand() % 100) + 1;
//       _rowsValues.append(random100);
//    }
//    qDebug() << _rowsValues;
}

void AllAnswersLayout::moveObjectsToLayout(qint64 currentTime)
{
    _groupTweenAnimator.reset(new GroupTweenAnimator());
    _groupTweenAnimator->setDuration(1000);
    _groupTweenAnimator->setEasingType(QEasingCurve::Linear);

    int iterator = 0;
    for (int rowIndex = 0; rowIndex < _rowsValues.size(); rowIndex++) {
        // #64994E => #467554 || #64994E => #546059 || #65bF40 => #395030
        QString color = getColorFromRange("#65bF40", "#395030", _rowsValues.at(rowIndex));

        for (int lineIndex = 0; lineIndex < _rowsValues.at(rowIndex); lineIndex++) {

            qreal width = _right - _left;
            qreal height = _top - _bottom;

            qreal spaceX = width / _rowsValues.at(rowIndex);
            qreal spaceY = height / _rowsValues.size();

            qreal x = _left + (lineIndex * spaceX);
            qreal y = _bottom - (-rowIndex * spaceY);
            qreal rotation = 0.0;

            PrisonerLine::ptr line = _barObjects->at(iterator);
            line->setColor(color);

            iterator++;

            _groupTweenAnimator->addSceneObjectToAnimate(line, x, y, rotation);
        }
    }

    _groupTweenAnimator->start(currentTime);
}

void AllAnswersLayout::updateBarsPosition(qint64 currentTime)
{
    if (! _groupTweenAnimator.isNull()) {
        _groupTweenAnimator->updateSceneObjectsPosition(currentTime);
        if (_groupTweenAnimator->isDone(currentTime)) {

            _doneMoving = !_doneMoving;

        }
    }
}

void AllAnswersLayout::showSceneObject(qint64 currentTime)
{
    for (auto line : *_barObjects) {
        if (line->getVisible()) {
            line->draw(currentTime);
        }
    }
}

QString AllAnswersLayout::getColorFromRange(const QString &hexFrom, const QString &hexTo, int number) const
{
    QColor colorFrom(hexFrom);
    QColor colorTo(hexTo);

    int hue = mapValue(number, 0, 100, colorFrom.hue(), colorTo.hue());
    int saturation = mapValue(number, 0, 100, colorFrom.saturation(), colorTo.saturation());
    int lightness = mapValue(number, 0, 100, colorFrom.lightness(), colorTo.lightness());

    QColor outputColor;
    outputColor.setHsl(hue, saturation, lightness);

    return outputColor.name();
}

qreal AllAnswersLayout::mapValue(qreal value, qreal istart, qreal istop, qreal ostart, qreal ostop) const
{
    return ostart + (ostop - ostart) * ((value - istart) / (istop - istart));
}
