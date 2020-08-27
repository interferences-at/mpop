import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

RowLayout {
    id: pageEntrance

    property int currentSelected: -1
    property variant model
    property BilingualText sideLabel

    signal choiceClicked(int index)

    spacing: 0

    // Page label to the left
    Label {
        Layout.preferredWidth: 435
        Layout.fillHeight: true
        Layout.leftMargin: 40
        Layout.topMargin: 100

        text: sideLabel.text
        font {
            pixelSize: 28
            capitalization: Font.AllUppercase
        }
        wrapMode: Text.WordWrap
    }

    Rectangle {
        Layout.preferredWidth: 480
        Layout.fillHeight: true
        Layout.topMargin: -1
        Layout.bottomMargin: -1

        color: "transparent"
        border.width: 1
        border.color: "#fff"

        // List of WidgetChoiceButton
        ListView {
            id: choiceList
            anchors.fill: parent
            snapMode: ListView.SnapToItem

            model: pageEntrance.model
            delegate: WidgetChoiceButton {
                BilingualText {
                    id: choiceButtonLabel
                    textFr: model.text_fr ? model.text_fr : (model.text ? model.text : modelData + ((index > 1) ? " ans" : " ans"))
                    textEn: model.text_en ? model.text_en : (model.text ? model.text : modelData + ((index > 1) ? " years" : " year"))
                }

                text: choiceButtonLabel.text
                width: choiceList.width
                height: choiceList.height / 6

                active: currentSelected < 0 || currentSelected === index
                onClicked: {
                    currentSelected = index;
                    choiceClicked(index);
                }
            }
        }
        ColumnLayout {
            anchors.fill: parent
            spacing: (parent.height / 6) - 1
            anchors.topMargin: parent.height / 6
            anchors.bottomMargin: parent.height / 6

            Repeater {
                model: 5

                Rectangle {
                    Layout.fillWidth: true
                    implicitHeight: 1
                    color: "#fff"
                }
            }
        }
    }
}
