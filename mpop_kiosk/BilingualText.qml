import QtQuick 2.0

/**
 * Provides a text string that changes dynamically
 * according to the currently selected language.
 *
 * You can then create aliases to its "text" property,
 * so that your UI is updated automatically.
 */
QtObject {
    property string textFr: ""
    property string textEn: ""
    property string language: window.lang
    property string text: language == "fr" ? textFr : textEn
}
