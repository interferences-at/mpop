#include "facade.h"
#include "sqlerror.h"
#include <QSqlQuery>
#include <QVariant>
#include <QSqlError>
#include <QDebug>
#include <QStandardPaths>
#include <QDir>
#include <QSqlRecord>
#include <QSqlDriver>
#include <exception>
#include <QString>
#include <QDateTime>

/**
 * @brief Returns true if the strings match.
 * @param str
 * @param compare
 * @return True if they match
 */
static bool stringMatches(const QString& str, const char *compare) {
    return 0 == QString::compare(str, compare, Qt::CaseInsensitive);
}

Facade::Facade(const Config& config, QObject* parent) : QObject(parent), _config(config), _databaseReconnectTimer()
{
    _database = QSqlDatabase::addDatabase("QMYSQL");

    if (_database.isValid()) {
        qDebug() << "Database is valid";
        qDebug() << "QSqlDriver hasFeature NamedPlaceholders:" << _database.driver()->hasFeature(QSqlDriver::NamedPlaceholders);
        // We don't allow to change the configuration options at runtime. Restart the service to change them.
        _database.setHostName(_config.mysql_host);
        _database.setDatabaseName(_config.mysql_database);
        _database.setUserName(_config.mysql_user);
        _database.setPassword(_config.mysql_password);
        _database.setPort(_config.mysql_port);

        this->openDatabaseIfNeeded();

    } else {
        qDebug() << "Database is not valid. The QMYSQL driver for Qt is probably not installed.";
        // TODO: Exit with an error.
    }

    // Periodically make sure that our connection with the database is active.
    // Attempt to re-open it if not.
    connect(&_databaseReconnectTimer, &QTimer::timeout, this, &Facade::reconnectTimerTriggered);
    static const int RECONNECT_INTERVAL_SECONDS = 5;
    static const int MS_PER_SECOND = 1000;
    _databaseReconnectTimer.setInterval(RECONNECT_INTERVAL_SECONDS * MS_PER_SECOND);
    _databaseReconnectTimer.start();
}


bool Facade::isDatabaseReady() {
    return _database.isOpen();
}


void Facade::reconnectTimerTriggered() {
    // Called every time the _databaseReconnectTimer's signal is triggered.
    this->openDatabaseIfNeeded();
}


void Facade::openDatabaseIfNeeded() {
    if (_database.isValid()) {
        if (_database.isOpen()) {
            // qDebug() << "Database is already open. Good. Nothing to do.";
        } else {
            bool _is_db_open = _database.open();
            if (_is_db_open) {
                qInfo() << "Success connecting to the database.";
            } else {
                qWarning() << "ERROR: Could not open database";
                if (_database.isOpenError()) {
                    auto lastDbError = _database.lastError();
                    qInfo() << lastDbError;
                }
            }
        }
    }
}


int Facade::getOrCreateUser(const QString& rfidTag) {
    qDebug() << "getOrCreateUser";
    // FIXME: it seems like this returns 0 when no connection to the database was possible.
    // It should probably throw an error, instead.

    int visitorId = -1;
    QString sql = "SELECT `visitor_id` FROM `tag` WHERE `rfid` = ?";

    QSqlQuery query;
    query.prepare(sql);

    // Value(s) that replace the question mark(s) (?):
    query.addBindValue(QVariant(rfidTag));
    bool ok = query.exec();
    if (! ok) {
        // FIXME: Check the exact error type and close the DB connection only if this is due to the connection being lost.
        // Then retry to connect when the reconnect timer triggers.
        qWarning() << "ERROR: " << query.lastError().text();
        throw SQLError{query.lastError().text()};
        _database.close();
    } else {
        bool foundSomeVisitorWhoseTagMatches = query.next();
        if (foundSomeVisitorWhoseTagMatches) {
            bool tagExistsButHasNoUser = query.isNull(0);
            if (tagExistsButHasNoUser) {
                visitorId = this->createNewUser(rfidTag);
                try {
                    updateTagSetVisitorId(rfidTag, visitorId);
                } catch (std::exception & e) {
                    qWarning() << "Internal Server Error ::" << e.what();
                    throw SQLError(e.what());
                }

            }
            else {
                visitorId = query.value(0).toInt(); // value("visitor_id") would also work, but is less efficient
            }
        } else {
            try {
                visitorId = this->createTagAndUser(rfidTag);
            } catch (std::exception & e) {
                // TODO: Answer with an error response
                qWarning() << "Internal Server Error ::" << e.what();
                throw SQLError(e.what());
                _database.close();
            }
        }
    }
    return visitorId;
}


int Facade::createTagAndUser(const QString& rfidTag) {
    qDebug() << "createTagAndUser";
    // First, create the new user:
    int  visitorId = createNewUser(rfidTag);
    QString createSql = "INSERT INTO tag (`rfid`, `visitor_id`) VALUES (?, ?)";
    QSqlQuery query;
    query.prepare(createSql);

    // Value(s) that replace the question mark(s) (?):
    query.addBindValue(QVariant(rfidTag));
    query.addBindValue(QVariant(visitorId));

    bool ok = query.exec();
    if (! ok) {
        qWarning() << "ERROR: " << query.lastError().text();
        throw(query.lastError().text());
    }
    return visitorId;
}


int Facade::createNewUser(const QString& rfidTag) {
    qDebug() << "createNewUser";
    QString sql = "INSERT INTO `visitor` (`id`,`rfid`) VALUES (NULL, ?)";
    QSqlQuery query;
    query.prepare(sql);

    // Value(s) that replace the question mark(s) (?):
    query.addBindValue(QVariant(rfidTag));
    bool ok = query.exec();
    int visitorId = -1;
    if (! ok) {
        qWarning() << "ERROR: " << query.lastError().text();
        throw SQLError{query.lastError().text()};
    } else {
        visitorId = query.lastInsertId().toInt();
    }
    return visitorId;
}


