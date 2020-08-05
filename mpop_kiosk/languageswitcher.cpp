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
    if (value == languageCodeFrench) {
        qDebug() << "Loading translation file" << translationFileFrench;
       _translatorFrench->load(translationFileFrench, translationDirectory);
       qApp->installTranslator(_translatorFrench);
    } else if (value == languageCodeEnglish) {
        qDebug() << "Loading translation file" << translationFileEnglish;
        _translatorFrench->load(translationFileEnglish, translationDirectory);
        qApp->installTranslator(_translatorEnglish);
    }
    emit languageChanged();
}
