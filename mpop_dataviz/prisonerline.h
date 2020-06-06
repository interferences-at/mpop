#pragma once

#include "sceneobject.h"
#include <QPoint>
#include <QPropertyAnimation>
#include <QEasingCurve>
#include <QColor>

// TODO: rename to Stick (it's easier to read/write/say)
class PrisonerLine : public SceneObject
{
public:
    explicit PrisonerLine();
    ~PrisonerLine() override;
    virtual void draw(const qint64& elapsed) override;

    // Set a custom color for sticks (overloaded)
    void setColor(const QString &colorCode = "#FFFFFF"); // Hex color
    void setColor(int red, int green, int blue, int alpha = 255); // RGBA color

    void setSize(qreal width, qreal height);


    QColor getColor() const { return _color; }

    // Alias for shared pointer of this class
    typedef QSharedPointer<PrisonerLine> ptr;

private:

    // Color variables
    qreal _red, _blue, _green, _alpha = 1.0;
    QColor _color;

    // Size variable
    qreal _width = 0.01;
    qreal _height = 0.1;
};
