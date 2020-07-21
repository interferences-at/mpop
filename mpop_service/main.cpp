/**
 * The MPOP service.
 *
 * Provides an API to access and edit the data for the MPOP project.
 */

#include <QtCore/QCoreApplication>
#include <QProcessEnvironment>
#include "mpopservice.h"
#include "config.h"
#include <QDebug>


/**
 * @brief Converts a string to a boolean.
 * @param value String to convert
 *
 * Values evaluated as true include: "True", "TRUE", "true", "1".
 * Otherwise, it's false.
 *
 * @return True if it's evaluated as true;
 */
bool toBoolean(const QString& value) {
    bool ret = false;
    if (value.toLower() == "true" || value.toInt() == 1) {
        ret = true;
    }
    // I think we could also simply do:
    // return QVariant(value).toBool();
    return ret;
}

/**
 * @brief Load configuration options from environment variables.
 * @param config The Config struct to populate.
 */
void load_config_from_env_vars(Config& config) {
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    config.mysql_port = env.value("CONFIG_MYSQL_PORT", "3306").toUInt();
    config.mysql_user = env.value("CONFIG_MYSQL_USER", "admin");
    config.mysql_database = env.value("CONFIG_MYSQL_DATABASE", "mpop_database");
    config.mysql_password = env.value("CONFIG_MYSQL_PASSWORD", "secret");
    config.mysql_host = env.value("CONFIG_MYSQL_HOST", "0.0.0.0"); // Use 'db' as a hostname, if you use within Docker Compose
    config.service_port_number = env.value("MPOP_SERVICE_PORT_NUMBER", "3333").toUInt();
    config.use_sqlite = toBoolean(env.value("CONFIG_USE_SQLITE", "false"));
    config.is_verbose = toBoolean(env.value("CONFIG_IS_VERBOSE", "true"));
    if (config.is_verbose) {
        qDebug() << "mysql_port:" << config.mysql_port;
        qDebug() << "mysql_user:" << config.mysql_user;
        qDebug() << "mysql_database:" << config.mysql_database;
        qDebug() << "mysql_password:" << config.mysql_password;
        qDebug() << "mysql_host:" << config.mysql_host;
        qDebug() << "service_port_number:" << config.service_port_number;
        qDebug() << "use_sqlite:" << config.use_sqlite;
        qDebug() << "is_verbose:" << config.is_verbose;

    }
}


int main(int argc, char *argv[])
{
    QCoreApplication application(argc, argv);
    application.setApplicationName("mpop_service");
    Config config;
    load_config_from_env_vars(config);
    MPopService service(config);
    return application.exec();
}
