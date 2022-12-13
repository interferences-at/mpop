#include "layout.h"

Layout::Layout()
{

}

Layout::~Layout()
{

}


void Layout::addBarObjects(QSharedPointer<QVector<PrisonerLine::ptr> > barObjects) {
    _barObjects = barObjects;
}

void Layout::setBarsSize(qreal width, qreal height) {
    for (auto line : *_barObjects) {
        line->setSize(width, height);
    }
    if (_barsWidth != width) {
        _barsWidth = width;
    }
    if (_barsHeight != height) {
        _barsHeight = height;
    }
}

void Layout::setBarsSize(const QPointF &size) {
    setBarsSize(size.x(), size.y());
}

void Layout::setBarsColor(const QString &colorHex) {
    for (auto line : *_barObjects) {
        line->setColor(colorHex);
    }
}

void Layout::setLayoutCoordinate(float left, float right, float bottom, float top) {
    _left = left;
    _right = right;
    _bottom = bottom;
    _top = top;
}
