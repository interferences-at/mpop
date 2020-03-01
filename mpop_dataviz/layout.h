#pragma once


#include "sceneobject.h"
#include <QVector>

class Layout
{
public:
    Layout();
    virtual ~Layout();
    void addObjects(const QVector<SceneObject*>& sceneObjects);
    virtual void moveObjectsToLayout() = 0;

protected:
    QVector<SceneObject*> _sceneObjects;
};
