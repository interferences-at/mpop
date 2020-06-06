#pragma once

#include <QPointF>
#include <QElapsedTimer>
#include <algorithm>

#include "barchartlayout.h"
#include "screensaverlayout.h"

class ViewModeManager
{
public:
    enum ViewMode {
        ScreenSaverMode, // Default Mode
        UserAnswersMode,
        FilteredAnswersMode,
        AnswerByAgeMode,
        AnswerByGenderMode,
        AnswerByCultureMode
    };

    typedef  QSharedPointer<QVector<PrisonerLine::ptr>> viewBars;


    ViewModeManager();
    ~ViewModeManager() {}

    void showViewManagerBars(ViewMode mode);

    void updateViewCoordinate(qreal left, qreal right, qreal bottom, qreal top);
    void updateViewSize(int width, int height);

    void setViewActiveMode(ViewMode mode);
    ViewMode getViewActiveMode() const { return _viewActiveMode; }

    void setBarChartRows(const QList<int> &list, ViewMode viewIndex);
    void moveBarsToLayouts(ViewModeManager::viewBars bars, ViewMode viewIndex);
    void setViewBarsQuantity(int number, ViewMode viewIndex);

    ViewModeManager::viewBars getBarsFromScreenSaver(const QList<int> &list);
    void restoreBarsToScreenSaver(ViewMode viewIndex);

    void setUserAnswerBars(const QList<int> &bars);

    QPointF coordinateFromPixel(qreal x, qreal y);
    QPointF sizeFromPixel(qreal width, qreal height);

    void setPointToPickFrom(const QPointF &point);

    qreal mapValue(qreal value, qreal istart, qreal istop, qreal ostart, qreal ostop);

    qint64 currentTime() { return _timer.elapsed(); }

    // View mode manager singleton
    static ViewModeManager *viewManager() {
        if (viewModeManager == nullptr)
            viewModeManager = new ViewModeManager;
        return viewModeManager;
    }


private:
    ViewMode _viewActiveMode;

    qreal _left, _right, _bottom, _top;

    int _width, _height;

    QPointF _pointToPickFrom;

    QVector<ViewModeManager::viewBars> _viewBars;
    QVector<QList<int>> _barChartRows;

    ScreensaverLayout _screensaver;
    BarChartLayout _answersBarchart;

    QElapsedTimer _timer;

    static ViewModeManager *viewModeManager;
};
