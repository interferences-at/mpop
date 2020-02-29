#pragma once

#include <QtGui/QWindow>
#include <QtGui/QOpenGLFunctions>

// Forward declarations:
QT_BEGIN_NAMESPACE
class QPainter;
class QOpenGLContext;
class QOpenGLPaintDevice;
QT_END_NAMESPACE


/**
 * @brief Base class for a window that renders an OpenGL scene.
 */
class OpenGLWindow : public QWindow, protected QOpenGLFunctions {
    Q_OBJECT
public:
    explicit OpenGLWindow(QWindow* parent = nullptr);
    ~OpenGLWindow() override;

    virtual void render(QPainter* painter);
    virtual void render();
    virtual void initialize();
    void setAnimating(bool animating);

public slots:
    void renderLater();
    void renderNow();

protected:
    bool event(QEvent* event) override;
    void exposeEvent(QExposeEvent* event) override;

private:
    bool _is_animating;
    QOpenGLContext* _context;
    QOpenGLPaintDevice* _device;
};
