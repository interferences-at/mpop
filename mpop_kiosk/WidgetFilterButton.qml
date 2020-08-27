import QtQuick 2.12
import QtQuick.Controls 2.12

Button {
    property BilingualText label

    id: widgetFilterButton
    text: label.text
    checkable: true

    leftPadding: 20
    rightPadding: 20

    font {
        pixelSize: 20
        letterSpacing: 20 * 25 / 1000
    }

    contentItem: Text {
        text: widgetFilterButton.text
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        color: checked ? "#000" : "#fff"
        font: widgetFilterButton.font
    }

    background: Rectangle {
        color: checked ? "#fff" : "transparent"
        implicitHeight: 40
        border.color: "#fff"
        radius: 30
    }
}
