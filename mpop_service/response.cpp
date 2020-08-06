#include "response.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <string>

Response Response::fromString(const QString& str) {
    // Unused
    Response ret;
    QByteArray byteArray = str.toUtf8();
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(byteArray, &error);
    //if (doc == nullptr) {
    //    qDebug << "Error parsing JSON: " << error.errorString();
    //} else {
    // TODO: catch exceptions
    QVariantMap map = doc.toVariant().toMap();
    ret.id = map["id"].toString();
    ret.result = map["result"].toMap();
    // TODO: parse the error structure as well
    return ret;
}

QString Response::toString() {
    QVariantMap map;

    map["id"] = this->id;
    if (this->error.message != "") {
        map["error"] = QVariantMap();
        map["error"].toMap()["message"] = QVariant::fromValue(QString(this->error.message));
        map["error"].toMap()["code"] = QVariant::fromValue(this->error.code);
        map["error"].toMap()["data"] = this->error.data;
    } else {
        map["result"] = this->result;
    }
    QJsonDocument doc = QJsonDocument::fromVariant(QVariant(map));
    QByteArray byteArray = doc.toJson(QJsonDocument::Compact);
    QString ret = QString::fromStdString(byteArray.toStdString());
    return ret;
}
