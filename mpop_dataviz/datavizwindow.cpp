#include "datavizwindow.h"
#include "prisonerline.h"
#include <QtGui/QGuiApplication>
#include <QtGui/QMatrix4x4>
#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QScreen>
#include <QtCore/qmath.h>


DatavizWindow::DatavizWindow() {
    //qDebug() << "Create a Window";
    _elapsedTimer.start();

    // Allocate all the bars to screen saver
    _viewModeManager = new ViewModeManager();
    _viewModeManager->setViewBarsQuantity(100, ViewModeManager::ScreenSaverMode);

    connect(&_updateIntervalTimer, SIGNAL(timeout()), this, SLOT(updateFramePerSecond()));
    _updateIntervalTimer.setInterval(200); // Set the update interval
    _updateIntervalTimer.start(); // Start listening
    _frameTimer.start(); // Start timer
}


qint64 DatavizWindow::elapsed() const {
    return this->_elapsedTimer.elapsed();
}

void DatavizWindow::toggleTestCard() {
    _showTestCard = !_showTestCard;
    restartTestCardTimer();
}

void DatavizWindow::setTestCardVisibility(bool visible) {
    _showTestCard = visible;

    restartTestCardTimer();
}

void DatavizWindow::restartTestCardTimer() {
    if (_showTestCard) {
        _testCardTimer.restart();
    }
}

void DatavizWindow::keyPressEvent(QKeyEvent *event) {
    if (event->modifiers() & Qt::ControlModifier) {
        switch (event->key()) {
        case Qt::Key_Q:
            // Emit signal to close the parent window with CTRL+Q
            emit closed();
            break;
        }
    } else {
        switch (event->key()) {
        case Qt::Key_Tab:
            _showHUD = !_showHUD; // Show/Hide FPS text with TAB

            toggleTestCard();
            break;
        }
    }
}

void DatavizWindow::updateFramePerSecond() {
    // Frame per second
    _framePerSecond = _framesCount / ((double)_frameTimer.elapsed() / 1000.0);
    // Restart timer
    _frameTimer.restart();
    // Reset frame counter
    _framesCount = 0;
}

void DatavizWindow::initializeGL() {
    // setSwapInterval(1);
    glClearColor(0, 0, 0, 0);
    // Enable painting on the OpenGL context
    _device = new  QOpenGLPaintDevice();
    // Create a text drawer
    _painter = new TextObjectPainter();
}


void DatavizWindow::resizeGL(int w, int h) {
    const qreal retinaScale = this->devicePixelRatio();

    // Note: Avoid issuing OpenGL commands from this function as
    // there may not be a context current when it is invoked.
    // If it cannot be avoided, call makeCurrent().
    this->makeCurrent();

    glViewport(0, 0,
               w * static_cast<GLsizei>(retinaScale),
               h * static_cast<GLsizei>(retinaScale));
//    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);

    // Note: Here, the center is (0, 0), and not the top-left corner, like it often is.
    GLdouble ratio = (static_cast<GLdouble>(w)) / static_cast<GLdouble>(h);
    GLdouble left = - ratio;
    GLdouble right = ratio;
    GLdouble top = 1.0;
    GLdouble bottom = -1.0;

    glLoadIdentity();
    glOrtho(left, right, bottom, top, -1, 1);

    // Instead, we could eventually use a pespective view, here:
    // gluPerspective(60.0f, ratio, 1.0f, 100.0f);

    // Resize the paint device
    _device->setSize(QSize(width(), height()));
    _device->setDevicePixelRatio(retinaScale);

    // Resize Layout
    _viewModeManager->updateViewCoordinate(left, right, bottom, top);
    _viewModeManager->updateViewSize(width(), height());
}


void DatavizWindow::paintGL() {
    _painter->setPaintDevice(_device);
    _painter->beginOpenGLPainting(); // Start OpenGL painting

    // Clear the background
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    ViewModeManager::ViewMode viewActiveMode = _viewModeManager->getViewActiveMode();
    _viewModeManager->showViewManagerBars(viewActiveMode);
    _painter->endOpenGLPainting(); // Finish OpenGL painting

    if (viewActiveMode >= ViewModeManager::MultiAnswersMode &&
        viewActiveMode <= ViewModeManager::AnswerByCultureMode) {
        _painter->drawViewElements(viewActiveMode,
                                   _viewModeManager->getViewTitles(viewActiveMode));
    }

    // Draw test card
    if (_showTestCard) {
        _painter->drawTestCard();
        if (_testCardTimer.elapsed() >= 1000 * 60 * 30) {
            _showTestCard = false;
        }
    }

    if (_showHUD) {
        // Draw Frame per second
        _painter->drawFramePerSecond(_framePerSecond);
        setCursor(QCursor(Qt::ArrowCursor));
    } else {
        setCursor(QCursor(Qt::BlankCursor));
    }

    ++_framesCount; // Update the amount of frame

    // swapBuffers();
    this->update(); // ask for a new render next time the screen refreshes.
}

