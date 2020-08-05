#include "languageswitcher.h"
#include <QApplication> // for the qApp macro
#include <QDebug>

// Values for the constants:
const QString LanguageSwitcher::translationFileEnglish = "mpop_kiosk_en";
const QString LanguageSwitcher::translationFileFrench = "mpop_kiosk_fr";
const QString LanguageSwitcher::translationDirectory = ".";
const QString LanguageSwitcher::languageCodeFrench = "fr";
const QString LanguageSwitcher::languageCodeEnglish = "en";


LanguageSwitcher::LanguageSwitcher(QObject *parent) : QObject(parent)
{
    this->_language = "en";
}


void LanguageSwitcher::setLanguage(const QString& value) {
    bool is_loaded = false;
    qDebug() << "Current directory is : " << qApp->applicationDirPath();
    if (value == languageCodeFrench) {
        qDebug() << "Loading translation file" << translationFileFrench << "from dir" << translationDirectory;
        is_loaded = _translatorFrench->load(translationFileFrench);
        // TODO: If already loaded, avoid to reload it
        if (is_loaded) {
            qDebug() << "installTranslator";
            qApp->installTranslator(_translatorFrench);
        } else {
            qDebug() << "Could not load the translation file.";
        }
    } else if (value == languageCodeEnglish) {
        qDebug() << "Loading translation file" << translationFileEnglish << "from dir" << translationDirectory;
        is_loaded = _translatorEnglish->load(translationFileEnglish);
        // TODO: If already loaded, avoid to reload it

        if (is_loaded) {
            qDebug() << "installTranslator";
            qApp->installTranslator(_translatorEnglish);
        } else {
            qDebug() << "Could not load the translation file.";
        }
    }
    if (is_loaded) {
        this->_language = value;
        emit languageChanged();
    }
}
