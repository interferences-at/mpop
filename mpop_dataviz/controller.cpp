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
static const QString VIEW_ANSWER_BY_LANGUAGE_METHOD = "view_answer_by_language";
static const QString VIEW_ALL_ANSWERS_METHOD = "all_results";
static const QString DATAVIZ_LANGUAGE_METHOD = "language";
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
static bool parseViewAnswers(const QVariantList& arguments, ViewModeManager::AnswerDataPtr toPopulate) {
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

            toPopulate->text.append(arguments.at(text_index).toString());
            toPopulate->my_answer.append(arguments.at(mine_index).toInt());
            toPopulate->their_answer.append(arguments.at(theirs_index).toInt());
        }
        return true;
    }
}

/**
 * Parse the OSC message for view_answer_by_culture and view_answer_by_gender
 * /dataviz/0/view_answer_by_culture
 * iiisisisisisi 5 2 50
 * "Québécoise" 50 "Canadienne" 50 "Autochtone" 50 "Européenne" 50 "Autre" 50
 */
static bool parseViewAnswerRows(const QVariantList& arguments, int& outMyAnswer, int& outMyRow, QList<ViewModeManager::TitleAndValuePtr>& toPopulate) {
    static const int MIN_NUM_ARGS = 3; // We expect at least this many args
    static const int INDEX_NUM_ROWS = 0; // 0th arg is how many rows there are
    static const int INDEX_MY_ROW = 1; // 1st arg is in which row my answer is.
    static const int INDEX_MY_VALUE = 2; // 2nd arg is that value to show for my answer
    static const int OFFSET_ROWS = 3; // And then we start to specify
    static const int NUM_ARGS_PER_ROW = 2; // Groups of 2 values for the text + number to show in each row

    // check number of arguments
    int numArgs = arguments.size();
    if (numArgs < MIN_NUM_ARGS) {
        qDebug() << "Not enough OSC arguments" << arguments;
        return false;
    }

    int num_rows = arguments.at(INDEX_NUM_ROWS).toInt();
    int expectedNumArgs = OFFSET_ROWS + NUM_ARGS_PER_ROW * num_rows;
    if (numArgs < expectedNumArgs) {
        qDebug() << "Not enough OSC arguments for parseViewAnswerRows";
        qDebug() << "Expected" << expectedNumArgs << "but got" << numArgs;
        qDebug() << arguments;
        return false;
    }

    // Retrieve my value and my row index
    int my_row = arguments.at(INDEX_MY_ROW).toInt();
    int my_value = arguments.at(INDEX_MY_VALUE).toInt();
    outMyAnswer = my_value;
    outMyRow = my_row;

    // Retrieve each title - value pair
    for (int i = OFFSET_ROWS; i < expectedNumArgs; i += NUM_ARGS_PER_ROW) {
        QString title = arguments.at(i).toString();
        int value = arguments.at(i + 1).toInt();
        auto item = ViewModeManager::TitleAndValuePtr::create();
        item->title = title;
        item->value = value;
        toPopulate.append(item);
    }
    return true;
}


/**
 * @brief Parses view_answer_by_age
 *
 * /dataviz/0/view_answer_by_age iiiiiiiiiiiiiiiiiiiii 99 21 5 10 15 20 25 30 35 40 45 50 55 60 65 70 75 80 85 90 95 100
 *
 * The first argument is my answer.
 * The second argument is the index of the row in which my age is. Example: If I am 21 years old, the index is 4. ([0,5], [5,10], [11,19], [20,25], etc.)
 * Finally, the value for each of the twenty (20) rows.
 *
 * @param outMyAnswer Result for my answer
 * @param outMyRowIndex Result for my row index
 * @param outValues Result for the list of values
 * @return true if successfully parsed
 */
bool parseViewAnswerByAge(const QVariantList& arguments, int& outMyAnswer, int& outMyRowIndex, QList<int>& outValues) {
    static const int NUM_ARGS = 22; // We expect exactly this many args
    static const int INDEX_MY_VALUE = 0; // 0th arg is that value to show for my answer
    static const int INDEX_MY_ROW = 1; // 1st arg is in which row my answer is.
    static const int OFFSET_ROWS = 2; // And then we start to specify

    // check number of arguments
    int numArgs = arguments.size();
    if (numArgs < NUM_ARGS) {
        qDebug() << "Not enough OSC arguments for parseViewAnswerByAge";
        qDebug() << "Expected" << NUM_ARGS << "but got" << numArgs;
        qDebug() << arguments;
        return false;
    }

    // Retrieve my value and my row index
    int my_row = arguments.at(INDEX_MY_ROW).toInt();
    int my_value = arguments.at(INDEX_MY_VALUE).toInt();
    outMyAnswer = my_value;
    outMyRowIndex = my_row;

    // Retrieve each value
    for (int i = OFFSET_ROWS; i < NUM_ARGS; i += 1) {
        int value = arguments.at(i).toInt();
        outValues.append(value);
    }

    return true;
}


