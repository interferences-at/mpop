import QtQuick 2.9
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3
import QtGraphicalEffects 1.0

ColumnLayout {
    id: widgetIconButton

    // general props
    property bool showButton: true
    property bool showLabel: true
    property alias labelText: buttonLabel.text
    property int buttonSize: 80

    // icon props
    property alias iconSource: arrowIcon.source
    property alias iconRotation: arrowIcon.rotation

    // signals
    signal clicked()

    enabled: showButton
    opacity: enabled
    spacing: 5

    Layout.fillWidth: false

    Item {
        Layout.preferredWidth: buttonSize
        Layout.preferredHeight: buttonSize

        RoundButton {
            id: button
            implicitWidth: buttonSize
            implicitHeight: buttonSize

            background: Rectangle {
                radius: buttonSize / 2
                color: button.down ? Palette.accent : Palette.white
            }

            Image {
                id: arrowIcon
                anchors.centerIn: parent
                source: "qrc:/arrow.svg"
                scale: buttonSize / 80
            }

            onClicked: widgetIconButton.clicked()
        }

        DropShadow {
            anchors.fill: button
            horizontalOffset: 1
            verticalOffset: 3
            radius: 5
            samples: 11
            color: "#66000000"
            source: button
        }
    }

    Label {
        id: buttonLabel
        visible: showLabel
        text: "Label"
        color: invertedTheme ? Palette.lightBlack : Palette.white
        font {
            pixelSize: 11
            letterSpacing: 11 * 25 / 1000
            capitalization: Font.AllUppercase
        }
        wrapMode: Label.WordWrap
        Layout.preferredWidth: buttonSize
        Layout.alignment: Qt.AlignHCenter
        horizontalAlignment: Text.AlignHCenter
    }
}
