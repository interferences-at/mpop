#pragma once

#include "layout.h"
#include <QList>

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
    void setBars(const QList<int>& values);
private:
    QList<int> _barValues;
};
