import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

/**
 * Page that contains a single question.
 */
ColumnLayout {

    id: thisPage

    function loadQuestions() {
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

    function toggleDataviz() {
        if (datavizIndex !== index_QUESTIONS) datavizIndex = index_QUESTIONS;
        else {
            // change index depending on quantity of questions
            if (hasMultipleQuestions) datavizIndex = index_CHOOSE_MULTIPLE;
            else datavizIndex = index_CHOOSE_SINGLE
        }
    }

    property var modelQuestions: null
    property var serviceClient: null
    property var datavizSender: null
    property string titleText: ""
    property int numberOfQuestions: questionIdentifiers.length // [1,5]
    property bool hasMultipleQuestions: numberOfQuestions > 1
    property var questionIdentifiers: []
    property string filterHighlighted: ""
    property bool buttonTextHighlight: true

    property alias datavizIndex: questionDatavizStackLayout.currentIndex
    readonly property int index_QUESTIONS: 0
    readonly property int index_CHOOSE_SINGLE: 1
    readonly property int index_CHOOSE_MULTIPLE: 2

    signal nextButtonClicked()
    signal previousButtonClicked()

    Layout.leftMargin: 40
    Layout.rightMargin: 40
    Layout.topMargin: 90

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

    // The main question text:
    Label {
        Layout.fillWidth: true
        text: mainQuestionText.text
        font {
            pixelSize: 45
            capitalization: Font.AllUppercase
        }
        wrapMode: Text.WordWrap

        leftPadding: 40
        rightPadding: 40
        topPadding: 90
        bottomPadding: 70
    }

    /**
     * StackLayout to switch from answering the question to data viz.
     */
    ColumnLayout {
        Layout.fillWidth: true

        StackLayout {
            id: questionDatavizStackLayout

            Layout.fillWidth: true
            currentIndex: 0

            // Index 0: answering the questions
            ColumnLayout {
                Layout.fillWidth: true
                spacing: 0

                // To answer the question(s) with the sliders:
                ColumnLayout {
                    Layout.preferredWidth: 920
                    Layout.leftMargin: 40
                    Layout.rightMargin: 40
                    spacing: 40

                    // first slider
                    ColumnLayout {
                        spacing: 15

                        Label {
                            Layout.alignment: Qt.AlignLeft
                            text: questionText1.text
                            font {
                                family: "Red Hat Display"
                                weight: Font.Medium
                                pixelSize: 24
                                letterSpacing: 24 * 10 / 1000
                            }

                            visible: hasMultipleQuestions
                        }
                        WidgetAnswerSlider {
                            sliderValue: 35
                            textLeft: leftText1.text
                            textRight: rightText1.text
                            showNumber: false
                        }
                    }

                    // second slider
                    ColumnLayout {
                        spacing: 15

                        Label {
                            Layout.alignment: Qt.AlignLeft
                            text: questionText2.text
                            font {
                                family: "Red Hat Display"
                                weight: Font.Medium
                                pixelSize: 24
                                letterSpacing: 24 * 10 / 1000
                            }

                            visible: hasMultipleQuestions && (numberOfQuestions >= 2)
                        }
                        WidgetAnswerSlider {
                            sliderValue: 35
                            textLeft: leftText2.text
                            textRight: rightText2.text
                            showNumber: false
                            visible: hasMultipleQuestions && (numberOfQuestions >= 2)
                        }
                    }

                    // third slider
                    ColumnLayout {
                        spacing: 15

                        Label {
                            Layout.alignment: Qt.AlignLeft
                            text: questionText3.text
                            font {
                                family: "Red Hat Display"
                                weight: Font.Medium
                                pixelSize: 24
                                letterSpacing: 24 * 10 / 1000
                            }

                            visible: hasMultipleQuestions && (numberOfQuestions >= 3)
                        }

                        WidgetAnswerSlider {
                            sliderValue: 35
                            textLeft: leftText3.text
                            textRight: rightText3.text
                            showNumber: false
                            visible: hasMultipleQuestions && (numberOfQuestions >= 3)
                        }
                    }

                    // fourth slider
                    ColumnLayout {
                        spacing: 15

                        Label {
                            Layout.alignment: Qt.AlignLeft
                            text: questionText4.text
                            font {
                                family: "Red Hat Display"
                                weight: Font.Medium
                                pixelSize: 24
                                letterSpacing: 24 * 10 / 1000
                            }

                            visible: hasMultipleQuestions && (numberOfQuestions >= 4)
                        }

                        WidgetAnswerSlider {
                            sliderValue: 35
                            textLeft: leftText4.text
                            textRight: rightText4.text
                            showNumber: false
                            visible: hasMultipleQuestions && (numberOfQuestions >= 4)
                        }
                    }

                    // fifth slider
                    ColumnLayout {
                        spacing: 15

                        Label {
                            Layout.alignment: Qt.AlignLeft
                            text: questionText5.text
                            font {
                                family: "Red Hat Display"
                                weight: Font.Medium
                                pixelSize: 24
                                letterSpacing: 24 * 10 / 1000
                            }

                            visible: hasMultipleQuestions && (numberOfQuestions >= 5)
                        }

                        WidgetAnswerSlider {
                            sliderValue: 35
                            textLeft: leftText5.text
                            textRight: rightText5.text
                            showNumber: false
                            visible: hasMultipleQuestions && (numberOfQuestions >= 5)
                        }
                    }
                }
            }

            // Index 1: single question first axis
            // this screen is automatically skipped if hasMultipleQuestions === true
            ColumnLayout {
                Layout.fillWidth: true
                spacing: 0

                ColumnLayout {
                    Layout.fillWidth: true
                    Layout.leftMargin: 40
                    Layout.rightMargin: 40
                    spacing: 0

                    Label {
                        Layout.alignment: Qt.AlignCenter
                        text: qsTr("Please select a basic viewing setting")
                        color: "#fff"
                        font.pixelSize: 30
                    }
                    // 1st-level filters
                    RowLayout {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        Layout.alignment: Qt.AlignHCenter
                        spacing: 24

                        // reset on RFID tag change
                        property variant resetter: window.rfidTag
                        onResetterChanged: filterHighlighted = ""

                        // TODO: wrap in Repeater and simplify identifier declarations
                        WidgetFilterButton {
                            label: BilingualText { textEn: "Age"; textFr: "Âge" }
                            checked: filterHighlighted === "ageBtn"
                            onClicked: filterHighlighted = "ageBtn"
                        }

                        WidgetFilterButton {
                            label: BilingualText { textEn: "Gender"; textFr: "Genre" }
                            checked: filterHighlighted === "genreBtn"
                            onClicked: filterHighlighted = "genreBtn"
                        }

                        WidgetFilterButton {
                            label: BilingualText { textEn: "Culture"; textFr: "Culture" }
                            checked: filterHighlighted === "cultureBtn"
                            onClicked: filterHighlighted = "cultureBtn"
                        }

                        WidgetFilterButton {
                            label: BilingualText { textEn: "Language"; textFr: "Langue" }
                            checked: filterHighlighted === "langBtn"
                            onClicked: filterHighlighted = "langBtn"
                        }
                    }


                }
            }

            // Index 2: multi-filter selection
            // filter rows are selectively hidden depending on first-axis selection
            ColumnLayout {
                Layout.fillWidth: true
                spacing: 0

                ColumnLayout {
                    Layout.fillWidth: true
                    Layout.leftMargin: 40
                    Layout.rightMargin: 40
                    spacing: 30

                    // age slider
                    ColumnLayout {
                        Layout.fillWidth: true
                        spacing: 0

                        Label {
                            text: ageLabel.text
                            color: "#fff"
                            font {
                                pixelSize: 11
                                letterSpacing: 11 * 25 / 1000
                                capitalization: Font.AllUppercase
                            }
                            background: Item {}

                            BilingualText {
                                id: ageLabel
                                textEn: "Age"
                                textFr: "Âge"
                            }
                        }

                        WidgetAnswerSlider {
                            textLeft: "0"
                            textRight: "120"
                            showNumber: true
                            sliderFrom: 0
                            sliderTo: 120
                            sliderValue: 18
                            fullWidth: true
                        }
                    }

                    Repeater {
                        // model
                        model: ListModel {
                            ListElement {
                                sectionTitleEn: "Culture"
                                sectionTitleFr: "Culture"
                                filters: [
                                    ListElement { textEn: "All"; textFr: "Tous" },
                                    ListElement { textEn: "Quebecois"; textFr: "Québécoise" },
                                    ListElement { textEn: "Canadian"; textFr: "Canadienne" },
                                    ListElement { textEn: "Native"; textFr: "Autochtone" },
                                    ListElement { textEn: "American"; textFr: "Américaine" },
                                    ListElement { textEn: "European"; textFr: "Européenne" },
                                    ListElement { textEn: "Other"; textFr: "Autre" }
                                ]
                            }

                            ListElement {
                                sectionTitleEn: "Gender"
                                sectionTitleFr: "Genre"
                                filters: [
                                    ListElement { textEn: "All"; textFr: "Tous" },
                                    ListElement { textEn: "Male"; textFr: "Homme" },
                                    ListElement { textEn: "Female"; textFr: "Femme" },
                                    ListElement { textEn: "Other"; textFr: "Autre" }
                                ]
                            }

                            ListElement {
                                sectionTitleEn: "Time"
                                sectionTitleFr: "Temps"
                                filters: [
                                    ListElement { textEn: "All"; textFr: "Tous" },
                                    ListElement { textEn: "Today"; textFr: "Aujourd'hui" },
                                    ListElement { textEn: "This year"; textFr: "Cette année" },
                                    ListElement { textEn: "From the beginning"; textFr: "Depuis le début" }
                                ]
                            }
                        }

                        // display
                        ColumnLayout {
                            id: subfilter

                            property int currentIndex: 0
                            // reset index when RFID tag changes
                            property variant resetter: window.rfidTag
                            onResetterChanged: currentIndex = 0

                            Layout.fillWidth: true

                            spacing: 15

                            // section title
                            Label {
                                text: sectionTitle.text
                                color: "#fff"
                                font {
                                    pixelSize: 11
                                    letterSpacing: 11 * 25 / 1000
                                    capitalization: Font.AllUppercase
                                }
                                background: Item {}

                                BilingualText {
                                    id: sectionTitle
                                    textEn: sectionTitleEn
                                    textFr: sectionTitleFr
                                }
                            }

                            Flow {
                                Layout.fillWidth: true
                                Layout.leftMargin: -10
                                spacing: 10

                                Repeater {
                                    model: filters

                                    // filter button
                                    WidgetFilterButton {
                                        label: BilingualText { textEn: model.textEn; textFr: model.textFr }

                                        checked: index === subfilter.currentIndex
                                        onClicked: subfilter.currentIndex = index
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        // filter navigation
        RowLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignRight | Qt.AlignTop
            Layout.topMargin: 50
            Layout.rightMargin: 100
            visible: datavizIndex > 0

            RoundButton {
                text: qsTr("Left")

                onClicked: {
                    if (hasMultipleQuestions) datavizIndex = index_QUESTIONS;
                    else datavizIndex--;
                }
            }
            RoundButton {
                text: qsTr("Right")
                enabled: datavizIndex !== index_CHOOSE_MULTIPLE
                opacity: enabled

                onClicked: datavizIndex++
            }
        }
    }


}
