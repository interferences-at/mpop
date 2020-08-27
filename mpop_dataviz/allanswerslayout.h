#pragma once

#include "layout.h"
#include "grouptweenanimator.h"

#include <random>

/**
 * @brief This layout display all the answers
 * - there are 25 questions
 * - each can be a series of 0-100 vertical lines
 * - that move a little (rotation and translation)
 */
class AllAnswersLayout : public Layout
{
public:
    AllAnswersLayout();
    virtual ~AllAnswersLayout() {}

    // Move bars objects to the layout
    void moveObjectsToLayout(qint64 currentTime) override;
    void updateBarsPosition(qint64 currentTime) override;
    void showSceneObject(qint64 currentTime) override;

    void setRows(const QList<int>& values) {
        _rowsValues = values;
    }

    int getBarsCount() const {
        return std::accumulate(_rowsValues.begin(),
                               _rowsValues.end(), 0);
    }

private:
    QString getColorFromRange(const QString &hexFrom, const QString &hexTo, int number) const;
    qreal mapValue(qreal value, qreal istart, qreal istop, qreal ostart, qreal ostop) const;

    QList<int> _rowsValues;
    QSharedPointer<GroupTweenAnimator> _groupTweenAnimator;

    static bool _doneMoving;
};
