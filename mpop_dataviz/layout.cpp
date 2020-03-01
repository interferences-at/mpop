#include "layout.h"

Layout::Layout()
{

}

Layout::~Layout()
{

}


void Layout::addObjects(const QVector<SceneObject*>& sceneObjects) {
    _sceneObjects.append(sceneObjects);
}
