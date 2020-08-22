#include "mpopservice.h"

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
    if (config.is_verbose) {
        qDebug() << "mysql_port:" << config.mysql_port;
        qDebug() << "mysql_user:" << config.mysql_user;
        qDebug() << "mysql_database:" << config.mysql_database;
        qDebug() << "mysql_password:" << config.mysql_password;
        qDebug() << "mysql_host:" << config.mysql_host;
        qDebug() << "service_port_number:" << config.service_port_number;
        qDebug() << "is_verbose:" << config.is_verbose;
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
        }
    }
    else if (method == "freeUnusedTags") {
        QTextStream(stdout) << "Method is: freeUnusedTags" << endl;
        try {
            this->_facade.freeUnusedTags();
        } catch (MissingParameterError& e) {
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
        }
    }
    else {
        QTextStream(stdout) << "Method " << method << " is unknown" << endl;
        response.error.message = QString("No such method: %1").arg(method);
    }

    // TODO QList<int> getStatsForQuestion(const QString& questionId);

    return true;
}
