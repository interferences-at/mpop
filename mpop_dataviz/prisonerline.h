#pragma once

#include "sceneobject.h"
#include <QPoint>
#include <QPropertyAnimation>
#include <QEasingCurve>

// TODO: rename to Stick (it's easier to read/write/say)
class PrisonerLine : public SceneObject
{
public:
    explicit PrisonerLine();
    ~PrisonerLine() override;
    virtual void draw(const qint64& elapsed) override;

    // Alias for shared pointer of this class
    typedef QSharedPointer<PrisonerLine> ptr;

private:
};
