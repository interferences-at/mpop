#include "screensaver.h"


QQuickFramebufferObject::Renderer *Screensaver::createRenderer() const
{
    return new StickRenderer();
}

StickRenderer::StickRenderer()
{

    _barSticks = QVector<Stick*>();

    for (int i = 0; i < 100; i++) {
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

    qreal right = qreal(width) / qreal(height);
    qreal left = -right;
    qreal top = 1;
    qreal bottom = -top;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glMatrixMode(GL_MODELVIEW );
    glLoadIdentity();

    glOrtho(left, right, bottom, top, -1, 1);


    randomX = std::uniform_real_distribution<qreal>(left, right);
    randomY = std::uniform_real_distribution<qreal>(bottom, top);
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

void StickRenderer::render()
{
    paintGLCanvas();
}

QOpenGLFramebufferObject *StickRenderer::createFramebufferObject(const QSize &size)
{
    initializeGLCanvas();
    resizeGLCanvas(size.width(), size.height());

    QOpenGLFramebufferObjectFormat format;
    format.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
    format.setSamples(16);

    return new QOpenGLFramebufferObject(size, format);
}

void StickRenderer::synchronize(QQuickFramebufferObject *item)
{
    Screensaver *screensaver = qobject_cast<Screensaver *>(item);

    screensaver->window()->resetOpenGLState();
}
