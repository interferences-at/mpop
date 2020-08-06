#ifndef RESPONSE_H
#define RESPONSE_H

#include <QString>
#include <QList>
#include <QVariant>
#include "request.h"


struct Response
{
public:
    struct Error {
        int code = 0;
        QString message = "";
        QVariant data;
    };
    QString jsonrpc = "2.0";
    QVariant result;
    Error error;
    QString stringId = "";
    int intId = 0;
    typedef enum { NUMBER_ID = 0, STRING_ID = 1, NULL_ID = 3} ResponseIdType;
    ResponseIdType idType;

    void copyIdFromRequest(const Request& request);

public: // methods
    static Response fromString(const QString& str);
    QString toString() const;
};


#endif // RESPONSE_H
