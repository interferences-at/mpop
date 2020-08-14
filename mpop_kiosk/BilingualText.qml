import QtQuick 2.0

/**
 * Provides a text string that changes dynamically
 * according to the currently selected language.
 *
 * You can then create aliases to its "text" property,
 * so that your UI is updated automatically.
 */
Item {
    property string textFr: ""
    property string textEn: ""
    property string language: "fr"
    property string text: ""

    Component.onCompleted: {
        text = language == "fr" ? textFr : textEn;
    }

    onLanguageChanged: {
        text = language == "fr" ? textFr : textEn;
    }

    onTextEnChanged: {
        if (language == "en") {
            text = textEn;
        }
    }

    onTextFrChanged: {
        if (language == "fr") {
            text = textFr;
        }
    }
}
