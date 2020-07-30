import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

/**
 * Page that contains the widget to select my ethnicity.
 */
RowLayout {
    id: thisPage

    signal nextButtonClicked()
    signal previousButtonClicked()

    property string ethnicityChoice: null

    ColumnLayout {

        Label {
            Layout.alignment: Qt.AlignCenter
            text: qsTr("To which nation")
            font.pixelSize: 36
        }
        Label {
            Layout.alignment: Qt.AlignCenter
            text: qsTr("do you identify")
            font.pixelSize: 36
        }
        Label {
            Layout.alignment: Qt.AlignCenter
            text: qsTr("the most?")
            font.pixelSize: 36
        }
    }

    ListView {
        Layout.margins: 0
        Layout.fillWidth: false
        Layout.fillHeight: true
        orientation: Qt.Vertical
        width: currentItem.width
        model: ModelNations {
            id: modelNations
        }

        delegate: WidgetChoiceButton {
            // TODO: Make this multilingual
            text: name_fr // Property of items in the model.
            height: parent.height / parent.count
            spacing: 0
        }
    }

    WidgetPreviousNext {
        onNextButtonClicked: thisPage.nextButtonClicked()
        onPreviousButtonClicked: thisPage.previoiusButtonClicked()
    }
}