bool Facade::updateTagSetVisitorId(const QString& rfidTag, int visitorId) {
    qDebug() << "updateTagSetVisitorId";
    QString sql = "UPDATE `tag` SET `visitor_id` = ? WHERE `rfid` = ?";
    QSqlQuery query;
    query.prepare(sql);

    // Value(s) that replace the question mark(s) (?)
    query.addBindValue(QVariant(visitorId));
    query.addBindValue(QVariant(rfidTag));

    bool ok = query.exec();
    if (! ok) {
        qWarning() << "ERROR: " << query.lastError().text();
        throw (query.lastError().text());
    } else {
        ok = query.numRowsAffected() == 1;
    }
    return ok;
}


/**
 * @brief Facade::getUserInfo
 * @param userId
 * @return an associative array where keys are fields name, and the values can be strings or ints.
 */
QMap<QString, QVariant> Facade::getUserInfo(int userId) {
    QMap<QString, QVariant> ret;
    QString sql = "SELECT "
                  "`visitor`.`age` AS `age`, "
                  "`visitor`.`rfid` AS `rfid`, "
                  "`visitor`.`gender` AS `gender`, "
                  "`visitor`.`language` AS `language`, "
                  "`ethnicity`.`identifier` AS `ethnicity` "
                  "FROM `visitor` "
                  "JOIN `ethnicity` ON `visitor`.`ethnicity` = `ethnicity`.`id` "
                  "WHERE visitor.id = ?";
    QSqlQuery query;
    query.prepare(sql);
    // Value(s) that replace the question mark(s) (?):
    query.addBindValue(QVariant(userId));
    bool ok = query.exec();
    if (! ok) {
        qWarning() << "ERROR: " << query.lastError().text();
        throw SQLError{query.lastError().text()};
    }

    bool retrieved = query.next();
    if (retrieved) {
        QSqlRecord rec = query.record();
        qDebug() << "Number of columns: " << rec.count();
        qDebug() << "getUserInfo for " << userId;
        for (int i = 0; i < rec.count(); ++ i) {
            QString keyName = rec.fieldName(i);
            QVariant value = rec.value(i);
            qDebug() << "User" << userId << ":" << keyName << value;
            ret.insert(keyName, value);
        }
    }
    else {
       // Retrieving the visitor info in case the Ethnicity isn't set
        QString sql1 = "SELECT "
                      "`visitor`.`age` AS `age`, "
                      "`visitor`.`rfid` AS `rfid`, "
                      "`visitor`.`gender` AS `gender`, "
                      "`visitor`.`language` AS `language`, "
                      "`visitor`.`ethnicity` AS `ethnicity` "
                      "FROM `visitor` "
                      "WHERE visitor.id = ?";
        QSqlQuery query1;
        query1.prepare(sql1);
        // Value(s) that replace the question mark(s) (?):
        query1.addBindValue(QVariant(userId));
        bool ok1 = query1.exec();
        if (! ok1) {
            qWarning() << "ERROR: " << query1.lastError().text();
            throw SQLError{query.lastError().text()};
        }
        bool retrieved1 = query1.next();
        if (retrieved1) {
            QSqlRecord rec1 = query1.record();
            for (int j = 0; j < rec1.count(); ++ j) {
                QString keyName1 = rec1.fieldName(j);
                QVariant value1 = rec1.value(j);
                ret.insert(keyName1, value1);
            }
        }
    }
    return ret;
}


void Facade::setUserAnswer(int userId, const QString& questionId, int value=50) {
    qDebug() << "setUserAnswer";
    QString sql=" INSERT INTO `answer`"
                " (`visitor_id`, `question_id`, `answer_value`)"
                " SELECT ?, `question`.`id`, ? "
                " FROM `question` "
                " WHERE `question`.`identifier` = ?"
                " ON DUPLICATE KEY"
                " UPDATE"
                " answer.answer_value = ?";
    QSqlQuery query;
    query.prepare(sql);
    // Value(s) that replace the question mark(s) (?):
    // Param values for new ans Insert
    query.addBindValue(QVariant(userId));
    query.addBindValue(QVariant(value));
    query.addBindValue(QVariant(questionId));
    // Param value for Update if exist
    query.addBindValue(QVariant(value));

    bool ok = query.exec();
    if (! ok) {
        qWarning() << "ERROR: " << query.lastError().text();
        throw SQLError{query.lastError().text()};
    }
}


QMap<QString, int> Facade::getUserAnswers(int userId) {
    QMap<QString, int> answers;
    QString sql = "SELECT "
                  "question.identifier, "
                  "answer.answer_value "
                  "FROM `answer` "
                  "JOIN `question` ON answer.question_id = question.id "
                  "WHERE answer.visitor_id = ?";
    QSqlQuery query;
    query.prepare(sql);
    // Value(s) that replace the question mark(s) (?):
    query.addBindValue(QVariant(userId));
    bool ok = query.exec();
    if (! ok) {
        qWarning() << "ERROR: " << query.lastError().text();
        throw SQLError{query.lastError().text()};
    }
    while (query.next()) {
        QString questionId = query.value(0).toString();
        int answerValue = query.value(1).toInt();
        answers.insert(questionId, answerValue);
    }

    return answers;
}


QList<int> Facade::getStatsForQuestion(const QString& questionId) {
    QList<int> ret;
    Q_UNUSED(questionId);
    return ret;
    // TODO not sure what to return
}


void Facade::freeTag(const QString& rfidTag) {
    qDebug() << "freeTag";
    QString sql= "UPDATE `tag` "
                 "SET `tag`.`visitor_id` = NULL "
                 "WHERE `tag`.`rfid` = ?";
    QSqlQuery query;
    query.prepare(sql);

    // Value(s) that replace the question mark(s) (?):
    query.addBindValue(QVariant(rfidTag));

    bool ok = query.exec();
    if (! ok) {
        qWarning() << "ERROR: " << query.lastError().text();
        throw SQLError{query.lastError().text()};
    }
}


