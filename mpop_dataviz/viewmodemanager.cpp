#include "viewmodemanager.h"

ViewModeManager *ViewModeManager::viewModeManager = nullptr;

ViewModeManager::ViewModeManager() :
    _viewActiveMode(ScreenSaverMode),
    _left(-1.0), _right(1.0),
    _bottom(-1.0), _top(1.0),
    _pointToPickFrom(QPointF(0, 0))
{
    _timer.start();

    _barChartRows = QVector<QList<int>>(6);

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
        _answersBarchart.updateBarsPosition(currentTime());
        _screensaver.showSceneObject(currentTime());
        _answersBarchart.showSceneObject(currentTime());
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
    //    _barChartRows[viewIndex] = bars;

    switch (viewIndex) {
    case ScreenSaverMode:

        break;
    case UserAnswersMode:
        _answersBarchart.setRows(bars);
        break;
    default:
        break;
    }
}

void ViewModeManager::moveBarsToLayouts(ViewModeManager::viewBars bars, ViewMode viewIndex)
{
    //    restoreBarsToScreenSaver(viewIndex);
    _viewBars[viewIndex] = bars;

    for (int i = 0; i < _viewBars.size(); i++) {
        qDebug() << "View bar size: " << _viewBars.at(i)->size();
    }
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

        _answersBarchart.addPrisonerLines(_viewBars[UserAnswersMode]);
        _answersBarchart.setBarsSize(sizeFromPixel(3, 74));
        _answersBarchart.setBarsColor("#FF0000");
        _answersBarchart.setAlignCenter(true);
        //        _answersBarchart.setStartPosition(coordinateFromPixel(100, 100));
        _answersBarchart.moveObjectsToLayout(currentTime());
        break;
    default:
        break;
    }
}

void ViewModeManager::setViewBarsQuantity(int number, ViewMode viewIndex)
{
    qDebug() << "Index: " << viewIndex;
    if (number > 0) {
        int barSize = _viewBars[viewIndex]->isEmpty() ? 0 : _viewBars[viewIndex]->size();
        qDebug () << "Number: " << number << " barSize: " << _viewBars[viewIndex]->size();
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
        if (i >= _viewBars[ScreenSaverMode]->size()) break;
        PrisonerLine::ptr line = _viewBars[ScreenSaverMode]->at(i);
        barChartBars->push_back(line);
        _viewBars[ScreenSaverMode]->remove(i);
    }

    return barChartBars;
}

void ViewModeManager::restoreBarsToScreenSaver(ViewModeManager::ViewMode viewIndex)
{
    if (! _viewBars[viewIndex]->isEmpty()) {
        for (auto bar : *_viewBars[viewIndex]) {
            _viewBars[ScreenSaverMode]->push_back(bar);
        }
    }
}

void ViewModeManager::setUserAnswerBars(const QList<int> &bars)
{
    int barSum = std::accumulate(bars.begin(), bars.end(), 0);
    setBarChartRows(bars, UserAnswersMode);
    //    setPointToPickFrom(coordinateFromPixel(100, 100));
    setViewBarsQuantity(barSum, UserAnswersMode);
//    moveBarsToLayouts(getBarsFromScreenSaver(bars), UserAnswersMode);
    setViewActiveMode(UserAnswersMode);
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





