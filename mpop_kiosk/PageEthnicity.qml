import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

/**
 * Page that contains the widget to select my ethnicity.
 */
RowLayout {
    id: thisPage

    signal nextButtonClicked()
    signal previousButtonClicked()

    property string ethnicityChoice: null

    ColumnLayout {
        Rectangle {
            width:  550
            height: 300
            color: "black"

            Label {
                Layout.alignment: Qt.AlignCenter
                text: qsTr("To which nation \n do you identify \n the most?")
                font.pixelSize: 36
            }
        }
    }

    ListView {
        Layout.topMargin: 10
        Layout.fillWidth: false
        Layout.fillHeight: true
        orientation: Qt.Vertical
        width: currentItem.width
        // create a model item instance
        model: ModelNations {
            id: modelNations
        }
        // provide delegate component.
         delegate: delegateComponent

        /*delegate: WidgetChoiceButton {
            // TODO: Make this multilingual
            text: text_fr // Property of items in the model.
            height: parent.height / parent.count
            spacing: 0
        }*/
        Component {
            id: delegateComponent

            Rectangle {
                width:  600
                height: 100
                color: "black"
                border.color: "white"
                border.width: 2

                Item {
                    width: 400; height: 100
                    Column{
                        Text {
                            id: myText
                            text: text_fr
                            color: "#ffffff"
                            font.pixelSize: 72
                            font.capitalization: Font.AllUppercase
                            font.weight: Font.ExtraBold
                        }
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
