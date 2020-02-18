#ifndef NOTIFICATION_H
#define NOTIFICATION_H


#include <QString>
#include <QList>
#include <QVariant>
#include "request.h"

struct Notification: public Request
{
public:
    QString id = "";
};
#endif // NOTIFICATION_H
