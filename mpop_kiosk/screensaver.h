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
class Framebuffer : public QQuickFramebufferObject
{
    Q_OBJECT
    /*
     *  Add QML property that allow to disable the rendering while the widget is not shown
    */
    Q_PROPERTY(bool render READ getEnable WRITE setEnable NOTIFY renderChanged)

public:
    Framebuffer(QQuickItem *parent = nullptr);

    // Getter and Setter of render enable/disable
    virtual bool getEnable() const = 0;
    virtual void setEnable(bool enable) = 0;

signals:
    void renderChanged();

protected:
    StickRenderer *_stickRenderer;

};

/*
 * This class for integrating OpenGL rendering using
 * a framebuffer object (FBO) with Qt Quick.
*/
class Screensaver : public Framebuffer
{
    Q_OBJECT
    /*
     *  Add QML property to Screensaver that allow to disable it while not shown
    */
//    Q_PROPERTY(bool render READ getEnable WRITE setEnable NOTIFY renderChanged)

public:
    /*
     * This function will be called on the rendering thread
     *  while the GUI thread is blocked
    */
    Renderer *createRenderer() const override;

    bool getEnable() const override;
    void setEnable(bool enable) override;
};

/*
 * This class for integrating OpenGL rendering using
 * a framebuffer object (FBO) with Qt Quick for the barchart layout.
*/
class AnswersView : public Screensaver
{
    Q_OBJECT
    /*
     *  Add QML property to ResponseBars that expose the responses number
    */
    Q_PROPERTY(QVariant myAnswers READ getUserBars WRITE setUserBars NOTIFY userBarsChanged)
    Q_PROPERTY(QVariant theirAnswers READ getTheirBars WRITE setTheirBars NOTIFY theirBarsChanged)

public:
    enum Answers {
        UserAnswer,
        TheirAnswer
    };

    /*
     * This function will be called on the rendering thread
     *  while the GUI thread is blocked
    */
    Renderer *createRenderer() const override;

    // Overriding parent method
    bool getEnable() const override;
    void setEnable(bool enable) override;

    QVariant getUserBars() const;
    void setUserBars(const QVariant &bars);

    QVariant getTheirBars() const;
    void setTheirBars(const QVariant &bars);

signals:
    void userBarsChanged();
    void theirBarsChanged();
};

/*
 * This class is used to implement the rendering logic of a QQuickFramebufferObject.
*/
class StickRenderer : public QQuickFramebufferObject::Renderer, protected QOpenGLFunctions
{
public:

    enum DatavizLayout {
        ScreensaverLayout,
        AnswersViewLayout
    };

    StickRenderer();
    // Get elapsed time since the screensaver is displayed
    qint64 elapsed() const { return _timer.elapsed(); }

    // Init, resize event and paint event
    void initializeGLCanvas();
    void resizeGLCanvas(int width, int height);
    void paintGLCanvas();

    // Set bars vector
    void setBars(int answerIndex, const QVariant &bars);

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
    // This function convert to OpenGL coordinate a given pixel position
    QPointF coordinateFromPixel(qreal x, qreal y);

    // Enable render
    QVector<bool> _render;

    //Barchart answers bars
    QVector<QVariant> _answersRows;

    QVector<Stick*> _answersSticks;

    void paintScreensaver();
    void paintAnswersView();

    // Allow these class to use this class members
    friend class Screensaver;
    friend class AnswersView;

    DatavizLayout _currentView;
};

#endif // SCREENSAVER_H
