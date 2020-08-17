import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

/**
 * Page that contains the widget to select my language.
 */
RowLayout {
    id: thisPage

    signal languageChosen(string value)

    property string lang: ""

    BilingualText {
        id: textChooseALanguage
        language: thisPage.lang
        textFr: "Choisir une langue"
        textEn: "Choose a language"
    }

    Rectangle {
        Layout.minimumWidth: 400
        Layout.minimumHeight: 500
        color: "black"

        Label {
            // FIXME: Shouldn't this text be in both languages?
            Layout.alignment: Qt.AlignCenter
            text: textChooseALanguage.text
            font.capitalization: Font.AllUppercase
            font.pixelSize: 36
        }
    }

    ListView {
        Layout.margins: 0
        Layout.topMargin: 10
        Layout.leftMargin: 80
        Layout.rightMargin: 50
        Layout.fillWidth: false
        Layout.fillHeight: true
        orientation: Qt.Vertical
        width: currentItem.width
        // This model contains only two items: fr and en.
        model: ListModel {
            ListElement {
                language_identifier: "fr"
                language_text: "Français"
            }
            ListElement {
                language_identifier: "en"
                language_text: "English"
            }
        }

        delegate: WidgetChoiceButton {
            text: language_text // Property of items in the model.
            height: parent.height / parent.count
            spacing: 0
            onClicked: thisPage.languageChosen(language_identifier)
        }
    }
}
