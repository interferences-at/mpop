#pragma once

#include "sceneobject.h"
#include <QtGui/QOpenGLShaderProgram>
#include <QElapsedTimer>
#include <QVector>
#include "barchartlayout.h"
#include <QOpenGLWindow>


class DatavizWindow : public QOpenGLWindow {
public:
    DatavizWindow();
    ~DatavizWindow() override;
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
    void showBarChartBars(const QList<int>& bars);
    void setAnimating(bool animating);

private:
    QElapsedTimer _elapsedTimer;
    QVector<SceneObject*> _sceneObjects;
    BarChartLayout _barChartLayout;
    bool _is_animating;
};
