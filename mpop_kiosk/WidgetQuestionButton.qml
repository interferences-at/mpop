import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

/**
 * Button for choosing a question.
 */
ColumnLayout {
    id: thisWidget

    signal buttonClicked()

    property string buttonTitle: ""
    property alias highlighted: childButton.highlighted

    // TODO: Improve the look of this buttons
    Button {
        id: childButton
        text: thisWidget.buttonTitle
        font.pixelSize: 12
        font.capitalization: Font.MixedCase
        Layout.fillWidth: true
        topPadding: 0
        bottomPadding: 0

        onClicked: {
            thisWidget.buttonClicked();
        }
    }
}
