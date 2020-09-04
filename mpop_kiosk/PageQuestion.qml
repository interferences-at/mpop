import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

/**
 * Page that contains a single question.
 */
Column {

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
        for (var i = 0; i < numberOfQuestions; i ++) {
            var key = questionIdentifiers[i];
            var value = 50; // default
            if (window.userProfile.answers.hasOwnProperty(key)) {
                value = window.userProfile.answers[key];
            }
            switch (i) {
            case 0:
                answerSlider1.sliderValue = value;
                break;
            case 1:
                answerSlider2.sliderValue = value;
                break;
            case 2:
                answerSlider3.sliderValue = value;
                break;
            case 3:
                answerSlider4.sliderValue = value;
                break;
            case 4:
                answerSlider5.sliderValue = value;
                break;
            }
        }
    }


    function handleSliderMoved(sliderIndex, value) {
        console.log("handleSliderMoved(" + sliderIndex + "," + value + ")");
        var identifier = questionIdentifiers[sliderIndex];
        var userId = window.userProfile.userId;

        window.userProfile.setUserAnswer(userId, identifier, value, function (err, user_id) {
            if (err) {
                console.log("Error calling setUserAnswer(" + userId + "," + identifier + "," + value + "): " + err.message);
            } else {
                console.log("Success calling setUserAnswer(" + userId + "," + identifier + "," + value + ")");
            }
        });

        sendDatavizShowQuestion(value);
    }

    function sendDatavizShowQuestion(currentValue) {
        window.datavizManager.my_answer(currentValue);
    }

    /**
     * Show/hide the dataviz section.
     */
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
    property int filterHighlighted: -1
    property bool buttonTextHighlight: true

    property alias datavizIndex: questionDatavizStackLayout.currentIndex
    readonly property int index_QUESTIONS: 0
    readonly property int index_CHOOSE_SINGLE: 1
    readonly property int index_CHOOSE_MULTIPLE: 2

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

    // The main question text:
    ColumnLayout {
        id: mainQuestion
        width: parent.width

        Label {
            Layout.fillWidth: true
            text: mainQuestionText.text
            font {
                pixelSize: 45
                capitalization: Font.AllUppercase
            }
            wrapMode: Label.WordWrap
            color: invertedTheme ? Palette.lightBlack : Palette.white

            leftPadding: 40
            rightPadding: 40
            topPadding: 90
            bottomPadding: 70
        }
    }

    /**
     * StackLayout to switch from answering the question to data viz.
     */
    ColumnLayout {
        width: parent.width

        StackLayout {
            id: questionDatavizStackLayout
            visible: false
            Layout.fillWidth: true
            currentIndex: 0
        }

        Item {
            Layout.fillWidth: true
            Layout.preferredHeight: 110
            Layout.bottomMargin: 15
            visible: datavizIndex > 0
            Rectangle {
                width: parent.width
                height: 1
                color: Palette.white
                anchors.bottom: parent.bottom
            }
        }

        // Index 0: answering the questions
        ColumnLayout {
            spacing: 0
            visible: datavizIndex === index_QUESTIONS

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
                        color: invertedTheme ? Palette.lightBlack : Palette.white

                        visible: hasMultipleQuestions
                    }
                    WidgetAnswerSlider {
                        id: answerSlider1

                        sliderValue: 35
                        textLeft: leftText1.text
                        textRight: rightText1.text
                        showNumber: false

                        onSliderMoved: {
                            thisPage.handleSliderMoved(0, intValue);
                        }
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
                        color: invertedTheme ? Palette.lightBlack : Palette.white

                        visible: hasMultipleQuestions && (numberOfQuestions >= 2)
                    }
                    WidgetAnswerSlider {
                        id: answerSlider2

                        sliderValue: 35
                        textLeft: leftText2.text
                        textRight: rightText2.text
                        showNumber: false
                        visible: hasMultipleQuestions && (numberOfQuestions >= 2)

                        onSliderMoved: {
                            thisPage.handleSliderMoved(1, intValue);
                        }
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
                        color: invertedTheme ? Palette.lightBlack : Palette.white

                        visible: hasMultipleQuestions && (numberOfQuestions >= 3)
                    }

                    WidgetAnswerSlider {
                        id: answerSlider3

                        sliderValue: 50
                        textLeft: leftText3.text
                        textRight: rightText3.text
                        showNumber: false
                        visible: hasMultipleQuestions && (numberOfQuestions >= 3)

                        onSliderMoved: {
                            thisPage.handleSliderMoved(2, intValue);
                        }
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
                        color: invertedTheme ? Palette.lightBlack : Palette.white

                        visible: hasMultipleQuestions && (numberOfQuestions >= 4)
                    }

                    WidgetAnswerSlider {
                        id: answerSlider4

                        sliderValue: 50
                        textLeft: leftText4.text
                        textRight: rightText4.text
                        showNumber: false
                        visible: hasMultipleQuestions && (numberOfQuestions >= 4)

                        onSliderMoved: {
                            thisPage.handleSliderMoved(3, intValue);
                        }
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
                        color: invertedTheme ? Palette.lightBlack : Palette.white

                        visible: hasMultipleQuestions && (numberOfQuestions >= 5)
                    }

                    WidgetAnswerSlider {
                        id: answerSlider5

                        sliderValue: 50
                        textLeft: leftText5.text
                        textRight: rightText5.text
                        showNumber: false
                        visible: hasMultipleQuestions && (numberOfQuestions >= 5)

                        onSliderMoved: {
                            thisPage.handleSliderMoved(4, intValue);
                        }
                    }
                }
            }
        }

        // Index 1: single question first axis
        // this screen is automatically skipped if hasMultipleQuestions === true
        ColumnLayout {
            spacing: 0
            visible: datavizIndex === index_CHOOSE_SINGLE

            ColumnLayout {
                Layout.fillWidth: true
                Layout.leftMargin: 40
                Layout.rightMargin: 40
                spacing: 0

                Label {
                    BilingualText {
                        id: firstFilterTooltip
                        textEn: "Please select a basic viewing setting"
                        textFr: "Veuillez choisir un réglage de visualisation de base"
                    }

                    text: firstFilterTooltip.text
                    color: "#fff"
                    font {
                        family: "Red Hat Display"
                        weight: Font.Medium
                        pixelSize: 24
                    }
                    padding: 0
                }
                // 1st-level filters
                RowLayout {
                    Layout.fillWidth: true
                    Layout.topMargin: 20
                    spacing: 24

                    // reset on RFID tag change
                    property variant resetter: window.rfidTag
                    onResetterChanged: filterHighlighted = -1

                    Repeater {
                        model: ListModel {
                            ListElement {
                                text_en: "Age"
                                text_fr: "Âge"
                            }

                            ListElement {
                                text_en: "Culture"
                                text_fr: "Culture"
                            }

                            ListElement {
                                text_en: "Gender"
                                text_fr: "Genre"
                            }

                            ListElement {
                                text_en: "Language"
                                text_fr: "Langue"
                            }
                        }

                        WidgetFilterButton {
                            label: BilingualText { textEn: model.text_en; textFr: model.text_fr }
                            highlighted: filterHighlighted === model.index
                            onClicked: {
                                filterHighlighted = model.index;
                                datavizIndex++;
                            }
                        }
                    }
                }
            }
        }

        // Index 2: multi-filter selection
        // filter rows are selectively hidden depending on first-axis selection
        ColumnLayout {
            spacing: 0
            visible: datavizIndex === index_CHOOSE_MULTIPLE

            ColumnLayout {
                Layout.fillWidth: true
                Layout.leftMargin: 40
                Layout.rightMargin: 40
                spacing: 30

                // age range slider
                ColumnLayout {
                    Layout.fillWidth: true
                    spacing: 0

                    visible: filterHighlighted !== 0

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

                    WidgetAgeRangeSlider {
                        textLeft: "0"
                        textRight: "120"
                        showNumbers: true
                        sliderFrom: 0
                        sliderTo: 120
                        valueFrom: 0
                        valueTo: 120
                        fullWidth: true

                        onValueFromChanged: {
                            // TODO
                        }
                        onValueToChanged: {
                            // TODO
                        }
                    }
                }

                Repeater {
                    id: filterRepeater

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
                        visible: filterHighlighted !== model.index + 1 || model.index === filterRepeater.count - 1

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

                                    highlighted: index === subfilter.currentIndex
                                    onClicked: subfilter.currentIndex = index
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
            visible: datavizIndex > 0

            WidgetIconButton {
                Layout.leftMargin: 30
                Layout.topMargin: 50
                Layout.alignment: Qt.AlignBottom

                BilingualText {
                    id: goBackLabel
                    textEn: "Back"
                    textFr: "Retour"
                }

                buttonSize: 50
                iconRotation: -90
                labelText: goBackLabel.text
                onClicked: {
                    if (hasMultipleQuestions) datavizIndex = index_QUESTIONS;
                    else datavizIndex--;
                }
            }
        }
    }
}
