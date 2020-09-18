#ifndef SCREENSAVER_H
#define SCREENSAVER_H

#include <QQuickWindow>

#include <random>
#include <QRandomGenerator>
#include <QElapsedTimer>

#include <QQuickFramebufferObject>
#include <QOpenGLFramebufferObject>

#include "stick.h"

// Forward declaration
class StickRenderer;

/*
 * This class for integrating OpenGL rendering using
 * a framebuffer object (FBO) with Qt Quick.
*/
class Screensaver : public QQuickFramebufferObject
{
    Q_OBJECT
    /*
     *  Add QML property to Screensaver that allow to disable it while not shown
    */
    Q_PROPERTY(bool render READ getEnable WRITE setEnable NOTIFY renderChanged)

public:
    /*
     * This function will be called on the rendering thread
     *  while the GUI thread is blocked
    */
    Renderer *createRenderer() const override;
    // Getter and Setter of render enable/disable
    bool getEnable() const { return _enable;}
    void setEnable(bool enable);

signals:
    void renderChanged();

private:
    bool _enable;
    static StickRenderer *_stickRenderer;
};

/*
 * This class is used to implement the rendering logic of a QQuickFramebufferObject.
*/
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
    // Enable of disable rendering
    void enableRendering(bool enable) { _enableRendering = enable; }
    bool renderingIsEnabled() const { return _enableRendering; }

protected:
    // This function is called when the FBO should be rendered into
    void render() override;
    // This function is called when a new FBO is needed.
    // This happens on the initial frame
    QOpenGLFramebufferObject *createFramebufferObject(const QSize &size) override;
    // This function is called as a result of QQuickFramebufferObject::update()
    void synchronize(QQuickFramebufferObject *item) override;




private:
    // bar stick vectors
    QVector<Stick*> _barSticks;
    // Random number generator
    std::uniform_real_distribution<qreal> randomX, randomY, randomRadius;
    std::uniform_real_distribution<qreal> randomFrequency, randomRatioRotation;
    // Timer for animation based on time
    QElapsedTimer _timer;

    // Default pixel ratio
    qreal _pixelRatio = 1;
    // Default size
    qreal _width = 1260;
    qreal _height = 1080;

    qreal _left, _right, _top, _bottom = 0;
    // This function convert size from pixel to OpenGL coordinate
    QPointF sizeFromPixel(qreal width, qreal height);
    // Remap values
    qreal mapValue(qreal value, qreal istart, qreal istop, qreal ostart, qreal ostop)
    {
        return ostart + (ostop - ostart) * ((value - istart) / (istop - istart));
    }

    // Enable render
    bool _enableRendering = false;
};

#endif // SCREENSAVER_H
