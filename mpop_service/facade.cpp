#include "facade.h"
#include <QSqlQuery>
#include <QVariant>
#include <QSqlError>
#include <QDebug>
#include <QStandardPaths>
#include <QDir>
#include <QSqlRecord>
#include <QSqlDriver>
#include <exception>
#include <QString>
#include <QDateTime>


Facade::Facade(const Config& config, QObject* parent) : QObject(parent), _config(config), _databaseReconnectTimer()
{
    _database = QSqlDatabase::addDatabase("QMYSQL");

    if (_database.isValid()) {
        qDebug() << "Database is valid";
        qDebug() << "QSqlDriver hasFeature NamedPlaceholders:" << _database.driver()->hasFeature(QSqlDriver::NamedPlaceholders);
        // We don't allow to change the configuration options at runtime. Restart the service to change them.
        _database.setHostName(_config.mysql_host);
        _database.setDatabaseName(_config.mysql_database);
        _database.setUserName(_config.mysql_user);
        _database.setPassword(_config.mysql_password);
        _database.setPort(_config.mysql_port);

        this->openDatabaseIfNeeded();

    } else {
        qDebug() << "Database is not valid. The QMYSQL driver for Qt is probably not installed.";
        // TODO: Exit with an error.
    }

    // Periodically make sure that our connection with the database is active.
    // Attempt to re-open it if not.
    connect(&_databaseReconnectTimer, &QTimer::timeout, this, &Facade::reconnectTimerTriggered);
    static const int RECONNECT_INTERVAL_SECONDS = 5;
    static const int MS_PER_SECOND = 1000;
    _databaseReconnectTimer.setInterval(RECONNECT_INTERVAL_SECONDS * MS_PER_SECOND);
    _databaseReconnectTimer.start();
}


bool Facade::isDatabaseReady() {
    return _database.isOpen();
}


void Facade::reconnectTimerTriggered() {
    // Called every time the _databaseReconnectTimer's signal is triggered.
    this->openDatabaseIfNeeded();
}


void Facade::openDatabaseIfNeeded() {
    if (_database.isValid()) {
        if (_database.isOpen()) {
            qDebug() << "Database is already open. Good. Nothing to do.";
        } else {
            bool _is_db_open = _database.open();
            if (_is_db_open) {
                qInfo() << "Success connecting to the database.";
            } else {
                qWarning() << "ERROR: Could not open database";
                if (_database.isOpenError()) {
                    auto lastDbError = _database.lastError();
                    qInfo() << lastDbError;
                }
            }
        }
    }
}


int Facade::getOrCreateUser(const QString& rfidTag) {
    qDebug() << "getOrCreateUser";

    // FIXME: it seems like this returns 0 when no connection to the database was possible.
    // It should probably throw an error, instead.

    int visitorId = -1;

    QString sql = "SELECT `visitor_id` FROM `tag` WHERE `rfid` = ?";

    QSqlQuery query;
    query.prepare(sql);

    // Value(s) that replace the question mark(s) (?):
    query.addBindValue(QVariant(rfidTag));
    bool ok = query.exec();
    if (! ok) {
        // FIXME: Check the exact error type and close the DB connection only if this is due to the connection being lost.
        // Then retry to connect when the reconnect timer triggers.
        qWarning() << "ERROR: " << query.lastError().text();
        _database.close();
    } else {
        bool foundSomeVisitorWhoseTagMatches = query.next();
        if (foundSomeVisitorWhoseTagMatches) {
            bool tagExistsButHasNoUser = query.isNull(0);
            if (tagExistsButHasNoUser) {
                visitorId = this->createNewUser(rfidTag);
                updateTagSetVisitorId(rfidTag, visitorId);
            }
            else {
                visitorId = query.value(0).toInt(); // value("visitor_id") would also work, but is less efficient
            }
        } else {
            try {
                visitorId = this->createTagAndUser(rfidTag);
            } catch (std::exception & e) {
                // TODO: Answer with an error response
                qWarning() << "Internal Server Error ::" << e.what();
            }
        }
    }
    return visitorId;
}


int Facade::createTagAndUser(const QString& rfidTag) {
    qDebug() << "createTagAndUser";

    // First, create the new user:
    int  visitorId = createNewUser(rfidTag);

    QString createSql = "INSERT INTO tag (`rfid`, `visitor_id`) VALUES (?, ?)";

    QSqlQuery query;
    query.prepare(createSql);

    // Value(s) that replace the question mark(s) (?):
    query.addBindValue(QVariant(rfidTag));
    query.addBindValue(QVariant(visitorId));

    bool ok = query.exec();
    if (! ok) {
        qWarning() << "ERROR: " << query.lastError().text();
        throw(query.lastError().text());
    }

    return visitorId;
}


