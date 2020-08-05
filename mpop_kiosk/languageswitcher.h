#ifndef LANGUAGESWITCHER_H
#define LANGUAGESWITCHER_H

#include <QObject>
#include <QTranslator>

/**
 * @brief Allows to dynamically switch the user interface from a language to another.
 *
 * Supports French and English.
 */
class LanguageSwitcher : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString language READ getLanguage WRITE setLanguage NOTIFY languageChanged)

public:
    explicit LanguageSwitcher(QObject *parent = nullptr);

private: // methods
    QString getLanguage() const {
        return this->_language;
    }

    void setLanguage(const QString& value);

signals:
 void languageChanged();

private: // attributes
    QTranslator* _translatorEnglish;
    QTranslator* _translatorFrench;
    QString _language;

    // constants:
    static const QString translationFileEnglish;
    static const QString translationFileFrench;
    static const QString translationDirectory;
    static const QString languageCodeFrench;
    static const QString languageCodeEnglish;
};

#endif // LANGUAGESWITCHER_H
