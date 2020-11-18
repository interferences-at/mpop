#include "screensaver.h"

Framebuffer::Framebuffer(QQuickItem *parent) : QQuickFramebufferObject(parent)
{
    _stickRenderer = new StickRenderer;
}
////////////////////////////////////////////////////////////////////
QQuickFramebufferObject::Renderer *Screensaver::createRenderer() const
{
    // Init OpenGL
    _stickRenderer->initializeGLCanvas();

    return _stickRenderer;
}

bool Screensaver::getEnable() const
{
    return _stickRenderer->_render.at(_stickRenderer->ScreensaverLayout);
}

void Screensaver::setEnable(bool enable)
{
    if (enable) {
        _stickRenderer->_currentView = _stickRenderer->ScreensaverLayout;
    }

    _stickRenderer->_render[_stickRenderer->ScreensaverLayout] = enable;

    emit renderChanged();
}
////////////////////////////////////////////////////////////////////
QQuickFramebufferObject::Renderer *AnswersView::createRenderer() const
{
    // Init OpenGL
    _stickRenderer->initializeGLCanvas();

    return _stickRenderer;
}

bool AnswersView::getEnable() const
{
    return _stickRenderer->_render.at(_stickRenderer->AnswersViewLayout);
}

void AnswersView::setEnable(bool enable)
{
    if (enable) {
        _stickRenderer->_currentView = _stickRenderer->AnswersViewLayout;
    }

    _stickRenderer->_render[_stickRenderer->AnswersViewLayout] = enable;

    emit renderChanged();
}

QVariant AnswersView::getUserBars() const
{
    return _stickRenderer->_answersRows.at(UserAnswer);
}

void AnswersView::setUserBars(const QVariant &bars)
{
    _stickRenderer->setBars(UserAnswer, bars);

    emit userBarsChanged();
}

QVariant AnswersView::getTheirBars() const
{
    return _stickRenderer->_answersRows.at(TheirAnswer);
}

void AnswersView::setTheirBars(const QVariant &bars)
{
    _stickRenderer->setBars(TheirAnswer, bars);

    emit theirBarsChanged();
}
////////////////////////////////////////////////////////////////////
StickRenderer::StickRenderer() :
    _currentView(ScreensaverLayout)
{

    _barSticks = QVector<Stick*>();
    // Reserve 150 bar sticks
    for (int i = 0; i < 150; i++) {
        Stick *stick = new Stick;
        stick->setColor("#FFFFFF");
        _barSticks.push_back(stick);
    }
    // Start timer
    _timer.start();

    randomRadius = std::uniform_real_distribution<qreal>(-1, 1);
    randomFrequency = std::uniform_real_distribution<qreal>(0.01, 0.1);
    randomRatioRotation = std::uniform_real_distribution<qreal>(-0.1, 0.1);

    // Define rows vector
    _answersRows = QVector<QVariant>(2);
    // Define answers sticks
    _answersSticks = QVector<Stick*>();

    _render = QVector<bool>(2, false);
}

void StickRenderer::initializeGLCanvas()
{
    // Init OpenGL function
    initializeOpenGLFunctions();
    // Setup background color and transparency
    glEnable(GL_ALPHA_TEST);
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(0, 0, 0, 0);

}

void StickRenderer::resizeGLCanvas(int width, int height)
{
    // Setup OpenGL viewport
    glViewport(0, 0,width, height);

    _right = qreal(width) / qreal(height);
    _left = -_right;
    _top = 1.0;
    _bottom = -1.0;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glOrtho(_left, _right, _top, _bottom, -1, 1);

    randomX = std::uniform_real_distribution<qreal>(_left * 1.2, _right * 1.2);
    randomY = std::uniform_real_distribution<qreal>(_bottom * 1.2, _top * 1.2);

    // Update bar size
    QPointF stickSize = sizeFromPixel(3.5, 35);
    for (auto &stick : _barSticks) {
        stick->setSize(stickSize.x(), stickSize.y());
    }
}

void StickRenderer::paintGLCanvas()
{
    // Clear background
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Reset drawer program
    glUseProgram(0);

    switch (_currentView) {
    case ScreensaverLayout:
        paintScreensaver();
        break;
    case AnswersViewLayout:
        paintAnswersView();
        break;
    }

}

void StickRenderer::setBars(int answerIndex, const QVariant &bars)
{
    _answersRows[answerIndex] = bars;
    int barSum = 0;

    for (auto &answers : _answersRows) {
        for (auto &row : answers.toList()) {
            barSum += row.toInt();
        }
    }

    if (barSum > 0) {
        int barSize = _answersSticks.size();
        if (barSum > barSize) {
            int diff = barSum - barSize;

            for (int i = 0; i < diff; i++) {
                Stick *stick = new Stick;
                _answersSticks.push_back(stick);
            }
        }

        if (barSum < barSize) {
            int diff = barSize - barSum;

            for (int i = 0; i < diff; i++) {
                _answersSticks.removeLast();
            }
        }
    }

}