/**
 * @brief Controller::messageReceivedCb
 * @param oscAddress
 * @param arguments
 */
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

        qDebug() << "methodName" << methodName;

        // Handle methods:
        // The barchart method: /dataviz/1/barchart iii 70 20 10
        if (methodName == BARCHART_METHOD) {
            QList<int> ints = toInts(arguments);
            qDebug() << "Calling showBarChart" << methodName << windowIndex << ints;
            this->showUserAnswer(windowIndex, ints);
        } else if (methodName == VIEW_ANSWERS_METHOD) {
            ViewModeManager::AnswerDataPtr toPopulate = ViewModeManager::AnswerDataPtr::create();
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
                // set Screensaver param
                this->setScreenSaverParam(windowIndex, paramName, paramValue);
            }
        } else if (methodName == VIEW_ANSWER_BY_CULTURE_METHOD) {
            int myAnswer = 0;
            int myRow = 0;
            QList<ViewModeManager::TitleAndValuePtr> toPopulate;
            bool ok = parseViewAnswerRows(arguments, myAnswer, myRow, toPopulate);
            if (ok) {
                this->showSingleAnswer(windowIndex, myRow, myAnswer, toPopulate);
            }
        } else if (methodName == VIEW_ANSWER_BY_GENDER_METHOD) {
            int myAnswer = 0;
            int myRow = 0;
            QList<ViewModeManager::TitleAndValuePtr> toPopulate;
            bool ok = parseViewAnswerRows(arguments, myAnswer, myRow, toPopulate);
            if (ok) {
                this->showSingleAnswer(windowIndex, myRow, myAnswer, toPopulate);
            }
        } else if (methodName == VIEW_ANSWER_BY_AGE_METHOD) {
            // TODO
            int myAnswer = 0;
            int myRowIndex = 0;
            QList<int> values; // value for each age range
            bool ok = parseViewAnswerByAge(arguments, myAnswer, myRowIndex, values);
            if (ok) {
                this->showSingleAnswerByAge(windowIndex, myAnswer, myRowIndex, values);
            }
        } else if (methodName == VIEW_ANSWER_BY_LANGUAGE_METHOD) {
            int myAnswer = 0;
            int myRow = 0;
            QList<ViewModeManager::TitleAndValuePtr> toPopulate;
            bool ok = parseViewAnswerRows(arguments, myAnswer, myRow, toPopulate);
            if (ok) {
                this->showSingleAnswer(windowIndex, myRow, myAnswer, toPopulate);
            }
        } else if (methodName == VIEW_ALL_ANSWERS_METHOD) {
            /* /all_results iiiiiiiiiiiiiiiiiiiiiiiii
             * 21 12 1 52 15 57 41 24 89 72 98 40 66 20 7 36 7 44 65 9 93 46 56 25 5
            */
            QList<int> answersValues = toInts(arguments);

            if (numArgs != 25){
                qDebug() << "Uncorrect number of arguments";
            } else {
                showAllAnswers(windowIndex, answersValues);
            }
        } else if (methodName == DATAVIZ_LANGUAGE_METHOD) {
            setDatavizLanguage(windowIndex, arguments.at(0).toString());
        } else {
            qDebug() << "Unhandled OSC method" << methodName;
        }
    } else {
        qDebug() << "Unhandle OSC path" << oscAddress;
    }
}


void Controller::setScreenSaverParam(int windowIndex, const QString& paramName, float paramValue) {
    DatavizWindow::ptr window = getWindowById(windowIndex);
    if (window) {
        window->viewManager()->getScreensaver()->setParam(paramName, paramValue);
    }
}


void Controller::showAnswers(int windowIndex, ViewModeManager::AnswerDataPtr answers) {
    DatavizWindow::ptr window = getWindowById(windowIndex);
    if (window) {
        window->viewManager()->setMultiAnswersBars(answers);
    }
}


void Controller::showSingleAnswer(int windowIndex, int myRow, int myAnswer, const QList<ViewModeManager::TitleAndValuePtr>& titlesAndValues) {
    DatavizWindow::ptr window = getWindowById(windowIndex);
    if (window) {
        int numRows = titlesAndValues.size();
        window->viewManager()->showOneAnswer(numRows, myRow, myAnswer, titlesAndValues);
    }
}

void Controller::showSingleAnswerByAge(int windowIndex, int myAnswer, int myRowIndex, const QList<int>& values) {
    DatavizWindow::ptr window = getWindowById(windowIndex);
    if (window) {
        window->viewManager()->showOneAnswerByAge(myRowIndex, myAnswer, values);
    }
}

void Controller::showAllAnswers(int windowIndex, const QList<int> &values)
{
    DatavizWindow::ptr window = getWindowById(windowIndex);
    if (window) {
        window->viewManager()->setAllAnswersBars(values);
    }
}

void Controller::setDatavizLanguage(int windowIndex, const QString &lang)
{
    DatavizWindow::ptr window = getWindowById(windowIndex);
    if (window) {
        window->setTextPainterLanguage(lang);
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

