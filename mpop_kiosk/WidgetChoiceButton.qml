import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

/**
 * Button for choosing a question.
 */
Button {
    id: control

    property string metaData: ""
    property int pageNumber: 0
    property bool active: true

    checkable: true
    padding: 0
    font {
        pixelSize: 71
        capitalization: Font.AllUppercase
    }

    contentItem: Text {
        font: control.font
        text: control.text
        color: active ? Palette.white : Palette.mediumGrey
        lineHeight: 1
        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignBottom
        leftPadding: 25
        bottomPadding: 5
        elide: Text.ElideRight
    }

    background: Rectangle {
        anchors.fill: parent
        color: "transparent"
    }

    onPressed: resetIdleTimer()
}
