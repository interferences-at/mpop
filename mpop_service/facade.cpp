#include "facade.h"
#include <QSqlQuery>
#include <QVariant>
#include <QSqlError>
#include <QDebug>
#include <QStandardPaths>
#include <QDir>
#include <QSqlRecord>
#include <QSqlDriver>

Facade::Facade(
        const Config& config, QObject* parent) : QObject(parent), _config(config)
{
    _database = QSqlDatabase::addDatabase("QMYSQL");
    if (_database.isValid()) {
        qDebug() << "Database is valid";
        qDebug() << "QSqlDriver hasFeature NamedPlaceholders:" << _database.driver()->hasFeature(QSqlDriver::NamedPlaceholders);
        _database.setHostName(_config.mysql_host);
        _database.setDatabaseName(_config.mysql_database);
        _database.setUserName(_config.mysql_user);
        _database.setPassword(_config.mysql_password);
        _database.setPort(_config.mysql_port);

        _is_db_open = _database.open();

        if (_is_db_open) {
            qInfo() << "Success connecting to the database.";
        } else {
            qWarning() << "ERROR: Could not open database";
        }
    } else {
        qDebug() << "Database is not valid. The QMYSQL driver for Qt is probably not installed.";
    }
    // TODO: Periodically make sure that our connection with the database is active.
    // Attempt to re-open it if not.
}


bool Facade::isDatabaseReady() {
    return _database.open();
}


int Facade::getOrCreateUser(const QString& rfidTag) {
    qDebug() << "getOrCreateUser";
    // TODO: make sure to call createNewUser() if no user is associated to this RFID tag.
    // (it seems like getUserForTag takes care of that - but it should probably be moved here,
    // so that it's simpler)
    // FIXME: it seems like this returns 0 when no connection to the database was possible.
    // It should probably throw an error, instead.

    // TODO: remove the following duplicate:
    return getUserForTag(rfidTag);
}


int Facade::getUserForTag(const QString& rfidTag) {
    qDebug() << "getUserForTag";
    int visitorId = -1;
    bool tagExists = false;

    QString sql = "SELECT `visitor_id` FROM `tag` WHERE `rfid` = ?";

    QSqlQuery query;
    query.prepare(sql);

    // Value(s) that replace the question mark(s) (?):
    query.addBindValue(QVariant(rfidTag));
    bool ok = query.exec();
    if (! ok) {
        qWarning() << "ERROR: " << query.lastError().text();
    }
    bool foundSomeVisitorWhoseTagMatches = query.next();
    if (foundSomeVisitorWhoseTagMatches) {
        tagExists = true;
        bool tagExistsButHasNoUser = query.isNull(0);
        if (tagExistsButHasNoUser) {
            visitorId = this->createNewUser();
            bool ok = updateTagSetVisitorId(rfidTag, visitorId);
            if (ok) {
                ok = updateVisitorSetRfid(visitorId, rfidTag);
            }
        } else {
            visitorId = query.value(0).toInt(); // value("visitor_id") would also work, but is less efficient
        }
    } else {
        visitorId = this->createTagAndUser(rfidTag);
    }
    return visitorId;
}


int Facade::createTagAndUser(const QString& rfidTag) {
    qDebug() << "createTagAndUser";

    // First, create the new user:
    int  visitorId = createNewUser();

    QString createSql = "INSERT INTO tag (`rfid`, `visitor_id`) VALUES (?, ?)";

    QSqlQuery query;
    query.prepare(createSql);

    // Value(s) that replace the question mark(s) (?):
    query.addBindValue(QVariant(rfidTag));
    query.addBindValue(QVariant(visitorId));

    bool ok = query.exec();
    if (! ok) {
        qWarning() << "ERROR: " << query.lastError().text();
    } else {
        // TODO: raise an exception, and make sure that it's catched by whoever calls it.
    }

    return visitorId;
}


