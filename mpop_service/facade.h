#ifndef FACADE_H
#define FACADE_H

#include <QObject>
#include <QSqlDatabase>
#include <QMap>

class Facade : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief Constructor.
     * @param host MySQL host.
     * @param database MySQL database.
     * @param username MyQSL username.
     * @param password MySQL password.
     * @param port MySQL port number.
     */
    explicit Facade(
            const QString& database,
            const QString& host="localhost",
            const QString& username="root",
            const QString& password="",
            quint16 port=3306,
            QObject* parent=nullptr);
    /**
     * @brief Retrieves the favourite language for a given user.
     * @param userId
     * @return The favourite language of the user. ("en" or "fr")
     */
    QString getUserLanguage(int userId);
    /**
     * @brief Returns an active user for a RFID tag
     * @param rfidTag The RFID tag that is associated to it, or to associate it with.
     * @return The user ID of the user.
     */
    int getOrCreateUser(const QString& rfidTag);
    /**
     * @brief Retrieves all answers for a given user.
     * @param userId The user ID of the user.
     * @return A number of each question identifier.
     * TODO: perhaps return a QVariant
     */
    QMap<QString, int> getUserAnswers(int userId);
    /**
     * @brief Creates or update the answer to a specific question for a given user.
     * @param userId The user ID of the user.
     * @param questionId The question identifier.
     * @param value The value of the answer.
     */
    void setUserAnswer(int userId, const QString& questionId, int value);
    /*
     * TODO: perhaps return a QVariant
     * TODO: sort out the format of the result.
     */
    QList<int> getStatsForQuestion(const QString& questionId);
    /**
     * @brief Frees a RFID tag that is no longer owned by a user.
     * @param rfidTag The RFID tag.
     */
    void freeTag(const QString& rfidTag);

    /**
     * @brief Frees all tags that have not been used in a while.
     * FIXME: Define what is the timeout?
     */
    void freeUnusedTags();

    bool setUserLanguage(int userId, const QString& language);
    bool setUserGender(int userId, const QString& gender);
    QString getUserGender(int userId);

signals:

private: // data members:
    QSqlDatabase _database;

private: // methods:
    bool createTables();
    /**
     * @brief Checks if the database is ready.
     * @return True if the database is ready.
     */
    bool isDatabaseReady();
    int getUserForTag(const QString& rfidTag);
    int createTagAndUser(const QString& rfidTag);
    int createNewUser();
    bool updateTagSetVisitorId(const QString& rfidTag, int visitorId);
};

#endif // FACADE_H
