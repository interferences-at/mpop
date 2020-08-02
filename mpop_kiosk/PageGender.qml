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

    Rectangle{
        Layout.minimumWidth: 400
        Layout.minimumHeight: 500
        color: "black"

        Label {
            Layout.alignment: Qt.AlignCenter
            text: qsTr("VOUS ETES...")
            color: "#ffffff"
            font.pixelSize: 40
        }
    }

    ListView {
        Layout.margins: 0
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

        Rectangle {
            width: 400
            height: 100
            color: "black"
            border.color: "white"
            border.width: 2

            Item {
                width: 400; height: 100

                Column{
                    Text {
                        text: name_fr
                        color: "#ffffff"
                        font.pixelSize: 72
                        //leftPadding: 10
                        font.capitalization: Font.AllUppercase
                        font.weight: Font.ExtraBold
                    }
                }
            }
        }
    }



    WidgetPreviousNext {
        onNextButtonClicked: thisPage.nextButtonClicked()
        onPreviousButtonClicked: thisPage.previousButtonClicked()
    }
}