int Facade::createNewUser(const QString& rfidTag) {

    qDebug() << "createNewUser";

    QString sql = "INSERT INTO `visitor` (`id`,`rfid`) VALUES (NULL, ?)";

    QSqlQuery query;
    query.prepare(sql);

    // Value(s) that replace the question mark(s) (?):
    query.addBindValue(QVariant(rfidTag));
    bool ok = query.exec();
    int visitorId = -1;
    if (! ok) {
        qWarning() << "ERROR: " << query.lastError().text();
    } else {
        visitorId = query.lastInsertId().toInt();
    }
    return visitorId;
}


bool Facade::updateTagSetVisitorId(const QString& rfidTag, int visitorId) {
    qDebug() << "updateTagSetVisitorId";

    QString sql = "UPDATE `tag` SET `visitor_id` = ? WHERE `rfid` = ?";

    QSqlQuery query;
    query.prepare(sql);

    // Value(s) that replace the question mark(s) (?)
    query.addBindValue(QVariant(visitorId));
    query.addBindValue(QVariant(rfidTag));

    bool ok = query.exec();
    if (! ok) {
        qWarning() << "ERROR: " << query.lastError().text();
    } else {
        ok = query.numRowsAffected() == 1;
    }
    return ok;
}


/**
 * @brief Facade::getUserInfo
 * @param userId
 * @return an associative array where keys are fields name, and the values can be strings or ints.
 */
QMap<QString, QVariant> Facade::getUserInfo(int userId) {
    QMap<QString, QVariant> ret;

    QString sql = "SELECT "
                  "`visitor`.`age` AS `age`, "
                  "`visitor`.`rfid` AS `rfid`, "
                  "`visitor`.`gender` AS `gender`, "
                  "`visitor`.`language` AS `language`, "
                  "`ethnicity`.`identifier` AS `ethnicity` "
                  "FROM `visitor` "
                  "JOIN `ethnicity` ON `visitor`.`ethnicity` = `ethnicity`.`id` "
                  "WHERE visitor.id = ?";

    QSqlQuery query;
    query.prepare(sql);

    // Value(s) that replace the question mark(s) (?):
    query.addBindValue(QVariant(userId));

    bool ok = query.exec();
    if (! ok) {
        qWarning() << "ERROR: " << query.lastError().text();
    }

    bool retrieved = query.next();
    if (retrieved) {
        QSqlRecord rec = query.record();
        qDebug() << "Number of columns: " << rec.count();
        qDebug() << "getUserInfo for " << userId;
        for (int i = 0; i < rec.count(); ++ i) {
            QString keyName = rec.fieldName(i);
            QVariant value = rec.value(i);
            qDebug() << "User" << userId << ":" << keyName << value;
            ret.insert(keyName, value);
        }
    }
    else {
       // Retriving the visitor info in case of Ethnicity isn't set
        QString sql1 = "SELECT "
                      "`visitor`.`age` AS `age`, "
                      "`visitor`.`rfid` AS `rfid`, "
                      "`visitor`.`gender` AS `gender`, "
                      "`visitor`.`language` AS `language`, "
                      "`visitor`.`ethnicity` AS `ethnicity` "
                      "FROM `visitor` "
                      "WHERE visitor.id = ?";

        QSqlQuery query1;
        query1.prepare(sql1);

        // Value(s) that replace the question mark(s) (?):
        query1.addBindValue(QVariant(userId));

        bool ok1 = query1.exec();
        if (! ok1) {
            qWarning() << "ERROR: " << query1.lastError().text();
        }
        bool retrieved1 = query1.next();
        if (retrieved1) {
            QSqlRecord rec1 = query1.record();
            for (int j = 0; j < rec1.count(); ++ j) {
                QString keyName1 = rec1.fieldName(j);
                QVariant value1 = rec1.value(j);
                ret.insert(keyName1, value1);
            }
        }


    }
    return ret;
}


