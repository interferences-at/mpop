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
    ~DatavizWindow() override {};
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    qint64 elapsed() const;
    // Set Offset ID
    void setOffsetId(uint offset) { windowId += offset; }
    void setWindowId(uint id) { windowId = id; }
    uint getWindowId() const { return windowId; }

    void setTextPainterLanguage(const QString &lang) { _painter->setTextLanguage(lang); }

    void setMinMaxLabels(const QString &minLabel, const QString& maxLabel) {
        _painter->setMinMaxLabels(minLabel, maxLabel);
    }

    ViewModeManager *viewManager() { return _viewModeManager; }

    // Show/Hide test card
    void toggleTestCard();
    void setTestCardVisibility(bool visible);
    void restartTestCardTimer();

    // Alias for shared pointer of this class
    typedef QSharedPointer<DatavizWindow> ptr;

signals:
    bool closed();

protected:
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void updateFramePerSecond();

private:
    QElapsedTimer _elapsedTimer;
    QVector<SceneObject::ptr> _sceneObjects;

    ViewModeManager *_viewModeManager;

    QOpenGLPaintDevice *_device;

    TextObjectPainter *_painter;
    // Window ID
    uint windowId;
    // TODO: add current layout (enum)
    // TODO: ajouter plusieurs objets TextSceneObject

    // Show/Hide FPS
    bool _showHUD = false;
    // Show/Hide test card
    bool _showTestCard = false;

    QElapsedTimer _frameTimer;
    QTimer _updateIntervalTimer;
    int _framesCount = 0;
    int _framePerSecond = 0;

    QElapsedTimer _testCardTimer;
};
