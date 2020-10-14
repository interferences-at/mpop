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
#ifndef CHATSERVER_H
#define CHATSERVER_H

#include <QtCore/QObject>
#include <QtCore/QList>
#include "facade.h"
#include "request.h"
#include "response.h"
#include "config.h"


QT_FORWARD_DECLARE_CLASS(QWebSocketServer)
QT_FORWARD_DECLARE_CLASS(QWebSocket)
QT_FORWARD_DECLARE_CLASS(QString)


class MPopService : public QObject
{
    Q_OBJECT
public:
    explicit MPopService(const Config& config, QObject* parent = nullptr);
    ~MPopService() override;

    /**
     * @brief Load configuration options from environment variables.
     * @param config The Config struct to populate.
     */
    static void load_config_from_env_vars(Config& config);

    /**
     * @brief Converts a string to a boolean.
     * @param value String to convert
     *
     * Values evaluated as true include: "True", "TRUE", "true", "1".
     * Otherwise, it's false.
     *
     * Useful for parsing the environment variables.
     *
     * @return True if it's evaluated as true;
     */
    static bool toBoolean(const QString& value);

    /**
     * @brief called every 1 min to check provided time in config is reach.
     * @param config The Config struct to populate.
     */
    static void timeWatcher(const Config& config);

    static QVariantMap stringIntMapToQVariantMap(const QMap<QString, int>& value) {
        QVariantMap ret;
        for (auto iter = value.constBegin(); iter != value.constEnd(); ++ iter) {
            ret[iter.key()] = QVariant(iter.value());
        }
        return ret;
    }


    static QVariantMap stringFloatMapToQVariantMap(const QMap<QString, float>& value) {
        QVariantMap ret;
        for (auto iter = value.constBegin(); iter != value.constEnd(); ++ iter) {
            ret[iter.key()] = QVariant(iter.value());
        }
        return ret;
    }

    static QVariantList intListToQvariantList(const QList<int>& value){

        QVariantList ret;

        foreach(int v, value){
          ret << v;
        }
        return ret;
    }

private slots:
    void newConnectionCb();
    void textMessageReceivedCb(const QString &message);
    void socketDisconnectedCb();

private: // methods
    QString handleJsonRpcTwoMethod(const QString& message, bool& broadcastNotification);
    bool handleFacadeMethod(const Request& request, Response& response);
private:
    QWebSocketServer* m_pWebSocketServer;
    QList<QWebSocket*> m_clients;
    Facade _facade;
    const Config& _config;
};

#endif