void Facade::setUserAnswer(int userId, const QString& questionId, int value=50) {
    qDebug() << "setUserAnswer";
    QString sql= "INSERT INTO `answer` "
                 "(`visitor_id`, `question_id`, `answer_value`) "
                 "SELECT ?, `question`.`id`, ? "
                 "FROM `question` "
                 "WHERE `question`.`identifier` = ?";
    QSqlQuery query;
    query.prepare(sql);

    // Value(s) that replace the question mark(s) (?):
    query.addBindValue(QVariant(userId));
    query.addBindValue(QVariant(value));
    query.addBindValue(QVariant(questionId));

    bool ok = query.exec();
    if (! ok) {
        qWarning() << "ERROR: " << query.lastError().text();
    }
}

QMap<QString, int> Facade::getUserAnswers(int userId) {
    QMap<QString, int> answers;

    QString sql = "SELECT "
                  "question.identifier, "
                  "answer.answer_value "
                  "FROM `answer` "
                  "JOIN `question` ON answer.question_id = question.id "
                  "WHERE answer.visitor_id = ?";

    QSqlQuery query;
    query.prepare(sql);

    // Value(s) that replace the question mark(s) (?):
    query.addBindValue(QVariant(userId));

    bool ok = query.exec();
    if (! ok) {
        qWarning() << "ERROR: " << query.lastError().text();
    }
    while (query.next()) {
        QString questionId = query.value(0).toString();
        int answerValue = query.value(1).toInt();
        answers.insert(questionId, answerValue);
    }

    return answers;
}


QList<int> Facade::getStatsForQuestion(const QString& questionId) {
    QList<int> ret;
    Q_UNUSED(questionId);
    return ret;
    // TODO not sure what to return
}


void Facade::freeTag(const QString& rfidTag) {
    qDebug() << "freeTag";
    QString sql= "UPDATE `tag` "
                 "SET `tag`.`visitor_id` = NULL "
                 "WHERE `tag`.`rfid` = ?";
    QSqlQuery query;
    query.prepare(sql);

    // Value(s) that replace the question mark(s) (?):
    query.addBindValue(QVariant(rfidTag));

    bool ok = query.exec();
    if (! ok) {
        qWarning() << "ERROR: " << query.lastError().text();
    }
}


void Facade::freeUnusedTags() {
    qDebug() << "freeUnusedTags";
    QString sql = "UPDATE `tag` SET `visitor_id` = NULL";
    QSqlQuery query;
    query.prepare(sql);
    bool ok = query.exec();
    if (! ok) {
        qWarning() << "ERROR: " << query.lastError().text();
    }
}


bool Facade::setUserLanguage(int userId, const QString& language) {
    qDebug() << "setUserLanguage";
    QString sql = "UPDATE `visitor` SET `language` = ?  WHERE `id` = ?";
    QSqlQuery query;
    query.prepare(sql);

    // Value(s) that replace the question mark(s) (?):
    query.addBindValue(QVariant(language));
    query.addBindValue(QVariant(userId));

    bool ok = query.exec();
    if (! ok) {
        qWarning() << "ERROR: " << query.lastError().text();
    }
     qDebug() << query.numRowsAffected();
    return query.numRowsAffected() == 1;
}


bool Facade::setUserGender(int userId, const QString& gender) {
    qDebug() << "setUserGender";
    QString sql = "UPDATE `visitor` SET `gender` = ?  WHERE `id` = ?";
    QSqlQuery query;
    query.prepare(sql);

    // Value(s) that replace the question mark(s) (?):
    query.addBindValue(QVariant(gender));
    query.addBindValue(QVariant(userId));

    bool ok = query.exec();
    if (! ok) {
        qWarning() << "ERROR: " << query.lastError().text();
    }
    qDebug() << query.numRowsAffected();
    return query.numRowsAffected() == 1;
}

bool Facade::setUserAge(int userId, int age) {
    qDebug() << "setUserAge";
    QString sql = "UPDATE `visitor` SET `age` = ? WHERE `id` = ?";
    QSqlQuery query;
    query.prepare(sql);

    // Value(s) that replace the question mark(s) (?):
    query.addBindValue(QVariant(age));
    query.addBindValue(QVariant(userId));

    bool ok = query.exec();
    if (! ok) {
        qWarning() << "ERROR: " << query.lastError().text();
    }
    return query.numRowsAffected() == 1;
}


