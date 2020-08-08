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

    if (mapHasKey(map, "id")) {
        if (map["id"].type() == QVariant::Int) {
            ret.intId = map["id"].toInt();
            ret.idType = Response::ResponseIdType::NUMBER_ID;
        } else if (map["id"].type() == QVariant::String) {
            ret.stringId = map["id"].toString();
            ret.idType = Response::ResponseIdType::STRING_ID;
        } else {
            ret.idType = Response::ResponseIdType::NULL_ID;
        }
    } else {
        ret.idType = Response::ResponseIdType::NULL_ID;
    }

    ret.result = map["result"].toMap();
    // TODO: parse the error structure as well
    return ret;
}

void Response::copyIdFromRequest(const Request& request) {
    // The ID might be a number:
    if (request.idType == Request::NUMBER_ID) {
        this->idType = Response::NUMBER_ID;
        this->intId = request.intId;

    // Or it might be a string:
    } else if (request.idType == Request::STRING_ID) {
        this->idType = Response::STRING_ID;
        this->stringId = request.stringId;

    // Or maybe there is no ID at all:
    } else if (request.idType == Request::NULL_ID) {
        this->idType = Response::NULL_ID;
    }
}

QString Response::toString() const {
    QVariantMap map;

    // TODO: Create a base class for Response and Request, to avoid duplicate code arount the id type.
    if (this->idType == Response::ResponseIdType::NULL_ID) {
        // TODO: provide null id
    } else if (this->idType == Response::ResponseIdType::STRING_ID) {
        map["id"] = QVariant::fromValue(this->stringId); // string
    } else if (this->idType == Response::ResponseIdType::NUMBER_ID) {
        map["id"] = QVariant::fromValue(this->intId); // int
    }

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
