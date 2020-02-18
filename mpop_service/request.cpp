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

QString Request::toString() {
    QVariantMap map;
    map["method"] = this->method;
    if (! this->paramsByName.empty()) { //FIXME: can we have both?
        map["params"] = this->paramsByName;
    } else {
        map["params"] = this->paramsByPosition;
    }
    map["id"] = this->id;
    QJsonDocument doc = QJsonDocument::fromVariant(map);
    QString ret = QString(doc.toBinaryData().constData());
    return ret;
}
