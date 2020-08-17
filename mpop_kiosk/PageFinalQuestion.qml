import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

/**
 * Page that contains final question.
 */
RowLayout {
    id: thisPage

    signal finalQuestionChosen(string value)

    property string lang: ""

    Label {
        Layout.alignment: Qt.AlignCenter
        text: qsTr("Final question")
        font.capitalization: Font.AllUppercase
        font.pixelSize: 36
    }

    // TODO
}
