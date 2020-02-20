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

    QString getUserLanguage(int userId);
    int getOrCreateUser(QString rfidTag);
    QMap<QString, int> getUserAnswers(int userId);
    void setUserAnswer(int userId, QString questionId, int value);
    QList<int> getStatsForQuestion(QString questionId);
    void freeTag(QString rfidTag);
    void freeUnusedTags();

signals:

private:
    QSqlDatabase _database;
    bool createTables();
    int createNewUser(QString rfidTag);
};

#endif // FACADE_H
