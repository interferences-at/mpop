#include "mpopservice.h"
#include "sqlerror.h"
#include <QtWebSockets>
#include <exception>
#include <QtCore>
#include <QDebug>
#include <cstdio>
#include "notification.h"
#include <iostream>

QT_USE_NAMESPACE

bool MPopService::toBoolean(const QString& value) {
    bool ret = false;
    if (value.toLower() == "true" || value.toInt() == 1) {
        ret = true;
    }
    // I think we could also simply do:
    // return QVariant(value).toBool();
    return ret;
}

void MPopService::load_config_from_env_vars(Config& config) {
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    config.mysql_port = env.value("CONFIG_MYSQL_PORT", "3306").toUInt();
    config.mysql_user = env.value("CONFIG_MYSQL_USER", "admin");
    config.mysql_database = env.value("CONFIG_MYSQL_DATABASE", "mpop_database");
    config.mysql_password = env.value("CONFIG_MYSQL_PASSWORD", "secret");
    config.mysql_host = env.value("CONFIG_MYSQL_HOST", "db"); // Use '0.0.0.0' as a hostname, if you use from outside docker-compose
    config.service_port_number = env.value("MPOP_SERVICE_PORT_NUMBER", "3333").toUInt();
    config.is_verbose = toBoolean(env.value("CONFIG_IS_VERBOSE", "true"));
    config.periodic_interval = env.value("CONFIG_PERIODIC_INTERVAL","60000").toUInt();
    config.time_at_free_all_tag = env.value("CONFIG_TIME_FREE_ALL_TAG","00:01");
    if (config.is_verbose) {
        qDebug() << "mysql_port:" << config.mysql_port;
        qDebug() << "mysql_user:" << config.mysql_user;
        qDebug() << "mysql_database:" << config.mysql_database;
        qDebug() << "mysql_password:" << config.mysql_password;
        qDebug() << "mysql_host:" << config.mysql_host;
        qDebug() << "service_port_number:" << config.service_port_number;
        qDebug() << "is_verbose:" << config.is_verbose;
        qDebug() << "periodic_interval" << config.periodic_interval;
        qDebug() << "time_at_free_all_tag" << config.time_at_free_all_tag;
    }
}

/**
 * @brief Returns an identifier for a given Websocket client.
 * @param peer Websocket client.
 * @return Host and port, concatenated.
 */
static QString getIdentifier(QWebSocket* peer) {
    return QStringLiteral("%1:%2").arg(peer->peerAddress().toString(),
                                       QString::number(peer->peerPort()));
}

MPopService::MPopService(const Config& config, QObject* parent) :
    QObject(parent),
    m_pWebSocketServer(new QWebSocketServer(QStringLiteral("MPOP Service"), QWebSocketServer::NonSecureMode, this)),
    _facade(config),
    _config(config)
{
    quint16 port = config.service_port_number;
    if (m_pWebSocketServer->listen(QHostAddress::Any, port)) {
        QTextStream(stdout) << "Websocket server listening on port " << port << '\n';
        connect(m_pWebSocketServer, &QWebSocketServer::newConnection,  this, &MPopService::newConnectionCb);
    }
}

MPopService::~MPopService() {
    m_pWebSocketServer->close();
}

void MPopService::newConnectionCb() {
    auto socket = m_pWebSocketServer->nextPendingConnection();
    QTextStream(stdout) << getIdentifier(socket) << " connected!\n";
    socket->setParent(this);
    connect(socket, &QWebSocket::textMessageReceived, this, &MPopService::textMessageReceivedCb);
    connect(socket, &QWebSocket::disconnected,  this, &MPopService::socketDisconnectedCb);
    m_clients << socket;
}

