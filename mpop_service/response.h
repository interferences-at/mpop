#ifndef RESPONSE_H
#define RESPONSE_H

#include <QString>
#include <QList>
#include <QVariant>

struct Response
{
public:
    struct Error {
        int code = 0;
        QString message = "";
        QVariant data;
    };
    QString jsonrpc = "2.0";
    QString method = "";
    //QList<QVariant> result; // FIXME: should contain any JSON object
    QVariant result;
    Error error;
    QString id = "";

public: // methods
    static Response fromString(const QString& str);
    QString toString();
};


#endif // RESPONSE_H