bool Facade::setUserEthnicity(int userId, const QString& ethnicity){

    QString sql = "UPDATE `visitor` SET `ethnicity` = "
                  "(SELECT `ethnicity`.`id` FROM `ethnicity` WHERE `ethnicity`.`identifier` = ?) "
                  "WHERE `visitor`.`id` = ?";
    QSqlQuery query;
    query.prepare(sql);

    // Value(s) that replace the question mark(s) (?):
    query.addBindValue(QVariant(ethnicity));
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
        QSqlQuery query;
        query.prepare(sql);

        bool ok = query.exec();
        if (! ok) {
            qWarning() << "ERROR: " << query.lastError().text();
        }
        ret = ret || query.numRowsAffected() > 0;
    }

    if (deleteAnswers) {
        QString sql = "DELETE FROM `answer`";
        QSqlQuery query;
        query.prepare(sql);
        bool ok = query.exec();
        if (! ok) {
            qWarning() << "ERROR: " << query.lastError().text();
        }
        ret = ret || query.numRowsAffected() > 0;
    }

    if (deleteTags) {
        QString sql = "DELETE FROM `tag`";
        QSqlQuery query;
        query.prepare(sql);
        bool ok = query.exec();
        if (! ok) {
            qWarning() << "ERROR: " << query.lastError().text();
        }
        ret = ret || query.numRowsAffected() > 0;
    }

    return ret;
}


bool Facade::deleteTagsVisitorsAndTheirAnswers(const QList<QString>& rfidTags) {
    // TODO: make sure it works with a unit test.

    qDebug() << "deleteTagsUsersAndTheirAnswers";

    bool ret = false;

    static const bool deleteVisitors = true;
    static const bool deleteAnswers = true;
    static const bool deleteTags = true;

    for (auto iter = rfidTags.begin(); iter != rfidTags.end(); ++ iter) {
        auto rfidTag = (*iter);

        if(deleteVisitors && deleteAnswers && deleteTags){
            QString sql = "DELETE `visitor`, `tag`, `answer`"
                          "FROM `visitor`"
                          "LEFT join `tag` on `visitor`.`id`=`tag`.`visitor_id`"
                          "LEFT join `answer` on `visitor`.`id`=`answer`.`visitor_id`"
                          "WHERE `tag`.`rfid` = ?";
            QSqlQuery query;
            query.prepare(sql);
            query.addBindValue(QVariant(rfidTag));

            bool ok = query.exec();
            if (! ok) {
                qWarning() << "ERROR: " << query.lastError().text();
            }
            ret = ret || query.numRowsAffected() > 0;

        }
    }

    return ret;
}

/**
 * @brief Facade::getAnswerByAge
 * @param questionId
 * @param ethenicity
 * @param gender
 * @param timeAnswered
 * @return an associative list's the values are ints.
 */

