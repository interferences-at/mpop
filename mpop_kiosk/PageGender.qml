import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

/**
 * Page that contains the widget to select my gender.
 */
ColumnLayout {
    id: thisPage

    signal nextButtonClicked()
    signal previousButtonClicked()

    Label {
        Layout.alignment: Qt.AlignCenter
        text: "Select your gender"
        font.pixelSize: 36
    }

    ListView {
        Layout.margins: 0
        Layout.fillWidth: false
        Layout.fillHeight: true
        orientation: Qt.Vertical
        width: currentItem.width
        model: ModelGenders {
            id: modelGenders
        }

        delegate: WidgetChoiceButton {
            questionName: question_fr
            height: parent.height / parent.count
            spacing: 0
        }
    }

}
