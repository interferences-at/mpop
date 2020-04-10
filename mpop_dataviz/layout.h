#pragma once


#include "prisonerline.h"
#include <QVector>

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
    void addPrisonerLines(const QVector<PrisonerLine*>& prisonerLines);

    /**
     * @brief Move the objects managed by this layout into their desired position.
     */
    virtual void moveObjectsToLayout(qint64 currentTime) = 0;

    virtual void updateObjectPosition(qint64 currentTime) = 0;

protected:
    QVector<PrisonerLine*> _prisonerLines;
};
