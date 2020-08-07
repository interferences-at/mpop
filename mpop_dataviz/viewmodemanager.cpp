#include "viewmodemanager.h"

ViewModeManager *ViewModeManager::viewModeManager = nullptr;

ViewModeManager::ViewModeManager() :
    _viewActiveMode(ScreenSaverMode),
    _left(-1.0), _right(1.0),
    _bottom(-1.0), _top(1.0),
    _pointToPickFrom(QPointF(0, 0))
{
    _timer.start();

    _viewTitles = QVector<QList<QString>>(6);

    for (int i = 0; i < 7; i++) {
        ViewModeManager::viewBars sharedVector = ViewModeManager::viewBars::create();
        _viewBars.push_back(sharedVector);
    }
}

void ViewModeManager::showViewManagerBars(ViewMode mode)
{
    /* Screensaver is shown by default
     * Except for the All Answer layout
     */
    if (mode != AllAnswersMode) {
        _screensaver.updateBarsPosition(currentTime());
        _screensaver.showSceneObject(currentTime());
    }

    switch (mode) {
    case UserAnswersMode:
        _userAnswers.updateBarsPosition(currentTime());
        _userAnswers.showSceneObject(currentTime());
        break;
    case MultiAnswersMode:
        _multiAverageAnswer.updateBarsPosition(currentTime());
        _multiAverageAnswer.showSceneObject(currentTime());

        _multiUserAnswer.updateBarsPosition(currentTime());
        _multiUserAnswer.showSceneObject(currentTime());
        break;
    case AnswerByAgeMode:
        _agesAnswerBarChart.updateBarsPosition(currentTime());
        _agesAnswerBarChart.showSceneObject(currentTime());

        _userAgeAnswer.updateBarsPosition(currentTime());
        _userAgeAnswer.showSceneObject(currentTime());
        break;
    case AnswerByGenderMode:
        _genderOtherAnswer.updateBarsPosition(currentTime());
        _genderOtherAnswer.showSceneObject(currentTime());

        _genderUserAnswer.updateBarsPosition(currentTime());
        _genderUserAnswer.showSceneObject(currentTime());
        break;
    case AnswerByCultureMode:
        _cultureOtherAnswer.updateBarsPosition(currentTime());
        _cultureOtherAnswer.showSceneObject(currentTime());

        _cultureUserAnswer.updateBarsPosition(currentTime());
        _cultureUserAnswer.showSceneObject(currentTime());
        break;
    case AllAnswersMode:
        _allAnswers.updateBarsPosition(currentTime());
        _allAnswers.showSceneObject(currentTime());
    default:
        break;
    }
}

void ViewModeManager::updateViewCoordinate(qreal left, qreal right, qreal bottom, qreal top)
{
    _left = left;
    _right = right;
    _bottom = bottom;
    _top = top;

    // Update layouts Coordinates
    _screensaver.setLayoutCoordinate(_left, _right, _bottom, _top);
    _allAnswers.setLayoutCoordinate(_left, _right, _bottom, _top);
}

void ViewModeManager::updateViewSize(int width, int height)
{
    _width = width;
    _height = height;
    // Update view at resize
    moveBarsToLayouts(getViewActiveMode());
}

void ViewModeManager::setViewActiveMode(ViewMode mode)
{
    if (_viewActiveMode != mode)
        _viewActiveMode = mode;
}

