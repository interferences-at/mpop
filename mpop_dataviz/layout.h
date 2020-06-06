#pragma once


#include "prisonerline.h"
#include <QVector>
#include <QDebug>

/**
 * @brief Layout for a way to group prisoner lines.
 */
class Layout
{
public:
    Layout();
    virtual ~Layout();

    /**
     * @brief Adds some prisoner lines to this layour.
     * @param prisonerLines
     */
    void addPrisonerLines(QSharedPointer<QVector<PrisonerLine::ptr>> prisonerLines);

    /**
     * @brief Move the objects managed by this layout into their desired position.
     */
    virtual void moveObjectsToLayout(qint64 currentTime) = 0;

    virtual void updateBarsPosition(qint64 currentTime) = 0;

    virtual void showSceneObject(qint64 currentTime) = 0;

    // Set the amount of bars or stick
    void setBarsQuantity(int number);

    void setBarsSize(qreal width, qreal height);
    void setBarsSize(const QPointF &size);

    void setBarsColor(const QString &colorHex);

    void setLayoutCoordinate(float left, float right, float bottom, float top);


protected:
    QSharedPointer<QVector<PrisonerLine::ptr>> _prisonerLines;

    float _left = -1.0;
    float _right = 1.0;
    float _bottom = -1.0;
    float _top = 1.0;

    qreal _barsWidth = 0.01;
    qreal _barsHeight = 0.1;
};