QList<int> Facade::getAnswerByAge(const QString& questionId,const QString& ethenicity="all",const QString& gender="all" ,const QString& timeAnswered="all") {

    qDebug() << "GetAnswerByAge";
    QList<int> avgAnsByAge;

    QString sqlQuery= "select  IFNULL(ROUND( AVG(CASE WHEN (v.age<=5 and v.age>0) THEN  a.answer_value ELSE 0 END ),2),'-1') AS '[0-5]',"
                      " IFNULL(ROUND( AVG (CASE WHEN (v.age>5 and v.age <=10) THEN  a.answer_value ELSE 0 END),2),'-1') AS '[6-10]',"
                      " IFNULL(ROUND(AVG (CASE WHEN (v.age>10 and v.age<=15) THEN a.answer_value ELSE 0 END),2),'-1') AS '[11-15]',"
                      " IFNULL(ROUND( AVG (CASE WHEN (v.age>15 and v.age <=20) THEN a.answer_value ELSE 0 END),2),'-1') AS '[16-20]',"
                      " IFNULL(ROUND( AVG (CASE WHEN (v.age>20 and v.age<=25) THEN a.answer_value ELSE 0 END),2),'-1') AS '[21-25]',"
                      " IFNULL(ROUND( AVG (CASE WHEN (v.age>25 and v.age<=30) THEN a.answer_value ELSE 0 END),2),'-1') AS '[26-30]',"
                      " IFNULL(ROUND(AVG (CASE WHEN (v.age>30 and v.age<=35) THEN a.answer_value ELSE 0 END),2),'-1') AS '[31-35]',"
                      " IFNULL(ROUND(AVG (CASE WHEN (v.age>35 and v.age<=40) THEN a.answer_value ELSE 0 END),2),'-1') AS '[36-40]',"
                      " IFNULL(ROUND( AVG (CASE WHEN (v.age>40 and v.age<=45) THEN a.answer_value ELSE 0 END),2),'-1') AS '[41-45]',"
                      " IFNULL(ROUND( AVG (CASE WHEN (v.age>45 and v.age<=50) THEN a.answer_value ELSE 0 END),2),'-1') AS '[46-50]',"
                      " IFNULL(ROUND(AVG (CASE WHEN (v.age>50 and v.age<=55) THEN a.answer_value ELSE 0 END),2),'-1') AS '[51-55]',"
                      " IFNULL(ROUND(AVG (CASE WHEN (v.age>55 and v.age<=60) THEN a.answer_value ELSE 0 END),2),'-1') AS '[56-60]',"
                      " IFNULL(ROUND(AVG (CASE WHEN (v.age>60 and v.age<=65) THEN a.answer_value ELSE 0 END),2),'-1') AS '[61-65]',"
                      " IFNULL(ROUND(AVG (CASE WHEN (v.age>65 and v.age<=70) THEN a.answer_value ELSE 0 END),2),'-1') AS '[66-70]',"
                      " IFNULL(ROUND(AVG (CASE WHEN (v.age>70 and v.age<=75) THEN a.answer_value ELSE 0 END),2),'-1') AS '[71-75]',"
                      " IFNULL(ROUND(AVG (CASE WHEN (v.age>75 and v.age<=80) THEN a.answer_value ELSE 0 END),2),'-1') AS '[76-80]',"
                      " IFNULL(ROUND(AVG (CASE WHEN (v.age>80 and v.age<=85) THEN a.answer_value ELSE 0 END),2),'-1') AS '[81-85]',"
                      " IFNULL(ROUND(AVG (CASE WHEN (v.age>85 and v.age<=90) THEN a.answer_value ELSE 0 END),2),'-1') AS '[86-90]',"
                      " IFNULL(ROUND(AVG (CASE WHEN (v.age>90 and v.age<=95) THEN a.answer_value ELSE 0 END),2),'-1') AS '[91-95]',"
                      " IFNULL(ROUND(AVG (CASE WHEN (v.age>95 and v.age<=100) THEN a.answer_value ELSE 0 END),2),'-1') AS '[96-100]'"
                      "FROM answer AS a "
                      "JOIN  visitor AS v ON a.visitor_id=v.id JOIN question AS q ON a.question_id = q.id JOIN ethnicity AS e"
                      " ON v.ethnicity= e.id  WHERE q.identifier= ?";

    if(ethenicity != "all") {

        sqlQuery += " and e.`identifier`= ?";
    }
    if (gender != "all"){
        if(gender == "male"|| gender =="female" || gender == "other"){
            sqlQuery += " and v.`gender`= ?";
        }

    }
    if(timeAnswered != "all") {

        if(timeAnswered == "this_year"){
            sqlQuery += " and YEAR(v.`created_at`)= ?";
        }
        else if (timeAnswered == "today"){
            sqlQuery += " and date(v.`created_at`)= ?";
        }
    }
    QSqlQuery query;
    query.prepare(sqlQuery);

    // ? replaces the perameter in query
    query.addBindValue(QVariant(questionId));

    if(ethenicity!= "all") {
        query.addBindValue(QVariant(ethenicity));
    }

    if(gender != "all"){
        query.addBindValue(QVariant(gender));
    }

    if(timeAnswered != "all")
    {
        QString timeFilter;
        if(timeAnswered =="this_year") {

            timeFilter = "YEAR(CURDATE())";
        }
        else if (timeAnswered =="today") {

            timeFilter = "date(CURRENT_TIMESTAMP())";
        }
        query.addBindValue(QVariant(timeFilter));
    }
    qDebug() << "getAnswerByAge ::: Query ::" <<sqlQuery;
    bool ok = query.exec();

    if(!ok) {
        qWarning()<<"ERROR :: "<< query.lastError().text();
    }
    while (query.next()) {

        //providers Question Identifier's List of avg answer_value by age range
        avgAnsByAge.insert(1,query.value(0).toInt()), //[0-5]
        avgAnsByAge.insert(2,query.value(1).toInt()); //[6-10]
        avgAnsByAge.insert(3,query.value(2).toInt()); //[11-15]
        avgAnsByAge.insert(4,query.value(3).toInt()); //[16-20]
        avgAnsByAge.insert(5,query.value(4).toInt()); //[21-25]
        avgAnsByAge.insert(6,query.value(5).toInt()); //[26-30]
        avgAnsByAge.insert(7,query.value(6).toInt()); //[31-35]
        avgAnsByAge.insert(8,query.value(7).toInt()); //[36-40]
        avgAnsByAge.insert(9,query.value(8).toInt());// [41-45]
        avgAnsByAge.insert(10,query.value(9).toInt());//[46-50]
        avgAnsByAge.insert(11,query.value(10).toInt()); // [51-55]
        avgAnsByAge.insert(12,query.value(11).toInt()); //[56-60]
        avgAnsByAge.insert(13,query.value(12).toInt()); //[61-65]
        avgAnsByAge.insert(14,query.value(13).toInt()); //[66-70]
        avgAnsByAge.insert(15,query.value(14).toInt()); //[71-75]
        avgAnsByAge.insert(16,query.value(15).toInt()), //[76-80]
        avgAnsByAge.insert(17,query.value(16).toInt()); //[81-85]
        avgAnsByAge.insert(18,query.value(17).toInt()); //[86-90]
        avgAnsByAge.insert(19,query.value(18).toInt()); //[91-95]
        avgAnsByAge.insert(20,query.value(19).toInt()); // [96-100]
    }

    return  avgAnsByAge;
}