QPointF StickRenderer::sizeFromPixel(qreal width, qreal height)
{
    /*
     * Translate a given value in pixel to the OpenGL coordinate
     * mapValue(
     *      width = stick width in pixel
     *      0 = from 0px
     *      _width / _pixelRatio = viewport width in pixel / pixel ratio for DPI adaptation
     *      Here we divide the width by the pixel ratio for DPI scale
     *      _right * 2 = (0 - -1) + 1 = 2 || 1 * 2. left -1 to right 1 = 2 || 1 * 2
     * )
    */
    return QPointF(mapValue(width, 0, _width / _pixelRatio, 0, _right * 2),
                   mapValue(height, 0, _height / _pixelRatio, 0, _top * 2));
}

QPointF StickRenderer::coordinateFromPixel(qreal x, qreal y)
{
    // Return a OpenGL coordinate from a given pixel position
    return QPointF(mapValue(x, 0, _width, _left, _right),
                   mapValue(y, _height, 0, _top, _bottom));
}

void StickRenderer::paintScreensaver()
{
    QRandomGenerator generator;

    for (auto &stick : _barSticks) {
        qreal centerX = randomX(generator);
        qreal centerY = randomY(generator);
        qreal radius = randomRadius(generator);
        qreal frequency = randomFrequency(generator);
        qreal ratioRotation = randomRatioRotation(generator);

        qreal timeNow = elapsed() / 1000.0;

        qreal posX = centerX + cos(timeNow * frequency) * radius;
        qreal posY = centerY + sin(timeNow * frequency) * radius;
        qreal rotation = (elapsed() + 10000) * frequency * ratioRotation;

        stick->setX(posX);
        stick->setY(posY);
        stick->setRotation(fmod(rotation, 360));

        stick->draw();
    }
}

void StickRenderer::paintAnswersView()
{
    qreal barHeight = 35;
    qreal startY = 118 + barHeight / 2;
    qreal marginLeft = 100;

    QPointF barSize = sizeFromPixel(3.5, barHeight);
    QPointF startPosition = coordinateFromPixel(marginLeft, startY);

    qreal barSpace = barSize.y() / 3.7;
    qreal rowSpace = sizeFromPixel(0, 310).y();

    QString barColor = "#FFFFFF";

    int lineIndex = 0;

    for (auto &answerBars : _answersRows) {
        auto bars = answerBars.toList();
        for (int rowIndex = 0; rowIndex < bars.size(); rowIndex++) {
            for (int barIndex = 0; barIndex < bars.at(rowIndex).toInt(); barIndex++) {

                Stick *stick = _answersSticks.at(lineIndex);
                stick->setSize(barSize.x(), barSize.y());
                stick->setColor(barColor);
                int moduloFive = barIndex % 5;

                qreal x = barIndex * barSpace;
                qreal y = -rowIndex * (rowSpace + barSize.y());
                qreal rotation = 0.0;

                if (moduloFive == 4) {
                    x = ((barIndex - 2) * barSpace) - (barSpace / 2);
                    rotation = -73;

                    stick->setSize(barSize.x(), barSize.y() * 1.25);
                }

                stick->setX(x + startPosition.x());
                stick->setY(y - startPosition.y());
                stick->setRotation(rotation);

                stick->draw();

                ++lineIndex; // Next index
            }
        }
        // Some change for second row
        startPosition = coordinateFromPixel(marginLeft, startY + barHeight + 12);
        barColor = "#80E2A7";
    }
}

void StickRenderer::render()
{
   // Check if rendering is enable on the QML side
    if (_render.at(ScreensaverLayout) || _render.at(AnswersViewLayout)) {
        // call painter function
        paintGLCanvas();
    }

    // Update to draw again
    update();
}

QOpenGLFramebufferObject *StickRenderer::createFramebufferObject(const QSize &size)
{
    // Make the width take account the pixel ratio
    // if pixel ratio is 2 width & height become 2 time bigger
    _width = size.width() * _pixelRatio;
    _height = size.height() * _pixelRatio;

    resizeGLCanvas(_width, _height);

    QOpenGLFramebufferObjectFormat format;
    format.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
    format.setSamples(8);

    return new QOpenGLFramebufferObject(size, format);
}

void StickRenderer::synchronize(QQuickFramebufferObject *item)
{
    // Reset OpenGL
    item->window()->resetOpenGLState();
    // Update pixel ratio
    _pixelRatio = item->window()->devicePixelRatio();
}
////////////////////////////////////////////////////////////////////
