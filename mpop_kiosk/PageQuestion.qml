import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

/**
 * Page that contains a single question.
 */
ColumnLayout {

    id: thisPage

    function loadQuestions() {
        numberOfQuestions = questionIdentifiers.length;

        hasMultipleQuestions = numberOfQuestions > 1;

        // Retrieve the first question model item:
        var firstQuestion = modelQuestions.findQuestion(questionIdentifiers[0]);

        // Set the text of the main question:
        mainQuestionText.textFr = firstQuestion.question_fr;
        mainQuestionText.textEn = firstQuestion.question_en;
        leftText1.textFr = firstQuestion.min_fr;
        leftText1.textEn = firstQuestion.min_en;
        rightText1.textFr = firstQuestion.max_fr;
        rightText1.textEn = firstQuestion.max_en;


        if (hasMultipleQuestions) {
            questionText1.textFr = firstQuestion.subtitle_fr;
            questionText1.textEn = firstQuestion.subtitle_en;
            leftText1.textFr = firstQuestion.min_fr;
            leftText1.textEn = firstQuestion.min_en;
            rightText1.textFr = firstQuestion.max_fr;
            rightText1.textEn = firstQuestion.max_en;


            if (numberOfQuestions >= 2) {
                var item2 = modelQuestions.findQuestion(questionIdentifiers[1]);
                questionText2.textFr = item2.subtitle_fr;
                questionText2.textEn = item2.subtitle_en;
                leftText2.textFr = item2.min_fr;
                leftText2.textEn = item2.min_en;
                rightText2.textFr = item2.max_fr;
                rightText2.textEn = item2.max_en;
            }
            if (numberOfQuestions >= 3) {
                var item3 = modelQuestions.findQuestion(questionIdentifiers[2]);
                questionText3.textFr = item3.subtitle_fr;
                questionText3.textEn = item3.subtitle_en;
                leftText3.textFr = item3.min_fr;
                leftText3.textEn = item3.min_en;
                rightText3.textFr = item3.max_fr;
                rightText3.textEn = item3.max_en;
            }
            if (numberOfQuestions >= 4) {
                var item4 = modelQuestions.findQuestion(questionIdentifiers[3]);
                questionText4.textFr = item4.subtitle_fr;
                questionText4.textEn = item4.subtitle_en;
                leftText4.textFr = item4.min_fr;
                leftText4.textEn = item4.min_en;
                rightText4.textFr = item4.max_fr;
                rightText4.textEn = item4.max_en;
            }
            if (numberOfQuestions >= 5) {
                var item5 = modelQuestions.findQuestion(questionIdentifiers[4]);
                questionText5.textFr = item5.subtitle_fr;
                questionText5.textEn = item5.subtitle_en;
                leftText5.textFr = item5.min_fr;
                leftText5.textEn = item5.min_en;
                rightText5.textFr = item5.max_fr;
                rightText5.textEn = item5.max_en;
            }
        }
    }

    function loadAnswersForCurrentVisitor() {
        // TODO: Retrieve value for user from service and populate the slider, if set.
    }

    property var modelQuestions: null
    property var serviceClient: null
    property var datavizSender: null
    property string titleText: ""
    property int numberOfQuestions: 0 // [1,5]
    property bool hasMultipleQuestions: false
    property var questionIdentifiers: []
    property bool buttonsVisibility: false
    property bool ageVisualization: false
    property string filterHighlighted: ""
    property bool buttonTextHighlight: true
    property string lang: ""

    signal nextButtonClicked()
    signal previousButtonClicked()

    BilingualText {
        id: mainQuestionText
    }

    BilingualText {
        id: questionText1
    }

    BilingualText {
        id: questionText2
    }

    BilingualText {
        id: questionText3
    }

    BilingualText {
        id: questionText4
    }

    BilingualText {
        id: questionText5
    }

    BilingualText {
        id: leftText1
    }

    BilingualText {
        id: rightText1
    }

    BilingualText {
        id: leftText2
    }

    BilingualText {
        id: rightText2
    }

    BilingualText {
        id: leftText3
    }

    BilingualText {
        id: rightText3
    }

    BilingualText {
        id: leftText4
    }

    BilingualText {
        id: rightText4
    }

    BilingualText {
        id: leftText5
    }

    BilingualText {
        id: rightText5
    }

    Component.onCompleted: {
        loadQuestions();
        loadAnswersForCurrentVisitor();

        // TODO: Retrieve value for user from service and populate the slider, if set.
    }

    BilingualText {
        id: textDataVisualization
        textFr: "Visualisation de données"
        textEn: "Data visualization"
    }

    /**
     * StackLayout to switch from answering the question to data viz.
     */
    StackLayout {
        id: questionDatavizStackLayout

        currentIndex: 0

        readonly property int index_QUESTIONS: 0
        readonly property int index_CHOOSE_DATAVIZ_MODE: 1
        readonly property int index_DATAVIZ_MULTIPLE_QUESTIONS: 2
        readonly property int index_DATAVIZ_SINGLE_BY_AGE: 3
        readonly property int index_DATAVIZ_SINGLE_BY_GENDER: 4
        readonly property int index_DATAVIZ_SINGLE_BY_ETHNICITY: 5

        // Index 0: answering the questions
        ColumnLayout {

            WidgetGoToDataviz {
                // FIXME: make this work
            }

            // To answer the question(s) with the sliders:
            ColumnLayout {

                // The main question text:
                Label {
                    Layout.alignment: Qt.AlignCenter
                    text: mainQuestionText.text
                    font.pixelSize: 30
                    bottomPadding: 30
                    leftPadding: 60
                }

                // first slider
                Label {
                    Layout.alignment: Qt.AlignLeft
                    text: questionText1.text
                    topPadding: 100
                    leftPadding: 80
                    font.pixelSize: 22
                    visible: hasMultipleQuestions
                }
                WidgetAnswerSlider {
                    Layout.fillWidth: true
                    width: 300
                    sliderValue: 35
                    textLeft: leftText1.text
                    textRight: rightText1.text
                    showNumber: false
                    Layout.bottomMargin: 15
                }

                // second slider
                Label {
                     Layout.alignment: Qt.AlignLeft
                    text: questionText2.text
                    leftPadding: 80
                    font.pixelSize: 22
                    visible: hasMultipleQuestions && (numberOfQuestions >= 2)
                }
                WidgetAnswerSlider {
                    Layout.fillWidth: true
                    width: 300
                    sliderValue: 35
                    textLeft: leftText2.text
                    textRight: rightText2.text
                    showNumber: false
                    Layout.bottomMargin: 15
                    visible: hasMultipleQuestions && (numberOfQuestions >= 2)
                }

                // third slider
                Label {
                    Layout.alignment: Qt.AlignLeft
                    text: questionText3.text
                    leftPadding: 80
                    font.pixelSize: 22
                    visible: hasMultipleQuestions && (numberOfQuestions >= 3)
                }

                WidgetAnswerSlider {
                    Layout.fillWidth: true
                    width: 300
                    sliderValue: 35
                    textLeft: leftText3.text
                    textRight: rightText3.text
                    showNumber: false
                    Layout.bottomMargin: 15
                    visible: hasMultipleQuestions && (numberOfQuestions >= 3)
                }

                // fourth slider
                Label {
                    Layout.alignment: Qt.AlignLeft
                    text: questionText4.text
                    leftPadding: 80
                    font.pixelSize: 22
                    visible: hasMultipleQuestions && (numberOfQuestions >= 4)
                }

                WidgetAnswerSlider {
                    Layout.fillWidth: true
                    width: 300
                    sliderValue: 35
                    textLeft: leftText4.text
                    textRight: rightText4.text
                    showNumber: false
                    Layout.bottomMargin: 15
                    visible: hasMultipleQuestions && (numberOfQuestions >= 4)
                }

                // fifth slider
                Label {
                    Layout.alignment: Qt.AlignLeft
                    text: questionText5.text
                    leftPadding: 80
                    font.pixelSize: 22
                    visible: hasMultipleQuestions && (numberOfQuestions >= 5)
                }

                WidgetAnswerSlider {
                    Layout.fillWidth: true
                    width: 300
                    sliderValue: 35
                    textLeft: leftText5.text
                    textRight: rightText5.text
                    showNumber: false
                    Layout.bottomMargin: 15
                    visible: hasMultipleQuestions && (numberOfQuestions >= 5)
                }
            }
        }
    }
}