/**
 * @brief Facade::getAnswerByGender
 * @param questionId
 * @param ethenicity
 * @param ageTo
 * @param ageFrom
 * @param gender
 * @param timeAnswered
 * @return an associative array where key is user gender, and the values are ints.
 */
QMap<QString,int> Facade::getAnswerByGender(const QString& questionId, const QString& ethenicity="all", int ageTo=-1, int ageFrom =-1  ,const QString& timeAnswered="all")
{

    qDebug() << "getAnswerByGender";
    QMap<QString,int> AvgAnsByGender;
    
    QString sqlQuery= "SELECT  IFNULL( ROUND(AVG( CASE WHEN v.gender='male' "
                      "THEN a.answer_value ELSE 0 END ),2),'-1') as 'Male', "
                      "IFNULL(ROUND(AVG( CASE WHEN v.gender='female' THEN "
                      " a.answer_value ELSE 0 END ),2),'-1') as 'Female',"
                      "IFNULL(ROUND(AVG(CASE WHEN v.gender='other' THEN a.answer_value"
                      " ELSE 0 END ),2) ,'-1')as 'Other' "
                      "FROM answer AS a JOIN visitor AS v ON a.visitor_id = v.id JOIN"
                      " question AS q ON a.question_id = q.id "
                      "JOIN ethnicity AS e ON v.ethnicity= e.id WHERE q.identifier=?";

    
    if(ethenicity != "all") {

        sqlQuery += " and e.`identifier`= ?";
    }

    if(ageTo != -1 && ageFrom != -1 ) {

        sqlQuery += " and v.age BETWEEN ? and ? ";
    }
    else if (ageTo != -1) {

        sqlQuery += " and v.age BETWEEN 0 and ? ";
    }
    else if (ageFrom !=-1 ) {

        sqlQuery += " and v.age BETWEEN ? and  100 ";
    }

    if(timeAnswered != "all") {

        if(timeAnswered == "this_year"){
            sqlQuery += " and YEAR(v.`created_at`)= ?";
        }
        else if (timeAnswered == "today"){
            sqlQuery += " and date(v.`created_at`)= ?";
        }
    }

    QSqlQuery query;
    query.prepare(sqlQuery);

    // ? replaces the perameter in query
    query.addBindValue(QVariant(questionId));

    if(ethenicity!= "all") {
        query.addBindValue(QVariant(ethenicity));
    }

    if(ageTo != -1 and ageFrom != -1 ) {
        query.addBindValue(QVariant(ageFrom));
        query.addBindValue(QVariant(ageTo));
    }
    else if (ageTo != -1) {

        query.addBindValue(QVariant(ageTo));
    }
    else if (ageFrom !=-1 ) {

        query.addBindValue(QVariant(ageFrom));
    }

    if(timeAnswered != "all")
    {
        QString timeFilter;
        if(timeAnswered =="this_year"){

            timeFilter = "YEAR(CURDATE())";
        }
        else if (timeAnswered =="today"){

            timeFilter = "date(CURRENT_TIMESTAMP())";
        }
        query.addBindValue(QVariant(timeFilter));
    }
    qDebug() << "getAnswerByGender ::: Query ::" <<sqlQuery;
    bool ok = query.exec();

    if(!ok) {
        qWarning()<<"ERROR :: "<< query.lastError().text();
    }
    while (query.next()){

        //providers Question Identifier's  avg answer_value by Gender
        AvgAnsByGender.insert("male",query.value(0).toInt()); //male
        AvgAnsByGender.insert("female",query.value(1).toInt()); //female
        AvgAnsByGender.insert("other",query.value(2).toInt());//other

    }

    return  AvgAnsByGender;
}


