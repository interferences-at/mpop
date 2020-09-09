#ifndef LOGUTILS_H
#define LOGUTILS_H


#define LOGSIZE 1024 * 100 //log size in bytes
#define LOGFILES 5 // keep recent 5 logs file

#include <QObject>
#include <QString>
#include <QDebug>

/**
 * @brief The Logutils class implements logs logic for this application.
 *
 * Different API can call the methods of the single instance of this object, if needed.
 */
class Logger : public QObject
{
    Q_OBJECT
public:
   /**
    * @brief Constructor.
    * set File Path and install qMessageHandler.
    */
    explicit Logger();

    /**
     * @brief myMessageHandler to write logs for different types.
     * @param type
     * @param context
     * @param msg
     */
    static void myMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString& msg);

    /**
     * @brief deleteOldLogs
     * delete oldlogs file except recent ones.
     */
    static void deleteOldLogs();

    /**
     * @brief initLogFileName
     * Intialize Logfile name with current date format (YYYY-MM-DD)
     */
    static void initLogFileName();
signals:
    // No signals

};

#endif // LOGUTILS_H