void ViewModeManager::moveBarsToLayouts(ViewMode viewIndex)
{   
    // Setup screensaver behavior
    setupScreensaverLayout(viewIndex);

    switch (viewIndex) {
    case UserAnswersMode:
    {
        // Setup user answer Layout
        int barHeight = fitToScreenHeight(35);
        _userAnswers.addBarObjects(_viewBars[viewIndex]);
        _userAnswers.setBarsSize(sizeFromPixel(3.5, barHeight));
        _userAnswers.setDistanceBetweenRows(heightFromPixel(fitToScreenHeight(12)));
        _userAnswers.setBarsColor("#80E2A7");
        _userAnswers.setAlignCenter(true);
        _userAnswers.moveObjectsToLayout(currentTime());
    }
        break;
    case MultiAnswersMode:
        // Setup multi answer layouts
        moveBarsToMultiAnswerLayout();
        break;
    case AnswerByAgeMode:
        // Setup answer by age layouts
        moveBarsToAnswerByAgeLayout();
        break;
    case AnswerByGenderMode:
        // Setup answer by gender layouts
        moveBarsToAnswerByGenderLayout();
        break;
    case AnswerByCultureMode:
        // Setup answer by culture layouts
        moveBarsToAnswerByCultureLayout();
        break;
    case AllAnswersMode:
        _allAnswers.addBarObjects(_viewBars[viewIndex]);
        _allAnswers.setBarsSize(sizeFromPixel(3, 60));
        _allAnswers.setBarsColor("#CCCCCC");
        _allAnswers.moveObjectsToLayout(currentTime());
    default:
        break;
    }
}

void ViewModeManager::setViewBarsQuantity(int number, ViewMode viewIndex)
{
    if (number > 0) {
        int barSize = _viewBars[viewIndex]->isEmpty() ? 0 : _viewBars[viewIndex]->size();

        if (number > barSize) {
            int diff = number - barSize;

            if (viewIndex == ScreenSaverMode) {
                for (int i = 0; i < diff; i++) {
                    PrisonerLine::ptr line = PrisonerLine::ptr::create();
                    _viewBars[viewIndex]->push_back(line);
                }
            } else {
                _viewBars[viewIndex]->append(*getBarsFromScreenSaver(diff));
            }
        }

        if ( number < barSize) {
            int diff = barSize - number;
            if (viewIndex == ScreenSaverMode) {
                for (int i = 0; i < diff; i++) {
                    _viewBars[viewIndex]->removeLast();
                }
            } else {
                for (int i = 0; i < diff; i++) {
                    _viewBars[ScreenSaverMode]->push_back(_viewBars[viewIndex]->last());
                    _viewBars[viewIndex]->removeLast();
                }
            }
        }
    }

    moveBarsToLayouts(viewIndex);
}

ViewModeManager::viewBars ViewModeManager::getBarsFromScreenSaver(int number)
{
    qreal pointX = _pointToPickFrom.x();
    qreal pointY = -_pointToPickFrom.y();

    // The Screensaver amount of bars must be greater by 100 than...
    while (_viewBars[ScreenSaverMode]->size() - number < 100) {
        // Add new line every time screensaver bar is about to finish
        // To avoid out of range error
        PrisonerLine::ptr line = PrisonerLine::ptr::create();
        _viewBars[ScreenSaverMode]->push_back(line);
    }
    // And get the closest ones from a predefined point
    _viewBars[ScreenSaverMode] = _screensaver.getClosestBars(QPointF(pointX, pointY));

    ViewModeManager::viewBars barChartBars = ViewModeManager::viewBars::create();

    for (int i = 0; i < number; i++) {
        PrisonerLine::ptr line = _viewBars[ScreenSaverMode]->at(i);
        barChartBars->push_back(line);
    }

    _viewBars[ScreenSaverMode]->remove(0 , number);

    return barChartBars;
}

void ViewModeManager::setUserAnswerBars(const QList<int> &bars)
{
    _userAnswers.setRows(bars);
    setPointToPickFrom(QPointF(0, 0));
    setViewBarsQuantity(_userAnswers.getBarsCount(), UserAnswersMode);
    setViewActiveMode(UserAnswersMode);
}

void ViewModeManager::setMultiAnswersBars(AnswerDataPtr answers)
{
    _multiAverageAnswer.setRows(answers->their_answer);
    _multiUserAnswer.setRows(answers->my_answer);

    int averageBarsCount = _multiAverageAnswer.getBarsCount();
    int userBarsCount = _multiUserAnswer.getBarsCount();

    setPointToPickFrom(coordinateFromPixel(_width, _height / 2));
    setViewBarsQuantity(averageBarsCount + userBarsCount, MultiAnswersMode);
    setViewTitles(answers->text, MultiAnswersMode);
    setViewActiveMode(MultiAnswersMode);
}

