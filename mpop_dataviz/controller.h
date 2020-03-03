#pragma once

#include "oscreceiver.h"
#include "datavizwindow.h"
#include <QVector>
#include <QObject>

/**
 * @brief Controller for the business logic of this application.
 *
 * Receives OSC and controls the OpenGL data vizualisation windows.
 */
class Controller : public QObject
{
    Q_OBJECT
public:

    Controller(OscReceiver* oscReceiver, const QVector<DatavizWindow*>& windows);

    void showBarChart(int windowIndex, const QList<int>& values);


private: // data members
    OscReceiver* _oscReceiver;
    QVector<DatavizWindow*> _windows;

private:
    DatavizWindow* getWindow(int windowIndex) const;

public slots:
    void messageReceivedCb(const QString& oscAddress, const QVariantList& arguments);
};
