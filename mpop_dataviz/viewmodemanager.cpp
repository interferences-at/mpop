#include "viewmodemanager.h"

ViewModeManager *ViewModeManager::viewModeManager = nullptr;

ViewModeManager::ViewModeManager() :
    _viewActiveMode(ScreenSaverMode),
    _left(-1.0), _right(1.0),
    _bottom(-1.0), _top(1.0),
    _pointToPickFrom(QPointF(0, 0))
{
    _timer.start();

    _fairnessAverageAnswer = QVector<BarChartLayout>(5);
    _fairnessUserAnswer = QVector<BarChartLayout>(5);

    for (int i = 0; i < 6; i++) {
        ViewModeManager::viewBars sharedVector = ViewModeManager::viewBars::create();
        _viewBars.push_back(sharedVector);
    }
}

void ViewModeManager::showViewManagerBars(ViewMode mode)
{
    switch (mode) {
    case ScreenSaverMode:
        _screensaver.updateBarsPosition(currentTime());
        _screensaver.showSceneObject(currentTime());
        break;
    case UserAnswersMode:
        _screensaver.updateBarsPosition(currentTime());
        _userAnswers.updateBarsPosition(currentTime());
        _screensaver.showSceneObject(currentTime());
        _userAnswers.showSceneObject(currentTime());
        break;
    case FairnessAnswersMode:
        _screensaver.updateBarsPosition(currentTime());
        _screensaver.showSceneObject(currentTime());
        for (int i = 0; i < _fairnessAverageAnswer.size(); i++) {
            _fairnessAverageAnswer[i].updateBarsPosition(currentTime());
            _fairnessAverageAnswer[i].showSceneObject(currentTime());

            _fairnessUserAnswer[i].updateBarsPosition(currentTime());
            _fairnessUserAnswer[i].showSceneObject(currentTime());
        }
        break;
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
}

void ViewModeManager::updateViewSize(int width, int height)
{
    _width = width;
    _height = height;

    switch (getViewActiveMode()) {
    case ScreenSaverMode:
        _screensaver.setBarsSize(sizeFromPixel(3, 60));
        break;
    case UserAnswersMode:

        break;
    default:
        break;
    }
}

void ViewModeManager::setViewActiveMode(ViewMode mode)
{
    if (_viewActiveMode != mode)
        _viewActiveMode = mode;
}

void ViewModeManager::setBarChartRows(const QList<int> &bars, ViewMode viewIndex)
{
    switch (viewIndex) {
    case ScreenSaverMode:

        break;
    case UserAnswersMode:
        _userAnswers.setRows(bars);
        break;
    default:
        break;
    }
}

void ViewModeManager::moveBarsToLayouts(ViewModeManager::viewBars bars, ViewMode viewIndex)
{
//    _viewBars[viewIndex] = bars;

    switch (viewIndex) {
    case ScreenSaverMode:
        _screensaver.addPrisonerLines(_viewBars[ScreenSaverMode]);
        _screensaver.setBarsColor("#CCCCCC");
        break;
    case UserAnswersMode:
        _screensaver.addPrisonerLines(_viewBars[ScreenSaverMode]);
        _screensaver.setBarsSize(sizeFromPixel(3, 40));
        _screensaver.setBarsColor("#CCCCCC");
        _screensaver.moveObjectsToLayout(currentTime());

        _userAnswers.addPrisonerLines(_viewBars[UserAnswersMode]);
        _userAnswers.setBarsSize(sizeFromPixel(3, 74));
        _userAnswers.setBarsColor("#FF0000");
        _userAnswers.setAlignCenter(true);
        _userAnswers.moveObjectsToLayout(currentTime());
        break;
    case FairnessAnswersMode:
        _screensaver.addPrisonerLines(_viewBars[ScreenSaverMode]);
        _screensaver.setBarsSize(sizeFromPixel(2, 35));
        _screensaver.setBarsColor("#3D3D3D");
        _screensaver.moveObjectsToLayout(currentTime());
    {
        int y = 129;
        int interval = 182;
        int index = 0;

        for (int i = 0; i < _fairnessAverageAnswer.size(); i++) {
            ViewModeManager::viewBars averageVect = ViewModeManager::viewBars::create();
            ViewModeManager::viewBars userVect = ViewModeManager::viewBars::create();
            for (int x = 0; x < _fairnessAverageAnswer[i].getRows().at(0); x++) {
                averageVect->push_back(_viewBars[FairnessAnswersMode]->at(index));
                index++;
            }
            for (int x = 0; x < _fairnessUserAnswer[i].getRows().at(0); x++) {
                userVect->push_back(_viewBars[FairnessAnswersMode]->at(index));
                index++;
            }

            _fairnessAverageAnswer[i].addPrisonerLines(averageVect);
            _fairnessUserAnswer[i].addPrisonerLines(userVect);

            _fairnessAverageAnswer[i].setBarsSize(sizeFromPixel(2, 35));
            _fairnessAverageAnswer[i].setBarsColor("#FFFFFF");
            _fairnessAverageAnswer[i].setStartPosition(coordinateFromPixel(95, y + i * interval));
            _fairnessAverageAnswer[i].moveObjectsToLayout(currentTime());

            _fairnessUserAnswer[i].setBarsSize(sizeFromPixel(2, 35));
            _fairnessUserAnswer[i].setBarsColor("#AB3D33");
            _fairnessUserAnswer[i].setStartPosition(coordinateFromPixel(95, y + 45 + i * interval));
            _fairnessUserAnswer[i].moveObjectsToLayout(currentTime());
        }
    }
        break;
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

    moveBarsToLayouts(_viewBars[viewIndex], viewIndex);
}

ViewModeManager::viewBars ViewModeManager::getBarsFromScreenSaver(int number)
{
    qreal pointX = _pointToPickFrom.x();
    qreal pointY = -_pointToPickFrom.y();

    // Sort bars by the closest to the point(x, y)
    std::sort(_viewBars[ScreenSaverMode]->begin(), _viewBars[ScreenSaverMode]->end(),
              [&](PrisonerLine::ptr a, PrisonerLine::ptr b) -> bool {
        return pow(abs(a->getX() - pointX), 2) + pow(abs(a->getY() - pointY), 2) <
                pow(abs(b->getX() - pointX), 2) + pow(abs(b->getY() - pointY), 2);
    });

    ViewModeManager::viewBars barChartBars = ViewModeManager::viewBars::create();

    for (int i = 0; i < number; i++) {
        // The Screensaver amount of bars must be greater by 100 than...
        while (_viewBars[ScreenSaverMode]->size() - i < 100) {
            // Add new line every time screensaver bar is about to finish
            // To avoid out of range error
            PrisonerLine::ptr line = PrisonerLine::ptr::create();
            _viewBars[ScreenSaverMode]->push_back(line);
        }

        barChartBars->push_back(_viewBars[ScreenSaverMode]->at(i));
    }
    _viewBars[ScreenSaverMode]->remove(0, number);

    return barChartBars;
}

void ViewModeManager::setUserAnswerBars(const QList<int> &bars)
{
    int barSum = std::accumulate(bars.begin(), bars.end(), 0);

    setBarChartRows(bars, UserAnswersMode);
    setViewBarsQuantity(barSum, UserAnswersMode);
    setViewActiveMode(UserAnswersMode);
}

void ViewModeManager::setFairnessAnswerBars()
{

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

void ViewModeManager::setPointToPickFrom(const QPointF &point)
{
    _pointToPickFrom = point;
}

qreal ViewModeManager::mapValue(qreal value, qreal istart, qreal istop, qreal ostart, qreal ostop)
{
    return ostart + (ostop - ostart) * ((value - istart) / (istop - istart));
}

void ViewModeManager::showAnswersData(const QList<AnswerDataPtr>& answers) {
    int answerTotal = 0;

    for (int i = 0; i < answers.size(); i++) {
        QList<int> list; list << answers.at(i)->their_answer;
        _fairnessAverageAnswer[i].setRows(list);
        QList<int> list2; list2 << answers.at(i)->my_answer;
        qDebug() << "User answer: " << list2.at(0);
        _fairnessUserAnswer[i].setRows(list2);

        answerTotal += answers.at(i)->their_answer;
        answerTotal += answers.at(i)->my_answer;
    }
    qDebug() << "Total bar: " << answerTotal;
    setViewBarsQuantity(answerTotal, FairnessAnswersMode);
    setViewActiveMode(FairnessAnswersMode);
}
