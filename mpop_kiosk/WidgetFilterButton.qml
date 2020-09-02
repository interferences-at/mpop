import QtQuick 2.9
import QtQuick.Controls 2.2

Button {
    property BilingualText label

    id: widgetFilterButton
    text: label.text

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
        color: highlighted ? "#000" : "#fff"
        font: widgetFilterButton.font
    }

    background: Rectangle {
        color: highlighted ? "#fff" : "transparent"
        implicitHeight: 40
        border.color: "#fff"
        radius: 30
    }
}
