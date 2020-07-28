import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

/**
 * Page that contains multiple questions. (namely about the user's gender or culture)
 */
ColumnLayout {
    id: thisPage

    /**
     * List model for the questions in this page.
     * TODO
     */
    property var questions

    signal nextButtonClicked()
    signal previousButtonClicked()

    Button {
        Layout.alignment: Qt.AlignRight
        text: "X"
        font.pixelSize: 24
    }

    Label {
        Layout.alignment: Qt.AlignCenter
        text: "Question 01"
        font.pixelSize: 36
    }
    Label {
        Layout.alignment: Qt.AlignCenter
        text: "Combien de bière par semaine buvez-vous ?"
        font.pixelSize: 36
    }
    AnswerSlider {
        Layout.fillWidth: true
        sliderValue: 50
        textLeft: "Peu"
        textRight: "Beaucoup"
        showNumber: false
    }
    Label {
        Layout.alignment: Qt.AlignCenter
        text: "Sous-titre pour le slider."
        font.pixelSize: 24
    }
    WidgetPreviousNext {

    }
}
