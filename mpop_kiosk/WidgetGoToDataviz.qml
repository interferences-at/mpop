import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

// Button to go to the dataviz
Label {
    property bool toggled: false
    property string arrow: toggled ? "↓" : "↑"

    signal clicked()

    width: parent.width
    height: 80

    horizontalAlignment: Text.AlignHCenter
    verticalAlignment: Text.AlignVCenter
    id: datavisual
    //text: [arrow, textDataVisualization.text, arrow].join(" ")
    text: [arrow, "Voir les données", arrow].join("  ")
    font {
        pixelSize: 45
        capitalization: Font.AllUppercase
    }
    color: Palette.accent

    background: Rectangle {
        anchors.fill: parent
        color: Palette.lightBlack
    }

    MouseArea {
        anchors.fill: parent
        onClicked: clicked()
    }
}
