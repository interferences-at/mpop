#include "controller.h"
#include <QDebug>
#include <QList>
#include <QString>

// constants
// For the OSC message handling:
static const QString NAMESPACE_PATH_PREFIX = "dataviz";
static const QString BARCHART_METHOD = "my_answer";
static const QString VIEW_ANSWERS_METHOD = "view_answers";
static const QString GOTO_SCREENSAVER_METHOD = "goto_screensaver";
static const QString SCREENSAVER_SET_PARAM_METHOD = "screensaver_set_param";
static const QString VIEW_ANSWER_BY_CULTURE_METHOD = "view_answer_by_culture";
static const QString VIEW_ANSWER_BY_GENDER_METHOD = "view_answer_by_gender";
static const QString VIEW_ANSWER_BY_AGE_METHOD = "view_answer_by_age";
static const int INDEX_NAMESPACE_PREFIX = 0;
static const int INDEX_WINDOW_NUMBER = 1;
static const int INDEX_METHOD = 2;
static const int EXPECTED_MINIMUM_PATH_TOKENS = 3;


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

/**
 * @brief Parse the OSC message for view_answers
 * @param arguments List of OSC arguments.
 * @param toPopulate List of answers to populate.
 * @return Success if it successfully parsed it.
 */
static bool parseViewAnswers(const QVariantList& arguments, QList<ViewModeManager::AnswerDataPtr>& toPopulate) {
    int num_answers = 0;
    static const int OFFSET_NUM_VALUES = 1;
    static const int NUM_ARGS_PER_ITEM = 3;
    static const int INDEX_TEXT = 0;
    static const int INDEX_MINE = 1;
    static const int INDEX_THEIRS = 2;

    if (arguments.size() >= OFFSET_NUM_VALUES) {
        num_answers = arguments[0].toInt();
    }

    if (arguments.size() != num_answers * NUM_ARGS_PER_ITEM + OFFSET_NUM_VALUES) {
        qDebug() << "Wrong number of arguments for " << VIEW_ANSWERS_METHOD;
        return false;
    } else {
        for (int i = 0; i < num_answers; i ++) {
            int text_index = ( i * NUM_ARGS_PER_ITEM ) + OFFSET_NUM_VALUES + INDEX_TEXT;
            int mine_index = ( i * NUM_ARGS_PER_ITEM ) + OFFSET_NUM_VALUES + INDEX_MINE;
            int theirs_index = ( i * NUM_ARGS_PER_ITEM ) + OFFSET_NUM_VALUES + INDEX_THEIRS;

            auto item = ViewModeManager::AnswerDataPtr(new ViewModeManager::AnswerData);
            item->text = arguments.at(text_index).toString();
            item->my_answer = arguments.at(mine_index).toInt();
            item->their_answer = arguments.at(theirs_index).toInt();
            toPopulate.append(item);
        }
        return true;
    }
}


void Controller::messageReceivedCb(const QString& oscAddress, const QVariantList& arguments) {
    // TODO: perhaps move the OSC receiver out of the controller.
    qDebug() << "Controller received" << oscAddress << arguments;

    QStringList pathTokens = splitPath(oscAddress);
    qDebug() << "OSC Path tokens" << pathTokens;
    int numTokens = pathTokens.size();
    if (numTokens < EXPECTED_MINIMUM_PATH_TOKENS) {
        qDebug() << "Invalid OSC path" << oscAddress;
        return;
    }

    if (pathTokens[INDEX_NAMESPACE_PREFIX] == NAMESPACE_PATH_PREFIX) {
        int windowIndex = pathTokens[INDEX_WINDOW_NUMBER].toInt();
        qDebug() << "windowIndex" << windowIndex;
        if (getWindowById(windowIndex) == nullptr) {
            qDebug() << "Invalid dataviz window index" << windowIndex;
            return;
        }

        QString methodName = pathTokens[INDEX_METHOD];
        int numArgs = arguments.size();

        // Handle methods:
        // The barchart method: /dataviz/1/barchart iii 70 20 10
        if (methodName == BARCHART_METHOD) {
            QList<int> ints = toInts(arguments);
            qDebug() << "Calling showBarChart" << methodName << windowIndex << ints;
            this->showUserAnswer(windowIndex, ints);
        } else if (methodName == VIEW_ANSWERS_METHOD) {
            QList<ViewModeManager::AnswerDataPtr> toPopulate;
            if (parseViewAnswers(arguments, toPopulate)) {
                qDebug() << "Calling view_answers";
                this->showAnswers(windowIndex, toPopulate);
            }
        } else if (methodName == GOTO_SCREENSAVER_METHOD) {
            this->goToScreensaver(windowIndex);
        } else if (methodName == SCREENSAVER_SET_PARAM_METHOD) {
            if (numArgs < 2) {
                qDebug() << "Invalid num of args";
                return;
            } else {
                QString paramName = arguments[0].toString();
                float paramValue = arguments[1].toFloat();
                // TODO controller=> set Screensaver param
            }
        } else if (methodName == VIEW_ANSWER_BY_CULTURE_METHOD) {
            // TODO
        } else if (methodName == VIEW_ANSWER_BY_GENDER_METHOD) {
            // TODO
        } else if (methodName == VIEW_ANSWER_BY_AGE_METHOD) {
            // TODO
        } else {
            qDebug() << "Unhandle OSC method" << methodName;
        }
    } else {
        qDebug() << "Unhandle OSC path" << oscAddress;
    }
}

void Controller::showAnswers(int windowIndex, const QList<ViewModeManager::AnswerDataPtr>& answers) {
    DatavizWindow::ptr window = getWindowById(windowIndex);
    if (window) {
        window->viewManager()->showAnswersData(answers);
    }
}

void Controller::showUserAnswer(int windowIndex, const QList<int>& values) {
    DatavizWindow::ptr window = getWindowById(windowIndex);
    if (window) {
        window->viewManager()->setUserAnswerBars(values);
    }
}

void Controller::goToScreensaver(int windowIndex) {
    DatavizWindow::ptr window = getWindowById(windowIndex);
    if (window) {
        // TODO
         window->viewManager()->goToScreensaver();
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

