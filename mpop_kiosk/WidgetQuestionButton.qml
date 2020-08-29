import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

/**
 * Button for choosing a question.
 */
Button {
    id: thisWidget

    width: parent.width
    implicitHeight: 57

    property bool afterCurrent: false

    // TODO: Improve the look of this buttons
    text: "01"
    font.pixelSize: 20
    font.capitalization: Font.AllUppercase
    topPadding: 0
    bottomPadding: 0

    contentItem: Text {
        text: thisWidget.text
        font: thisWidget.font
        horizontalAlignment: Qt.AlignHCenter
        verticalAlignment: Qt.AlignVCenter
        color: afterCurrent ? Palette.mediumGrey : Palette.lightBlack
    }

    background: Item {}

    // highlight
    Rectangle {
        width: 64
        height: 64
        color: Palette.lightGrey
        anchors.centerIn: parent
        visible: highlighted
        radius: 32
        z: -1
    }
}
