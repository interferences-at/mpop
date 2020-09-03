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


int main(int argc, char *argv[])
{
    QCoreApplication application(argc, argv);
    application.setApplicationName("mpop_service");
    Config config;
    MPopService::load_config_from_env_vars(config);
    MPopService service(config);
    QTimer mytimer;
    QObject::connect(&mytimer,&QTimer::timeout, std::bind(MPopService::timeWatcher, config));
    mytimer.start(config.periodic_interval);

    return application.exec();
}
