#include "facade.h"
#include <QSqlQuery>
#include <QVariant>
#include <QSqlError>
#include <QDebug>

#include <QStandardPaths>
#include <QDir>

static const int EXPECTED_NUM_TABLES = 3;

static QString getSQLitePath() {
    //QString dataLocation = QStandardPaths::displayName(QStandardPaths::AppDataLocation);
    //QString dataLocation = QStandardPaths::displayName(QStandardPaths::HomeLocation);
    QString dataLocation = QDir::homePath();
    QDir dir = QDir(dataLocation);
    QString fileName = ".mpop_database.sqlite";
    return dir.absoluteFilePath(fileName);
}

Facade::Facade(
        const QString& database, const QString& host, const QString& username,
        const QString& password, quint16 port, QObject* parent) : QObject(parent)
{
    Q_UNUSED(database);
    Q_UNUSED(host);
    Q_UNUSED(username);
    Q_UNUSED(password);
    Q_UNUSED(port);

    QString sqlitePath = getSQLitePath();
    qInfo() << "Open database " << sqlitePath << endl;

    _database = QSqlDatabase::addDatabase("QSQLITE", sqlitePath);
    //_database.setHostName(host);
    _database.setDatabaseName(sqlitePath);
    //_database.setUserName(username);
    //_database.setPassword(password);
    //_database.setPort(static_cast<int>(port));

    bool ok = _database.open();

    if (! ok) {
        qWarning() << "ERROR: Could not open database";
    } else {

        if (! this->isDatabaseReady()) {
            ok = this->createTables();
        }
    }
}

bool Facade::isDatabaseReady() {
    QStringList tables = _database.tables();
    if (tables.length() == EXPECTED_NUM_TABLES) {
        qInfo() << "Found " << EXPECTED_NUM_TABLES << " tables, as expected." << endl;
        return true;
    } else {
        return false;
    }
}

bool Facade::createTables() {
    qInfo() << "Create tables" << endl;
    QStringList sqls;
    bool success = true;
    sqls.append(
        " CREATE TABLE IF NOT EXISTS tag "
        " ( "
        "   id BIGINT UNSIGNED NOT NULL PRIMARY KEY, "
        "   rfid VARCHAR(256) NOT NULL UNIQUE, "
        "   visitor_id BIGINT UNSIGNED NULL UNIQUE, "
        "   last_used_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP "
        " ) "
        );
    sqls.append(
        " CREATE TABLE IF NOT EXISTS visitor "
        " ( "
        "   id BIGINT UNSIGNED NOT NULL PRIMARY KEY, "
        "   gender VARCHAR(6) DEFAULT 'other', "
        "   language VARCHAR(2) DEFAULT 'fr', "
        "   created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP "
        " ) "
        );
    sqls.append(
        " CREATE TABLE IF NOT EXISTS answer "
        " ( "
        "   id BIGINT UNSIGNED NOT NULL PRIMARY KEY, "
        "   question_identifier VARCHAR(100), "
        "   value TINYINT UNSIGNED, "
        "   created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP "
        " ) "
        );

    for (int i = 0; i < sqls.size(); ++ i) {
        qInfo() << "Running SQL Query: ";
        qInfo() << sqls[i];
        QSqlQuery query(sqls[i], _database);
        bool ok = query.exec();
        if (! ok) {
            qWarning() << "ERROR: " << query.lastError().text();
            success = false;
        }
    }
    return success;
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
