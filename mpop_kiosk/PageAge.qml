import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

/**
 * Page that contains the widget to select my age.
 */
RowLayout {
    id: thisPage

    signal nextButtonClicked()
    signal previousButtonClicked()

    // There should be no age set if we show this page.
    property int selectedAge: -1

    Label {
        Layout.alignment: Qt.AlignCenter
        text: qsTr("How old are you?")
        font.capitalization: Font.AllUppercase
        font.pixelSize: 36
    }

    ListView {
        id: listView0

        Layout.margins: 0
        Layout.fillWidth: false
        Layout.fillHeight: true
        orientation: Qt.Vertical
        width: currentItem.width
        model: 120 // There are 120 items in this model.

        delegate: WidgetChoiceButton {
            text: index + ((index > 1) ? " ans" : " an") // Index of the item in the model.
            height: parent.height / parent.count
            spacing: 0

            onClicked: {
                thisPage.selectedAge = index;
            }
        }
    }

    WidgetPreviousNext {
        onNextButtonClicked: thisPage.nextButtonClicked()
        onPreviousButtonClicked: thisPage.previousButtonClicked()
    }
}
