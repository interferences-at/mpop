import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

/**
 * Button for choosing a question.
 */
ColumnLayout {
    property string questionName: ""

    Button {
        text: questionName
        font.pixelSize: 12
        font.capitalization: Font.MixedCase
        Layout.fillWidth: true
        topPadding: 0
        bottomPadding: 0
    }
}
