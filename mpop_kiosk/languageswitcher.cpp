#include "languageswitcher.h"
#include <QApplication> // for the qApp macro
#include <QDebug>

// Values for the constants:
const QString LanguageSwitcher::translationFileEnglish = "mpop_kiosk_en";
const QString LanguageSwitcher::translationFileFrench = "mpop_kiosk_fr";
const QString LanguageSwitcher::translationDirectory = ".";
const QString LanguageSwitcher::languageCodeFrench = "fr";
const QString LanguageSwitcher::languageCodeEnglish = "en";


LanguageSwitcher::LanguageSwitcher(QObject *parent) : QObject(parent), _translatorEnglish(), _translatorFrench()
{
    this->_language = "en";
}


void LanguageSwitcher::setLanguage(const QString& value) {
    static const QLocale fr(QLocale::French);
    static const QLocale en(QLocale::English);

    bool is_loaded = false;
    qDebug() << "Current directory is : " << qApp->applicationDirPath();
    if (value == languageCodeFrench) {
        qDebug() << "Loading translation file" << translationFileFrench << "from dir" << translationDirectory;
        is_loaded = _translatorFrench.load(fr, translationFileFrench);
        // TODO: If already loaded, avoid to reload it
        if (is_loaded) {
            qDebug() << "installTranslator";
            qApp->installTranslator(& _translatorFrench);
        } else {
            qDebug() << "Could not load the translation file." << translationFileFrench;
        }
    } else if (value == languageCodeEnglish) {
        qDebug() << "Loading translation file" << translationFileEnglish << "from dir" << translationDirectory;
        is_loaded = _translatorEnglish.load(en, translationFileEnglish);
        // TODO: If already loaded, avoid to reload it

        if (is_loaded) {
            qDebug() << "installTranslator";
            qApp->installTranslator(& _translatorEnglish);
        } else {
            qDebug() << "Could not load the translation file." << translationFileEnglish;
        }
    }
    if (is_loaded) {
        this->_language = value;
        emit languageChanged();
    }
}
