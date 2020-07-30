import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

/**
 * Page that contains a single question.
 */
ColumnLayout {
    id: thisPage

    property var modelQuestions: null
    property var serviceClient: null
    property var datavizSender: null
    property string minText: ""
    property string maxText: ""
    property string titleText: ""
    property var questionIdentifier: null
    property string questionText: null

    signal nextButtonClicked()
    signal previousButtonClicked()

    Component.onCompleted: {
        var item = modelQuestions.findQuestion(questionIdentifier);

        var current_lang = "fr"; // TODO: check what current language is
        if (current_lang === "fr") {
            questionText = item.question_fr;
            minText = item.min_fr;
            maxText = item.max_fr;
        } else {
            questionText = item.question_en;
            minText = item.min_en;
            maxText = item.max_en;
        }

        // TODO: Retrieve value for user from service and populate the slider, if set.
    }

    Label {
        Layout.alignment: Qt.AlignCenter
        text: titleText
        font.pixelSize: 36
    }
    Label {
        Layout.alignment: Qt.AlignCenter
        text: questionText
        font.pixelSize: 36
    }
    WidgetAnswerSlider {
        Layout.fillWidth: true
        sliderValue: 50
        textLeft: minText
        textRight: maxText
        showNumber: false
    }

    // FIXME: Should the WidgetPreviousNext be part of the main.qml instead?
    WidgetPreviousNext {
        onNextButtonClicked: thisPage.nextButtonClicked()
        onPreviousButtonClicked: thisPage.previousButtonClicked()
    }

    // TODO: Sub-page: Choose Single Question Mode
    // TODO: Sub-page: Dataviz Single Question By Age
    // TODO: Sub-page: Dataviz Single Question By Gender
    // TODO: Sub-page: Dataviz Single Question By Ethnicity
}