void Facade::freeAllTags() {
    qDebug() << "freeUnusedTags";
    QString sql = "UPDATE `tag` SET `visitor_id` = NULL";
    QSqlQuery query;
    query.prepare(sql);
    bool ok = query.exec();
    if (! ok) {
        qWarning() << "ERROR: " << query.lastError().text();
        throw SQLError{query.lastError().text()};
    }
}


bool Facade::setUserLanguage(int userId, const QString& language) {
    qDebug() << "setUserLanguage";
    QString sql = "UPDATE `visitor` SET `language` = ?  WHERE `id` = ?";
    QSqlQuery query;
    query.prepare(sql);
    // Value(s) that replace the question mark(s) (?):
    query.addBindValue(QVariant(language));
    query.addBindValue(QVariant(userId));
    bool ok = query.exec();
    if (! ok) {
        qWarning() << "ERROR: " << query.lastError().text();
        throw SQLError{query.lastError().text()};
    }
    qDebug() << query.numRowsAffected();
    return query.numRowsAffected() == 1;
}


bool Facade::setUserGender(int userId, const QString& gender) {
    qDebug() << "setUserGender";
    QString sql = "UPDATE `visitor` SET `gender` = ?  WHERE `id` = ?";
    QSqlQuery query;
    query.prepare(sql);
    // Value(s) that replace the question mark(s) (?):
    query.addBindValue(QVariant(gender));
    query.addBindValue(QVariant(userId));
    bool ok = query.exec();
    if (! ok) {
        qWarning() << "ERROR: " << query.lastError().text();
        throw SQLError{query.lastError().text()};
    }
    qDebug() << query.numRowsAffected();
    return query.numRowsAffected() == 1;
}

bool Facade::setUserAge(int userId, int age) {
    qDebug() << "setUserAge";
    QString sql = "UPDATE `visitor` SET `age` = ? WHERE `id` = ?";
    QSqlQuery query;
    query.prepare(sql);
    // Value(s) that replace the question mark(s) (?):
    query.addBindValue(QVariant(age));
    query.addBindValue(QVariant(userId));
    bool ok = query.exec();
    if (! ok) {
        qWarning() << "ERROR: " << query.lastError().text();
        throw SQLError{query.lastError().text()};
    }
    return query.numRowsAffected() == 1;
}


bool Facade::setUserEthnicity(int userId, const QString& ethnicity) {
    QString sql = "UPDATE `visitor` SET `ethnicity` = "
                  "(SELECT `ethnicity`.`id` FROM `ethnicity` WHERE `ethnicity`.`identifier` = ?) "
                  "WHERE `visitor`.`id` = ?";
    QSqlQuery query;
    query.prepare(sql);
    // Value(s) that replace the question mark(s) (?):
    query.addBindValue(QVariant(ethnicity));
    query.addBindValue(QVariant(userId));
    bool ok = query.exec();
    if (! ok) {
        qWarning() << "ERROR: " << query.lastError().text();
        throw SQLError{query.lastError().text()};
    }
    return query.numRowsAffected() == 1;
}


bool Facade::deleteAllFromDatabase() {
    qDebug() << "deleteAllFromDatabase";
    bool ret = false;
    static const bool deleteVisitors = true;
    static const bool deleteAnswers = true;
    static const bool deleteTags = true;

    if (deleteVisitors) {
        QString sql = "DELETE FROM `visitor`";
        QSqlQuery query;
        query.prepare(sql);

        bool ok = query.exec();
        if (! ok) {
            qWarning() << "ERROR: " << query.lastError().text();
        }
        ret = ret || query.numRowsAffected() > 0;
    }

    if (deleteAnswers) {
        QString sql = "DELETE FROM `answer`";
        QSqlQuery query;
        query.prepare(sql);
        bool ok = query.exec();
        if (! ok) {
            qWarning() << "ERROR: " << query.lastError().text();
            throw SQLError{query.lastError().text()};
        }
        ret = ret || query.numRowsAffected() > 0;
    }

    if (deleteTags) {
        QString sql = "DELETE FROM `tag`";
        QSqlQuery query;
        query.prepare(sql);
        bool ok = query.exec();
        if (! ok) {
            qWarning() << "ERROR: " << query.lastError().text();
            throw SQLError{query.lastError().text()};
        }
        ret = ret || query.numRowsAffected() > 0;
    }

    return ret;
}


bool Facade::deleteTagsVisitorsAndTheirAnswers(const QList<QString>& rfidTags) {
    // TODO: make sure it works with a unit test.
    qDebug() << "deleteTagsUsersAndTheirAnswers";
    bool ret = false;
    static const bool deleteVisitors = true;
    static const bool deleteAnswers = true;
    static const bool deleteTags = true;

    for (auto iter = rfidTags.begin(); iter != rfidTags.end(); ++ iter) {
        auto rfidTag = (*iter);
        if (deleteVisitors && deleteAnswers && deleteTags) {
            QString sql = "DELETE `visitor`, `tag`, `answer`"
                          " FROM `visitor`"
                          " LEFT join `tag` ON `visitor`.`id`=`tag`.`visitor_id`"
                          " LEFT join `answer` ON `visitor`.`id`=`answer`.`visitor_id`"
                          " WHERE `tag`.`rfid` = ?";
            QSqlQuery query;
            query.prepare(sql);
            query.addBindValue(QVariant(rfidTag));
            bool ok = query.exec();
            if (! ok) {
                qWarning() << "ERROR: " << query.lastError().text();
                throw SQLError{query.lastError().text()};
            }
            ret = ret || query.numRowsAffected() > 0;
        }
    }
    return ret;
}


