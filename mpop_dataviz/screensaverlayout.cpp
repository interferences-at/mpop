#include "screensaverlayout.h"

ScreensaverLayout::ScreensaverLayout()
{
    _groupTweenAnimator.reset(new GroupTweenAnimator());

}

void ScreensaverLayout::moveObjectsToLayout(qint64 currentTime)
{
    _groupTweenAnimator.reset(new GroupTweenAnimator());
    _groupTweenAnimator->setDuration(1000);
    _groupTweenAnimator->setEasingType(QEasingCurve::InOutQuad);

    QRandomGenerator randomGenerator;

    if (!_previousBars.isEmpty() && _previousBars.size() < _prisonerLines->size()) {

        for (int i = _previousBars.size(); i < _prisonerLines->size(); i++) {

            std::uniform_real_distribution<qreal> randomCenterX(_left, _right);
            qreal centerX = randomCenterX(randomGenerator);
            std::uniform_real_distribution<qreal> randomCenterY(_bottom, _top);
            qreal centerY = randomCenterY(randomGenerator);

            qreal radius = randomCenterX(randomGenerator);

            std::uniform_real_distribution<qreal> randomFrequency(0.01, 0.1);
            qreal frequency = randomFrequency(randomGenerator);

            std::uniform_real_distribution<qreal> randomRatioRotation(-0.1, 0.1);
            qreal ratioRotation = randomRatioRotation(randomGenerator);

            qreal timeNow = currentTime / 1000.0;

            qreal posX = centerX + cos(timeNow * frequency) * radius;
            qreal posY = centerY + sin(timeNow * frequency) * radius;
            qreal rotation = currentTime * frequency * ratioRotation;

            _groupTweenAnimator->addSceneObjectToAnimate(_prisonerLines->at(i), posX, posY, rotation);
        }
    }

    _groupTweenAnimator->start(currentTime);

    // When everything is done keep track of previous bars data
    _previousBars = *_prisonerLines;
}

void ScreensaverLayout::updateBarsPosition(qint64 currentTime)
{
    QRandomGenerator randomGenerator;

    for (auto line : *_prisonerLines) {

        std::uniform_real_distribution<qreal> randomCenterX(_left, _right);
        qreal centerX = randomCenterX(randomGenerator);
        std::uniform_real_distribution<qreal> randomCenterY(_bottom, _top);
        qreal centerY = randomCenterY(randomGenerator);

        qreal radius = randomCenterX(randomGenerator);

        std::uniform_real_distribution<qreal> randomFrequency(0.01, 0.1);
        qreal frequency = randomFrequency(randomGenerator);

        std::uniform_real_distribution<qreal> randomRatioRotation(-0.1, 0.1);
        qreal ratioRotation = randomRatioRotation(randomGenerator);

        qreal timeNow = currentTime / 1000.0;

        qreal posX = centerX + cos(timeNow * frequency) * radius;
        qreal posY = centerY + sin(timeNow * frequency) * radius;
        qreal rotation = currentTime * frequency * ratioRotation;

        line->setX(posX);
        line->setY(posY);
        line->setRotation(rotation);
    }

    if (! _groupTweenAnimator.isNull()) {
        _groupTweenAnimator->updateSceneObjectsPosition(currentTime);
        if (_groupTweenAnimator->isDone(currentTime)) {
            _groupTweenAnimator.clear();
        }
    }

}

void ScreensaverLayout::showSceneObject(qint64 currentTime)
{
    for (auto line : *_prisonerLines) {
        if (line->getVisible()) {

            line->draw(currentTime);
        }
    }
}

QVector<PrisonerLine::ptr> ScreensaverLayout::getLayoutClosestBars(const QPointF &pos)
{
    qreal pointX = pos.x();
    qreal pointY = pos.y();

    // Sort bars by the closest to the point(x, y)
    std::sort(_prisonerLines->begin(), _prisonerLines->end(),
              [&](PrisonerLine::ptr a, PrisonerLine::ptr b) -> bool {
        return pow(abs(a->getX() - pointX), 2) + pow(abs(a->getY() - pointY), 2) <
                pow(abs(b->getX() - pointX), 2) + pow(abs(b->getY() - pointY), 2);
    });

    return *_prisonerLines;
}

ScreensaverLayout::~ScreensaverLayout()
{

}
