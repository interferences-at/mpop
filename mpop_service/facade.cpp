#include "facade.h"
#include <QSqlQuery>

static const int EXPECTED_NUM_TABLES = 3;

Facade::Facade(
        const QString& host, const QString& database, const QString& username,
        const QString& password, quint16 port, QObject *parent) : QObject(parent)
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

}

int Facade::createNewUser(QString rfidTag) {
    QString sql = "INSERT INTO users VALUES ()";
    QSqlQuery query(sql, _database);
    // FIXME
    int ret = 0;
    return ret;
}



QString Facade::getUserLanguage(int userId) {
    QString ret = "fr";
    return ret; // FIXME
}
int Facade::getOrCreateUser(QString rfidTag) {
    return 0; // FIXME
}
QMap<QString, int> Facade::getUserAnswers(int userId) {
    QMap<QString, int> ret;
    return ret;
    // TODO
}
void Facade::setUserAnswer(int userId, QString questionId, int value) {

}
QList<int> Facade::getStatsForQuestion(QString questionId) {
    QList<int> ret;
    return ret;
    // TODO
}
void Facade::freeTag(QString rfidTag) {

}
void Facade::freeUnusedTags() {

}
