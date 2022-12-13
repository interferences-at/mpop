#pragma once

#include "sceneobject.h"
#include "viewmodemanager.h"

#include <QOpenGLPaintDevice>
#include <QPainter>
#include <QPen>
#include <QDebug>
#include <QFontDatabase>

/**
 * @brief Draws our text objects.
 */
class TextObjectPainter
{
public:
    TextObjectPainter();
    ~TextObjectPainter();

    // Reset painter
    void setPaintDevice(QOpenGLPaintDevice *device);

    // Draws the X Axis line and text
    void drawHorizontalNumbers();
    // Draws the Y Axis line and text
    void drawVerticalNumbers();
    // Draws top right info
    void drawTopRightInfos();
    // Draws frame per second
    void drawFramePerSecond(const int &framePerSecond);
    // Draws view answer element
    void drawViewElements(ViewModeManager::ViewMode view, const QList<QString> &title = {});
    // Draws test card
    void drawTestCard();

    void beginOpenGLPainting(); // QPainter beginNativePainting wrapper
    void endOpenGLPainting();  // QPainter endNativePainting wrapper

    void setTextLanguage(const QString &lang) { _lang = lang; }

    /**
     * @brief Sets the text for the min and max labels.
     * @param minLabel Text for the min label.
     * @param maxLabel Text for the max label.
     */
    void setMinMaxLabels(const QString &minLabel, const QString& maxLabel) {
        _minLabel = minLabel;
        _maxLabel = maxLabel;
    }

private:
    QPainter _painter;
    QPen _linePen;
    // Font settings
    QFont _fpsTextFont;
    QFont _answersTitlesFont;
    QFont _smallFont;

    int _width;
    int _height;
    int _pixelRatio;

    QString _lang = "en";
    QString _minLabel = "0%"; // Changed dynamically
    QString _maxLabel = "100%"; // Changed dynamically
    // All translatable text
    QMap<QString, QMap<QString, QString>> _texts;

    // Abscissa X Axis
    const int X_AXIS_HEIGHT = 78; // From botton to the horizontal line
    const int X_LINE_LEFT_MARGIN = 76; // == Y_AXIS_WIDTH
    const int X_LINE_RIGHT_MARGIN = 146;

    // Ordinate Y Axis
    const int Y_AXIS_WIDTH = X_LINE_LEFT_MARGIN; // From the left side to the vertical line
    const int Y_LINE_TOP_MARGIN = 50;
    // const int Y_LINE_BOTTOM_MARGIN = X_AXIS_HEIGHT;

    qreal fitToScreenWidth(qreal size) {
        return _width * (size / 1260);
    }

    qreal fitToScreenHeight(qreal size) {
        return _height * (size / 1080);
    }
};
