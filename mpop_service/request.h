#ifndef REQUEST_H
#define REQUEST_H

#include <QString>
#include <QList>
#include <QVariant>
#include "missingparametererror.h"

/**
 * @brief A JSON-RPC 2.0 request received.
 */
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
    QString toString() const;

    // Might throw a MissingParameterByName exception
    QVariant getParamByName(const QString& name) const;

    // Might throw a MissingParameterByPosition exception
    QVariant getParamByPosition(int position) const;
};

#endif // REQUEST_H