QList<int> Facade::getAnswerByAge(const QString& questionId, const QString& ethenicity="all", const QString& gender="all", const QString& timeAnswered="all") {
    qDebug() << "GetAnswerByAge";
    bool retrieveResult = false ; // to check weather result set is null.
    int numOfAns = 20 ; // total num of ans by age group
    int DEFAULT_ANSWER = -1;
    QList<int> avgAnsByAge;
    // We start to set up the SQL query as a string, and will append to it:
    // We need to join the answer table with the visitor table
    // so that we know the age of each visitor.
    // We then if a CASE statement to retrieve the
    // average answer for each age group for a given question.
    // There is also the possibility to keep answer that match only a given ethnicity, gender
    // or time answered. (if it's for "this_year" or "today")
    QString sqlQuery= "SELECT"
        " IFNULL( ROUND( AVG( CASE WHEN (v.age >= 0 AND v.age <= 5) THEN a.answer_value ELSE NULL END)), '-1') AS '[0-5]',"
        " IFNULL( ROUND( AVG( CASE WHEN (v.age > 5 AND v.age <= 10) THEN a.answer_value ELSE NULL END)), '-1') AS '[6-10]',"
        " IFNULL( ROUND( AVG( CASE WHEN (v.age > 10 AND v.age <= 15) THEN a.answer_value ELSE NULL END)), '-1') AS '[11-15]',"
        " IFNULL( ROUND( AVG( CASE WHEN (v.age > 15 AND v.age <= 20) THEN a.answer_value ELSE NULL END)), '-1') AS '[16-20]',"
        " IFNULL( ROUND( AVG( CASE WHEN (v.age > 20 AND v.age <= 25) THEN a.answer_value ELSE NULL END)), '-1') AS '[21-25]',"
        " IFNULL( ROUND( AVG( CASE WHEN (v.age > 25 AND v.age <= 30) THEN a.answer_value ELSE NULL END)), '-1') AS '[26-30]',"
        " IFNULL( ROUND( AVG( CASE WHEN (v.age > 30 AND v.age <= 35) THEN a.answer_value ELSE NULL END)), '-1') AS '[31-35]',"
        " IFNULL( ROUND( AVG( CASE WHEN (v.age > 35 AND v.age <= 40) THEN a.answer_value ELSE NULL END)), '-1') AS '[36-40]',"
        " IFNULL( ROUND( AVG( CASE WHEN (v.age > 40 AND v.age <= 45) THEN a.answer_value ELSE NULL END)), '-1') AS '[41-45]',"
        " IFNULL( ROUND( AVG( CASE WHEN (v.age > 45 AND v.age <= 50) THEN a.answer_value ELSE NULL END)), '-1') AS '[46-50]',"
        " IFNULL( ROUND( AVG( CASE WHEN (v.age > 50 AND v.age <= 55) THEN a.answer_value ELSE NULL END)), '-1') AS '[51-55]',"
        " IFNULL( ROUND( AVG( CASE WHEN (v.age > 55 AND v.age <= 60) THEN a.answer_value ELSE NULL END)), '-1') AS '[56-60]',"
        " IFNULL( ROUND( AVG( CASE WHEN (v.age > 60 AND v.age <= 65) THEN a.answer_value ELSE NULL END)), '-1') AS '[61-65]',"
        " IFNULL( ROUND( AVG( CASE WHEN (v.age > 65 AND v.age <= 70) THEN a.answer_value ELSE NULL END)), '-1') AS '[66-70]',"
        " IFNULL( ROUND( AVG( CASE WHEN (v.age > 70 AND v.age <= 75) THEN a.answer_value ELSE NULL END)), '-1') AS '[71-75]',"
        " IFNULL( ROUND( AVG( CASE WHEN (v.age > 75 AND v.age <= 80) THEN a.answer_value ELSE NULL END)), '-1') AS '[76-80]',"
        " IFNULL( ROUND( AVG( CASE WHEN (v.age > 80 AND v.age <= 85) THEN a.answer_value ELSE NULL END)), '-1') AS '[81-85]',"
        " IFNULL( ROUND( AVG( CASE WHEN (v.age > 85 AND v.age <= 90) THEN a.answer_value ELSE NULL END)), '-1') AS '[86-90]',"
        " IFNULL( ROUND( AVG( CASE WHEN (v.age > 90 AND v.age <= 95) THEN a.answer_value ELSE NULL END)), '-1') AS '[91-95]',"
        " IFNULL( ROUND( AVG( CASE WHEN (v.age > 95 AND v.age <= 100) THEN a.answer_value ELSE NULL END)), '-1') AS '[96-100]'"
        " FROM answer AS a "
        " JOIN visitor AS v ON a.visitor_id = v.id JOIN question AS q ON a.question_id = q.id";

    bool doBindEthnicityValues = false;
    bool doBindGenderValues = false;
    // bind values replace the :nameBindValues in the query

    if (! stringMatches(ethenicity, "all")) {
        sqlQuery += " JOIN ethnicity AS e ON v.ethnicity = e.id"
            " WHERE q.`identifier` = :questionId"
            " AND e.`identifier` = :ethnicity ";
        doBindEthnicityValues = true;
    } else {
        sqlQuery += " WHERE q.`identifier` = :questionId ";
    }

    if (! stringMatches(gender, "all")) {
        if (stringMatches(gender, "male") || stringMatches(gender, "female") || stringMatches(gender, "other")) {
            sqlQuery += " AND v.`gender` = :gender";
            doBindGenderValues = true;
        }
    }

    if (! stringMatches(timeAnswered, "all")) {
        if (stringMatches(timeAnswered, "this_year")) {
            sqlQuery = " AND YEAR(a.`created_at`) = YEAR(CURDATE())";
        }
        else if (stringMatches(timeAnswered, "today")) {
            sqlQuery = " AND DATE(a.`created_at`) = date(CURRENT_TIMESTAMP())";
        }
    }

    QSqlQuery query;
    // We need to first prepare the query, then add the bind values:
    query.prepare(sqlQuery);
    // Add the bind values:
    query.bindValue(":questionId", questionId);
    if (doBindEthnicityValues) {
        query.bindValue(":ethnicity", ethenicity);
    }
    if (doBindGenderValues) {
        query.bindValue(":gender", gender);
    }

    bool ok = query.exec();
    qDebug() << "getAnswerByAge ::: Query ::" << query.executedQuery();

    if (! ok) {
        qWarning()<<"ERROR :: " << query.lastError().text();
        throw SQLError{query.lastError().text()};
    }

    while (query.next()) {
        //providers Question Identifier's List of avg answer_value by age range
        retrieveResult = true;
        avgAnsByAge.insert(1, static_cast<int>(query.value(0).toDouble())); // [0-5]
        avgAnsByAge.insert(2, static_cast<int>(query.value(1).toDouble())); // [6-10]
        avgAnsByAge.insert(3, static_cast<int>(query.value(2).toDouble())); // [11-15]
        avgAnsByAge.insert(4, static_cast<int>(query.value(3).toDouble())); // [16-20]
        avgAnsByAge.insert(5, static_cast<int>(query.value(4).toDouble())); // [21-25]
        avgAnsByAge.insert(6, static_cast<int>(query.value(5).toDouble())); // [26-30]
        avgAnsByAge.insert(7, static_cast<int>(query.value(6).toDouble())); // [31-35]
        avgAnsByAge.insert(8, static_cast<int>(query.value(7).toDouble())); // [36-40]
        avgAnsByAge.insert(9, static_cast<int>(query.value(8).toDouble())); // [41-45]
        avgAnsByAge.insert(10, static_cast<int>(query.value(9).toDouble())); // [46-50]
        avgAnsByAge.insert(11, static_cast<int>(query.value(10).toDouble())); // [51-55]
        avgAnsByAge.insert(12, static_cast<int>(query.value(11).toDouble())); // [56-60]
        avgAnsByAge.insert(13, static_cast<int>(query.value(12).toDouble())); // [61-65]
        avgAnsByAge.insert(14, static_cast<int>(query.value(13).toDouble())); // [66-70]
        avgAnsByAge.insert(15, static_cast<int>(query.value(14).toDouble())); // [71-75]
        avgAnsByAge.insert(16, static_cast<int>(query.value(15).toDouble())); // [76-80]
        avgAnsByAge.insert(17, static_cast<int>(query.value(16).toDouble())); // [81-85]
        avgAnsByAge.insert(18, static_cast<int>(query.value(17).toDouble())); // [86-90]
        avgAnsByAge.insert(19, static_cast<int>(query.value(18).toDouble())); // [91-95]
        avgAnsByAge.insert(20, static_cast<int>(query.value(19).toDouble())); // [96-100]
    }
    if (retrieveResult == false) {
        // stores default value when null resultset.
        for (int i = 1; i <= numOfAns; i++) {
            avgAnsByAge.insert(i, DEFAULT_ANSWER);
        }
    }
    return avgAnsByAge;
}

