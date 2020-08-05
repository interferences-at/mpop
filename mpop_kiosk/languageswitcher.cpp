#include "languageswitcher.h"
#include <QApplication> // for the qApp macro
#include <QDebug>

// Values for the constants:
const QString LanguageSwitcher::translationFileEnglish = ":/mpop_kiosk_en";
const QString LanguageSwitcher::translationFileFrench = ":/mpop_kiosk_fr";
const QString LanguageSwitcher::translationDirectory = ".";
const QString LanguageSwitcher::languageCodeFrench = "fr";
const QString LanguageSwitcher::languageCodeEnglish = "en";


LanguageSwitcher::LanguageSwitcher(QObject *parent) : QObject(parent), _translatorEnglish(), _translatorFrench()
{
    this->setLanguage("en");
    bool is_loaded = _translatorFrench.load(QLocale(QLocale::French), translationFileFrench);
    if (is_loaded) {
        qDebug() << "Loaded translation file." << translationFileFrench;
    } else {
        qDebug() << "Could not load the translation file." << translationFileFrench;
    }
    is_loaded = _translatorEnglish.load(QLocale(QLocale::English), translationFileEnglish);
    if (is_loaded) {
        qDebug() << "Loaded translation file." << translationFileEnglish;
    } else {
        qDebug() << "Could not load the translation file." << translationFileEnglish;
    }
}


void LanguageSwitcher::setLanguage(const QString& value) {
    static QLocale frenchLocale(QLocale::French);
    static QLocale englishLocale(QLocale::English);

    if (this->_language == value) {
        qDebug() << "We are already using this language." << value;
        return;
    }

    bool is_loaded = false;
    QLocale* newLocale = NULL;
    QTranslator* newTranslator = NULL;
    QTranslator* oldTranslator = NULL;

    // Setup the variables:
    if (value == languageCodeFrench) {
        newLocale = &frenchLocale;
        newTranslator = &_translatorFrench;
        oldTranslator = &_translatorEnglish;
    } else if (value == languageCodeEnglish) {
        newLocale = &englishLocale;
        newTranslator = &_translatorEnglish;
        oldTranslator = &_translatorFrench;
    } else {
        qDebug() << "We don't support that language code:" << value;
        return;
    }

    // Do the switch:
    bool is_removed = qApp->removeTranslator(oldTranslator);
    qDebug() << "Removed previous translator" << is_removed;

    // Install the new translator
    qDebug() << "installTranslator";
    is_loaded = qApp->installTranslator(newTranslator);
    if (is_loaded) {
        qDebug() << "Successfully installed the new translator.";
        QLocale::setDefault(*newLocale);
    } else {
        qDebug() << "Failed to install the new translator.";
    }

    if (is_loaded) {
        this->_language = value;
        emit languageChanged();
    }
}