int Facade::createNewUser() {
    // TODO: Add rfidTag argument and INSERT it at the same time
    // instead of calling UPDATE to the same table, later.
    qDebug() << "createNewUser";

    QString sql = "INSERT INTO `visitor` (`id`) VALUES (NULL)";

    QSqlQuery query;
    bool ok = query.exec(sql);
    if (! ok) {
        qWarning() << "ERROR: " << query.lastError().text();
    }
    int visitorId = query.lastInsertId().toInt();

    return visitorId;
}


bool Facade::updateTagSetVisitorId(const QString& rfidTag, int visitorId) {
    qDebug() << "updateTagSetVisitorId";

    QString sql = "UPDATE `tag` SET `visitor_id` = ? WHERE `rfid` = ?";

    QSqlQuery query;
    query.prepare(sql);

    // Value(s) that replace the question mark(s) (?):
    query.addBindValue(QVariant(visitorId));
    query.addBindValue(QVariant(rfidTag));

    bool ok = query.exec();
    if (! ok) {
        qWarning() << "ERROR: " << query.lastError().text();
    }
    ok = query.numRowsAffected() == 1;
    return ok;
}


bool Facade::updateVisitorSetRfid(int visitorId, const QString& rfidTag) {
    qDebug() << "updateVisitorSetRfid";

    QString sql = "UPDATE `visitor` SET `rfid` = ? WHERE `visitor_id` = ?";

    QSqlQuery query;
    query.prepare(sql);

    // Value(s) that replace the question mark(s) (?):
    query.addBindValue(QVariant(rfidTag));
    query.addBindValue(QVariant(visitorId));

    bool ok = query.exec();
    if (! ok) {
        qWarning() << "ERROR: " << query.lastError().text();
    }
    ok = query.numRowsAffected() == 1;
    return ok;
}


QString Facade::getUserLanguage(int userId) {
    qDebug() << "getUserLanguage";
    QString userLanguage;

    QString sql = "SELECT `language` FROM `visitor` WHERE `id` = ?";

    QSqlQuery query;
    query.prepare(sql);

    // Value(s) that replace the question mark(s) (?):
    query.addBindValue(QVariant(userId));
    bool ok = query.exec();
    if (! ok) {
        qWarning() << "ERROR: " << query.lastError().text();
    }
    while (query.next()) {
        userLanguage = query.value(0).toString();
    }
    return userLanguage;
}

QString Facade::getUserGender(int userId) {
    qDebug() << "getUserGender";
    QString userGender;

    QString sql = "SELECT `gender` FROM `visitor` WHERE `id` = ?";

    QSqlQuery query;
    query.prepare(sql);

    // Value(s) that replace the question mark(s) (?):
    query.addBindValue(QVariant(userId));
    bool ok = query.exec();
    if (! ok) {
        qWarning() << "ERROR: " << query.lastError().text();
    }
    while (query.next()) {
        userGender = query.value(0).toString();
    }
    return userGender;
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
    }
    while (query.next()) {
        QString questionId = query.value(0).toString();
        int answerValue = query.value(1).toInt();
        answers.insert(questionId, answerValue);
    }

    // TODO: Perhaps, for each answer that is not set,
    // we could create the key in the answers QMap, and set its value to -1. (?)
    return answers;
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
                  "`nation`.`identifier` AS `ethnicity` "
                  "FROM `visitor` "
                  "JOIN `nation` ON `visitor`.`nation` = `nation`.`id` "
                  "WHERE visitor.id = ?";

    QSqlQuery query;
    query.prepare(sql);

    // Value(s) that replace the question mark(s) (?):
    query.addBindValue(QVariant(userId));

    bool ok = query.exec();
    if (! ok) {
        qWarning() << "ERROR: " << query.lastError().text();
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
    return ret;
}


