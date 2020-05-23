#pragma once

#include "sceneobject.h"
#include <QtGui/QOpenGLShaderProgram>
#include <QElapsedTimer>
#include <QVector>
#include "barchartlayout.h"
#include <QOpenGLWindow>
#include <QOpenGLPaintDevice>

#include "textobjectpainter.h"


class DatavizWindow : public QOpenGLWindow {
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

    void addLayoutTitles(const QList<QString> &titles, bool topTitle = false);

    qint64 elapsed() const;
    // Set Offset ID
    void setOffsetId(uint offset) { windowId += offset; }
    void setWindowId(uint id) { windowId = id; }
    uint getWindowId() const { return windowId; }

    // Alias for shared pointer of this class
    typedef QSharedPointer<DatavizWindow> ptr;

private:
    QElapsedTimer _elapsedTimer;
    QVector<SceneObject::ptr> _sceneObjects;
    BarChartLayout _barChartLayout;

    QOpenGLPaintDevice *_device;

    TextObjectPainter *_painter;
    // Window ID
    uint windowId;
    // TODO: add current layout (enum)
    // TODO: ajouter plusieurs objets TextSceneObject
};