void ViewModeManager::showOneAnswerByAge(int myAge, int myAnswer, const QList<int>& values)
{
    QList<int> ageRows = values;
    std::reverse(ageRows.begin(), ageRows.end());
    _agesAnswerBarChart.setRows(ageRows);
    _userAgeAnswer.setRows({myAnswer});

    int barSum = _agesAnswerBarChart.getBarsCount() + myAnswer;
    _myAgeReverseIndex = values.size() - floor(std::max(myAge, 5) / 5);

    setPointToPickFrom(coordinateFromPixel(_width, _height / 2));
    setViewBarsQuantity(barSum, AnswerByAgeMode);
    setViewActiveMode(AnswerByAgeMode);
}

void ViewModeManager::showOneAnswer(int numRows, int myRow, int myAnswer, const QList<TitleAndValuePtr>& titlesAndValues)
{
    if (numRows == 3) {
        QList<int> genderRows;
        QList<QString> genderTitles;
        for (auto gender : titlesAndValues) {
            genderRows << gender->value;
            genderTitles << gender->title;
        }
        _genderOtherAnswer.setRows(genderRows);
        _genderUserAnswer.setRows({myAnswer});

        int genderBarsCount = _genderOtherAnswer.getBarsCount();
        _myGenderIndex = myRow;

        setPointToPickFrom(coordinateFromPixel(_width / 2, _height / 2));
        setViewBarsQuantity(genderBarsCount + myAnswer, AnswerByGenderMode);
        setViewTitles(genderTitles, AnswerByGenderMode);
        setViewActiveMode(AnswerByGenderMode);
    } else {
        QList<int> cultureRows;
        QList<QString> cultureTitles;

        for (auto culture : titlesAndValues) {
            cultureRows << culture->value;
            cultureTitles << culture->title;
        }

        _cultureOtherAnswer.setRows(cultureRows);
        _cultureUserAnswer.setRows({myAnswer});

        int cultureBarsCount = _cultureOtherAnswer.getBarsCount();
        _myCultureIndex = myRow;

        setPointToPickFrom(QPointF(0, 0));
        setViewBarsQuantity(cultureBarsCount + myAnswer, AnswerByCultureMode);
        setViewTitles(cultureTitles, AnswerByCultureMode);
        setViewActiveMode(AnswerByCultureMode);
    }
}

void ViewModeManager::goToScreensaver()
{
    if (_viewActiveMode != ScreenSaverMode) {
        int totalBars = _viewBars[_viewActiveMode]->size();
        _viewBars[ScreenSaverMode]->append(*_viewBars[_viewActiveMode]);
        *_viewBars[ScreenSaverMode] = _viewBars[_viewActiveMode]->mid(totalBars - 101, 100);
        _viewBars[_viewActiveMode]->clear();

        moveBarsToLayouts(ScreenSaverMode);
        setViewActiveMode(ScreenSaverMode);
    }
}

void ViewModeManager::setViewTitles(const QList<QString> &titles, ViewModeManager::ViewMode viewIndex)
{
    _viewTitles[viewIndex] = titles;
}

QPointF ViewModeManager::coordinateFromPixel(qreal x, qreal y)
{
    // Return a OpenGL coordinate from a given pixel
    return QPointF(mapValue(x, 0, _width, _left, _right),
                   // From top-left corner
                   mapValue(y, _height, 0, _top, _bottom));
}

QPointF ViewModeManager::sizeFromPixel(qreal width, qreal height)
{
    return QPointF(mapValue(width, 0, _width, 0, _right * 2),
                   mapValue(height, 0, _height, 0, _top * 2));
}

qreal ViewModeManager::heightFromPixel(qreal height)
{
    return mapValue(height, 0, _height, 0, _top * 2);
}

void ViewModeManager::setPointToPickFrom(const QPointF &point)
{
    _pointToPickFrom = point;
}

qreal ViewModeManager::mapValue(qreal value, qreal istart, qreal istop, qreal ostart, qreal ostop) const
{
    return ostart + (ostop - ostart) * ((value - istart) / (istop - istart));
}