void MPopService::textMessageReceivedCb(const QString &message) {
    QWebSocket *pSender = qobject_cast<QWebSocket *>(sender());
    bool broadcastNotification = false;

    QTextStream(stdout) << "got a request";
    try {
        QString response = this->handleJsonRpcTwoMethod(message, broadcastNotification);

        if (broadcastNotification) {
            QString notification = response;
            for (QWebSocket *pClient : qAsConst(m_clients)) {
                if (pClient != pSender) { //don't echo message back to sender
                    pClient->sendTextMessage(notification);
                }
            }
        } else {
            pSender->sendTextMessage(response);
        }
    } catch (const std::exception& e) {
        //qDebug << e.what();
    }
}


void MPopService::socketDisconnectedCb() {
    QWebSocket *client = qobject_cast<QWebSocket*>(sender());
    QTextStream(stdout) << getIdentifier(client) << " disconnected!\n";
    if (client) {
        m_clients.removeAll(client);
        client->deleteLater();
    }
}


QString MPopService::handleJsonRpcTwoMethod(const QString& message, bool &broadcastNotification) {
    //std::string str = message.toStdString();
    //QJsonDocument requestDocument = QJsonDocument::fromRawData(str.c_str(), str.size(), QJsonDocument::Validate);
    Request request = Request::fromString(message);
    QTextStream(stdout) << "Request received: " << request.toString() << endl;
    //if (document == nullptr) {
    //    qDebug << "Could not parse JSON from JSON-RPC 2.0 message.";
    //}
    Response response;
    response.copyIdFromRequest(request);
    QTextStream(stdout) << "The ID of our response will be: " << response.intId << endl;
    // response.method = request.method;
    // bool sendResponse = true;

    if (request.method == "message") {
        QTextStream(stdout) << "Got method message" << endl;
        //response.result = QVariant::fromValue(QString("pong"));
        QTextStream(stdout) << "Answer with pong" << endl;
        // sendResponse = false;
        broadcastNotification = true;
        Notification notification;
        notification.method = "message";
        notification.paramsByPosition = request.paramsByPosition;
        QString ret = notification.toString();
        QTextStream(stdout) << "Response: " << ret;
        return ret;
    } else if (request.method == "ping") {
        QTextStream(stdout) << "Got method ping" << endl;
        response.result = QVariant::fromValue(QString("pong"));
        QTextStream(stdout) << "Answer with pong" << endl;
    } else if (request.method == "echo") {
        QTextStream(stdout) << "Got method echo" << endl;
        response.result = QVariant(request.paramsByName);
        QTextStream(stdout) << "Answer with echo" << endl;
    } else if (this->handleFacadeMethod(request, response)) {
        QTextStream(stdout) << "Successfully handled a facade method." << endl;
        // success
    } else {
        QTextStream(stdout) << "unhandled request" << endl;
    }

    QString ret = response.toString(); // return string response (JSON)
    QTextStream(stdout) << "Response: " << ret;
    return ret;
}

