#include "facade.h"
#include <QSqlQuery>
#include <QVariant>
#include <QSqlError>
#include <QDebug>
#include <QStandardPaths>
#include <QDir>


Facade::Facade(
        const Config& config, QObject* parent) : QObject(parent), _config(config)
{
    _database = QSqlDatabase::addDatabase("QMYSQL");
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
    return getUserForTag(rfidTag);
}

int Facade::getUserForTag(const QString& rfidTag) {
    int visitorId = -1;
    bool tagExists = false;
    //QString selectSql = "SELECT visitor.id FROM visitor JOIN tag on tag.id = visitor.tag_id WHERE tag.rfid = :rfid";
    //QString sql = "INSERT INTO users VALUES ()";
    QString sql = "SELECT visitor_id FROM tag WHERE rfid = :rfid";
    QSqlQuery query(sql, _database);
    query.bindValue(":rfid", QVariant(rfidTag));
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
    // rreturn the created user id
    int visitorId = createNewUser();
    QString createSql = "INSERT INTO tag (`rfid`, `visitorId`) VALUES (:rfid, :visitor_id)";
    QSqlQuery query(createSql, _database);
    query.bindValue(":rfid", QVariant(rfidTag));
    query.bindValue(":visitorId", QVariant(visitorId));
    query.exec();
    bool ok = query.exec();
    if (! ok) {
        qWarning() << "ERROR: " << query.lastError().text();
    }
    return visitorId;
}

int Facade::createNewUser() {
    QString sql = "INSERT INTO visitor";
    QSqlQuery query(sql, _database);
    bool ok = query.exec();
    if (! ok) {
        qWarning() << "ERROR: " << query.lastError().text();
    }
    return query.lastInsertId().toInt();
}

bool Facade::updateTagSetVisitorId(const QString& rfidTag, int visitorId) {
    QString sql = "UPDATE tag SET `visitor_id` = :visitorId WHERE `rfid` = :rfid";
    QSqlQuery query(sql, _database);
    query.bindValue(":rfid", QVariant(rfidTag));
    query.bindValue(":visitorId", QVariant(visitorId));
    bool ok = query.exec();
    if (! ok) {
        qWarning() << "ERROR: " << query.lastError().text();
    }
    return query.numRowsAffected() == 1;
}

QString Facade::getUserLanguage(int userId) {
    QString ret;
    QString sql = "SELECT language FROM visitor WHERE id = :userId";
    QSqlQuery query(sql, _database);
    query.bindValue(":userId", QVariant(userId));
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
    QString ret;
    QString sql = "SELECT gender FROM visitor WHERE id = :userId";
    QSqlQuery query(sql, _database);
    query.bindValue(":userId", QVariant(userId));
    bool ok = query.exec();
    if (! ok) {
        qWarning() << "ERROR: " << query.lastError().text();
    }
    while (query.next()) {
        ret = query.value(0).toString();
    }
    return ret;
}

QMap<QString, int> Facade::getUserAnswers(int userId) {
    QMap<QString, int> ret;
    Q_UNUSED(userId);
    return ret;
    // TODO
}

void Facade::setUserAnswer(int userId, const QString& questionId, int value) {
    Q_UNUSED(userId);
    Q_UNUSED(questionId);
    Q_UNUSED(value);
}

QList<int> Facade::getStatsForQuestion(const QString& questionId) {
    QList<int> ret;
    Q_UNUSED(questionId);
    return ret;
    // TODO not sure what to return
}

void Facade::freeTag(const QString& rfidTag) {
    Q_UNUSED(rfidTag);
}

void Facade::freeUnusedTags() {

}

bool Facade::setUserLanguage(int userId, const QString& language) {
    QString sql = "UPDATE visitor SET `language` = :language WHERE `id` = :userId";
    QSqlQuery query(sql, _database);
    query.bindValue(":userId", QVariant(userId));
    query.bindValue(":language", QVariant(language));
    bool ok = query.exec();
    if (! ok) {
        qWarning() << "ERROR: " << query.lastError().text();
    }
    return query.numRowsAffected() == 1;
}

bool Facade::setUserGender(int userId, const QString& gender) {
    QString sql = "UPDATE visitor SET `gender` = :gender WHERE `id` = :userId";
    QSqlQuery query(sql, _database);
    query.bindValue(":userId", QVariant(userId));
    query.bindValue(":gender", QVariant(gender));
    bool ok = query.exec();
    if (! ok) {
        qWarning() << "ERROR: " << query.lastError().text();
    }
    return query.numRowsAffected() == 1;
}
