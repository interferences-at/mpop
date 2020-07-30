import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

/**
 * Page that contains the widget to select my language.
 */
RowLayout {
    id: thisPage

    signal nextButtonClicked()
    signal previousButtonClicked()

    Label {
        Layout.alignment: Qt.AlignCenter
        text: qsTr("Choose a language")
        font.pixelSize: 36
    }

    ListView {
        Layout.margins: 0
        Layout.fillWidth: false
        Layout.fillHeight: true
        orientation: Qt.Vertical
        width: currentItem.width
        // This model contains only two items: fr and en.
        model: ListModel {
            ListElement {
                language_identifier: "fr"
                language_text: qsTr("French");
            }
            ListElement {
                language_identifier: "en"
                language_text: qsTr("English");
            }
        }

        delegate: WidgetChoiceButton {
            text: language_text // Property of items in the model.
            height: parent.height / parent.count
            spacing: 0
        }
    }

    WidgetPreviousNext {
        onNextButtonClicked: thisPage.nextButtonClicked()
        onPreviousButtonClicked: thisPage.previoiusButtonClicked()
    }
}