//Rectangle {
//    width: 600
//    height: 80
//    Layout.leftMargin: 250
//    color: "black"

//    Label {
//        Layout.alignment: Qt.AlignCenter
//        text: qsTr("Please select a basic viewing setting")
//        font.pixelSize: 30
//        visible: buttonsVisibility
//    }
//}
//// 1st-level filters
//RowLayout {
//    id: root
//    Layout.fillHeight: true
//    //Layout.margins: 0
//    Layout.alignment: Qt.AlignHCenter
//    //Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
//    spacing: 24
//    visible: buttonsVisibility

//    // reset on RFID tag change
//    property variant resetter: window.rfidTag
//    onResetterChanged: filterHighlighted = ""

//    // TODO: wrap in Repeater and simplify identifier declarations
//    WidgetFilterButton {
//        label: BilingualText {
//            textEn: "Age"
//            textFr: "Âge"
//        }
//        checked: filterHighlighted === "ageBtn"
//        onClicked: filterHighlighted = "ageBtn"
//    }

//    WidgetFilterButton {
//        label: BilingualText { textEn: "Gender"; textFr: "Genre"; language: window.lang }
//        checked: filterHighlighted === "genreBtn"
//        onClicked: filterHighlighted = "genreBtn"
//    }

//    WidgetFilterButton {
//        label: BilingualText { textEn: "Culture"; textFr: "Culture"; language: window.lang }
//        checked: filterHighlighted === "cultureBtn"
//        onClicked: filterHighlighted = "cultureBtn"
//    }

