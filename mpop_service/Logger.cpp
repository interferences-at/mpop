#include "Logger.h"
#include <QTime>
#include <QFile>
#include <QFileInfo>
#include <QDebug>
#include <QDir>
#include <QFileInfoList>
#include <iostream>

static QString logFileName;
static QString logFolderPath;

Logger::Logger()
{
    // set logfiles path.
    logFolderPath = "/var/log/";

    deleteOldLogs(); //delete old log files
    initLogFileName(); //create the logfile name

    QFile outFile(logFileName);
    if(outFile.open(QIODevice::WriteOnly | QIODevice::Append))
    {
        qInstallMessageHandler(Logger::myMessageHandler);
    }
}


void Logger::initLogFileName()
{

    logFileName = QString( logFolderPath + "mpop-service-%1.log")
            .arg(QDate::currentDate().toString("yyyy-MM-dd"));
}

/**
 * @brief deletes old log files, only the last ones are kept
 */
void Logger::deleteOldLogs()
{
    QDir dir;
    dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    dir.setSorting(QDir::Time | QDir::Reversed);
    dir.setPath(logFolderPath);

    QStringList nameFilter;
    nameFilter << "mpop-service" ;
    QFileInfoList list = dir.entryInfoList(nameFilter, QDir::Files);

    if (list.size() <= LOGFILES)
    {
        return; //no files to delete
    } else
    {
        for (int i = 0; i < (list.size() - LOGFILES); i++)
        {
            QString path = list.at(i).absoluteFilePath();
            QFile file(path);
            file.remove();
        }
    }
}


void Logger::myMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString& msg)
{
    QFile outFile(logFileName);
    outFile.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream ostream(&outFile);

    switch (type) {
    case QtDebugMsg:
        ostream << "Debugging Message:" << msg << endl;
        break;
    case QtInfoMsg:
        ostream << "Information Message:" << msg << endl;
        break;
    case QtWarningMsg:
        ostream << "Warnning Message:" << msg << endl;
        break;
    case QtFatalMsg:
        ostream << "Fatal Message: " << msg << endl;
        break;
    case QtCriticalMsg:
        ostream << "Critical Message: " << msg << endl;
        break;
    default:
        break;
    }
}

