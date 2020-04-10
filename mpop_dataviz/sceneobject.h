#pragma once

#include <QElapsedTimer>
#include <QtGui/QOpenGLFunctions>
#include <QObject>

class SceneObject : public QObject
{
    Q_OBJECT
public:
    explicit SceneObject( QObject* parent = nullptr);
    virtual ~SceneObject();
    /**
     * @brief Draws the scene object.
     * @param timer How much time has elapsed since the startup of the application.
     */
    virtual void draw(const qint64& elapsed) = 0;
    void setVisible(bool isVisible);
    bool getVisible() const;
protected:
    bool _isVisible;
};
