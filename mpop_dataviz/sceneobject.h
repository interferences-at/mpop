#pragma once

#include <QElapsedTimer>
#include <QtGui/QOpenGLFunctions>

class SceneObject
{
public:
    SceneObject();
    virtual ~SceneObject();
    virtual void draw(const QElapsedTimer& timer) = 0;
    void setVisible(bool isVisible);
    bool getVisible() const;
protected:
    bool _isVisible;
};