void ViewModeManager::setupScreensaverLayout(ViewModeManager::ViewMode activeView)
{
    // Add object
    _screensaver.addBarObjects(_viewBars[ScreenSaverMode]);
    
    switch (activeView) {
    case ScreenSaverMode:
        _screensaver.setBarsSize(sizeFromPixel(3.5, 35));
        _screensaver.setBarsColor("#CCCCCC");
        break;
    case UserAnswersMode:
        _screensaver.setBarsSize(sizeFromPixel(3.5, 35));
        _screensaver.setBarsColor("#CCCCCC");
        break;
    default:
        _screensaver.setBarsSize(sizeFromPixel(3.5, 35));
        _screensaver.setBarsColor("#3D3D3D");
        break;
    }
    
    _screensaver.moveObjectsToLayout(currentTime());
}

void ViewModeManager::moveBarsToMultiAnswerLayout()
{
    qreal barHeight = fitToScreenHeight(35);
    qreal startY = fitToScreenHeight(143) + (barHeight / 2);
    qreal blockSpace = fitToScreenHeight(154);
    qreal rowSpace = fitToScreenHeight(12);
    qreal marginLeft = 100;


    ViewModeManager::viewBars averageVect = ViewModeManager::viewBars::create();
    ViewModeManager::viewBars userVect = ViewModeManager::viewBars::create();

    int averageBarsCount = _multiAverageAnswer.getBarsCount();
    int userBarsCount = _multiUserAnswer.getBarsCount();

    *averageVect = _viewBars[MultiAnswersMode]->mid(0, averageBarsCount);
    *userVect = _viewBars[MultiAnswersMode]->mid(averageBarsCount, userBarsCount);

    _multiAverageAnswer.addBarObjects(averageVect);
    _multiUserAnswer.addBarObjects(userVect);

    _multiAverageAnswer.setBarsSize(sizeFromPixel(3.5, barHeight));
    _multiAverageAnswer.setBarsColor("#FFFFFF");
    _multiAverageAnswer.setStartPosition(coordinateFromPixel(marginLeft, startY));
    _multiAverageAnswer.setDistanceBetweenRows(heightFromPixel(blockSpace));
    _multiAverageAnswer.moveObjectsToLayout(currentTime());

    _multiUserAnswer.setBarsSize(sizeFromPixel(3.5, barHeight));
    _multiUserAnswer.setBarsColor("#80E2A7");
    _multiUserAnswer.setStartPosition(coordinateFromPixel(marginLeft, startY + barHeight + rowSpace));
    _multiUserAnswer.setDistanceBetweenRows(heightFromPixel(blockSpace));
    _multiUserAnswer.moveObjectsToLayout(currentTime());
}

void ViewModeManager::moveBarsToAnswerByAgeLayout()
{
    qreal barHeight = fitToScreenHeight(35);
    qreal startY = fitToScreenHeight(50) + (barHeight / 2);
    qreal rowSpace = fitToScreenHeight(12.5);
    qreal marginLeft = 100;

    ViewModeManager::viewBars otherAgeVect = ViewModeManager::viewBars::create();
    ViewModeManager::viewBars myAgeVect = ViewModeManager::viewBars::create();

    int otherAgeBarsCount = _agesAnswerBarChart.getBarsCount();
    int userAgeBarsCount = _userAgeAnswer.getBarsCount();

    *otherAgeVect = _viewBars[AnswerByAgeMode]->mid(0, otherAgeBarsCount);
    *myAgeVect = _viewBars[AnswerByAgeMode]->mid(otherAgeBarsCount, userAgeBarsCount);

    _agesAnswerBarChart.addBarObjects(otherAgeVect);
    _agesAnswerBarChart.setBarsSize(sizeFromPixel(3.5, barHeight));
    _agesAnswerBarChart.setBarsColor("#FFFFFF");
    _agesAnswerBarChart.setStartPosition(coordinateFromPixel(marginLeft, startY));
    _agesAnswerBarChart.setDistanceBetweenRows(heightFromPixel(rowSpace));
    _agesAnswerBarChart.moveObjectsToLayout(currentTime());

    _userAgeAnswer.addBarObjects(myAgeVect);
    _userAgeAnswer.setBarsSize(sizeFromPixel(3.5, barHeight));
    _userAgeAnswer.setBarsColor("#80E2A7");
    _userAgeAnswer.setStartPosition(
                coordinateFromPixel(
                    marginLeft + 2,
                    startY + 2 + _myAgeReverseIndex * (barHeight + rowSpace)));
    _userAgeAnswer.moveObjectsToLayout(currentTime());
}

