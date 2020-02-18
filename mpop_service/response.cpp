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
    ret.method = map["method"].toString();
    ret.result = map["result"].toMap();
    return ret;
}

QString Response::toString() {
    QVariantMap map;
    map["method"] = QVariant(this->method);
    map["result"] = this->result;
    map["id"] = this->id;
    QJsonDocument doc = QJsonDocument::fromVariant(QVariant(map));
    QByteArray byteArray = doc.toJson(QJsonDocument::Compact);
    QString ret = QString::fromStdString(byteArray.toStdString());
    return ret;
}