void Facade::setUserAnswer(int userId, const QString& questionId, int value) {
    qDebug() << "setUserAnswer";
    QString sql= "INSERT INTO `answer` "
                 "(`visitor_id`, `question_id`, `answer_value`) "
                 "SELECT ?, `question`.`id`, ? "
                 "FROM `question` "
                 "WHERE `question`.`identifier` = ?";
    QSqlQuery query;
    query.prepare(sql);

    // Value(s) that replace the question mark(s) (?):
    query.addBindValue(QVariant(userId));
    query.addBindValue(QVariant(value));
    query.addBindValue(QVariant(questionId));

    bool ok = query.exec();
    if (! ok) {
        qWarning() << "ERROR: " << query.lastError().text();
    }
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
    }
}


void Facade::freeUnusedTags() {
    qDebug() << "freeUnusedTags";
    QString sql = "UPDATE `tag` SET `visitor_id` = NULL";
    QSqlQuery query;
    query.prepare(sql);
    bool ok = query.exec();
    if (! ok) {
        qWarning() << "ERROR: " << query.lastError().text();
    }
}


bool Facade::setUserLanguage(int userId, const QString& language) {
    qDebug() << "setUserLanguage";
    QString sql = "UPDATE `visitor` SET `language` = ? WHERE `id` = ?";
    QSqlQuery query(sql, _database);

    // Value(s) that replace the question mark(s) (?):
    query.addBindValue(QVariant(language));
    query.addBindValue(QVariant(userId));

    bool ok = query.exec();
    if (! ok) {
        qWarning() << "ERROR: " << query.lastError().text();
    }
    return query.numRowsAffected() == 1;
}


bool Facade::setUserGender(int userId, const QString& gender) {\
    qDebug() << "setUserGender";
    QString sql = "UPDATE `visitor` SET `gender` = ? WHERE `id` = ?";
    QSqlQuery query;
    query.prepare(sql);

    // Value(s) that replace the question mark(s) (?):
    query.addBindValue(QVariant(gender));
    query.addBindValue(QVariant(userId));

    bool ok = query.exec();
    if (! ok) {
        qWarning() << "ERROR: " << query.lastError().text();
    }
    return query.numRowsAffected() == 1;
}


bool Facade::setUserNation(int userId, const QString& nation){

    QString sql = "UPDATE `visitor` SET `nation` = "
                  "(SELECT `nation`.`id` FROM `nation` WHERE `nation`.`identifier` = ?) "
                  "WHERE `visitor`.`id` = ?";
    QSqlQuery query;
    query.prepare(sql);

    // Value(s) that replace the question mark(s) (?):
    query.addBindValue(QVariant(nation));
    query.addBindValue(QVariant(userId));

    bool ok = query.exec();
    if (! ok) {
        qWarning() << "ERROR: " << query.lastError().text();
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
        }
        ret = ret || query.numRowsAffected() > 0;
    }

    return ret;
}


bool Facade::deleteTagsVisitorsAndTheirAnswers(const QList<QString>& rfidTags) {

    // TODO: complete this method, and make sure it works with a unit test.

    qDebug() << "deleteTagsUsersAndTheirAnswers";

    bool ret = false;

    static const bool deleteVisitors = true;
    static const bool deleteAnswers = true;
    static const bool deleteTags = true;

    for (auto iter = rfidTags.begin(); iter != rfidTags.end(); ++ iter) {
        auto rfidTag = (*iter);
        if (deleteVisitors) {
            QString sql = "DELETE FROM `visitor` "
                          "WHERE `visitor`.`rfidTag` = ?";
            QSqlQuery query;
            query.prepare(sql);

            // Value(s) that replace the question mark(s) (?):
            query.addBindValue(QVariant(rfidTag));

            bool ok = query.exec();
            if (! ok) {
                qWarning() << "ERROR: " << query.lastError().text();
            }
            ret = ret || query.numRowsAffected() > 0;
        }
        /*
         * TODO:
         *
        if (deleteAnswers) {
            QString sql = "DELETE FROM `answer`";
            QSqlQuery query;
            query.prepare(sql);
            bool ok = query.exec();
            if (! ok) {
                qWarning() << "ERROR: " << query.lastError().text();
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
            }
            ret = ret || query.numRowsAffected() > 0;
        }
        */
    }

    return ret;
}
