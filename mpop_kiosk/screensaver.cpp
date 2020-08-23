#include "screensaver.h"


QQuickFramebufferObject::Renderer *Screensaver::createRenderer() const
{
    return new StickRenderer();
}

StickRenderer::StickRenderer()
{

    _barSticks = QVector<Stick*>();

    for (int i = 0; i < 150; i++) {
        Stick *stick = new Stick;
        stick->setSize(0.00585366, 0.117073);
        stick->setColor("#FFFFFF");
        _barSticks.push_back(stick);
    }

    _timer.start();

    randomRadius = std::uniform_real_distribution<qreal>(-1, 1);
    randomFrequency = std::uniform_real_distribution<qreal>(0.01, 0.1);
    randomRatioRotation = std::uniform_real_distribution<qreal>(-0.1, 0.1);

    initializeGLCanvas();
}

void StickRenderer::initializeGLCanvas()
{
    initializeOpenGLFunctions();

    glEnable(GL_ALPHA_TEST);
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(0, 0, 0, 0);

}

void StickRenderer::resizeGLCanvas(int width, int height)
{
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
    for (auto stick : _barSticks) {
        stick->setSize(stickSize.x(), stickSize.y());
    }
}

void StickRenderer::paintGLCanvas()
{
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(0);

    QRandomGenerator generator;

    for (auto stick : _barSticks) {
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

    update();
}

QPointF StickRenderer::sizeFromPixel(qreal width, qreal height)
{
    return QPointF(mapValue(width, 0, _width / _pixelRatio, 0, _right * 2),
                   mapValue(height, 0, _height / _pixelRatio, 0, _top * 2));
}

void StickRenderer::render()
{
    paintGLCanvas();
}

QOpenGLFramebufferObject *StickRenderer::createFramebufferObject(const QSize &size)
{
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

    screensaver->window()->resetOpenGLState();
    // Update pixel ratio
    _pixelRatio = screensaver->window()->devicePixelRatio();
}
