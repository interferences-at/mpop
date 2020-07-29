#ifndef STICK_H
#define STICK_H

#include <QObject>
#include <QOpenGLFunctions>
#include <QtQuick/QQuickItem>


class Stick
{
public:
    Stick();
    virtual ~Stick() {};
    // Just draw stick a single stick on the screen
    void draw();
    // Get stick position x, y, z
    qreal getX() const { return _x; }
    qreal getY() const { return _y; }
    qreal getRotation() const { return _rotation; }
    // Set position x, y, z
    void setX(qreal value) { _x = value; }
    void setY(qreal value) { _y = value; }
    void setRotation(qreal value) { _rotation = value; }
    // Set size for each stick
    void setSize(qreal width, qreal height);
    // Set Color
    void setColor(const QString &colorCode = "#FFFFFF"); // Hex color
    void setColor(int red, int green, int blue, int alpha = 255); // RGBA color

private:
    qreal _x, _y, _rotation;
    qreal _width, _height;

    // Color variables
    qreal _red, _blue, _green, _alpha = 1.0;
    QColor _color;
};

#endif // STICK_H
