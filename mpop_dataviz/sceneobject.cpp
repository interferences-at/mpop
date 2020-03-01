#include "sceneobject.h"

SceneObject::SceneObject() {
    _isVisible = true;
}

SceneObject::~SceneObject() {

}

void SceneObject::setVisible(bool isVisible) {
    _isVisible = isVisible;
}

bool SceneObject::getVisible() const {
    return this->_isVisible;
}
