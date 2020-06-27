#include "rfidreader.h"
#include <QCoreApplication>
#include <QCoreApplication>
#include <QSerialPort>
#include <QStringList>
#include <QTextStream>
#include <QSerialPortInfo>
#include <QTextStream>
#include <QDebug>


static const int SERIAL_READ_INTERVAL_MS = 30;
static const int SERIAL_REOPEN_INTERVAL_MS = 1000 * 60; // Every minute, attempt to open the USB RFID reader if it's not open.


RFIDReader::RFIDReader(QObject* parent) :
    QObject(parent),
    _serialPort(),
    _standardOutput(stdout) {

    this->resetSerialPort();

    connect(&this->_readTimer, &QTimer::timeout, this, &RFIDReader::readTimerCb);
    this->_readTimer.start(SERIAL_READ_INTERVAL_MS);

    connect(&this->_reopenTimer, &QTimer::timeout, this, &RFIDReader::reopenTimerCb);
    this->_reopenTimer.start(SERIAL_REOPEN_INTERVAL_MS);
}


void RFIDReader::resetSerialPort() {
    if (this->_serialPort.isOpen()) {
        this->_serialPort.close();
    }

    const QString serialPortName = this->guessSerialPortName();
    if (serialPortName.isEmpty()) {
        this->_standardOutput << "No serial port to open." << endl;
    } else {
        qDebug() << "Open" << serialPortName << "at 9600 bauds";
        this->_serialPort.setPortName(serialPortName);

        // The device is 9600 bauds
        const int serialPortBaudRate = QSerialPort::Baud9600;
        this->_serialPort.setBaudRate(serialPortBaudRate);

        bool isOpen = this->_serialPort.open(QIODevice::ReadOnly);
        if (isOpen) {
            connect(&this->_serialPort, &QSerialPort::readyRead, this, &RFIDReader::handleReadyRead);
            connect(&this->_serialPort, &QSerialPort::errorOccurred, this, &RFIDReader::handleError);
        } else {
            this->_standardOutput <<
                QObject::tr("Failed to open port %1, error: %2")
                    .arg(serialPortName)
                    .arg(this->_serialPort.errorString())
                << endl;
            // Note: It will try again in a few seconds.
        }
    }
}

QString RFIDReader::guessSerialPortName() {
    QString ret = ""; // Returns an empty string if none found.
    bool verbose = true;
    bool veryVerbose = false;

    // List of file name patterns for the serial ports to open.
    static const QStringList serialPortExpectedFileNamePatterns = {
        "/dev/ttyUSB", // on GNU/Linux
        "/dev/cu.usbserial", // on macOS
        "/dev/tty.usbserial" // on macOS
        // TODO: COM1 on Windows
    };

    // List all the available serial ports on the system:
    const auto serialPortInfos = QSerialPortInfo::availablePorts();
    QTextStream out(stdout);
    if (verbose) {
        out << "Total number of serial ports available: " << serialPortInfos.count() << endl;
    }

    static const QString blankString = "N/A";
    QString description;
    QString manufacturer;
    QString serialNumber;

    for (const QSerialPortInfo &serialPortInfo : serialPortInfos) {
        description = serialPortInfo.description();
        manufacturer = serialPortInfo.manufacturer();
        serialNumber = serialPortInfo.serialNumber();
        if (veryVerbose) {
            out << endl;
            out << "Port: " << serialPortInfo.portName() << endl;
        }
        if (verbose) {
            out << "Location: " << serialPortInfo.systemLocation() << endl;
        }
        if (veryVerbose) {
            out << "Description: " << (! description.isEmpty() ? description : blankString) << endl;
            out << "Manufacturer: " << (! manufacturer.isEmpty() ? manufacturer : blankString) << endl;
            out << "Serial number: " << (! serialNumber.isEmpty() ? serialNumber : blankString) << endl;
            out << "Vendor Identifier: " << (serialPortInfo.hasVendorIdentifier()
                                             ? QByteArray::number(serialPortInfo.vendorIdentifier(), 16)
                                             : blankString) << endl;
            out << "Product Identifier: " << (serialPortInfo.hasProductIdentifier()
                                              ? QByteArray::number(serialPortInfo.productIdentifier(), 16)
                                              : blankString) << endl;
            out << "Busy: " << (serialPortInfo.isBusy() ? "Yes" : "No") << endl;
        }
        for (int i = 0; i < serialPortExpectedFileNamePatterns.size(); ++ i) {
            QString item = serialPortExpectedFileNamePatterns.at(i).toLocal8Bit().constData();
            QString systemLocation = serialPortInfo.systemLocation();
            if (verbose) {
                out << "Check if" << systemLocation << "contains the string" << item;
            }
            if (systemLocation.contains(item, Qt::CaseInsensitive)) {
                if (veryVerbose) {
                    out << "yes";
                }
                if (verbose) {
                    out << "Found FTDI device: " << serialPortInfo.systemLocation() << endl;
                }
                ret = serialPortInfo.systemLocation();
            } else {
                if (veryVerbose) {
                    out << "no";
                }
            }
        }
    }

    return ret;
}

QString RFIDReader::getLastRfidRead() const {
    return this->_lastRfidRead;
}

void RFIDReader::handleReadyRead() {
    // TODO: read lines
    // TODO: tag = line.replace(/[\W_]+/g, ''); // Remove characters that are not word-characters
    // TODO: trigger signal with the RFID tag
    // this->_readData.append(_serialPort.readAll());
    // if (! this->_readTimer.isActive()) {
    //     this->_readTimer.start(SERIAL_READ_INTERVAL_MS);
    // }
    if (this->_serialPort.canReadLine()) {
        static const qint64 maxSize = 100;
        QByteArray line;
        line = this->_serialPort.readLine(maxSize);
        QString tag = QString::fromUtf8(line.data()).trimmed();

        // Replace some undesired characters that might occur.
        tag = tag.replace(QString(u8"\u0002"), QString(""));
        tag = tag.replace(QString(u8"\u0003"), QString(""));

        qDebug() << "Read" << tag;

        emit tagRead(tag); // FIXME
        this->_lastRfidRead = tag;
    }
}


/**
 * @brief Periodically attempts to read from the USB RFID reader.
 */
void RFIDReader::readTimerCb() {
    // qDebug() << "readtimer";
    if (this->_serialPort.isOpen()) {
        // qDebug() << "check for new data";
        bool hasNewData = this->_serialPort.waitForReadyRead(10);
        if (hasNewData) {
            qDebug() << "Has new data";
        }
    }

//    if (this->_readData.isEmpty()) {
//        this->_standardOutput << QObject::tr(
//                                     "No data was currently available for reading from port %1")
//                                 .arg(this->_serialPort.portName())
//                              << endl;
//    } else {
//        this->_standardOutput <<
//                                 QObject::tr("Data successfully received from port %1")
//                                 .arg(this->_serialPort.portName())
//                              << endl;
//        this->_standardOutput << this->_readData << endl;
//    }
}


/**
 * @brief Periodically checks if we need to try to reconnect to the USB RFID reader.
 */
void RFIDReader::reopenTimerCb() {
    if (! this->_serialPort.isOpen()) {
        this->resetSerialPort();
    }
}


void RFIDReader::handleError(QSerialPort::SerialPortError serialPortError) {
    if (serialPortError == QSerialPort::ReadError) {
        this->_standardOutput <<
            QObject::tr("An I/O error occurred while reading data from port %1, error: %2")
                .arg(this->_serialPort.portName())
                .arg(this->_serialPort.errorString())
            << endl;
        // TODO: handle error
    }
}
