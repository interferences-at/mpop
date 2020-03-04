#pragma once

#include <QElapsedTimer>
#include <QtGui/QOpenGLFunctions>
#include <QObject>

class SceneObject : public QObject
{
    Q_OBJECT
public:
    SceneObject( QObject* parent = nullptr);
    virtual ~SceneObject();
    virtual void draw(const QElapsedTimer& timer) = 0;
    void setVisible(bool isVisible);
    bool getVisible() const;
protected:
    bool _isVisible;
};
