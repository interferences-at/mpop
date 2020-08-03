import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

/**
 * Button for choosing a question.
 */
ColumnLayout {
    id: thisWidget

    signal clicked()

    property string text: ""
    property string metaData: ""
    property int pageNumber: 0

    Button {
        id: control

        text: thisWidget.text
        font.pixelSize: 72
        Layout.fillWidth: true
        topPadding: 0
        bottomPadding: 0
        // To disable AllUppercase: font.capitalization: Font.MixedCase
        font.capitalization: Font.AllUppercase
        font.weight: Font.ExtraBold

        onClicked: {
            thisWidget.clicked();
        }

        contentItem: Text {
            text: control.text
            font: control.font
            opacity: enabled ? 1.0 : 0.3
            color: control.down ? "#999999" : "#ffffff"
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            elide: Text.ElideRight
        }

        background: Rectangle {
            implicitWidth: 400
            implicitHeight: 100
            height: 100
            opacity: enabled ? 1.0 : 0.3
            border.color: control.down ? "#999999" : "#ffffff"
            color: control.down ? "#333333" : "#000000"
            border.width: 2
            radius: 2
        }
    }
}