/**
 * @brief Facade::getAnswerByEthnicity
 * @param questionId
 * @param ageTo
 * @param ageFrom
 * @param gender
 * @param timeAnswered
 * @return an associative array where key is user Ethnicity, and the values are ints.
 */

QMap<QString, int> Facade::getAnswerByEthnicity(const QString& questionId,int ageFrom=-1, int ageTo =-1, const QString& gender="all",const QString& timeAnswered="all"){

    qDebug() << "GetAnswerByEthnicity";
    QMap<QString, int> AvgAnsByEthnicity;

    QString sqlQuery="SELECT IFNULL(ROUND(AVG( CASE WHEN e.identifier='quebecer' THEN a.answer_value ELSE 0 END ),2),'-1') as 'Quebecer',"
                        "IFNULL( ROUND(AVG( CASE WHEN e.identifier='canadian' THEN a.answer_value ELSE 0 END ),2),'-1') as 'Canadian',"
                        "IFNULL( ROUND(AVG( CASE WHEN e.identifier='american' THEN a.answer_value ELSE 0 END ),2),'-1') as 'American', "
                        "IFNULL( ROUND(AVG( CASE WHEN e.identifier='european' THEN a.answer_value ELSE 0 END ),2),'-1') as 'European' ,"
                        "IFNULL( ROUND(AVG( CASE WHEN e.identifier='native' THEN a.answer_value ELSE 0 END ),2),'-1') as 'Native' ,"
                        "IFNULL( ROUND(AVG( CASE WHEN e.identifier='other' THEN a.answer_value ELSE 0 END ),2),'-1') as 'other' "
                        "from answer AS a JOIN visitor AS v ON a.visitor_id = v.id JOIN question AS q ON a.question_id = q.id"
                        " JOIN ethnicity AS e on v.ethnicity = e.id WHERE q.identifier = ? ";

    if(ageTo != -1 && ageFrom != -1 ){

        sqlQuery += " and v.age BETWEEN ? and ? ";
    }
    else if (ageTo != -1){

        sqlQuery += " and v.age BETWEEN 0 and ? ";
    }
    else if (ageFrom != -1 ){

        sqlQuery += " and v.age BETWEEN ? and  100 ";
    }

    if(gender != "all"){

        sqlQuery += " and v.gender = ? ";
    }

    if(timeAnswered != "all"){

        if(timeAnswered == "this_year"){
            sqlQuery += " and YEAR(v.`created_at`) = ?";
        }
        else if (timeAnswered == "today"){
            sqlQuery += " and date(v.`created_at`) = ?";
        }
    }

    QSqlQuery query;

    query.prepare(sqlQuery);

    // ? replaces the perameter in query
    query.addBindValue(QVariant(questionId));



    if(ageTo != -1 and ageFrom != -1 ){
        query.addBindValue(QVariant(ageFrom));
        query.addBindValue(QVariant(ageTo));
    }
    else if (ageTo != -1){

        query.addBindValue(QVariant(ageTo));
    }
    else if (ageFrom != -1 ){

        query.addBindValue(QVariant(ageFrom));
    }

    if(gender!="all"){

        query.addBindValue(QVariant(gender));
    }

    if(timeAnswered != "all")
    {
        QString timeFilter;
        if(timeAnswered == "this_year"){

            timeFilter = "YEAR(CURDATE())";
        }
        else if (timeAnswered == "today"){

            timeFilter = "date(CURRENT_TIMESTAMP())";
        }
        query.addBindValue(QVariant(timeFilter));
    }

    qDebug() << "getAnswerByEthnicity ::: Query ::" <<sqlQuery;

    bool ok = query.exec();

    if(!ok){
        qWarning()<<"ERROR :: "<< query.lastError().text();
    }

    while (query.next()) {

        //providers Question Identifier's  avg answer_value by Ethnicity
        AvgAnsByEthnicity.insert("quebecer",query.value(0).toInt());
        AvgAnsByEthnicity.insert("canadian",query.value(1).toInt());
        AvgAnsByEthnicity.insert("american",query.value(2).toInt());
        AvgAnsByEthnicity.insert("european",query.value(3).toInt());
        AvgAnsByEthnicity.insert("native",query.value(4).toInt());
        AvgAnsByEthnicity.insert("other",query.value(5).toInt());
    }

    return  AvgAnsByEthnicity;

}

