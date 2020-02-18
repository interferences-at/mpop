#include "response.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <string>

Response Response::fromString(const QString& str) {
    Response ret;
    // TODO

    return ret;
}

QString Response::toString() {
    QVariantMap map;
    map["method"] = this->method;
    map["result"] = this->result;
    map["id"] = this->id;
    QJsonDocument doc = QJsonDocument::fromVariant(QVariant(map));
    QString ret = QString(doc.toBinaryData().constData());

    QJsonObject obj;
    obj["method"] = this->method;
    obj["result"] = QJsonValue::fromVariant(this->result);
    obj["id"] = this->id;
    // QString ret = obj.


    //QJsonObject obj = QJsonObject::frm
    return ret;
}
