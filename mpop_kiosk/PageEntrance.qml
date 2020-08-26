import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

RowLayout {
    property int currentSelected: -1

    spacing: 0

    // Page label to the left
    Label {
        Layout.preferredWidth: 435
        Layout.fillHeight: true
        Layout.leftMargin: 40
        Layout.topMargin: 100

        text: "Vous êtes..."
        font {
            pixelSize: 28
            family: "Trim SemiBold"
            capitalization: Font.AllUppercase
        }
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

            model: ModelEthnicities {}
            delegate: WidgetChoiceButton {
                text: text_en
                width: choiceList.width
                height: choiceList.height / 6

                active: currentSelected < 0 || currentSelected === index
                onClicked: currentSelected = index
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
