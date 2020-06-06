#include "layout.h"

Layout::Layout()
{

}

Layout::~Layout()
{

}


void Layout::addPrisonerLines(QSharedPointer<QVector<PrisonerLine::ptr> > prisonerLines) {
    _prisonerLines = prisonerLines;
}

void Layout::setBarsQuantity(int number)
{
    if (number > 0) {
        int barSize = _prisonerLines->isEmpty() ? 0 : _prisonerLines->size();

        if (number > barSize) {
           int diff = number - barSize;
           for (int i = 0; i < diff; i++) {
               PrisonerLine::ptr line = PrisonerLine::ptr::create();
               _prisonerLines->push_back(line);
           }
        }

        if ( number < barSize) {
            int diff = barSize - number;
            for (int i = 0; i < diff; i++) {
                _prisonerLines->pop_back();
            }
        }
    }
}

void Layout::setBarsSize(qreal width, qreal height)
{
    for (auto line : *_prisonerLines) {
        line->setSize(width, height);
    }

    if (_barsWidth != width)
        _barsWidth = width;
    if (_barsHeight != height)
        _barsHeight = height;
    qDebug() << "Bar width: " << _barsWidth << " height: " << _barsHeight;
}

void Layout::setBarsSize(const QPointF &size)
{
    setBarsSize(size.x(), size.y());
}

void Layout::setBarsColor(const QString &colorHex)
{
    for (auto line : *_prisonerLines) {
        line->setColor(colorHex);
    }
}

void Layout::setLayoutCoordinate(float left, float right, float bottom, float top)
{
    _left = left;
    _right = right;
    _bottom = bottom;
    _top = top;
}
