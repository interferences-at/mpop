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
    /**
     * @brief Shows a bar chart with the given values.
     * @param bars List of number values for each column.
     */
    void showBarChartBars(const QList<int>& bars);

    qint64 elapsed() const;

private:
    QElapsedTimer _elapsedTimer;
    QVector<SceneObject::ptr> _sceneObjects;
    BarChartLayout _barChartLayout;
};
