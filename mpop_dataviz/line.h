#pragma once

#include "sceneobject.h"

class Line : public SceneObject
{
public:
    Line();
    ~Line() override;
    virtual void draw(const QElapsedTimer& timer) override;
};