//    WidgetFilterButton {
//        BilingualText {
//            id: languageButtonText
//            textEn: "Language"
//            textFr: "Langue"
//        }
//        label: languageButtonText.text
//        checked: filterHighlighted === "langBtn"
//        onClicked: filterHighlighted = "langBtn"
//    }
//}

//// filter navigation
//RowLayout {
//    Layout.fillWidth: true
//    Layout.alignment: Qt.AlignCenter | Qt.AlignCenter
//    Layout.topMargin: 50
//    Layout.leftMargin: 400
//    visible: buttonsVisibility

//    RoundButton {
//        text: qsTr("Left")
//        //icon.source: "leftarrow.svg"
//        Layout.fillHeight: true
//        Layout.fillWidth: true
//        Layout.maximumWidth: 80
//        Layout.maximumHeight: 80

//        onClicked: {
//            sliderWidgetVisibility = true
//            buttonsVisibility = false
//            ageVisualization = false
//        }
//    }
//    RoundButton {
//        text: qsTr("Right")
//        //icon.source: "rightarrow.svg"
//        Layout.fillHeight: true
//        Layout.fillWidth: true
//        Layout.maximumWidth: 80
//        Layout.maximumHeight: 80

//        onClicked: {
//            sliderWidgetVisibility = false
//            buttonsVisibility = false
//            ageVisualization = true
//        }
//    }

//}

//Label {
//    Layout.alignment: Qt.AlignLeft
//    text: qsTr("Visualization by age of respondents")
//    font.pixelSize: 24
//    Layout.leftMargin: 30
//    Layout.topMargin: 10
//    visible: ageVisualization
//    leftPadding: 35
//}
//Label {
//    Layout.alignment: Qt.AlignLeft
//    text: qsTr("You can select the following filters to discover new correlations")
//    font.pixelSize: 24
//    Layout.leftMargin: 30
//    visible: ageVisualization
//    leftPadding: 35
//    bottomPadding: 30
//}

