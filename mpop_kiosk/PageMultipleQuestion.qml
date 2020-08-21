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
    property bool sliderWidgetVisibility: true
    // List for the questions in this page:
    property var questionIdentifiers: null

    property string lang: ""

    signal nextButtonClicked()
    signal previousButtonClicked()

    BilingualText {
        id: mainQuestionText
        language: thisPage.lang
        textFr: ""
        textEn: ""
    }

    BilingualText {
        id: leftText
        language: thisPage.lang
        textFr: ""
        textEn: ""
    }

    BilingualText {
        id: rightText
        language: thisPage.lang
        textFr: ""
        textEn: ""
    }

    BilingualText {
        id: questionText0
        language: thisPage.lang
        textFr: ""
        textEn: ""
    }

    BilingualText {
        id: questionText1
        language: thisPage.lang
        textFr: ""
        textEn: ""
    }

    BilingualText {
        id: questionText2
        language: thisPage.lang
        textFr: ""
        textEn: ""
    }

    BilingualText {
        id: questionText3
        language: thisPage.lang
        textFr: ""
        textEn: ""
    }

    BilingualText {
        id: questionText4
        language: thisPage.lang
        textFr: ""
        textEn: ""
    }

    Component.onCompleted: {
        // Retrieve the first question model item:
        var firstQuestion = modelQuestions.findQuestion(questionIdentifiers[0]);
        // Set the text of the main question:
        mainQuestionText.textFr = firstQuestion.question_fr;
        mainQuestionText.textEn = firstQuestion.question_en;

        leftText.textFr = firstQuestion.min_fr;
        leftText.textEn = firstQuestion.min_en;

        rightText.textFr = firstQuestion.max_fr;
        rightText.textEn = firstQuestion.max_en;

        questionText0.textFr = modelQuestions.findQuestion(questionIdentifiers[0]).question_fr;
        questionText0.textEn = modelQuestions.findQuestion(questionIdentifiers[0]).question_en;

        questionText1.textFr = modelQuestions.findQuestion(questionIdentifiers[1]).question_fr;
        questionText1.textEn = modelQuestions.findQuestion(questionIdentifiers[1]).question_en;

        questionText2.textFr = modelQuestions.findQuestion(questionIdentifiers[2]).question_fr;
        questionText2.textEn = modelQuestions.findQuestion(questionIdentifiers[2]).question_en;

        questionText3.textFr = modelQuestions.findQuestion(questionIdentifiers[3]).question_fr;
        questionText3.textEn = modelQuestions.findQuestion(questionIdentifiers[3]).question_en;

        questionText4.textFr = modelQuestions.findQuestion(questionIdentifiers[4]).question_fr;
        questionText4.textEn = modelQuestions.findQuestion(questionIdentifiers[4]).question_en;

        // TODO three more

        // TODO: Retrieve value for user from service and populate the slider, if set.
    }

    BilingualText {
        id: textDataVisualization
        language: thisPage.lang
        textFr: "Visualisation de données"
        textEn: "Data visualization"
    }

    //first slider
    Label {
        Layout.alignment: Qt.AlignLeft
        text: questionText0.text
        topPadding: 100
        leftPadding: 80
        font.pixelSize: 22
    }
    WidgetAnswerSlider {
        Layout.fillWidth: true
        width: 300
        sliderValue: 35
        textLeft: leftText.text
        textRight: rightText.text
        showNumber: false
        visible: sliderWidgetVisibility
        Layout.bottomMargin: 15
    }

    //second slider
    Label {
         Layout.alignment: Qt.AlignLeft
        text: questionText1.text
        leftPadding: 80
        font.pixelSize: 22
    }
    WidgetAnswerSlider {
        Layout.fillWidth: true
        width: 300
        sliderValue: 35
        textLeft: leftText.text
        textRight: rightText.text
        showNumber: false
        visible: sliderWidgetVisibility
        Layout.bottomMargin: 15
    }

    //third slider
    Label {
        Layout.alignment: Qt.AlignLeft
        text: questionText2.text
        leftPadding: 80
        font.pixelSize: 22
    }
    WidgetAnswerSlider {
        Layout.fillWidth: true
        width: 300
        sliderValue: 35
        textLeft: leftText.text
        textRight: rightText.text
        showNumber: false
        visible: sliderWidgetVisibility
        Layout.bottomMargin: 15
    }

    //fourth slider
    Label {
        Layout.alignment: Qt.AlignLeft
        text: questionText3.text
        leftPadding: 80
        font.pixelSize: 22
    }
    WidgetAnswerSlider {
        Layout.fillWidth: true
        width: 300
        sliderValue: 35
        textLeft: leftText.text
        textRight: rightText.text
        showNumber: false
        visible: sliderWidgetVisibility
        Layout.bottomMargin: 15
    }

    //fifth slider
    Label {
        Layout.alignment: Qt.AlignLeft
        text: questionText4.text
        leftPadding: 80
        font.pixelSize: 22
    }
    WidgetAnswerSlider {
        Layout.fillWidth: true
        width: 300
        sliderValue: 35
        textLeft: leftText.text
        textRight: rightText.text
        showNumber: false
        visible: sliderWidgetVisibility
        Layout.bottomMargin: 15
    }


    // TODO: Sub-page: Dataviz Multiple Questions
}