/**
 * @brief Facade::getAnswerByGender
 * @param questionId Id of the question
 * @param ethenicity
 * @param ageTo
 * @param ageFrom
 * @param gender
 * @param timeAnswered
 * @return an associative array where key is user gender, and the values are ints.
 */
QMap<QString,int> Facade::getAnswerByGender(
        const QString& questionId,
        const QString& ethenicity="all",
        int ageTo=-1,
        int ageFrom =-1,
        const QString& timeAnswered="all")
{
    qDebug() << "getAnswerByGender";
    int ansMale = -1 , ansFemale = -1, ansOther = -1;
    QMap<QString,int> AvgAnsByGender;
    QString sqlQuery= "SELECT IFNULL(ROUND(AVG( CASE WHEN v.gender='male' "
                      "THEN a.answer_value ELSE -1 END ),2),'-1') AS 'Male', "
                      "IFNULL(ROUND(AVG( CASE WHEN v.gender='female' THEN "
                      " a.answer_value ELSE -1 END ),2),'-1') AS 'Female',"
                      "IFNULL(ROUND(AVG(CASE WHEN v.gender='other' THEN a.answer_value"
                      " ELSE -1 END ),2) ,'-1') AS 'Other' "
                      "FROM answer AS a JOIN visitor AS v ON a.visitor_id = v.id JOIN"
                      " question AS q ON a.question_id = q.id " ;
    if (! stringMatches(ethenicity, "all")) {
        sqlQuery += " JOIN ethnicity AS e ON v.ethnicity= e.id WHERE q.identifier=? AND e.`identifier`= ?";
    } else {
        sqlQuery += " WHERE q.identifier=? ";
    }

    if (ageTo != -1 && ageFrom != -1) {
        sqlQuery += " AND v.age BETWEEN ? AND ? ";
    }
    else if (ageTo != -1) {
        sqlQuery += " AND v.age BETWEEN 0 AND ? ";
    }
    else if (ageFrom != -1) {
        sqlQuery += " AND v.age BETWEEN ? AND 100 ";
    }

    if (! stringMatches(timeAnswered, "all")) {
        if (stringMatches(timeAnswered, "this_year")) {
            sqlQuery = " AND YEAR(a.`created_at`) = YEAR(CURDATE())";
        } else if (stringMatches(timeAnswered, "today")) {
            sqlQuery = " AND DATE(a.`created_at`) = date(CURRENT_TIMESTAMP())";
        }
    }

    QSqlQuery query;
    query.prepare(sqlQuery);

    // ? replaces the perameter in query
    query.addBindValue(QVariant(questionId));

    // FIXME: avoid to redo all the checks to add the bind params
    // FIXME: use named bind params instead of positional ones.
    if(! stringMatches(ethenicity, "all")) {
        query.addBindValue(QVariant(ethenicity));
    }

    if (ageTo != -1 and ageFrom != -1 ) {
        query.addBindValue(QVariant(ageFrom));
        query.addBindValue(QVariant(ageTo));
    } else if (ageTo != -1) {
        query.addBindValue(QVariant(ageTo));
    } else if (ageFrom !=-1 ) {
        query.addBindValue(QVariant(ageFrom));
    }

    qDebug() << "getAnswerByGender ::: Query ::" <<sqlQuery;
    bool ok = query.exec();

    if (! ok) {
        qWarning()<<"ERROR :: "<< query.lastError().text();
        throw SQLError{query.lastError().text()};
    }
    while (query.next()) {
        //providers Question Identifier's  avg answer_value by Gender
        ansMale = static_cast<int>(query.value(0).toDouble());
        ansFemale = static_cast<int>(query.value(1).toDouble());
        ansOther = static_cast<int>(query.value(2).toDouble());
        AvgAnsByGender.insert("male", ansMale); //male
        AvgAnsByGender.insert("female", ansFemale); //female
        AvgAnsByGender.insert("other", ansOther);//other
    }

    if (ansMale == -1) {
        AvgAnsByGender.insert("male", ansMale); //male
        AvgAnsByGender.insert("female", ansFemale); //female
        AvgAnsByGender.insert("other", ansOther);//other
    }
    return  AvgAnsByGender;
}