//// 2nd-level filters
//ColumnLayout {
//    Layout.fillWidth: true
//    Layout.leftMargin: 30
//    spacing: 30

//    Repeater {
//        // model
//        model: ListModel {
//            ListElement {
//                sectionTitleEn: "Culture"
//                sectionTitleFr: "Culture"
//                filters: [
//                    ListElement { textEn: "All"; textFr: "Tous" },
//                    ListElement { textEn: "Quebecois"; textFr: "Québécoise" },
//                    ListElement { textEn: "Canadian"; textFr: "Canadienne" },
//                    ListElement { textEn: "Native"; textFr: "Autochtone" },
//                    ListElement { textEn: "American"; textFr: "Américaine" },
//                    ListElement { textEn: "European"; textFr: "Européenne" },
//                    ListElement { textEn: "Other"; textFr: "Autre" }
//                ]
//            }

//            ListElement {
//                sectionTitleEn: "Gender"
//                sectionTitleFr: "Genre"
//                filters: [
//                    ListElement { textEn: "All"; textFr: "Tous" },
//                    ListElement { textEn: "Male"; textFr: "Homme" },
//                    ListElement { textEn: "Female"; textFr: "Femme" },
//                    ListElement { textEn: "Other"; textFr: "Autre" }
//                ]
//            }

//            ListElement {
//                sectionTitleEn: "Time"
//                sectionTitleFr: "Temps"
//                filters: [
//                    ListElement { textEn: "All"; textFr: "Tous" },
//                    ListElement { textEn: "Today"; textFr: "Aujourd'hui" },
//                    ListElement { textEn: "This year"; textFr: "Cette année" },
//                    ListElement { textEn: "From the beginning"; textFr: "Depuis le début" }
//                ]
//            }
//        }

//        // display
//        ColumnLayout {
//            id: subfilter

//            property int currentIndex: 0
//            // reset index when RFID tag changes
//            property variant resetter: window.rfidTag
//            onResetterChanged: currentIndex = 0

//            Layout.fillWidth: true
//            Layout.alignment: Qt.AlignLeft

//            spacing: 15
//            visible: ageVisualization

//            // section title
//            Text {
//                text: sectionTitle.text
//                color: "#fff"
//                font {
//                    family: "Trim SemiBold"
//                    weight: Font.DemiBold
//                    pixelSize: 11
//                    letterSpacing: 11 * 25 / 1000
//                    capitalization: Font.AllUppercase
//                }

//                BilingualText {
//                    id: sectionTitle
//                    textEn: sectionTitleEn
//                    textFr: sectionTitleFr
//                }
//            }

//            Flow {
//                Layout.fillWidth: true
//                Layout.leftMargin: -10
//                spacing: 10

//                Repeater {
//                    model: filters

//                    // filter button
//                    WidgetFilterButton {
//                        label: BilingualText { textEn: model.textEn; textFr: model.textFr }

//                        checked: index === subfilter.currentIndex
//                        onClicked: subfilter.currentIndex = index
//                    }
//                }
//            }
//        }
//    }
//}

//RowLayout {
//    Layout.fillWidth: true
//    Layout.alignment: Qt.AlignCenter | Qt.AlignCenter
//    Layout.topMargin: 30
//    Layout.leftMargin: 500
//    visible: ageVisualization

//    RoundButton {
//        text: qsTr("Left")
//        //icon.source: "leftarrow.svg"
//        Layout.fillHeight: true
//        Layout.fillWidth: true
//        Layout.maximumWidth: 80
//        Layout.maximumHeight: 80

//        onClicked: {
//            buttonsVisibility = true
//            ageVisualization = false
//        }
//    }
//}
