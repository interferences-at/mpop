import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

/**
 * Page that contains multiple questions.
 */
ColumnLayout {
    id: thisPage

    property var modelQuestions: null
    property var serviceClient: null
    property var datavizSender: null
    property string minText: ""
    property string maxText: ""
    property string titleText: ""
    // List for the questions in this page:
    property var questionIdentifiers: null
    property string mainQuestionText: null
    property string lang: ""

    signal nextButtonClicked()
    signal previousButtonClicked()

    Button {
        Layout.alignment: Qt.AlignRight
        text: "X"
        font.pixelSize: 24
    }

    Label {
        Layout.alignment: Qt.AlignCenter
        text: titleText
        font.pixelSize: 36
    }
    Label {
        Layout.alignment: Qt.AlignCenter
        text: mainQuestionText
        font.pixelSize: 36
    }
    WidgetAnswerSlider {
        Layout.fillWidth: true
        sliderValue: 50
        textLeft: minText
        textRight: maxText
        showNumber: false
    }

    // TODO: Sub-page: Dataviz Multiple Questions
}
