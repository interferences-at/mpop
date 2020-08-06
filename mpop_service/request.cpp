#include "request.h"
#include <QJsonDocument>
#include <string>


Request Request::fromString(const QString& str) {
    Request ret;
    QString method;
    QVariantList paramsByPosition;
    QVariantMap paramsByName;
    QString id;

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
    //if (map["params"].Size)
    ret.paramsByName = map["params"].toMap();
    // TODO: ret.paramsByPosition = map["params"].toList();
    //}

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
    map["id"] = this->id;
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
