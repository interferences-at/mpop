#pragma once

#include "sceneobject.h"
#include <QPoint>

class PrisonerLine : public SceneObject
{
public:
    PrisonerLine();
    ~PrisonerLine() override;
    virtual void draw(const QElapsedTimer& timer) override;
    /**
     * @brief Sets the position of the line. (x, y)
     * @param point Position
     */
    void setPosition(double x, double y);
    /**
     * @brief Sets the angle of the line to the given angle (in degrees)
     * @param degrees Angle in degrees.
     */
    void setOrientation(qreal degrees);
private:
    qreal _angleDegrees;
    double _x;
    double _y;
};