/**
 * @brief Facade::getAnswerByEthnicity
 * @param questionId
 * @param ageTo
 * @param ageFrom
 * @param gender
 * @param timeAnswered
 * @return an associative array where key is user Ethnicity, and the values are ints.
 */

QMap<QString, int> Facade::getAnswerByEthnicity(
        const QString& questionId,
        int ageFrom=-1, int ageTo =-1,
        const QString& gender="all",
        const QString& timeAnswered="all")
{
    qDebug() << "GetAnswerByEthnicity";
    qDebug() << questionId;
    int ansQc=-1, ansCad=-1, ansAme=-1, ansEur=-1, ansNativ=-1, ansOth=-1;
    QMap<QString, int> AvgAnsByEthnicity;

    QString sqlQuery="SELECT IFNULL(ROUND(AVG( CASE WHEN e.identifier='quebecer' THEN a.answer_value ELSE -1 END ),2),'-1') as 'Quebecer',"
                        "IFNULL( ROUND(AVG( CASE WHEN e.identifier='canadian' THEN a.answer_value ELSE -1 END ),2),'-1') as 'Canadian',"
                        "IFNULL( ROUND(AVG( CASE WHEN e.identifier='american' THEN a.answer_value ELSE -1 END ),2),'-1') as 'American', "
                        "IFNULL( ROUND(AVG( CASE WHEN e.identifier='european' THEN a.answer_value ELSE -1 END ),2),'-1') as 'European' ,"
                        "IFNULL( ROUND(AVG( CASE WHEN e.identifier='native' THEN a.answer_value ELSE -1 END ),2),'-1') as 'Native' ,"
                        "IFNULL( ROUND(AVG( CASE WHEN e.identifier='other' THEN a.answer_value ELSE -1 END ),2),'-1') as 'other' "
                        "FROM answer AS a JOIN visitor AS v ON a.visitor_id = v.id JOIN question AS q ON a.question_id = q.id"
                        " JOIN ethnicity AS e on v.ethnicity = e.id WHERE q.identifier = ? ";

    if (ageTo != -1 && ageFrom != -1 ) {
        sqlQuery += " and v.age BETWEEN ? and ? ";
    } else if (ageTo != -1) {
        sqlQuery += " and v.age BETWEEN 0 and ? ";
    } else if (ageFrom != -1 ) {
        sqlQuery += " and v.age BETWEEN ? and  100 ";
    }
    if (! stringMatches(gender, "all")) {
        sqlQuery += " and v.gender = ? ";
    }
    if (! stringMatches(timeAnswered, "all")) {
        if (stringMatches(timeAnswered, "this_year")) {
            sqlQuery = " AND YEAR(a.`created_at`) = YEAR(CURDATE())";
        } else if (stringMatches(timeAnswered, "today")) {
            sqlQuery = " AND DATE(a.`created_at`) = date(CURRENT_TIMESTAMP())";
        }
    }

    QSqlQuery query;
    query.prepare(sqlQuery);

    // ? replaces the perameter in query
    query.addBindValue(QVariant(questionId));

    if (ageTo != -1 && ageFrom != -1) {
        query.addBindValue(QVariant(ageFrom));
        query.addBindValue(QVariant(ageTo));
    }
    else if (ageTo != -1) {
        query.addBindValue(QVariant(ageTo));
    }
    else if (ageFrom != -1) {
        query.addBindValue(QVariant(ageFrom));
    }

    if (! stringMatches(gender, "all")) {
        query.addBindValue(QVariant(gender));
    }

    //qDebug() << "getAnswerByEthnicity ::: Query ::" <<sqlQuery;

    bool ok = query.exec();

    if (! ok) {
        qWarning()<<"ERROR :: "<< query.lastError().text();
        throw SQLError{query.lastError().text()};
    }

    while (query.next()) {
        //providers Question Identifier's  avg answer_value by Ethnicity
        ansQc = static_cast<int>(query.value(0).toDouble());
        ansCad = static_cast<int>(query.value(1).toDouble());
        ansAme = static_cast<int>(query.value(2).toDouble());
        ansEur = static_cast<int>(query.value(3).toDouble());
        ansNativ = static_cast<int>(query.value(4).toDouble());
        ansOth = static_cast<int>(query.value(5).toDouble());
        AvgAnsByEthnicity.insert("quebecer", ansQc);
        AvgAnsByEthnicity.insert("canadian", ansCad);
        AvgAnsByEthnicity.insert("american", ansAme);
        AvgAnsByEthnicity.insert("european", ansEur);
        AvgAnsByEthnicity.insert("native", ansNativ);
        AvgAnsByEthnicity.insert("other", ansOth);
    }

    if (ansQc == -1){
        AvgAnsByEthnicity.insert("quebecer", ansQc);
        AvgAnsByEthnicity.insert("canadian", ansCad);
        AvgAnsByEthnicity.insert("american", ansAme);
        AvgAnsByEthnicity.insert("european", ansEur);
        AvgAnsByEthnicity.insert("native", ansNativ);
        AvgAnsByEthnicity.insert("other", ansOth);
    }
    return AvgAnsByEthnicity;
}


