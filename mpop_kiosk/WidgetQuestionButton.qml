import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

/**
 * Button for choosing a question.
 */
Button {
    id: thisWidget

    width: parent.width
    implicitHeight: 56

    property bool afterCurrent: false
    property bool bgCoverAll: false

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
        implicitWidth: 64
        implicitHeight: 64
        color: Palette.lightGrey
        anchors.centerIn: parent
        anchors.fill: bgCoverAll ? parent : undefined
        visible: highlighted
        radius: bgCoverAll ? 0 : 32
        z: -1
    }

    onPressed: resetIdleTimer()
}
