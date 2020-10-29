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
    Q_PROPERTY(QString kiosk_mode READ get_kiosk_mode CONSTANT)
    Q_PROPERTY(bool is_fullscreen READ get_is_fullscreen CONSTANT)
    Q_PROPERTY(bool show_cursor READ get_show_cursor CONSTANT)
public:
    KioskConfig();

public: // attributes
    QString send_osc_dataviz_host;
    quint16 send_osc_dataviz_port;
    quint16 receive_osc_port;
    quint16 service_port_number;
    QString service_host;
    QString kiosk_mode; // either "entrance", "central" or "exit"
    bool is_fullscreen;
    bool is_verbose; // if true, print a lot of info
    bool show_cursor = false;

public: // static methods:
    static bool toBoolean(const QString& value) {
        bool ret = false;
        if (value.toLower() == "true" || value.toInt() == 1) {
            ret = true;
        }
        // I think we could also simply do:
        // return QVariant(value).toBool();
        return ret;
    };

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

    QString get_kiosk_mode() const {
        return this->kiosk_mode;
    }

    bool get_is_fullscreen() const {
        return this->is_fullscreen;
    }

    bool get_show_cursor() const {
        return this->show_cursor;
    }
};

#endif // KIOSKCONFIG_H
