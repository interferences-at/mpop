#include "textobjectpainter.h"
#include <QPaintEngine>

TextObjectPainter::TextObjectPainter() :
    _showBottomTitle(true), // TODO: Make it false by default
    _showPercentage(true) // TODO: Make it false by default
{
    _linePen = QPen(Qt::white, 2); // Line style

    // Add a custom font
    int id = QFontDatabase::addApplicationFont(":/base-font");
    QString fontFamily = QFontDatabase::applicationFontFamilies(id).at(0);

    _axisNumberFont = QFont(fontFamily, 11, QFont::DemiBold);
    _bottomTitleFont = QFont(fontFamily, 24);
    _topTitleFont = QFont(fontFamily, 22);
    _percentageFont = QFont(fontFamily, 22);
}

TextObjectPainter::~TextObjectPainter()
{
    _painter.end();
}

void TextObjectPainter::setPaintDevice(QOpenGLPaintDevice *device)
{
    if (_painter.isActive())
        _painter.end();

    _painter.begin(device);
    _painter.setPen(Qt::white);
    _painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

    _width = device->width(); // Reset width
    _height = device->height(); // Reset height;
    _pixelRatio = device->devicePixelRatio();
}

void TextObjectPainter::drawAbscissa(int xMaxValue, int xIntervalUnit)
{
    _painter.save(); // Start of using painter
    // Compute new height if we decide to show bottom title
    int height = _showBottomTitle ? _height - TITLES_HEIGHT : _height;
    // The length of the horizontal line
    int lineWidth = _width - (X_LINE_LEFT_MARGIN + X_LINE_RIGHT_MARGIN);
    // Number of interval eg: maxValue(100) / intervalUnit(10) = 10 intervals
    int intervalCount = xMaxValue / xIntervalUnit; // How many interval exist
    QRect xAxisTextRect(0, 0, lineWidth / intervalCount, X_AXIS_HEIGHT);
    _painter.setPen(_linePen);
    _painter.drawLine(QPoint(X_LINE_LEFT_MARGIN, height - X_AXIS_HEIGHT),
                      QPoint(_width - X_LINE_RIGHT_MARGIN, height - X_AXIS_HEIGHT));
    _painter.translate(X_LINE_LEFT_MARGIN, height - X_AXIS_HEIGHT);
    _painter.setFont(_axisNumberFont);
    for (int xAxis = xIntervalUnit; xAxis <= xMaxValue; xAxis += xIntervalUnit) {
        _painter.drawText(xAxisTextRect, Qt::AlignRight | Qt::AlignVCenter, QString::number(xAxis));
        _painter.translate(lineWidth / intervalCount, 0);
    }
    _painter.restore(); // Finish using painter
}

void TextObjectPainter::drawOrdinate(int yMaxValue, int yIntervalUnit)
{
    _painter.save(); // Start of using painter
    // Compute new height if we decide to show bottom title
    int height = _showBottomTitle ? _height - TITLES_HEIGHT : _height;
    int yLineTopMargin = _showPercentage ? Y_AXIS_WIDTH : Y_LINE_TOP_MARGIN;
    // The height of the vertical line
    int lineHeight = height - (yLineTopMargin + Y_LINE_BOTTOM_MARGIN);
    // Number of interval eg: maxValue(100) / intervalUnit(10) = 10 intervals
    int intervalCount = yMaxValue / yIntervalUnit;
    QRect yAxisTextRect(0, 0, Y_AXIS_WIDTH, lineHeight / intervalCount);
    _painter.setPen(_linePen);
    _painter.drawLine(QPoint(Y_AXIS_WIDTH, yLineTopMargin),
                      QPoint(Y_AXIS_WIDTH, height - Y_LINE_BOTTOM_MARGIN));
    _painter.translate(0, yLineTopMargin);
     _painter.setFont(_axisNumberFont);
    for (int yAxis = yMaxValue; yAxis >= yIntervalUnit; yAxis -= yIntervalUnit) {
        _painter.drawText(yAxisTextRect, Qt::AlignTop | Qt::AlignHCenter, QString::number(yAxis));
        _painter.translate(0, lineHeight / intervalCount);
    }
    _painter.restore(); // Finish using painter
}

void TextObjectPainter::drawTopTitles()
{
    _painter.save(); // Save painter state
    int lineWidth = _width - (X_LINE_LEFT_MARGIN + X_LINE_RIGHT_MARGIN);
    int titleCount = _topTitles.size();
    QRect titleTextRect(0, 0, lineWidth / titleCount, TITLES_HEIGHT);
    _painter.translate(X_LINE_LEFT_MARGIN, 0);
    _painter.setFont(_topTitleFont);
    for (int i = 0; i < titleCount; i++) {
        _painter.drawText(titleTextRect, Qt::AlignBottom | Qt::AlignCenter, _topTitles.at(i));
        _painter.translate(lineWidth / titleCount, 0);
    }
    _painter.restore(); // Restore painter state
}

void TextObjectPainter::drawButtonTitles()
{
    _painter.save(); // Start of using painter
    int lineWidth = _width - (X_LINE_LEFT_MARGIN + X_LINE_RIGHT_MARGIN);
    int titleCount = _bottomTitles.size();
    QRect titleTextRect(0, 0, lineWidth / titleCount, TITLES_HEIGHT);
    _painter.translate(X_LINE_LEFT_MARGIN, _height - TITLES_HEIGHT);
    _painter.setFont(_bottomTitleFont);
    for (int i = 0; i < titleCount; i++) {
        _painter.drawText(titleTextRect, Qt::AlignCenter, _bottomTitles.at(i));
        _painter.translate(lineWidth / titleCount, 0);
    }
    _painter.restore(); // Finish using painter
}

void TextObjectPainter::drawPercentage()
{
    _painter.save();
    QRect percentageRect(0, 0, Y_AXIS_WIDTH, Y_AXIS_WIDTH);
    _painter.setFont(_percentageFont);
    _painter.drawText(percentageRect, Qt::AlignCenter, QString("%"));
    _painter.restore();
}

void TextObjectPainter::beginOpenGLPainting()
{
    // It seems like beginNativePainting makes all
    // the OpenGL drawing invisible on macOS and Linux,
    // but it seems ok on Windows.
#if TARGET_OS_WIN32
    _painter.beginNativePainting();
#endif
}

void TextObjectPainter::endOpenGLPainting()
{
#if TARGET_OS_WIN32
    _painter.endNativePainting();

#endif
}
