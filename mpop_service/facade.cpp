#include "facade.h"
#include <QSqlQuery>
#include <QVariant>
#include <QSqlError>
#include <QDebug>
#include <QStandardPaths>
#include <QDir>
#include <QSqlRecord>


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
    QString sql = "INSERT INTO visitor(	created_at) values(now())";
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

QMap<QString, QVariant> Facade::getUserAnswers(int userId) {
    QMap<QString, QVariant> ret;
        QString sql = "SELECT question.identifier,answer.answer_value FROM answer join question on"
                      "answer.question_id=question.id  WHERE answer.visitor_id = :userId";
        QSqlQuery query(sql, _database);
        query.bindValue(":userId", QVariant(userId));
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
        QString sql = "SELECT visitor.age, visitor.gender,visitor.language, nation.identifier from visitor join nation on visitor.nation=nation.id  WHERE visitor.id = :userId";
        QSqlQuery query(sql, _database);
        query.bindValue(":userId", QVariant(userId));
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
   QString sql= "insert into answer(visitor_id,question_id,answer_value) select :userId, id, :answer_value from question where identifier=:questionId ";
   QSqlQuery query(sql, _database);
   query.bindValue(":userId", QVariant(userId));
   query.bindValue(":questionId", QVariant(questionId));
   query.bindValue(":answer_value", QVariant(value));
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

    QString sql= "update tag set visitor_id=null where rfid=:rfidTag";
    QSqlQuery query(sql, _database);
    query.bindValue(":rfidTag", QVariant(rfidTag));
    bool ok = query.exec();
    if (! ok) {
        qWarning() << "ERROR: " << query.lastError().text();
    }


}

void Facade::freeUnusedTags() {

    QString sql= "";
    QSqlQuery query(sql, _database);
    bool ok = query.exec();
    if (! ok) {
        qWarning() << "ERROR: " << query.lastError().text();
    }
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

bool Facade::setUserNation(int userId, const QString& nation){

    QString sql = "UPDATE visitor SET `nation` = (select id from nation  WHERE `identifier`=:nation) where id= :userId ";
    QSqlQuery query(sql, _database);
    query.bindValue(":userId", QVariant(userId));
    query.bindValue(":nation", QVariant(nation));
    bool ok = query.exec();
    if (! ok) {
        qWarning() << "ERROR: " << query.lastError().text();
    }
    return query.numRowsAffected() == 1;
}
