#include "facade.h"
#include <QSqlQuery>
#include <QVariant>

static const int EXPECTED_NUM_TABLES = 3;

Facade::Facade(
        const QString& database, const QString& host, const QString& username,
        const QString& password, quint16 port, QObject* parent) : QObject(parent)
{
    _database = QSqlDatabase::addDatabase("QMYSQL");
    _database.setHostName(host);
    _database.setDatabaseName(database);
    _database.setUserName(username);
    _database.setPassword(password);
    _database.setPort((int) port);
    bool ok = _database.open();

    if (! this->isDatabaseReady()) {
        this->createTables();
    }
}

bool Facade::isDatabaseReady() {
    QStringList tables = _database.tables();
    if (tables.length() == EXPECTED_NUM_TABLES) {
        return true;
    } else {
        return false;
    }
}

bool Facade::createTables() {
    return false; // TODO - or never do it.
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
    query.exec();
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
    QSqlQuery createQuery(createSql, _database);
    createQuery.bindValue(":rfid", QVariant(rfidTag));
    createQuery.bindValue(":visitorId", QVariant(visitorId));
    createQuery.exec();
    return visitorId;
}

int Facade::createNewUser() {
    QString sql = "INSERT INTO visitor";
    QSqlQuery query(sql, _database);
    query.exec();
    return query.lastInsertId().toInt();
}

bool Facade::updateTagSetVisitorId(const QString& rfidTag, int visitorId) {
    QString sql = "UPDATE tag SET `visitor_id` = :visitorId WHERE `rfid` = :rfid";
    QSqlQuery query(sql, _database);
    query.bindValue(":rfid", QVariant(rfidTag));
    query.bindValue(":visitorId", QVariant(visitorId));
    query.exec();
    return query.numRowsAffected() == 1;
}

QString Facade::getUserLanguage(int userId) {
    QString ret;
    QString sql = "SELECT language FROM visitor WHERE id = :userId";
    QSqlQuery query(sql, _database);
    query.bindValue(":userId", QVariant(userId));
    query.exec();
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
    query.exec();
    while (query.next()) {
        ret = query.value(0).toString();
    }
    return ret;
}

QMap<QString, int> Facade::getUserAnswers(int userId) {
    QMap<QString, int> ret;
    return ret;
    // TODO
}

void Facade::setUserAnswer(int userId, const QString& questionId, int value) {

}

QList<int> Facade::getStatsForQuestion(const QString& questionId) {
    QList<int> ret;
    return ret;
    // TODO not sure what to return
}

void Facade::freeTag(const QString& rfidTag) {

}

void Facade::freeUnusedTags() {

}

bool Facade::setUserLanguage(int userId, const QString& language) {
    QString sql = "UPDATE visitor SET `language` = :language WHERE `id` = :userId";
    QSqlQuery query(sql, _database);
    query.bindValue(":userId", QVariant(userId));
    query.bindValue(":language", QVariant(language));
    query.exec();
    return query.numRowsAffected() == 1;
}

bool Facade::setUserGender(int userId, const QString& gender) {
    QString sql = "UPDATE visitor SET `gender` = :gender WHERE `id` = :userId";
    QSqlQuery query(sql, _database);
    query.bindValue(":userId", QVariant(userId));
    query.bindValue(":gender", QVariant(gender));
    query.exec();
    return query.numRowsAffected() == 1;
}
