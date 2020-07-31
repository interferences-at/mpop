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

    // TODO: Periodically make sure that our connection with the database is active.
    // Attempt to re-open it if not.
}

bool Facade::isDatabaseReady() {
    return _database.open();
}


int Facade::getOrCreateUser(const QString& rfidTag) {
    qDebug() << "getOrCreateUser";
    return getUserForTag(rfidTag);
}

int Facade::getUserForTag(const QString& rfidTag) {
    qDebug() << "getUserForTag";
    int visitorId = -1;
    bool tagExists = false;
    //QString selectSql = "SELECT visitor.id FROM visitor JOIN tag on tag.id = visitor.tag_id WHERE tag.rfid = :rfid";
    //QString sql = "INSERT INTO users VALUES ()";
    QString sql = "SELECT `visitor_id` FROM `tag` WHERE `rfid` = ?";
    QSqlQuery query(sql, _database);
    query.addBindValue(QVariant(rfidTag));
    bool ok = query.exec();
    if (! ok) {
        qWarning() << "ERROR: " << query.lastError().text();
    }
    while (query.next()) {
        tagExists = true;
        bool tagExistsButHasNoUser = query.isNull(0);
        if (tagExistsButHasNoUser) {
            visitorId = this->createNewUser();
            updateTagSetVisitorId(rfidTag, visitorId);
        } else {
            visitorId = query.value(0).toInt(); // value("visitor_id") would also work, but is less efficient
        }
    }
    if (! tagExists) {
        visitorId = this->createTagAndUser(rfidTag);
    } else {
        // should not happen
    }
    return visitorId;
}


int Facade::createTagAndUser(const QString& rfidTag) {
    qDebug() << "createTagAndUser";
    // return the created user id
    // FIXME: It seems that createNewUser() should not be called every time. Check the logic.
    int visitorId = createNewUser();
    QString createSql = "INSERT INTO tag (`rfid`, `visitor_id`) VALUES (?, ?)";
    QSqlQuery query(createSql, _database);
    query.addBindValue(QVariant(rfidTag));
    query.addBindValue(QVariant(visitorId));
    query.exec();
    bool ok = query.exec();
    if (! ok) {
        qWarning() << "ERROR: " << query.lastError().text();
    }
    return visitorId;
}

int Facade::createNewUser() {
    qDebug() << "createNewUser";
    QString sql = "INSERT INTO `visitor` (`id`) VALUES (NULL)";
    QSqlQuery query(sql, _database);
    bool ok = query.exec();
    if (! ok) {
        qWarning() << "ERROR: " << query.lastError().text();
    }
    return query.lastInsertId().toInt();
}

bool Facade::updateTagSetVisitorId(const QString& rfidTag, int visitorId) {
    qDebug() << "updateTagSetVisitorId";
    QString sql = "UPDATE `tag` SET `visitor_id` = ? WHERE `rfid` = ?";
    QSqlQuery query(sql, _database);
    query.addBindValue(QVariant(visitorId));
    query.addBindValue(QVariant(rfidTag));

    bool ok = query.exec();
    if (! ok) {
        qWarning() << "ERROR: " << query.lastError().text();
    }
    return query.numRowsAffected() == 1;
}

QString Facade::getUserLanguage(int userId) {
    qDebug() << "getUserLanguage";
    QString ret;
    QString sql = "SELECT `language` FROM `visitor` WHERE `id` = ?";
    QSqlQuery query(sql, _database);
    query.addBindValue(QVariant(userId));
    bool ok = query.exec();
    if (! ok) {
        qWarning() << "ERROR: " << query.lastError().text();
    }
    while (query.next()) {
        ret = query.value(0).toString();
    }
    return ret;
}

QString Facade::getUserGender(int userId) {
    qDebug() << "getUserGender";
    QString ret;
    QString sql = "SELECT `gender` FROM `visitor` WHERE `id` = ?";
    QSqlQuery query(sql, _database);
    query.addBindValue(QVariant(userId));
    bool ok = query.exec();
    if (! ok) {
        qWarning() << "ERROR: " << query.lastError().text();
    }
    while (query.next()) {
        ret = query.value(0).toString();
    }
    return ret;
}