QMap<QString, int > Facade:: getAllAnswers(){

    QMap<QString,int> avgQueAns ;

    QString sqlQuery="select q.identifier as 'Question', avg(a.answer_value) as 'Average' "
                     "from answer as a join question as q on a.question_id = q.id group by q.id";

    QSqlQuery query;

    bool ok = query.exec(sqlQuery);
    if (! ok) {
        qWarning() << "ERROR: " << query.lastError().text();
    }
    while (query.next()) {
        QString questionId = query.value(0).toString();
        int answerValue = query.value(1).toInt();
        avgQueAns.insert(questionId, answerValue);
    }

    return avgQueAns;
}



/**
 * @brief Facade::getAnswers
 * @param questionIds The List of question identifier.
 * @param ageFrom The ageFrom identifier
 * @param ageTo The ageTo identifier
 * @param gender identifier
 * @param timeAnswered identifier.
 * @return an associative array where key is questionIds, and their average values are ints.
 */
QMap<QString, int> Facade:: getAnswers(const QList<QString>& questionIds, int ageFrom=-1, int ageTo=-1, const QString& ethnicity="all", const QString& gender="all", const QString& timeAnswered="all") {

    qDebug() << "getAnswers";
    QMap<QString,int> avgAnsQueList;
    const int DEFAULT_AGE = -1;
    const QString DEFAULT_FILTER = "all";

    for (auto iter = questionIds.begin(); iter != questionIds.end(); ++ iter) {

        //retrive each Question Id from the List to calculate the avg ans.
        auto questionId = (*iter);

        QString sqlQuery=" SELECT q.identifier AS 'Question', AVG(a.answer_value) AS 'Average' "
                         " FROM answer AS a JOIN question AS q ON a.question_id = q.id"
                         " WHERE q.identifier= ?";


        // filter age group
        if(ageTo != DEFAULT_AGE && ageFrom != DEFAULT_AGE){

            sqlQuery += " AND v.age BETWEEN ? AND ? ";
        }

        else if (ageTo != DEFAULT_AGE){

            sqlQuery += " AND v.age BETWEEN 0 AND ? ";
        }
        else if (ageFrom != DEFAULT_AGE){

            sqlQuery += " and v.age BETWEEN ? AND  100 ";
        }

        //filter by ethnicity
        if(ethnicity != DEFAULT_FILTER) {

            sqlQuery += " AND e.`identifier`= ?";
        }

        //filter by gender
        if(gender != DEFAULT_FILTER){

            sqlQuery += " AND v.gender = ? ";
        }

        //filter by time answered
        if(timeAnswered != DEFAULT_FILTER){

            if(timeAnswered == "this_year"){
                sqlQuery += " AND YEAR(v.`created_at`) = ?";
            }
            else if (timeAnswered == "today"){
                sqlQuery += " AND date(v.`created_at`) = ?";
            }
        }

        sqlQuery += " GROUP BY q.id";


        QSqlQuery query;

        query.prepare(sqlQuery);

        // ? replaces the perameter in query
        query.addBindValue(QVariant(questionId));



        if(ageTo != DEFAULT_AGE and ageFrom != DEFAULT_AGE){
            query.addBindValue(QVariant(ageFrom));
            query.addBindValue(QVariant(ageTo));
        }
        else if (ageTo != DEFAULT_AGE){

            query.addBindValue(QVariant(ageTo));
        }
        else if (ageFrom != DEFAULT_AGE){

            query.addBindValue(QVariant(ageFrom));
        }


        if(ethnicity != DEFAULT_FILTER) {
            query.addBindValue(QVariant(ethnicity));
        }


        if(gender != DEFAULT_FILTER){

            query.addBindValue(QVariant(gender));
        }

        if(timeAnswered != DEFAULT_FILTER)
        {
            QString timeFilter;
            if(timeAnswered == "this_year"){

                timeFilter = "YEAR(CURDATE())";
            }
            else if (timeAnswered == "today"){

                timeFilter = "date(CURRENT_TIMESTAMP())";
            }
            query.addBindValue(QVariant(timeFilter));
        }

        bool ok = query.exec();

        if(!ok){
            qWarning() <<"ERROR :: "<< query.lastError().text();
        }

        while (query.next()) {

            //providers Question Identifier's  avg answer_value with optional filters.
            QString questionId = query.value(0).toString();
            int answerValue = query.value(1).toInt();
            avgAnsQueList.insert(questionId, answerValue);
        }

    }
    return avgAnsQueList;
}
