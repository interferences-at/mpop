#ifndef KIOSKCONFIG_H
#define KIOSKCONFIG_H

#include <Qt>
#include <QString>
#include <QObject>


/**
 * @brief Configuration options for the kiosk.
 */
class KioskConfig : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString service_host READ get_service_host CONSTANT)
    Q_PROPERTY(quint16 service_port_number READ get_service_port_number CONSTANT)
    Q_PROPERTY(bool is_verbose READ get_is_verbose CONSTANT)
public:
    KioskConfig();

public: // attributes
    QString send_osc_dataviz_host = "127.0.0.1";
    quint16 send_osc_dataviz_port = 14444;
    quint16 receive_osc_port = 14444;
    quint16 service_port_number = 3333;
    QString service_host = "0.0.0.0";
    bool is_verbose = false; // if true, print a lot of info

public: // methods
    quint16 get_service_port_number() const {
        return this->service_port_number;
    }

    QString get_service_host() const {
        return this->service_host;
    }

    bool get_is_verbose() const {
        return this->is_verbose;
    }
};

#endif // KIOSKCONFIG_H
