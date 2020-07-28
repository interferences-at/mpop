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
        text: thisWidget.text
        font.pixelSize: 12
        font.capitalization: Font.MixedCase
        Layout.fillWidth: true
        topPadding: 0
        bottomPadding: 0

        onClicked: {
            thisWidget.clicked();
        }
    }
}
