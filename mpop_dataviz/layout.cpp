#include "layout.h"

Layout::Layout()
{

}

Layout::~Layout()
{

}


void Layout::addPrisonerLines(const QVector<PrisonerLine::ptr> &prisonerLines) {
    _prisonerLines.append(prisonerLines);
}
