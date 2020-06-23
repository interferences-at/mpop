#pragma once

#include "layout.h"
#include <QList>
#include "grouptweenanimator.h"
#include <QSharedPointer>

/**
 * @brief Layout that places the prisoner lines to form a bar chart.
 *
 * Each bar in the chart has a different height. They should sum up to 100.
 */
class BarChartLayout : public Layout
{
public:
    BarChartLayout();
    virtual ~BarChartLayout() override;
    virtual void moveObjectsToLayout(qint64 currentTime) override;
    /**
     * @brief Sets the values for each bar to show.
     * @param values
     */
    void setRows(const QList<int>& values);
    // return list of rows
    QList<int> getRows() const { return _rowsValues; }
    // Return a single row
    int getRow(int i = 0) const { return _rowsValues.at(i); }


    void updateBarsPosition(qint64 currentTime) override;

    void showSceneObject(qint64 currentTime) override;

    void setAlignCenter(bool center) {_centered = center; }
    void setStartPosition(const QPointF &pos);

private:
    QList<int> _rowsValues;
    QSharedPointer<GroupTweenAnimator> _groupTweenAnimator;

    bool _centered = false;

    qreal _x, _y = 0.0;
};
