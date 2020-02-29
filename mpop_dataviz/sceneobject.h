#pragma once

#include <QElapsedTimer>
#include <QtGui/QOpenGLFunctions>

class SceneObject
{
public:
    SceneObject();
    virtual ~SceneObject();
    virtual void draw(const QElapsedTimer& timer) = 0;
};
