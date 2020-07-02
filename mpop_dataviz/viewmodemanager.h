#pragma once

#include <QPointF>
#include <QElapsedTimer>
#include <algorithm>
#include <QSharedPointer>

#include "barchartlayout.h"
#include "screensaverlayout.h"

class ViewModeManager
{
public:
    // Types:
    enum ViewMode {
        ScreenSaverMode, // Default Mode
        UserAnswersMode, // See My answer
        MultiAnswersMode, // Multiple answer
        AnswerByAgeMode,
        AnswerByGenderMode,
        AnswerByCultureMode
    };

    typedef  QSharedPointer<QVector<PrisonerLine::ptr>> viewBars;

    struct AnswerData {
        QList<QString> text;
        QList<int> my_answer;
        QList<int> their_answer;
    };

    typedef QSharedPointer<AnswerData> AnswerDataPtr;

    struct TitleAndValue {
        QString title;
        int value;
    };

    typedef QSharedPointer<TitleAndValue> TitleAndValuePtr;

    // Methods:
    ViewModeManager();
    ~ViewModeManager() {}

    void showViewManagerBars(ViewMode mode);

    void updateViewCoordinate(qreal left, qreal right, qreal bottom, qreal top);
    void updateViewSize(int width, int height);

    void setViewActiveMode(ViewMode mode);
    ViewMode getViewActiveMode() const { return _viewActiveMode; }

    void moveBarsToLayouts(ViewMode viewIndex);

    void setViewBarsQuantity(int number, ViewMode viewIndex);

    ViewModeManager::viewBars getBarsFromScreenSaver(int number);

    void setUserAnswerBars(const QList<int> &bars);
    void setMultiAnswersBars(AnswerDataPtr answers);
    void showOneAnswer(int numRows, int myRow, int myAnswer, const QList<TitleAndValuePtr>& titlesAndValues);
    void showOneAnswerByAge(int myRowIndex, int myAnswer, const QList<int>& values);
    void goToScreensaver();

    void setViewTitles(const QList<QString> &titles, ViewMode viewIndex);
    QList<QString> getViewTitles(ViewMode view) const { return _viewTitles[view]; }

    QPointF coordinateFromPixel(qreal x, qreal y);
    QPointF sizeFromPixel(qreal width, qreal height);
    qreal heightFromPixel(qreal height);

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

    // Viewport coordinates and dimensions
    qreal _left, _right, _bottom, _top;
    int _width, _height;
    // Point to pick bar from when needed by a layout
    QPointF _pointToPickFrom;

    QVector<ViewModeManager::viewBars> _viewBars;
    // Screensaver layout
    ScreensaverLayout _screensaver;
    BarChartLayout _userAnswers; // User answer layout

    // Multi answer layouts
    BarChartLayout _multiAverageAnswer;
    BarChartLayout _multiUserAnswer;

    // Answers by age layouts
    BarChartLayout _agesAnswerBarChart;
    BarChartLayout _userAgeAnswer;
    int _myAgeReverseIndex;

    // Answer by gender layouts
    BarChartLayout _genderOtherAnswer;
    BarChartLayout _genderUserAnswer;
    int _myGenderIndex;

    // Answer by culture layouts
    BarChartLayout _cultureOtherAnswer;
    BarChartLayout _cultureUserAnswer;
    int _myCultureIndex;

    QVector<QList<QString>> _viewTitles;

    QElapsedTimer _timer;

    static ViewModeManager *viewModeManager;

    // Private functions
    void setupScreensaverLayout(ViewMode activeView);
    void moveBarsToMultiAnswerLayout();
    void moveBarsToAnswerByAgeLayout();
    void moveBarsToAnswerByGenderLayout();
    void moveBarsToAnswerByCultureLayout();

    qreal fitToScreenWidth(qreal size) {
        return _width * (size / 1260);
    }

    qreal fitToScreenHeight(qreal size) {
        return _height * (size / 1080);
    }
};
