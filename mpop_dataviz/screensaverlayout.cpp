#include "screensaverlayout.h"

ScreensaverLayout::ScreensaverLayout() {
    _groupTweenAnimator.reset(new GroupTweenAnimator());
    randomX = std::uniform_real_distribution<qreal>(_left * 1.2, _right * 1.2);
    randomY = std::uniform_real_distribution<qreal>(_bottom * 1.2, _top * 1.2);
    randomRadius = std::uniform_real_distribution<qreal>(-1, 1);
    randomFrequency = std::uniform_real_distribution<qreal>(0.01, 0.1);
    randomRatioRotation = std::uniform_real_distribution<qreal>(-0.1, 0.1);
}


void ScreensaverLayout::moveObjectsToLayout(qint64 currentTime) {
    _groupTweenAnimator.reset(new GroupTweenAnimator());
    _groupTweenAnimator->setDuration(1000);
    _groupTweenAnimator->setEasingType(QEasingCurve::InOutQuad);

    QRandomGenerator generator;

    if (!_previousBars.isEmpty() && _previousBars.size() < _barObjects->size()) {
        for (int i = _previousBars.size(); i < _barObjects->size(); i++) {
            qreal posX = randomX(generator);
            qreal posY = randomY(generator);
            std::uniform_real_distribution<qreal> randomRotation(0, 360);
            qreal rotation = randomRotation(generator);
            PrisonerLine::ptr line = _barObjects->at(i);
            _groupTweenAnimator->addSceneObjectToAnimate(line, posX, posY, rotation);
        }
        _groupTweenAnimator->start(currentTime);
    }
    // When everything is done keep track of previous bars data
    _previousBars = *_barObjects;
}

void ScreensaverLayout::updateBarsPosition(qint64 currentTime) {
    QRandomGenerator generator;
    for (auto line : *_barObjects) {
        qreal centerX = randomX(generator);
        qreal centerY = randomY(generator);
        qreal radius = randomRadius(generator);
        qreal frequency = randomFrequency(generator) * _speedRatio;
        qreal ratioRotation = randomRatioRotation(generator) * _speedRatio;

        qreal timeNow = currentTime / 1000.0;

        qreal posX = centerX + cos(timeNow * frequency) * radius;
        qreal posY = centerY + sin(timeNow * frequency) * radius;
        qreal rotation = (currentTime + 10000) * frequency * ratioRotation;

        line->setX(line->getX() + (posX - line->getX()));
        line->setY(line->getY() + (posY - line->getY()));
        line->setRotation(fmod(rotation, 360));
    }

    if (! _groupTweenAnimator.isNull()) {
        _groupTweenAnimator->updateSceneObjectsPosition(currentTime);
        if (_groupTweenAnimator->isDone(currentTime)) {
            _groupTweenAnimator.clear();
        }
    }
}

void ScreensaverLayout::showSceneObject(qint64 currentTime) {
    for (auto line : *_barObjects) {
        if (line->getVisible()) {
            line->draw(currentTime);
        }
    }
}

QSharedPointer<QVector<PrisonerLine::ptr> > ScreensaverLayout::getClosestBars(const QPointF &pos) {
    // Sort bars by the closest to the point(x, y)
    std::sort(std::begin(*_barObjects), std::end(*_barObjects),
              [&](PrisonerLine::ptr a, PrisonerLine::ptr b) -> bool {
        return pow(abs(a->getX() - pos.x()), 2) + pow(abs(a->getY() - pos.y()), 2) <
               pow(abs(b->getX() - pos.x()), 2) + pow(abs(b->getY() - pos.y()), 2);
    });
    return _barObjects;
}

void ScreensaverLayout::setParam(const QString& paramName, float value) {
    if (paramName == "speed") {
        _speedRatio = value;
    }
}

ScreensaverLayout::~ScreensaverLayout()
{

}
