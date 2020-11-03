#pragma once

#include "oscreceiver.h"
#include "datavizwindow.h"
#include "viewmodemanager.h"
#include <QVector>
#include <QObject>
#include <QSharedPointer>

/**
 * @brief Controller for the business logic of this application.
 *
 * Receives OSC and controls the OpenGL data vizualisation windows.
 */
class Controller : public QObject
{
    Q_OBJECT
public:

    Controller(OscReceiver* oscReceiver, const QVector<QSharedPointer<DatavizWindow>>& windows);

    void showUserAnswer(int windowIndex, const QList<int>& values);
    void showAnswers(int windowIndex, ViewModeManager::AnswerDataPtr answers);
    void goToScreensaver(int windowIndex);
    void setScreenSaverParam(int windowIndex, const QString& paramName, float paramValue);
    void showSingleAnswer(int windowIndex, int myRow, int myAnswer, const QList<ViewModeManager::TitleAndValuePtr>& titlesAndValues);
    void showSingleAnswerByAge(int windowIndex, int myAnswer, int myRowIndex, const QList<int>& values);
    void showAllAnswers(int windowIndex, const QList<int> &values);
    void setDatavizLanguage(int windowIndex, const QString &lang);
    // Show/Hide test card via OSC
    void showTestCard(int windowIndex, bool visible);
    void setDatavizLanguage(int windowIndex, const QString &lang);

private:
    // data members
    OscReceiver* _oscReceiver;
    QVector<QSharedPointer<DatavizWindow>> _windows;
    // Make easy to find window by ID
    QMap<uint, DatavizWindow::ptr> _windowsMap;


private:
    DatavizWindow* getWindow(int windowIndex) const Q_DECL_DEPRECATED;
    DatavizWindow::ptr getWindowById(uint windowId) const;

public slots:
    void messageReceivedCb(const QString& oscAddress, const QVariantList& arguments);
};
