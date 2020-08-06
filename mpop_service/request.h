#ifndef REQUEST_H
#define REQUEST_H

#include <QString>
#include <QList>
#include <QVariant>
#include "missingparametererror.h"


/**
 * Check that a map has a given key
 */
bool mapHasKey(const QVariantMap& map, const QString& key);


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
    QString stringId;
    int intId;

    typedef enum { NUMBER_ID = 0, STRING_ID = 1, NULL_ID = 3} RequestIdType;
    RequestIdType idType;
public: // methods
    static Request fromString(const QString& str);
    QString toString() const;

    // Might throw a MissingParameterByName exception
    QVariant getParamByName(const QString& name) const;

    // Might throw a MissingParameterByPosition exception
    QVariant getParamByPosition(int position) const;
};

#endif // REQUEST_H
