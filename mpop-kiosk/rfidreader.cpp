#include "rfidreader.h"
#include <QCoreApplication>
#include <QCoreApplication>
#include <QSerialPort>
#include <QStringList>
#include <QTextStream>
#include <QSerialPortInfo>
#include <QTextStream>

static const int SERIAL_READ_INTERVAL_MS = 30;
static const int SERIAL_REOPEN_INTERVAL_MS = 1000 * 60;


RFIDReader::RFIDReader(QObject* parent) :
    QObject(parent),
    _serialPort(),
    _standardOutput(stdout) {

    this->resetSerialPort();

    connect(&this->_readTimer, &QTimer::timeout, this, &RFIDReader::readTimerCb);
    this->_readTimer.start(SERIAL_READ_INTERVAL_MS);

    connect(&this->_reopenTimer, &QTimer::timeout, this, &RFIDReader::reopenTimerCb);
    this->_readTimer.start(SERIAL_REOPEN_INTERVAL_MS);
}


void RFIDReader::resetSerialPort() {
    if (this->_serialPort.isOpen()) {
        this->_serialPort.close();
    }

    const QString serialPortName = this->guessSerialPortName();
    if (serialPortName.isEmpty()) {
        this->_standardOutput << "No serial port to open." << endl;
    } else {
        this->_serialPort.setPortName(serialPortName);

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
            // TODO: try again
        }
    }
}

QString RFIDReader::guessSerialPortName() {
    QString ret = ""; // Returns an empty string if none found.
    bool verbose = true;

    QStringList expected = {
        "/dev/ttyUSB", // on GNU/Linux
        "/dev/cu.usbserial" // on macOS
        // TODO: COM1 on Windows
    };

    const auto serialPortInfos = QSerialPortInfo::availablePorts();
    QTextStream out(stdout);
    out << "Total number of ports available: " << serialPortInfos.count() << endl;

    static const QString blankString = "N/A";
    QString description;
    QString manufacturer;
    QString serialNumber;

    for (const QSerialPortInfo &serialPortInfo : serialPortInfos) {
        description = serialPortInfo.description();
        manufacturer = serialPortInfo.manufacturer();
        serialNumber = serialPortInfo.serialNumber();
        if (verbose) {
            out << endl;
            out << "Port: " << serialPortInfo.portName() << endl;
        }
        out << "Location: " << serialPortInfo.systemLocation() << endl;
        if (verbose) {
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
        for (int i = 0; i < expected.size(); ++ i) {
            QString item = expected.at(i).toLocal8Bit().constData();
            QString systemLocation = serialPortInfo.systemLocation();
            if (systemLocation.contains(item, Qt::CaseInsensitive)) {
                out << "Found FTDI device!" << endl;
                ret = serialPortInfo.systemLocation();
            }
        }
    }

    return ret;
}

void RFIDReader::handleReadyRead() {
    // TODO: read lines
    // TODO: tag = line.replace(/[\W_]+/g, ''); // Remove characters that are not word-characters
    // TODO: trigger signal with the RFID tag
    _readData.append(_serialPort.readAll());
    if (! this->_readTimer.isActive()) {
        this->_readTimer.start(SERIAL_READ_INTERVAL_MS);
    }
}

void RFIDReader::readTimerCb() {
    if (this->_readData.isEmpty()) {
        this->_standardOutput << QObject::tr(
            "No data was currently available for reading from port %1")
            .arg(this->_serialPort.portName())
            << endl;
    } else {
        this->_standardOutput <<
            QObject::tr("Data successfully received from port %1")
            .arg(this->_serialPort.portName())
            << endl;
        this->_standardOutput << this->_readData << endl;
    }
    QCoreApplication::quit();
}

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
