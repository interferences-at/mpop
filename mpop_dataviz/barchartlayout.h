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
    // Return the total number of bar
    int getBarsCount() const {
        return std::accumulate(_rowsValues.begin(),
                               _rowsValues.end(), 0);
    }


    void updateBarsPosition(qint64 currentTime) override;

    void showSceneObject(qint64 currentTime) override;

    void setAlignCenter(bool center) {_centered = center; }
    void setStartPosition(const QPointF &pos);
    void setDistanceBetweenRows(qreal rowSpace) { _spaceBetweenRows = rowSpace; }

private:
    QList<int> _rowsValues;
    QSharedPointer<GroupTweenAnimator> _groupTweenAnimator;

    bool _centered = false;

    qreal _x, _y = 0.0;
    qreal _spaceBetweenRows = 0.018;
};
