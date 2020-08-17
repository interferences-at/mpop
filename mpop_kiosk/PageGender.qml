import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

/**
 * Page that contains the widget to select my gender.
 */
RowLayout {
    id: thisPage

    signal genderChosen(string value)

    property alias index_MALE: modelGenders.index_MALE
    property int index_FEMALE: modelGenders.index_FEMALE
    property int index_OTHER: modelGenders.index_OTHER
    property string lang: ""

    BilingualText {
        id: textYouAre
        textEn: "You are..."
        textFr: "Vous êtes..."
        language: thisPage.lang
    }

    Rectangle {
        Layout.minimumWidth: 400
        Layout.minimumHeight: 500
        color: "black"

        Label {
            Layout.alignment: Qt.AlignCenter
            text: textYouAre.text
            font.capitalization: Font.AllUppercase
            color: "#ffffff"
            font.pixelSize: 40
        }
    }

    ListView {
        Layout.margins: 0
        Layout.topMargin: 10
        Layout.rightMargin: 50
        Layout.fillWidth: false
        Layout.fillHeight: true
        orientation: Qt.Vertical
        width: currentItem.width
        // create a model item instance
        model: ModelGenders {
            id: modelGenders
        }
        // provide delegate component.
        delegate: delegateComponent
    }

    Component {
        id: delegateComponent

        WidgetChoiceButton {
            BilingualText {
                id: textThisButton
                textEn: name_en // from the model
                textFr: name_fr // from the model
                language: lang
            }
            text: textThisButton.text
            onClicked: {
                thisPage.genderChosen(identifier);
            }
        }
    }
}
