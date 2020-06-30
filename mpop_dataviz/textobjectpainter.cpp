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

    _numbersFont = QFont(fontFamily, 11, QFont::DemiBold);
    _bottomTitleFont = QFont(fontFamily, 24);
    _topTitleFont = QFont(fontFamily, 22);
    _percentageFont = QFont(fontFamily, 22);
    _fpsTextFont = QFont(fontFamily, 12, QFont::DemiBold);
    _answersTitlesFont = QFont(fontFamily, 14, QFont::DemiBold);
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

    _painter.setViewport(0, 0, _width / _pixelRatio, _height / _pixelRatio);
}

void TextObjectPainter::drawHorizontalNumbers()
{
    _painter.save(); // Start of using painter
    qreal rowHeight = fitToScreenHeight(32);
    qreal startY = _height - fitToScreenHeight(X_AXIS_HEIGHT);
    qreal numberRectWidth = fitToScreenHeight(35) / 3.6 * 11;
    QRect hNumbersRect(0, 0, numberRectWidth, rowHeight);

    _painter.setFont(_numbersFont);
    _painter.translate(Y_AXIS_WIDTH - numberRectWidth / 8, startY);
    for (int hUnit = 10; hUnit <= 100; hUnit += 10) {
        _painter.drawText(hNumbersRect, Qt::AlignRight | Qt::AlignBottom, QString::number(hUnit));
        _painter.translate(numberRectWidth, 0);
    }
    _painter.restore(); // Finish using painter
}

void TextObjectPainter::drawVerticalNumbers()
{
    _painter.save(); // Start of using painter
    qreal columnHeight = fitToScreenHeight(45.5 * 20);
    qreal startY = fitToScreenHeight(102);

    QRect vNumbersRect(0, 0, Y_AXIS_WIDTH, columnHeight / 10);
    _painter.translate(0, startY);
     _painter.setFont(_numbersFont);
    for (int vUnit = 100; vUnit >= 10; vUnit -= 10) {
        _painter.drawText(vNumbersRect, Qt::AlignTop | Qt::AlignHCenter, QString::number(vUnit));
        _painter.translate(0, columnHeight / 10);
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

void TextObjectPainter::drawFramePerSecond(const int &framePerSecond)
{
    _painter.save();
    _painter.setFont(_fpsTextFont);
    _painter.drawText(10, 10, QString::number(framePerSecond) + " FPS");
//    _painter.drawLine(0, 0, _width, _height);
//    _painter.drawLine(_width, 0, 0, _height);
    _painter.setPen(QPen(Qt::red, 1));
    //    for (int x = 0; x < _width; x++) {
    //        for (int y = 0; y < _height; y++) {
    //            _painter.drawLine(y * 10, 0, y * 10, _height);
    //            _painter.drawLine(0, x * 10, _width, x * 10);
    //        }
    //    }
    _painter.restore();
}

void TextObjectPainter::drawViewElements(ViewModeManager::ViewMode view, const QList<QString> &title)
{
    _painter.save();
    qreal vLineMrgLeft = Y_AXIS_WIDTH;
    qreal vLineMrgTop = fitToScreenHeight(Y_LINE_TOP_MARGIN);
    qreal hLineMrgBottom = fitToScreenHeight(X_AXIS_HEIGHT);
    qreal hLineMrgRight  = fitToScreenWidth(X_LINE_RIGHT_MARGIN);

    _painter.setPen(_linePen);
    _painter.drawLine(vLineMrgLeft, vLineMrgTop, vLineMrgLeft, _height - hLineMrgBottom);
    _painter.drawLine(vLineMrgLeft + 17, _height - hLineMrgBottom, _width - hLineMrgRight, _height - hLineMrgBottom);

    auto drawRangeBottomText = [&]() {
        qreal bottomTitlePosY = _height - fitToScreenHeight(hLineMrgBottom - 32);
        _painter.drawText(64, bottomTitlePosY, "Pas du tout");
        _painter.drawText(_width - 170, bottomTitlePosY, "Tout à fait");
    };

    switch (view) {
    case ViewModeManager::MultiAnswersMode:
    {
        int y = fitToScreenHeight(114);
        int interval = fitToScreenHeight(148 + 35);
        int marginLeft = 93;

        _painter.setFont(_answersTitlesFont);
        for (int i = 0; i < title.size(); i++) {
            _painter.drawText(marginLeft, y + i * interval, title.at(i));
        }
        drawRangeBottomText();
    }
        break;
    case ViewModeManager::AnswerByAgeMode:
    {
        QRect ageTextRect(0, 0, Y_AXIS_WIDTH, Y_LINE_TOP_MARGIN);
        _painter.setFont(_topTitleFont);
        _painter.drawText(ageTextRect, Qt::AlignBottom | Qt::AlignCenter, "Âge");
        drawVerticalNumbers();
        drawHorizontalNumbers();
        QRect responseTextRect(0, 0, _width, X_AXIS_HEIGHT);
        _painter.setFont(_bottomTitleFont);
        _painter.translate(0, _height - X_AXIS_HEIGHT);
        _painter.drawText(responseTextRect, Qt::AlignBottom | Qt::AlignCenter, "Réponses (%)");
        break;
    }
    case ViewModeManager::AnswerByGenderMode:
    {
        int y = fitToScreenHeight(213.5);
        int interval = fitToScreenHeight(213 + 35);
        int marginLeft = 93;

        _painter.setFont(_answersTitlesFont);
        for (int i = 0; i < title.size(); i++) {
            _painter.drawText(marginLeft, y + i * interval, title.at(i));
        }
        drawRangeBottomText();
    }
        break;
    default:
        break;
    }

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
