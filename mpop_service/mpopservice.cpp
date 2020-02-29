/****************************************************************************
**
** Copyright (C) 2016 Kurt Pattyn <pattyn.kurt@gmail.com>.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtWebSockets module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/
#include "mpopservice.h"

#include <QtWebSockets>
#include <exception>

#include <QtCore>
#include <QDebug>
#include <cstdio>

#include "notification.h"
#include <iostream>

QT_USE_NAMESPACE

static QString getIdentifier(QWebSocket* peer) {
    return QStringLiteral("%1:%2").arg(peer->peerAddress().toString(),
        QString::number(peer->peerPort()));
}

MPopService::MPopService(quint16 port, QObject* parent) :
        QObject(parent),
        m_pWebSocketServer(new QWebSocketServer(QStringLiteral("MPOP Service"), QWebSocketServer::NonSecureMode, this)),
        _facade("mpop")
    {
    if (m_pWebSocketServer->listen(QHostAddress::Any, port)) {
        QTextStream(stdout) << "Server listening on port " << port << '\n';
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
    //if (document == nullptr) {
    //    qDebug << "Could not parse JSON from JSON-RPC 2.0 message.";
    //}
    Response response;
    response.id = request.id; // FIXME: should allow string, int or null
    response.method = request.method;
    bool sendResponse = true;

    if (request.method == "message") {
        QTextStream(stdout) << "Got method message" << endl;
        //response.result = QVariant::fromValue(QString("pong"));
        QTextStream(stdout) << "Answer with pong" << endl;
        sendResponse = false;
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
        // success
    } else {
        QTextStream(stdout) << "unhandled request";
    }

    QString ret = response.toString(); // return string response (JSON)
    QTextStream(stdout) << "Response: " << ret;
    return ret;
}

bool MPopService::handleFacadeMethod(const Request& request, Response& response) {
    // Write to the response object.
    QString method = request.method;
    if (method == "getOrCreateUser") {
        QString rfidTag = request.paramsByPosition[0].toString();
        response.result = QVariant(this->_facade.getOrCreateUser(rfidTag));
// TODO int (const QString& rfidTag);
    }

    // TODO QString getUserLanguage(int userId);

    // TODO QMap<QString, int> getUserAnswers(int userId);
    // TODO void setUserAnswer(int userId, const QString& questionId, int value);
    // TODO QList<int> getStatsForQuestion(const QString& questionId);
    // TODO void freeTag(const QString& rfidTag);
    // TODO void freeUnusedTags();
    // TODO bool setUserLanguage(int userId, const QString& language);
    // TODO bool setUserGender(int userId, const QString& gender);
    // TODO QString getUserGender(int userId);

    return true;
}
