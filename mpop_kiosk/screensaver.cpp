#include "screensaver.h"


QQuickFramebufferObject::Renderer *Screensaver::createRenderer() const
{
    StickRenderer *stickRenderer = new StickRenderer();
    // Enable/disable renderer on the OpenGL side
    stickRenderer->enableRendering(getEnable());
    return stickRenderer;
}

void Screensaver::setEnable(bool enable)
{
    if (enable != _enable) {
        _enable = enable;
        emit renderChanged();
    }
}

StickRenderer::StickRenderer()
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
    // Init OpenGL
    initializeGLCanvas();
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
    _top = 1;
    _bottom = -_top;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glMatrixMode(GL_MODELVIEW );
    glLoadIdentity();

    glOrtho(_left, _right, _bottom, _top, -1, 1);

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
    // Reset drawer program
    glUseProgram(0);

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
    // Update to draw again
    update();
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

void StickRenderer::render()
{
    // Check if rendering is enable on the QML side
    if (renderingIsEnabled()) {
        // call painter function
        paintGLCanvas();
    }
}

QOpenGLFramebufferObject *StickRenderer::createFramebufferObject(const QSize &size)
{
    // Make the width take account the pixel ratio
    // if pixel ratio is 2 width & height become 2 time bigger
    _width = size.width() * _pixelRatio;
    _height = size.height() * _pixelRatio;

    initializeGLCanvas();
    resizeGLCanvas(_width, _height);

    QOpenGLFramebufferObjectFormat format;
    format.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
    format.setSamples(8);

    return new QOpenGLFramebufferObject(size, format);
}

void StickRenderer::synchronize(QQuickFramebufferObject *item)
{
    Screensaver *screensaver = qobject_cast<Screensaver *>(item);
    // Reset OpenGL
    screensaver->window()->resetOpenGLState();
    // Update pixel ratio
    _pixelRatio = screensaver->window()->devicePixelRatio();
}
