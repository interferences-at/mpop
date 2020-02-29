#pragma once

#include "openglwindow.h"
#include <QtGui/QOpenGLShaderProgram>


class DatavizWindow : public OpenGLWindow {
public:
    DatavizWindow();
    ~DatavizWindow() override;
    void initialize() override;
    void render() override;

private:
    GLuint _posAttr;
    GLuint _colAttr;
    GLuint _matrixUniform;
    QOpenGLShaderProgram* _program;
    int _frame;
};
