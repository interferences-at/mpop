#pragma once

#include "sceneobject.h"
#include <QtGui/QOpenGLShaderProgram>
#include <QElapsedTimer>
#include <QVector>
#include "barchartlayout.h"
#include <QOpenGLWindow>
#include <QOpenGLPaintDevice>
#include <QKeyEvent>
#include <QElapsedTimer>
#include <QTimer>

#include "textobjectpainter.h"
#include "screensaverlayout.h"

#include "viewmodemanager.h"


class DatavizWindow : public QOpenGLWindow {
    Q_OBJECT
public:
    DatavizWindow();
    ~DatavizWindow() override;
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
    /**
     * @brief Shows a bar chart with the given values.
     * @param bars List of number values for each column.
     */
    void showBarChartBars(const QList<int>& bars);

    qint64 elapsed() const;
    // Set Offset ID
    void setOffsetId(uint offset) { windowId += offset; }
    void setWindowId(uint id) { windowId = id; }
    uint getWindowId() const { return windowId; }

    ViewModeManager *viewManager() { return ViewModeManager::viewManager(); }

    // Alias for shared pointer of this class
    typedef QSharedPointer<DatavizWindow> ptr;

    ScreensaverLayout* getScreensaver() { return &(this->_screensaverLayout); }

protected:
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void updateFramePerSecond();

private:
    QElapsedTimer _elapsedTimer;
    QVector<SceneObject::ptr> _sceneObjects;
    BarChartLayout _barChartLayout;
    ScreensaverLayout _screensaverLayout;

    ViewModeManager *_viewModeManager;

    QOpenGLPaintDevice *_device;

    TextObjectPainter *_painter;
    // Window ID
    uint windowId;
    // TODO: add current layout (enum)
    // TODO: ajouter plusieurs objets TextSceneObject

    // Show/Hide FPS
    bool _showFPS = false;

    QElapsedTimer _frameTimer;
    QTimer _updateIntervalTimer;
    int _framesCount = 0;
    int _framePerSecond = 0;
};
