#include "textobjectpainter.h"
#include <QPaintEngine>

TextObjectPainter::TextObjectPainter()
{
    _linePen = QPen(Qt::white, 2); // Line style

    /* This font family needs to be install
     * sh_install_trim_font_linux.sh for Linux
     * ps_install_trim_font_windows.ps1 for Windows 10
     */
    QString fontFamily = "Trim SemiBold";

    _fpsTextFont = QFont(fontFamily, 12, QFont::DemiBold);
    _answersTitlesFont = QFont(fontFamily, 14, QFont::DemiBold);
    _smallFont = QFont(fontFamily, 11, QFont::DemiBold);

    // Moyenne text
    _texts["average"].insert("en", "AVERAGE");
    _texts["average"].insert("fr", "MOYENNES");
    // Ma reponse text
    _texts["my_answer"].insert("en", "MY ANSWER");
    _texts["my_answer"].insert("fr", "MA RÉPONSE");
    // Pas du tout text
    _texts["not_at_all"].insert("en", "Not at all");
    _texts["not_at_all"].insert("fr", "Pas du tout");
    // Moyenne text
    _texts["absolutely"].insert("en", "Absolutely");
    _texts["absolutely"].insert("fr", "Tout à fait");
    // Age text
    _texts["age"].insert("en", "Age");
    _texts["age"].insert("fr", "Âge");
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
    qreal numberRectWidth = fitToScreenHeight(35) / 3.7 * 11;
    QRect hNumbersRect(0, 0, numberRectWidth, rowHeight);

    _painter.setFont(_smallFont);
    _painter.translate((Y_AXIS_WIDTH + 7) - numberRectWidth / 8, startY);
    for (int hUnit = 10; hUnit <= 100; hUnit += 10) {
        _painter.drawText(hNumbersRect, Qt::AlignRight | Qt::AlignBottom, QString::number(hUnit));
        _painter.translate(numberRectWidth, 0);
    }
    _painter.restore(); // Finish using painter
}

void TextObjectPainter::drawVerticalNumbers()
{
    _painter.save(); // Start of using painter
    qreal columnHeight = fitToScreenHeight(47.5 * 20);
    qreal startY = fitToScreenHeight(58);

    QRect vNumbersRect(0, 0, Y_AXIS_WIDTH, columnHeight / 10);
    _painter.translate(0, startY);
     _painter.setFont(_smallFont);
    for (int vUnit = 100; vUnit >= 10; vUnit -= 10) {
        _painter.drawText(vNumbersRect, Qt::AlignTop | Qt::AlignHCenter, QString::number(vUnit));
        _painter.translate(0, columnHeight / 10);
    }
    _painter.restore(); // Finish using painter
}

void TextObjectPainter::drawTopRightInfos()
{

    QRect infoRect(_width - fitToScreenWidth(X_LINE_RIGHT_MARGIN),
                   fitToScreenHeight(50),
                   fitToScreenHeight(X_LINE_RIGHT_MARGIN),
                   fitToScreenWidth(65));

    QRect barStick(infoRect.x() + infoRect.width() / 2, infoRect.y(), 3, 35);
    QRect textRect(infoRect.x(), (infoRect.y() + infoRect.height()) - 20, infoRect.width(), 20);
    _painter.save();
    _painter.setFont(_smallFont);
    _painter.setPen(QPen(Qt::white, 3.5));
    _painter.drawLine(barStick.x() + barStick.width() / 2, barStick.y(),
                      barStick.x() + barStick.width() / 2, barStick.y() + 35);
    _painter.drawText(textRect, Qt::AlignCenter | Qt::AlignBottom, _texts["average"][_lang]);

    _painter.translate(0, 95);
    _painter.setPen(QPen(QColor("#80E2A7"), 3.5));
    _painter.drawLine(barStick.x() + barStick.width() / 2, barStick.y(),
                      barStick.x() + barStick.width() / 2, barStick.y() + 35);
    _painter.drawText(textRect, Qt::AlignCenter | Qt::AlignBottom, _texts["my_answer"][_lang]);
    _painter.restore();
}

void TextObjectPainter::drawFramePerSecond(const int &framePerSecond)
{
    _painter.save();
    _painter.setFont(_fpsTextFont);
    _painter.drawText(30, 30, QString::number(framePerSecond) + " FPS");
    _painter.setPen(QPen(Qt::red, 1));
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
    _painter.drawLine(vLineMrgLeft + 24, _height - hLineMrgBottom, _width - hLineMrgRight, _height - hLineMrgBottom);

    auto drawRangeBottomText = [&]() {
        _painter.setFont(_smallFont);
        qreal bottomTitlePosY = _height - fitToScreenHeight(hLineMrgBottom - 32);
        _painter.drawText(97, bottomTitlePosY, _texts["not_at_all"][_lang].toUpper());
        _painter.drawText(_width - (hLineMrgRight + 75), bottomTitlePosY, _texts["absolutely"][_lang].toUpper());
    };

    auto drawResponseBottomText = [&]() {
        QRect responseTextRect(0, 0, hLineMrgRight, fitToScreenHeight(32));
        _painter.setFont(_smallFont);
        _painter.translate(_width - hLineMrgRight, _height - fitToScreenHeight(X_AXIS_HEIGHT));
        _painter.drawText(responseTextRect, Qt::AlignHCenter | Qt::AlignBottom, "(%)");
    };

    drawTopRightInfos();

    switch (view) {
    case ViewModeManager::MultiAnswersMode:
    {
        int y = fitToScreenHeight(122);
        int interval = fitToScreenHeight(154 + 35);
        int marginLeft = 97;

        _painter.setFont(_answersTitlesFont);
        for (int i = 0; i < title.size(); i++) {
            _painter.drawText(marginLeft, y + i * interval, title.at(i).toUpper());
        }
        drawRangeBottomText();
    }
        break;
    case ViewModeManager::AnswerByAgeMode:
    {
        QRect ageTextRect(0, 0, Y_AXIS_WIDTH, Y_LINE_TOP_MARGIN);
        _painter.setFont(_smallFont);
        _painter.drawText(ageTextRect, Qt::AlignTop | Qt::AlignCenter, _texts["age"][_lang]);
        drawVerticalNumbers();
        drawHorizontalNumbers();
        drawResponseBottomText();
        break;
    }
    case ViewModeManager::AnswerByGenderMode:
    {
        int y = fitToScreenHeight(310);
        int interval = fitToScreenHeight(153 + 35);
        int marginLeft = 97;

        _painter.setFont(_answersTitlesFont);
        for (int i = 0; i < title.size(); i++) {
            _painter.drawText(marginLeft, y + i * interval, title.at(i).toUpper());
        }
        drawRangeBottomText();
    }
        break;
    case ViewModeManager::AnswerByCultureMode:
    {
        int y = fitToScreenHeight(110);
        int interval = fitToScreenHeight(115 + 35);
        int marginLeft = 97;

        _painter.setFont(_answersTitlesFont);
        for (int i = 0; i < title.size(); i++) {
            _painter.drawText(marginLeft, y + i * interval, title.at(i));
        }
        drawHorizontalNumbers();
        drawResponseBottomText();
    }
        break;
    default:
        break;
    }

    _painter.restore();
}

void TextObjectPainter::drawTestCard()
{
    _painter.drawPixmap(0, 0, _width, _height, QPixmap(":/test-card"));
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
