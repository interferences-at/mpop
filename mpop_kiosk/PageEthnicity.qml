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
    signal ethnicityChosen(string value)

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

        Component {
            id: delegateComponent

            WidgetChoiceButton {
                // TODO: Make this multilingual
                text: text_en // Property of items in the model.
                height: parent.height / parent.count
                spacing: 0
                onClicked: {
                    thisPage.ethnicityChosen(identifier);
                }
            }
        }
    }

    WidgetPreviousNext {
        onNextButtonClicked: thisPage.nextButtonClicked()
        onPreviousButtonClicked: thisPage.previousButtonClicked()
    }
}
