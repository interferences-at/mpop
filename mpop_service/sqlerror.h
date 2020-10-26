#pragma once
#include <QException>
#include <QString>


/**
 * @brief Generic error about a my sql query execution.
 */
class SQLError : public QException
{
public:
    SQLError(const QString& errorMsg) {
        this->errorMsg = errorMsg;
        this->message = QString("Internal Server Error : %1.");
        message = message.arg(this->errorMsg);
    }
    void raise() const override {
        throw *this;
    }
    SQLError* clone() const override {
        return new SQLError(*this);
    }

    virtual const char* what() const noexcept override {

        return qPrintable(message);
    }
protected:
    QString errorMsg;
    QString message;
};

