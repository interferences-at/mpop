#pragma once

#include "layout.h"

class BarChartLayout : public Layout
{
public:
    BarChartLayout();
    virtual ~BarChartLayout() override;
    virtual void moveObjectsToLayout() override;
    void setBars(const QVector<quint8>& values);
private:
    QVector<quint8> _barValues;
};
