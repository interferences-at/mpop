#ifndef REQUEST_H
#define REQUEST_H

#include <QString>
#include <QList>
#include <QVariant>

struct Request
{
public:
    QString jsonrpc = "2.0";
    QString method;
    QVariantList paramsByPosition;
    QVariantMap paramsByName;
    QString id;
public: // methods
    static Request fromString(const QString& str);
    QString toString();
};

#endif // REQUEST_H
