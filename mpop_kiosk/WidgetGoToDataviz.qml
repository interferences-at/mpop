import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

// Button to go to the dataviz
RowLayout {
    Layout.alignment: Qt.AlignCenter | Qt.AlignCenter

    Rectangle {
        color: '#000'
        Layout.fillWidth: true
        Layout.preferredHeight: 60
        Layout.preferredWidth: 600

        Button {
            id: button
            icon.source: "uparrow.svg"
            anchors.right: datavisual.left;
            background: Rectangle {
                color: "#000"
                implicitWidth: 60
                implicitHeight: 40
            }

            onClicked: {
                buttonsVisibility = false
                ageVisualization = false
            }
        }
        Button {
            id:datavisual
            anchors.centerIn: parent
            text: textDataVisualization.text
            background: Rectangle {
                color: "#000"
                implicitWidth: 60
                implicitHeight: 40
            }
            font.pixelSize: 30

            onClicked: {
                buttonsVisibility = true
                ageVisualization = false
            }
        }
        Button {
            id:button2
            icon.source: "uparrow.svg"
            anchors.left: datavisual.right;
            background: Rectangle {
                color: "#000"
                implicitWidth: 60
                implicitHeight: 40
            }
            onClicked: {
                buttonsVisibility = false
                ageVisualization = false
            }
        }
        RoundButton {
            anchors.left: button2.right;
            anchors.leftMargin: 250;
            text: "X" // TODO: change this
            font.bold: true
            font.pixelSize: 20
            padding: 20
        }
    }
}
