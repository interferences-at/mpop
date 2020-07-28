#pragma once

#include <Qt>
#include <QString>

/**
 * @brief Configuration options for this program.
 */
struct Config {
public:
    QString mysql_database;
    QString mysql_host;
    QString mysql_password;
    quint16 mysql_port;
    QString mysql_user;
    quint16 service_port_number;
    bool is_verbose; // if true, print a lot of info
};
