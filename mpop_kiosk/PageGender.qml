import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

/**
 * Page that contains the widget to select my gender.
 */
RowLayout {
    id: thisPage

    signal nextButtonClicked()
    signal previousButtonClicked()
    signal genderChosen(string value)

    Rectangle {
        Layout.minimumWidth: 400
        Layout.minimumHeight: 500
        color: "black"

        Label {
            Layout.alignment: Qt.AlignCenter
            text: qsTr("You are...")
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
            text: name_en // TODO: if language is French, show it in French
            onClicked: {
                thisPage.genderChosen(identifier);
            }
        }
    }

    WidgetPreviousNext {
        onNextButtonClicked: thisPage.nextButtonClicked()
        onPreviousButtonClicked: thisPage.previousButtonClicked()
    }
}
