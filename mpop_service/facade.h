#ifndef FACADE_H
#define FACADE_H

#include <QObject>
#include <QSqlDatabase>

class Facade : public QObject
{
    Q_OBJECT
public:
    explicit Facade(
            const QString& host, const QString& database, const QString& username,
            const QString& password, quint16 port, QObject *parent = nullptr);

    bool isDatabaseReady();
    int createNewUser(QString rfidTag);
signals:

private:
    QSqlDatabase _database;
    bool createTables();
};

#endif // FACADE_H
