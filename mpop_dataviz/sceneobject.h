#pragma once

#include <QObject>
#include <QSharedPointer>

class SceneObject
{
public:
    explicit SceneObject();
    virtual ~SceneObject();
    /**
     * @brief Draws the scene object.
     * @param timer How much time has elapsed since the startup of the application.
     */
    virtual void draw(const qint64& elapsed) = 0;
    void setVisible(bool isVisible);
    bool getVisible() const;
    /**
     * @brief Sets the position of the line. (x, y)
     * @param point Position
     */
    void setPosition(qreal x, qreal y);
    /**
     * @brief Sets the angle of the line to the given angle (in degrees)
     * @param degrees Angle in degrees.
     */
    void setOrientation(qreal degrees);

    qreal getX() const { return this->_x; }
    qreal getY() const { return this->_y; }
    qreal getZ() const { return this->_z; }
    qreal getRotation() const { return this->_rotation; }
    qreal getScale() const { return this->_scale; }

    void setX(qreal value);
    void setY(qreal value);
    void setZ(qreal value);
    void setRotation(qreal value);
    void setScale(qreal value);

    // Alias for shared pointer of this class
    typedef QSharedPointer<SceneObject> ptr;

protected:
    bool _isVisible;
    qreal _x;
    qreal _y;
    qreal _z;
    qreal _rotation;
    qreal _scale;
};