void ViewModeManager::moveBarsToAnswerByGenderLayout()
{
    qreal barHeight = fitToScreenHeight(35);
    qreal startY = fitToScreenHeight(332) + (barHeight / 2);
    qreal rowSpace = fitToScreenHeight(153);
    qreal marginLeft = 100;
    qreal answerStartY = startY + barHeight + 12;

    ViewModeManager::viewBars otherGenderBars = ViewModeManager::viewBars::create();
    ViewModeManager::viewBars userGenderBars = ViewModeManager::viewBars::create();

    int otherGenderBarsCount = _genderOtherAnswer.getBarsCount();
    int userGenderBarsCount = _genderUserAnswer.getBarsCount();

    *otherGenderBars = _viewBars[AnswerByGenderMode]->mid(0, otherGenderBarsCount);
    *userGenderBars = _viewBars[AnswerByGenderMode]->mid(otherGenderBarsCount, userGenderBarsCount);

    _genderOtherAnswer.addBarObjects(otherGenderBars);
    _genderOtherAnswer.setBarsSize(sizeFromPixel(3.5, barHeight));
    _genderOtherAnswer.setBarsColor("#FFFFFF");
    _genderOtherAnswer.setStartPosition(coordinateFromPixel(marginLeft, startY));
    _genderOtherAnswer.setDistanceBetweenRows(heightFromPixel(rowSpace));
    _genderOtherAnswer.moveObjectsToLayout(currentTime());

    _genderUserAnswer.addBarObjects(userGenderBars);
    _genderUserAnswer.setBarsSize(sizeFromPixel(3.5, barHeight));
    _genderUserAnswer.setBarsColor("#80E2A7");
    _genderUserAnswer.setStartPosition(coordinateFromPixel(marginLeft, answerStartY + _myGenderIndex * (barHeight + rowSpace)));
    _genderUserAnswer.moveObjectsToLayout(currentTime());
}

void ViewModeManager::moveBarsToAnswerByCultureLayout()
{
    qreal barHeight = fitToScreenHeight(35);
    qreal startY = fitToScreenHeight(130) + (barHeight / 2);
    qreal rowSpace = fitToScreenHeight(115);
    qreal marginLeft = 100;
    qreal answerStartY = startY + barHeight + 12;

    ViewModeManager::viewBars otherCultureBars = ViewModeManager::viewBars::create();
    ViewModeManager::viewBars userCultureBars = ViewModeManager::viewBars::create();

    int otherCultureBarsCount = _cultureOtherAnswer.getBarsCount();
    int userCultureBarsCount = _cultureUserAnswer.getBarsCount();

    *otherCultureBars = _viewBars[AnswerByCultureMode]->mid(0, otherCultureBarsCount);
    *userCultureBars = _viewBars[AnswerByCultureMode]->mid(otherCultureBarsCount, userCultureBarsCount);

    _cultureOtherAnswer.addBarObjects(otherCultureBars);
    _cultureOtherAnswer.setBarsSize(sizeFromPixel(3.5, barHeight));
    _cultureOtherAnswer.setBarsColor("#FFFFFF");
    _cultureOtherAnswer.setStartPosition(coordinateFromPixel(marginLeft, startY));
    _cultureOtherAnswer.setDistanceBetweenRows(heightFromPixel(rowSpace));
    _cultureOtherAnswer.moveObjectsToLayout(currentTime());

    _cultureUserAnswer.addBarObjects(userCultureBars);
    _cultureUserAnswer.setBarsSize(sizeFromPixel(3.5, barHeight));
    _cultureUserAnswer.setBarsColor("#80E2A7");
    _cultureUserAnswer.setStartPosition(coordinateFromPixel(marginLeft, answerStartY + _myCultureIndex * (barHeight + rowSpace)));
    _cultureUserAnswer.moveObjectsToLayout(currentTime());
}

