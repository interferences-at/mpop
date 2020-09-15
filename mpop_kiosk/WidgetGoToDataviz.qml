import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

// Button to go to the dataviz
Label {
    id: goToDataviz

    property list<BilingualText> labels: [
        BilingualText {
            textEn: "Visualize data"
            textFr: "Visualiser les données"
        },
        BilingualText {
            textEn: "Return to questions"
            textFr: "Retour aux questions"
        }
    ]

    property bool toggled: false
    property string arrow: toggled ? "↓" : "↑"

    signal clicked()

    width: parent.width
    height: 80

    horizontalAlignment: Text.AlignHCenter
    verticalAlignment: Text.AlignVCenter
    text: [arrow, labels[toggled ? 1 : 0].text, arrow].join("  ")
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
        onClicked: goToDataviz.clicked()
        onPressed: resetIdleTimer()
    }
}