QMap<QString,int> Facade::getAnswerByLanguage(const QString& questionId, int ageFrom=-1, int ageTo=-1, const QString& ethnicity="all", const QString& gender="all", const QString& timeAnswered="all")
{
    qDebug() << "GetAnswerByLanguage";
    qDebug() << questionId;
    int ansEn = -1;
    int ansFr = -1;
    QMap<QString, int> ansByLang;
    QString sqlQuery = "SELECT IFNULL(ROUND(AVG( CASE WHEN v.language='en' THEN a.answer_value ELSE -1 END ),2),'-1') as 'English',"
                        "IFNULL( ROUND(AVG( CASE WHEN v.language='fr' THEN a.answer_value ELSE -1 END ),2),'-1') as 'French'"
                        "from answer AS a JOIN visitor AS v ON a.visitor_id = v.id JOIN question AS q ON a.question_id = q.id";
    if (! stringMatches(ethnicity, "all")) {
        sqlQuery += " JOIN ethnicity AS e ON v.ethnicity= e.id WHERE q.identifier=? AND e.`identifier`= ?";
    } else {
         sqlQuery += " WHERE q.identifier=? ";
    }
    if (ageTo != -1 && ageFrom != -1 ){
        sqlQuery += " AND v.age BETWEEN ? AND ? ";
    }
    else if (ageTo != -1) {
        sqlQuery += " AND v.age BETWEEN 0 AND ? ";
    }
    else if (ageFrom != -1 ) {
        sqlQuery += " AND v.age BETWEEN ? AND 100 ";
    }

    if (! stringMatches(gender, "all")) {
        sqlQuery += " AND v.gender = ? ";
    }

    if (! stringMatches(timeAnswered, "all")) {
        if (stringMatches(timeAnswered, "this_year")) {
            sqlQuery = " AND YEAR(a.`created_at`) = YEAR(CURDATE())";
        }
        else if (stringMatches(timeAnswered, "today")) {
            sqlQuery = " AND DATE(a.`created_at`) = DATE(CURRENT_TIMESTAMP())";
        }
    }

    QSqlQuery query;
    query.prepare(sqlQuery);
    // ? replaces the perameter in query
    query.addBindValue(QVariant(questionId));
    // FIXME: avoid to redo all checks in order to add the bind params
    // FIXME: use named bind params instead of positional ones.
    if(! stringMatches(ethnicity, "all")) {
        query.addBindValue(QVariant(ethnicity));
    }
    if (ageTo != -1 && ageFrom != -1) {
        query.addBindValue(QVariant(ageFrom));
        query.addBindValue(QVariant(ageTo));
    } else if (ageTo != -1){
        query.addBindValue(QVariant(ageTo));
    } else if (ageFrom != -1 ) {
        query.addBindValue(QVariant(ageFrom));
    }
    if (! stringMatches(gender, "all")) {
        query.addBindValue(QVariant(gender));
    }
    bool ok = query.exec();
    if (!ok) {
        qWarning()<<"ERROR :: "<< query.lastError().text();
        throw SQLError{query.lastError().text()};
    }

    while (query.next()) {
        //providers Question Identifier's  avg answer_value by Language
        ansEn = static_cast<int>(query.value(0).toDouble());
        ansFr = static_cast<int>(query.value(1).toDouble());
        ansByLang.insert("en", ansEn);
        ansByLang.insert("fr", ansFr);
    }
    if (ansEn == -1) {
        ansByLang.insert("en", ansEn);
        ansByLang.insert("fr", ansFr);
    }
    return  ansByLang;
}


QMap<QString, int > Facade:: getAllAnswers() {
    QMap<QString,int> avgQueAns;
    int DEFAULT_VALUE = -1 ;
    bool isResultRetrive = false;
    QString sqlQuery="SELECT q.identifier AS 'Question', IFNULL(AVG(a.answer_value), -1) AS 'Average' "
                     "FROM answer AS a JOIN question AS q ON a.question_id = q.id GROUP BY q.id";
    QSqlQuery query;
    bool ok = query.exec(sqlQuery);
    if (! ok) {
        qWarning() << "ERROR: " << query.lastError().text();
        throw SQLError{query.lastError().text()};
    }
    while (query.next()) {
        isResultRetrive = true ;
        QString questionId = query.value(0).toString();
        int answerValue = static_cast<int>(query.value(1).toDouble());
        avgQueAns.insert(questionId, answerValue);
    }

    // retrive question identifire and set default answer.
    if (isResultRetrive == false) {
        QString sqlQuery1 = "SELECT identifier FROM question " ;
        QSqlQuery query1;
        bool ok = query1.exec(sqlQuery1);
        if (! ok) {
            qWarning() << "ERROR: " << query.lastError().text();
            throw SQLError{query.lastError().text()};
        }
        while (query.next()) {
            QString questionId = query.value(0).toString();
            int answerValue = DEFAULT_VALUE; // assign default value.
            avgQueAns.insert(questionId, answerValue);
        }
    }
    return avgQueAns;
}


/**
 * @brief Facade::getAnswers
 * @param questionIds The List of question identifier
 * @param ageFrom The ageFrom identifier
 * @param ageTo The ageTo identifier
 * @param gender identifier
 * @param timeAnswered identifier.
 * @return an associative array where key is questionIds, and their average values are ints.
 */
