#ifndef SCREENSAVER_H
#define SCREENSAVER_H

#include <QQuickWindow>

#include <random>
#include <QRandomGenerator>
#include <QElapsedTimer>

#include <QQuickFramebufferObject>
#include <QOpenGLFramebufferObject>

#include "stick.h"

class Screensaver : public QQuickFramebufferObject
{
    Q_OBJECT

public:
    Renderer *createRenderer() const override;
};

class StickRenderer : public QQuickFramebufferObject::Renderer, protected QOpenGLFunctions
{
public:
    StickRenderer();
    // Get elapsed time since the screensaver is displayed
    qint64 elapsed() const { return _timer.elapsed(); }

    // Init, resize event and paint event
    void initializeGLCanvas();
    void resizeGLCanvas(int width, int height);
    void paintGLCanvas();

protected:
    void render() override;
    QOpenGLFramebufferObject *createFramebufferObject(const QSize &size) override;
    void synchronize(QQuickFramebufferObject *item) override;




private:
    QVector<Stick*> _barSticks;

    std::uniform_real_distribution<qreal> randomX, randomY, randomRadius;
    std::uniform_real_distribution<qreal> randomFrequency, randomRatioRotation;

    QElapsedTimer _timer;

    qreal _pixelRatio = 1;
    qreal _width = 1260;
    qreal _height = 1080;

    qreal _left, _right, _top, _bottom = 0;

    QPointF sizeFromPixel(qreal width, qreal height);

    qreal mapValue(qreal value, qreal istart, qreal istop, qreal ostart, qreal ostop)
    {
        return ostart + (ostop - ostart) * ((value - istart) / (istop - istart));
    }
};

#endif // SCREENSAVER_H
