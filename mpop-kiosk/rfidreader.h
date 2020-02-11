#ifndef RFIDREADER_H
#define RFIDREADER_H

#include <QObject>
#include <QByteArray>
#include <QSerialPort>
#include <QTextStream>
#include <QTimer>

class RFIDReader : public QObject
{
    Q_OBJECT

public:
    explicit RFIDReader(QObject* parent = nullptr);

private slots:
    void handleReadyRead();
    void readTimerCb();
    void reopenTimerCb();
    void handleError(QSerialPort::SerialPortError error);

private:
    void resetSerialPort();
    QString guessSerialPortName();

private:
    QSerialPort _serialPort;
    QByteArray _readData;
    QTextStream _standardOutput;
    QTimer _readTimer;
    QTimer _reopenTimer;

signals:

};

#endif // RFIDREADER_H
