import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

/**
 * Page that contains the widget to select my ethnicity.
 */
RowLayout {
    id: thisPage

    signal ethnicityChosen(string value)
    property string lang: ""

    ColumnLayout {
        Rectangle {
            width:  550
            height: 300
            color: "black"

            Label {
                Layout.alignment: Qt.AlignCenter
                text: qsTr("To which nation \n do you identify \n the most?")
                font.capitalization: Font.AllUppercase
                font.pixelSize: 36
            }
        }
    }

    ListView {

        id: ethnicityListView

        /**
         * Sets one button to be highlighted.
         * @param int indexButton Index to highlight - or -1 to make none highlighted.
         */
        function setHightlighted(indexButton) {
            for (var i = 0; i < modelEthnicities.count; i ++) {
                var item = modelEthnicities.get(i);
                if (indexButton === i) {
                    item.highlighted = true;
                } else {
                    item.highlighted = false;
                }
            }
        }

        Layout.margins: 0
        Layout.topMargin: 10
        Layout.rightMargin: 50
        Layout.fillWidth: false
        Layout.fillHeight: true
        orientation: Qt.Vertical
        width: currentItem.width

        // create a model item instance
        model: ModelEthnicities {
            id: modelEthnicities
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
                highlighted: highlighted // The model has a propery with the same name
                onClicked: {
                    thisPage.ethnicityChosen(identifier);
                    ethnicityListView.setHightlighted(index);
                }
            }
        }
    }
}
