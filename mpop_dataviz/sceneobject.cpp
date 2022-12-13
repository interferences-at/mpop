#include "sceneobject.h"


SceneObject::SceneObject() { // QObject* parent) : QObject(parent) {
    _isVisible = true;
    _x = 0.0;
    _y = 0.0;
    _z = 0.0;
    _scale = 1.0;
    _rotation = 0.0;
}


SceneObject::~SceneObject() {

}


void SceneObject::setVisible(bool isVisible) {
    _isVisible = isVisible;
}


bool SceneObject::getVisible() const {
    return this->_isVisible;
}


void SceneObject::setPosition(qreal x, qreal y) {
    setX(x);
    setY(y);
}


// FIXME: deprecated
void SceneObject::setOrientation(qreal degrees) {
    //_angleDegrees = degrees;
    setRotation(degrees);
}


void SceneObject::setX(qreal value) {
    _x = value;
    //emit xChanged(value);
}


void SceneObject::setY(qreal value) {
    _y = value;
    //emit yChanged(value);
}


void SceneObject::setZ(qreal value) {
    _z = value;
    //emit zChanged(value);
}


void SceneObject::setRotation(qreal value) {
    _rotation = value;
    //emit rotationChanged(value);
}


void SceneObject::setScale(qreal value) {
    _scale = value;
    //emit scaleChanged(value);
}
