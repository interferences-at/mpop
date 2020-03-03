#include "controller.h"
#include <QDebug>

Controller::Controller(OscReceiver* oscReceiver, const QVector<DatavizWindow*>& windows) :
    _oscReceiver(oscReceiver),
    _windows(windows)
{
    connect(oscReceiver, &OscReceiver::messageReceived, this, &Controller::messageReceivedCb);
}


void Controller::messageReceivedCb(const QString& oscAddress, const QVariantList& arguments) {
    qDebug() << "Controller received" << oscAddress << arguments;
}
