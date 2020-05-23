#pragma once

#include "sceneobject.h"

#include <QOpenGLPaintDevice>
#include <QPainter>
#include <QPen>
#include <QDebug>
#include <QFontDatabase>


class TextObjectPainter
{
public:
    TextObjectPainter();
    ~TextObjectPainter();

    // Reset painter
    void setPaintDevice(QOpenGLPaintDevice *device);
    // Set one or many top titles.
    void setTopTitles(const QList<QString> &titles) { _topTitles = titles; }
    // Set one or many bottom titles
    void setBottomTitles(const QList<QString> &titles) { _bottomTitles = titles; }

    void showTopTitle(bool visible = false); // Show/Hide top title
    void showBottonTitle(bool visible = false); // Show/Hide bottom title

    // Draw the X Axis line and text
    void drawAbscissa(int xMaxValue = 100, int xIntervalUnit = 10);
    // Draw the Y Axis line and text
    void drawOrdinate(int yMaxValue = 100, int yIntervalUnit = 10);
    // Draw the top title
    void drawTopTitles();
    // Draw the bottom title
    void drawButtonTitles();
    // Draw the percentage icon
    void drawPercentage();

    void beginOpenGLPainting(); // QPainter beginNativePainting wrapper
    void endOpenGLPainting();  // QPainter endNativePainting wrapper

private:
    QPainter _painter;

    QPen _linePen;

    // Font settings
    QFont _axisNumberFont;
    QFont _bottomTitleFont;
    QFont _topTitleFont;
    QFont _percentageFont;

    // List of top and bottom titles
    QList<QString> _topTitles;
    QList<QString> _bottomTitles;

    int _width;
    int _height;
    int _pixelRatio;

    bool _showBottomTitle;
    bool _showPercentage;

    // Constantes

    // Abscissa X Axis
    const int X_AXIS_HEIGHT = 60; // From botton to the horizontal line
    const int X_LINE_LEFT_MARGIN = 60; // == Y_AXIS_WIDTH
    const int X_LINE_RIGHT_MARGIN = 30;

    // Ordinate Y Axis
    const int Y_AXIS_WIDTH = 60; // From the left side to the vertical line
    const int Y_LINE_TOP_MARGIN = 30;
    const int Y_LINE_BOTTOM_MARGIN = 60;

    // Titles text boxes height
    const int TITLES_HEIGHT = 60;
};