bool MPopService::handleFacadeMethod(const Request& request, Response& response) {
    // write error message in case of exception
    QString msg;

    QTextStream(stdout) << "Attempt to handle a facade method." << endl;

    qDebug() << "paramsByPosition: ";
    QVariantList paramsByPosition = request.paramsByPosition;
    for (QVariantList::iterator iter = paramsByPosition.begin(); iter != paramsByPosition.end(); iter ++)
    {
        qDebug() << (* iter).typeName();
        qDebug() << (* iter).toString(); // Print QVariant
    }

    // Write to the response object.
    QString method = request.method;
    if (method == "getOrCreateUser") {
        QTextStream(stdout) << "Method is: getOrCreateUser" << endl;
        try {
            QTextStream(stdout) << "getParamByPosition 0..." << endl;
            QString rfidTag = request.getParamByPosition(0).toString();
            QTextStream(stdout) << "getOrCreateUser: parsed rfidTag: " << rfidTag << endl;
            QTextStream(stdout) << "getOrCreateUser: calling the Facade method with arg " << rfidTag << endl;
            response.result = QVariant(this->_facade.getOrCreateUser(rfidTag));
            // If the result is -1, it didn't work!
            // FIXME: Properly respond with a JSON-RPC error response
        } catch(MissingParameterError& e) {
            msg.append(e.what());
            response.error.message = msg;
        } catch(SQLError& e){
            msg.append(e.what());
            response.error.message = msg;
        }
    }
    else if (method == "getUserInfo") {
        QTextStream(stdout) << "Method is: getUserInfo" << endl;
        try {
            int userId = request.getParamByPosition(0).toInt();
            response.result = QVariant(this->_facade.getUserInfo(userId));
        } catch (MissingParameterError& e) {
            msg.append(e.what());
            response.error.message = msg;
        } catch(SQLError& e){
            msg.append(e.what());
            response.error.message = msg;
        }

    }

    else if (method == "getUserAnswers") {
        QTextStream(stdout) << "Method is: getUserAnswers" << endl;
        try {
            int userId = request.getParamByPosition(0).toInt();
            QMap<QString, int> answers = this->_facade.getUserAnswers(userId);
            response.result = QVariant(MPopService::stringIntMapToQVariantMap(answers));
        } catch (MissingParameterError& e) {
            msg.append(e.what());
            response.error.message = msg;
        } catch(SQLError& e){
            msg.append(e.what());
            response.error.message = msg;
        }
    }
    else if (method == "setUserAnswer") {
        QTextStream(stdout) << "Method is: setUserAnswer" << endl;
        try {
            int userId = request.getParamByPosition(0).toInt();
            QString question= request.getParamByPosition(1).toString();
            int value = request.getParamByPosition(2).toInt();
            this->_facade.setUserAnswer(userId, question, value);
        } catch (MissingParameterError& e) {
            msg.append(e.what());
            response.error.message = msg;
        } catch(SQLError& e){
            msg.append(e.what());
            response.error.message = msg;
        }
    }
    else if (method=="freeTag") {
        QTextStream(stdout) << "Method is: freeTag" << endl;
        try {
            QString rfidTag = request.getParamByPosition(0).toString();
            this->_facade.freeTag(rfidTag);
        } catch (MissingParameterError& e) {
            msg.append(e.what());
            response.error.message = msg;
        } catch(SQLError& e){
            msg.append(e.what());
            response.error.message = msg;
        }
    }
    else if (method == "freeUnusedTags") {
        QTextStream(stdout) << "Method is: freeUnusedTags" << endl;
        try {
            this->_facade.freeAllTags();
        } catch (MissingParameterError& e) {
            msg.append(e.what());
            response.error.message = msg;
        } catch(SQLError& e){
            msg.append(e.what());
            response.error.message = msg;
        }
    }
    else if (method == "setUserLanguage") {
        QTextStream(stdout) << "Method is: setUserLanguage" << endl;
        try {
            int userId = request.getParamByPosition(0).toInt();
            QString language = request.getParamByPosition(1).toString();
            response.result = QVariant(this->_facade.setUserLanguage(userId, language));
        } catch (MissingParameterError& e) {
            msg.append(e.what());
            response.error.message = msg;
        } catch(SQLError& e){
            msg.append(e.what());
            response.error.message = msg;
        }
    }
    else if (method == "setUserGender") {
        QTextStream(stdout) << "Method is: setUserGender" << endl;
        try {
            int userId = request.getParamByPosition(0).toInt();
            QString gender = request.getParamByPosition(1).toString();
            response.result = QVariant(this->_facade.setUserGender(userId, gender));
        } catch (MissingParameterError& e) {
            msg.append(e.what());
            response.error.message = msg;
        } catch(SQLError& e){
            msg.append(e.what());
            response.error.message = msg;
        }
    }
    else if (method == "setUserEthnicity") {
        QTextStream(stdout) << "Method is: setUserEthnicity" << endl;
        try {
            int userId = request.getParamByPosition(0).toInt();
            QString ethnicity = request.getParamByPosition(1).toString();
            response.result = QVariant(this->_facade.setUserEthnicity(userId, ethnicity));
        } catch (MissingParameterError &e) {
            msg.append(e.what());
            response.error.message = msg;
        } catch(SQLError& e){
            msg.append(e.what());
            response.error.message = msg;
        }
    }
    else if (method == "setUserAge") {
        QTextStream(stdout) << "Method is: setUserAge" << endl;
        try {
            int userId = request.getParamByPosition(0).toInt();
            int age = request.getParamByPosition(1).toInt();
            response.result = QVariant(this->_facade.setUserAge(userId, age));
        } catch (MissingParameterError &e) {
            msg.append(e.what());
            response.error.message = msg;
        } catch(SQLError& e){
            msg.append(e.what());
            response.error.message = msg;
        }

    }
    else if (method == "getAnswers") {
        QTextStream(stdout) << "Method is: getAnswers" << endl;
        try {

            const QList<QString>& questionIds = request.getParamByPosition(0).toStringList();
            int ageFrom =  request.getParamByPosition(1).toInt();
            int ageTo = request.getParamByPosition(2).toInt() ;
            const QString& ethnicity = request.getParamByPosition(3).toString().trimmed().length()==0 ? "all" :  request.getParamByPosition(3).toString();
            const QString& gender = request.getParamByPosition(4).toString().trimmed().length()==0 ? "all" : request.getParamByPosition(4).toString();
            const QString& timeAnswered =request.getParamByPosition(5).toString().trimmed().length()==0 ? "all" : request.getParamByPosition(5).toString();
            QMap<QString,int> avgOfAns = this->_facade.getAnswers(questionIds, ageFrom, ageTo, ethnicity, gender, timeAnswered);
            response.result = QVariant(MPopService::stringIntMapToQVariantMap(avgOfAns));
        } catch (MissingParameterError &e) {
            msg.append(e.what());
            response.error.message = msg;
        } catch(SQLError& e){
            msg.append(e.what());
            response.error.message = msg;
        }
    }
    else if (method == "getAnswerByAge") {
        QTextStream(stdout) << "Method is: getAnswerByAge" << endl;
        try {
            const QString& questionId = request.getParamByPosition(0).toString();
            const QString& ethnicity = request.getParamByPosition(1).toString().trimmed().length()==0 ? "all" :  request.getParamByPosition(1).toString();
            const QString& gender = request.getParamByPosition(2).toString().trimmed().length()==0 ? "all" : request.getParamByPosition(2).toString();
            const QString& timeAnswered = request.getParamByPosition(3).toString().trimmed().length()==0 ? "all" : request.getParamByPosition(3).toString();
            QList<int> ansByAge = this->_facade.getAnswerByAge(questionId, ethnicity, gender, timeAnswered);
            QVariant varLst = QVariant::fromValue(ansByAge);
            response.result = varLst;
        } catch (MissingParameterError &e) {
            msg.append(e.what());
            response.error.message = msg;
        } catch(SQLError& e){
            msg.append(e.what());
            response.error.message = msg;
        }
    }
    else if (method == "getAnswerByGender") {
        QTextStream(stdout) << "Method is: getAnswerByGender" << endl;
        try {
            const QString& questionId = request.getParamByPosition(0).toString();
            const QString& ethnicity = request.getParamByPosition(1).toString().trimmed().length()==0 ? "all" :  request.getParamByPosition(1).toString();
            int ageTo = request.getParamByPosition(2).toInt() ;
            int ageFrom =  request.getParamByPosition(3).toInt();
            const QString& timeAnswered = request.getParamByPosition(4).toString().trimmed().length()==0 ? "all" : request.getParamByPosition(4).toString();
            QMap<QString,int> ansByGender = this->_facade.getAnswerByGender(questionId, ethnicity, ageTo, ageFrom, timeAnswered);
            response.result = QVariant(MPopService::stringIntMapToQVariantMap(ansByGender));
        } catch (MissingParameterError &e) {
            msg.append(e.what());
            response.error.message = msg;
        } catch(SQLError& e){
            msg.append(e.what());
            response.error.message = msg;
        }
    }
    else if (method == "getAnswerByEthnicity") {

        QTextStream(stdout) << "Method is: getAnswerByEthnicity" << endl;
        try {
            const QString& questionId = request.getParamByPosition(0).toString();
            int ageFrom =  request.getParamByPosition(1).toInt();
            int ageTo = request.getParamByPosition(2).toInt() ;
            const QString& gender = request.getParamByPosition(3).toString().trimmed().length()==0 ? "all" : request.getParamByPosition(3).toString();
            const QString& timeAnswered = request.getParamByPosition(4).toString().trimmed().length()==0 ? "all" : request.getParamByPosition(4).toString();
            QMap<QString,int> ansByEthnicity = this->_facade.getAnswerByEthnicity(questionId, ageFrom, ageTo, gender, timeAnswered);
            response.result = QVariant(MPopService::stringIntMapToQVariantMap(ansByEthnicity));
        } catch (MissingParameterError &e) {
            msg.append(e.what());
            response.error.message = msg;
        } catch(SQLError& e){
            msg.append(e.what());
            response.error.message = msg;
        }
    }
    else if (method == "getAllAnswers") {

        QTextStream(stdout) << "Method is: getAllAnswers" << endl;
        try {
            QMap<QString,int> ansByEthnicity = this->_facade.getAllAnswers();
            response.result = QVariant(MPopService::stringIntMapToQVariantMap(ansByEthnicity));
        } catch (MissingParameterError &e) {
            msg.append(e.what());
            response.error.message = msg;
        } catch(SQLError& e){
            msg.append(e.what());
            response.error.message = msg;
        }
    }
    else if (method == "getRandomValue") {

        QTextStream(stdout) << "Method is: getRandomValue" ;
        QString strkeyTotalAns = "total_num_answers";
        QString strkeyAnsLastHour = "num_answer_last_hour";
        QString strkeyAvgAns = "overall_average_answer";
        QString strkeyTotalVisitor = "total_num_visitors";
        QString strkeyTodayVisitor = "visitors_today";

        QMap<QString,float> listOfRanVal;
        try {
            float randValTotalAns = this->_facade.getRandomValueByTotalAns();
            listOfRanVal.insert(strkeyTotalAns,randValTotalAns);

            float randValAnsLastHour = this->_facade.getRandomValueByAnsLastHour();
            listOfRanVal.insert(strkeyAnsLastHour,randValAnsLastHour);

            float randValAvgAllAns = this->_facade.getRandomValueByAvgofAllAns();
            listOfRanVal.insert(strkeyAvgAns,randValAvgAllAns);

            float randValTotalVistr = this->_facade.getRandomValueByTotalVisitors();
            listOfRanVal.insert(strkeyTotalVisitor,randValTotalVistr);

            float randValTodayVistor = this->_facade.getRandomValueByTodaysVisitors();
            listOfRanVal.insert(strkeyTodayVisitor,randValTodayVistor);

            response.result = QVariant(MPopService::stringFloatMapToQVariantMap(listOfRanVal));
        } catch (MissingParameterError &e) {
            msg.append(e.what());
            response.error.message = msg;
        } catch(SQLError& e){
            msg.append(e.what());
            response.error.message = msg;
        }
    }
    else {
        QTextStream(stdout) << "Method " << method << " is unknown" << endl;
        response.error.message = QString("No such method: %1").arg(method);
    }

    // TODO QList<int> getStatsForQuestion(const QString& questionId);

    return true;
}


void MPopService::timeWatcher(const Config& config){

    QTextStream(stdout) << "Method is: timeWatcher";
    QDateTime local = QDateTime::currentDateTime();
    QString curTime = local.toString("hh:mm");
    QTextStream(stdout) << "Current time  is : " << curTime;
    if(curTime == config.time_at_free_all_tag) {

        //TODO: Exception Handling should be Uncommented when PR with SQLError.h Merge
//       try {
           Facade::freeAllTags();
//       } catch(SQLError& e){
//             qWarning() << "Internal Server Error :: " << e.what();

//       }
   }

}
