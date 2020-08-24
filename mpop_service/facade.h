#ifndef FACADE_H
#define FACADE_H

#include <QObject>
#include <QSqlDatabase>
#include <QMap>
#include <QTimer>
#include "config.h"


/**
 * @brief The Facade class implements most business logic for this application.
 *
 * Different API can call the methods of the single instance of this object, if needed.
 */
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
            const Config& config,
            QObject* parent=nullptr);

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

    /**
     * @brief Set the age for a user.
     */
    bool setUserAge(int userId, int age);

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
    bool setUserEthnicity(int userId, const QString& ethnicity);
    bool setUserLanguage(int userId, const QString& language);
    bool setUserGender(int userId, const QString& gender);



    QMap<QString, QVariant> getUserInfo(int userId);

    /**
     * @brief Delete all the entries in the database. (the user/answer ones)
     * @return true if some rows were affected.
     * @deprecated Use deleteTagsVisitorsAndTheirAnswers instead
     */
    bool deleteAllFromDatabase();

    bool deleteTagsVisitorsAndTheirAnswers(const QList<QString>& rfidTags);

    /**
     * @brief Checks if the database is ready.
     * @return True if the database is ready.
     */
    bool isDatabaseReady();
    
    /**
     * @brief Get user answers average by user age.
     * @param questionId The question identifier.
     * @param ethenicity The ethenicity identifier default value 'all'
     * @param gender identifier default value 'all'
     * @param timeAnswered identifier default value 'all'
     * @param value The average of the answers.
     */
    QList<int> getAnswerByAge(const QString& questionId,const QString& ethenicity,const QString& gender ,const QString& timeAnswered);



    /**
     * @brief Get user answers average by user gender.
     * @param questionId The question identifier
     * @param ethenicity The ethenicity identifier default value 'all'
     * @param ageTo The ageTo identifier default value -1 and upto 100
     * @param ageFrom The ageFrom identifier default value -1 and start from 0
     * @param timeAnswered identifier default value 'all'
     * @param value is average of answers.
     */
   QMap<QString,int> getAnswerByGender(const QString& questionId, const QString& ethenicity, int ageTo, int ageFrom  ,const QString& timeAnswered);



   /**
    * @brief Get user answers average by user Ethnicity.
    * @param questionId The question identifier.
    * @param ageTo identifier  default value -1 and upto 100
    * @param ageFrom identifier default value -1 and start from 0
    * @param gender identifier default value 'all'
    * @param timeAnswered identifier default value 'all'
    * @param value is average of answers.
    */
   QMap<QString, int> getAnswerByEthnicity(const QString& questionId,int ageFrom, int ageTo , const QString& gender,const QString& timeAnswered);


   /**
    * @brief Get user answers average for multiple Question Ids
    * @param questionIds The List of question identifier of 2, 3 or 5
    * @param ageFrom identifier default value -1 and start from 0
    * @param ageTo identifier default value -1 and upto 100
    * @param gender identifier default value 'all'
    * @param timeAnswered identifier default value 'all'
    * @param value is average of answers.
    */
   QMap<QString, int> getAnswers(const QList<QString>& questionIds, int ageFrom, int ageTo, const QString& ethnicity, const QString& gender, const QString& timeAnswered);


   /**
    * @brief Get user's all answers average .
    * @param value is average answer of each Question.
    */
   QMap<QString, int > getAllAnswers();

public slots:
   void reconnectTimerTriggered();

signals:
   // No signals

private: // data members:
    QSqlDatabase _database;
    bool _is_db_open = false;
    const Config& _config;
    QTimer _databaseReconnectTimer;

private: // methods:

    void openDatabaseIfNeeded();
    int createTagAndUser(const QString& rfidTag);
    int createNewUser(const QString& rfidTag);
    bool updateTagSetVisitorId(const QString& rfidTag, int visitorId);
};

#endif // FACADE_H
