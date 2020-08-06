#include "request.h"
#include <QJsonDocument>
#include <string>
#include <QDebug>


bool mapHasKey(const QVariantMap& map, const QString& key) {
    return (map.constFind(key) != map.constEnd());
}


Request Request::fromString(const QString& str) {
    Request ret;
    QString method;
    QVariantList paramsByPosition;
    QVariantMap paramsByName;
    QString id;

    qDebug() << "Request is " << str;

    QByteArray byteArray = str.toUtf8();
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(byteArray, &error);
    if (QJsonParseError::NoError != error.error) {
        qWarning() << "ERROR: Parsing json data: " << error.errorString();
        // TODO: throw parse error
    } else {

        //if (doc == nullptr) {
        //    qDebug << "Error parsing JSON: " << error.errorString();
        //} else {
        // TODO: catch exceptions
        QVariantMap map = doc.toVariant().toMap();

        // id can be a string or an int:
        if (mapHasKey(map, "id")) {
            if (map["id"].type() == QVariant::Int) {
                ret.intId = map["id"].toInt();
                ret.idType = Request::RequestIdType::NUMBER_ID;
                qDebug() << "Request id is a number and its value is " << ret.intId;
            } else if (map["id"].type() == QVariant::Double) {
                ret.intId = map["id"].toInt(); // JSON-RPC 2.0 supports int id, but not double.
                ret.idType = Request::RequestIdType::NUMBER_ID;
                qDebug() << "Request id is a number and its value is " << ret.intId;
            } else if (map["id"].type() == QVariant::String) {
                ret.stringId = map["id"].toString();
                ret.idType = Request::RequestIdType::STRING_ID;
                qDebug() << "Request id is a string and its value is " << ret.stringId;
            } else {
                ret.idType = Request::RequestIdType::NULL_ID;
                qDebug() << "Request id type is " << map["id"].typeName();
            }
        } else {
            ret.idType = Request::RequestIdType::NULL_ID;
            qDebug() << "Request has no id key.";
        }

        if (mapHasKey(map, "method")) {
            ret.method = map["method"].toString();
            qDebug() << "Request method name is " << ret.method;
        } else {
            qDebug() << "Request has no method key.";
        }

        // Params can be a list or a map:
        if (mapHasKey(map, "params")) {
            if (map["params"].type() == QVariant::List) {
                ret.paramsByPosition = map["params"].toList();
                qDebug() << "Request has a list of params.";
            } else if (map["params"].type() == QVariant::Map) {
                ret.paramsByName = map["params"].toMap();
                qDebug() << "Request has a map of params.";
            }
        } else {
            qDebug() << "Request has no params key.";
        }

    }

    return ret;
}

QString Request::toString() const {
    QVariantMap map;
    map["method"] = this->method;
    // Params are either a list, or a map.
    // JSON-RPC 2.0 support only either one, but not both at the same time.
    if (! this->paramsByName.empty()) {
        map["params"] = this->paramsByName; // a QVariantMap
    } else {
        map["params"] = this->paramsByPosition; // a QVariantList
    }
    if (this->idType == Request::RequestIdType::NULL_ID) {
        // TODO: provide null id
    } else if (this->idType == Request::RequestIdType::STRING_ID) {
        map["id"] = this->stringId; // string
    } else if (this->idType == Request::RequestIdType::NUMBER_ID) {
        map["id"] = this->intId; // int
    }

    QJsonDocument doc = QJsonDocument::fromVariant(map);
    QString ret = doc.toJson(QJsonDocument::Compact);
    // QString(doc.toBinaryData().constData());
    return ret;
}

QVariant Request::getParamByName(const QString& name) const {
    if (this->paramsByName.find(name) == this->paramsByName.end()) {
        throw MissingParamemeterByName(this->method, name);
    }
    return this->paramsByName[name];
}

QVariant Request::getParamByPosition(int position) const {
    if (this->paramsByPosition.size() < position) {
        throw MissingParamemeterByPosition(this->method, position);
    }
    return this->paramsByPosition[position];
}
