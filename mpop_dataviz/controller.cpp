#include "controller.h"
#include <QDebug>
#include <QList>

Controller::Controller(OscReceiver* oscReceiver, const QVector<QSharedPointer<DatavizWindow>>& windows) :
    _oscReceiver(oscReceiver),
    _windows(windows)
{
    // FIXME: Maybe it will be less expensive if we pass
    // QMap through the constructor parameter instead of
    // Qvector specially if we start using custom window id
    for (auto window : _windows) {
        _windowsMap.insert(window->getWindowId(), window);
    }

    connect(oscReceiver, &OscReceiver::messageReceived, this, &Controller::messageReceivedCb);
}

QList<int> toInts(const QVariantList& arguments) {
    QList<int> ret;
    for (int i = 0; i < arguments.size(); i ++) {
        ret.append(arguments[i].toInt());
    }
    return ret;
}

QStringList splitPath(const QString& oscPath) {
    QRegExp regex("(\\/)"); // Regular expression for '/'
    QStringList ret = oscPath.split(regex);
    ret.removeFirst();
    return ret;
}


void Controller::messageReceivedCb(const QString& oscAddress, const QVariantList& arguments) {
    // TODO: perhaps move the OSC receiver out of the controller.
    qDebug() << "Controller received" << oscAddress << arguments;

    static const QString DATAVIZ_PREFIX = "dataviz";
    static const QString BARCHART_METHOD = "barchart";
    static const int INDEX_PREFIX = 0;
    static const int INDEX_WINDOW_NUMBER = 1;
    static const int INDEX_METHOD = 2;
    static const int EXPECTED_MINIMUM_PATH_TOKENS = 3;

    QStringList pathTokens = splitPath(oscAddress);
    qDebug() << "OSC Path tokens" << pathTokens;
    int numTokens = pathTokens.size();
    if (numTokens < EXPECTED_MINIMUM_PATH_TOKENS) {
        qDebug() << "Invalid OSC path" << oscAddress;
        return;
    }

    if (pathTokens[INDEX_PREFIX] == DATAVIZ_PREFIX) {
        int windowIndex = pathTokens[INDEX_WINDOW_NUMBER].toInt();
        qDebug() << "windowIndex" << windowIndex;
        if (getWindowById(windowIndex) == nullptr) {
            qDebug() << "Invalid dataviz window index" << windowIndex;
            return;
        }

        QString methodName = pathTokens[INDEX_METHOD];

        // Handle methods:
        // The barchart method: /dataviz/1/barchar iii 70 20 10
        if (methodName == BARCHART_METHOD) {
            QList<int> ints = toInts(arguments);
            qDebug() << "Calling showBarChart" << methodName << windowIndex << ints;
            this->showBarChart(windowIndex, ints);

        } else {
            qDebug() << "Unhandle OSC method" << methodName;
        }
    } else {
        qDebug() << "Unhandle OSC path" << oscAddress;
    }
}


void Controller::showBarChart(int windowIndex, const QList<int>& values) {
    DatavizWindow::ptr window = getWindowById(windowIndex);
    if (window) {
        window->showBarChartBars(values);
    }
}


DatavizWindow* Controller::getWindow(int windowIndex) const { // Q_DECL_DEPRECATED
    if (windowIndex < _windows.size() && windowIndex >= 0) {
        return _windows[windowIndex].get();
    } else {
        qDebug() << "No such window" << windowIndex;
        return nullptr;
    }
}

DatavizWindow::ptr Controller::getWindowById(uint windowId) const
{
    if (!_windowsMap.contains(windowId)) {
        qDebug() << "No such window" << windowId;
        return nullptr;
    }

    return _windowsMap[windowId];
}