QMap<QString, QVariant> Facade::getUserAnswers(int userId) {
    QMap<QString, QVariant> ret;
        QString sql = "SELECT question.identifier, answer.answer_value FROM `answer` JOIN `question` ON "
                      "answer.question_id=question.id  WHERE answer.visitor_id = ?";
        QSqlQuery query(sql, _database);
        query.addBindValue(QVariant(userId));
        bool ok = query.exec();
        if (! ok) {
            qWarning() << "ERROR: " << query.lastError().text();
        }
        while (query.next()) {
            ret.insert( query.value(0).toString(), query.value(1).toString());
        }
        return ret;
}


QMap<QString, QVariant> Facade::getUserInfo(int userId) {
    QMap<QString, QVariant> ret;
        QString sql = "SELECT visitor.age, visitor.gender,visitor.language, nation.identifier FROM `visitor` JOIN nation ON visitor.nation=nation.id  WHERE visitor.id = ?";
        QSqlQuery query(sql, _database);
        query.addBindValue(QVariant(userId));
        bool ok = query.exec();
        if (! ok) {
            qWarning() << "ERROR: " << query.lastError().text();
        }
       QSqlRecord rec = query.record();
        query.next();
        for (int i=0; i<rec.count(); ++i) {
            ret.insert(rec.fieldName(i), query.value(i));
        }
        return ret;
}


void Facade::setUserAnswer(int userId, const QString& questionId, int value) {
    qDebug() << "setUserAnswer";
    QString sql= "INSERT INTO answer(visitor_id, question_id, answer_value) SELECT ?, id, ? FROM question WHERE identifier = ?";
    QSqlQuery query(sql, _database);
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
    QString sql= "UPDATE `tag` SET visitor_id = NULL WHERE rfid = ?";
    QSqlQuery query(sql, _database);
    query.addBindValue(QVariant(rfidTag));
    bool ok = query.exec();
    if (! ok) {
        qWarning() << "ERROR: " << query.lastError().text();
    }
}

void Facade::freeUnusedTags() {
    qDebug() << "freeUnusedTags";
    QString sql = "UPDATE `tag` SET visitor_id = NULL";
    QSqlQuery query(sql, _database);
    bool ok = query.exec();
    if (! ok) {
        qWarning() << "ERROR: " << query.lastError().text();
    }
}

bool Facade::setUserLanguage(int userId, const QString& language) {
    qDebug() << "setUserLanguage";
    QString sql = "UPDATE `visitor` SET `language` = ? WHERE `id` = ?";
    QSqlQuery query(sql, _database);
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
    QSqlQuery query(sql, _database);
    query.addBindValue(QVariant(gender));
    query.addBindValue(QVariant(userId));

    bool ok = query.exec();
    if (! ok) {
        qWarning() << "ERROR: " << query.lastError().text();
    }
    return query.numRowsAffected() == 1;
}


bool Facade::setUserNation(int userId, const QString& nation){

    QString sql = "UPDATE visitor SET `nation` = (SELECT `id` FROM `nation` WHERE `identifier` = ?) WHERE `id` = ?";
    QSqlQuery query(sql, _database);
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
        QSqlQuery query(sql, _database);
        bool ok = query.exec();
        if (! ok) {
            qWarning() << "ERROR: " << query.lastError().text();
        }
        ret = ret || query.numRowsAffected() > 0;
    }

    if (deleteAnswers) {
        QString sql = "DELETE FROM `answer`";
        QSqlQuery query(sql, _database);
        bool ok = query.exec();
        if (! ok) {
            qWarning() << "ERROR: " << query.lastError().text();
        }
        ret = ret || query.numRowsAffected() > 0;
    }

    if (deleteTags) {
        QString sql = "DELETE FROM `tag`";
        QSqlQuery query(sql, _database);
        bool ok = query.exec();
        if (! ok) {
            qWarning() << "ERROR: " << query.lastError().text();
        }
        ret = ret || query.numRowsAffected() > 0;
    }

    return ret;
}
