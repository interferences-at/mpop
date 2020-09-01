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
    }
    void raise() const override {
        throw *this;
    }
    SQLError* clone() const override {
        return new SQLError(*this);
    }

    virtual const char* what() const noexcept override {
        QString message("Internal Server Error : %1.");
        message = message.arg(this->errorMsg);
        return message.toStdString().c_str();
    }
protected:
    QString errorMsg;
};