QMap<QString, int> Facade:: getAnswers(const QList<QString>& questionIds, int ageFrom=-1, int ageTo=-1, const QString& ethnicity="all", const QString& gender="all", const QString& timeAnswered="all") {

    qDebug() << "getAnswers";
    QMap<QString,int> avgAnsQueList;
    const int DEFAULT_AGE = -1;
    const QString DEFAULT_FILTER = "all";

    for (auto iter = questionIds.begin(); iter != questionIds.end(); ++ iter) {

        int answerValue = -1;
        //retrive each Question Id from the List to calculate the avg ans.
        auto questionId = (*iter);

        QString sqlQuery=" SELECT q.identifier AS 'Question', IFNULL(AVG(a.answer_value), -1) AS 'Average' "
                         " FROM answer AS a JOIN visitor AS v ON a.visitor_id = v.id  JOIN question AS q ON a.question_id = q.id ";
        //filter by ethnicity
        if (! stringMatches(ethnicity, "all")) {
            sqlQuery += " JOIN ethnicity AS e ON v.ethnicity= e.id  WHERE q.identifier= ? AND e.`identifier`= ? ";
        } else {
             sqlQuery += " WHERE q.identifier= ? ";
        }

        // filter age group
        if (ageTo != DEFAULT_AGE && ageFrom != DEFAULT_AGE) {
            sqlQuery += " AND v.age BETWEEN ? AND ? ";
        } else if (ageTo != DEFAULT_AGE) {
            sqlQuery += " AND v.age BETWEEN 0 AND ? ";
        } else if (ageFrom != DEFAULT_AGE) {
            sqlQuery += " AND v.age BETWEEN ? AND  100 ";
        }
        //filter by gender
        if (gender != DEFAULT_FILTER) {
            sqlQuery += " AND v.gender = ? ";
        }

        //filter by time answered
        if (! stringMatches(timeAnswered, "all")) {
            if (stringMatches(timeAnswered, "this_year")) {
                sqlQuery = " AND YEAR(a.`created_at`) = YEAR(CURDATE())";
            } else if (stringMatches(timeAnswered, "today")) {
                sqlQuery = " AND DATE(a.`created_at`) = date(CURRENT_TIMESTAMP())";
            }
        }
        sqlQuery += " GROUP BY q.id";
        QSqlQuery query;
        query.prepare(sqlQuery);

        // ? replaces the perameter in query
        query.addBindValue(QVariant(questionId));

        if (! stringMatches(ethnicity, "all")) {
            query.addBindValue(QVariant(ethnicity));
        }

        if (ageTo != DEFAULT_AGE && ageFrom != DEFAULT_AGE) {
            query.addBindValue(QVariant(ageFrom));
            query.addBindValue(QVariant(ageTo));
        } else if (ageTo != DEFAULT_AGE) {
            query.addBindValue(QVariant(ageTo));
        } else if (ageFrom != DEFAULT_AGE) {
            query.addBindValue(QVariant(ageFrom));
        }
        if (gender != DEFAULT_FILTER) {
            query.addBindValue(QVariant(gender));
        }

        bool ok = query.exec();
        if (! ok){
            qWarning() <<"ERROR :: "<< query.lastError().text();
            throw SQLError{query.lastError().text()};
        }
        while (query.next()) {
            //providers Question Identifier's  avg answer_value with optional filters.
            QString questionId = query.value(0).toString();
            answerValue = static_cast<int>(query.value(1).toDouble());
            avgAnsQueList.insert(questionId, answerValue);
        }
        if (answerValue == -1) {
            avgAnsQueList.insert(questionId, -1);
        }
    }
    return avgAnsQueList;
}


/**
 * @brief getRandomValueByTotalAns
 * @return total number of answer
 */
float  Facade::getRandomValueByTotalAns() {
    QString strkey = "total_num_answers";
    float totalAns;
    QString sqlQuery = "SELECT COUNT(*) AS 'Total Answer' "
                     "FROM answer";
    QSqlQuery query;
    bool ok = query.exec(sqlQuery);
    if (! ok) {
        qWarning() << "ERROR: " << query.lastError().text();
        throw SQLError{query.lastError().text()};
    }
    while (query.next()) {
        totalAns = query.value(0).toFloat();
    }

    return totalAns;
}


/**
 * @brief getRandomValueByAnsLastHour
 * @return total answers during the last hour
 */
float  Facade:: getRandomValueByAnsLastHour(){
    float totalAnsLstHour;
    QString sqlQuery = "SELECT COUNT(*) AS 'AnsLastHour' "
                     "FROM answer WHERE `created_at` >= DATE_SUB(CURDATE(), INTERVAL 1 HOUR) ";
    QSqlQuery query;
    bool ok = query.exec(sqlQuery);
    if (! ok) {
        qWarning() << "ERROR: " << query.lastError().text();
        throw SQLError{query.lastError().text()};
    }
    while (query.next()) {
         totalAnsLstHour = query.value(0).toFloat();
    }
    return totalAnsLstHour;
}


/**
 * @brief getRandomValueByAvgofAllAns.
 * @return average of all answers.
 */
float  Facade:: getRandomValueByAvgofAllAns(){
    float avgOfAns;
    QString sqlQuery = "SELECT AVG(answer_value) AS 'AvgOfAns' "
                     "FROM answer ";
    QSqlQuery query;
    bool ok = query.exec(sqlQuery);
    if (! ok) {
        qWarning() << "ERROR: " << query.lastError().text();
        throw SQLError{query.lastError().text()};
    }
    while (query.next()) {
         avgOfAns = query.value(0).toFloat();
    }
    return avgOfAns;

}


/**
 * @brief getRandomValueByTotalVisitors
 * @return  total number of visitors
 */
float Facade:: getRandomValueByTotalVisitors(){
    float  totalVisitor;
    QString sqlQuery = "SELECT COUNT(*) AS 'TotalVisitor' "
                     "FROM visitor ";
    QSqlQuery query;
    bool ok = query.exec(sqlQuery);
    if (! ok) {
        qWarning() << "ERROR: " << query.lastError().text();
        throw SQLError{query.lastError().text()};
    }
    while (query.next()) {
         totalVisitor = query.value(0).toFloat();
    }
    return totalVisitor;
}


/**
 * @brief getRandomValueByTodaysVisitors
 * @return todays total visitors
 */
float  Facade:: getRandomValueByTodaysVisitors(){
    float  todaysVisitor;
    QString sqlQuery = "SELECT COUNT(*) AS 'TotalVisitor' "
                     "FROM visitor WHERE `created_at` >= CURDATE()";
    QSqlQuery query;
    bool ok = query.exec(sqlQuery);
    if (! ok) {
        qWarning() << "ERROR: " << query.lastError().text();
        throw SQLError{query.lastError().text()};
    }
    while (query.next()) {
          todaysVisitor = query.value(0).toFloat();
    }
    return todaysVisitor;
}
