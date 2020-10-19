import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

/**
 * Page that contains a single question.
 */
Column {

    id: thisPage

    function loadAnswersForCurrentVisitor() {
        // TODO: Retrieve value for user from service and populate the slider, if set.
        for (var i = 0; i < numberOfQuestions; i ++) {
            var key = model.subquestions ? model.subquestions.get(i).identifier : model.identifier;
            var value = 50; // default
            if (window.userProfile.answers.hasOwnProperty(key)) {
                value = window.userProfile.answers[key];
            }

            sliderRepeater.itemAt(i).sliderValue = value;
        }
    }

    function getQuestionIdentifiers() {
        var ret = [];
        for (var i = 0; i < numberOfQuestions; i ++) {
            var key = model.subquestions ? model.subquestions.get(i).identifier : model.identifier;
            ret.push(key);
        }
        return ret;
    }


    function resetToDefaultAnswer() {
        for (var i = 0; i < numberOfQuestions; i ++) {
            var value = 50; // default
            sliderRepeater.itemAt(i).sliderValue = value;
        }
    }

    function handleSliderMoved(sliderIndex, value) {
        console.log("handleSliderMoved(" + sliderIndex + "," + value + ")");
        var identifier = model.identifier || model.subquestions.get(sliderIndex).identifier;
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
        if (datavizIndex !== index_QUESTIONS) {
            datavizIndex = index_QUESTIONS;
        } else {
            // change index depending on quantity of questions
            if (hasMultipleQuestions) datavizIndex = index_CHOOSE_MULTIPLE;
            else datavizIndex = index_CHOOSE_SINGLE
        }
    }

    // property var modelQuestions: null
    property var questionModel: model
    property string titleText: ""
    property int numberOfQuestions: subquestions ? subquestions.rowCount() : 1 // [1,5]
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

    Component.onCompleted: {
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
            BilingualText {
                id: mainQuestionText
                textEn: model.question_en
                textFr: model.question_fr
            }

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

                Repeater {
                    id: sliderRepeater
                    model: subquestions || 1

                    ColumnLayout {
                        spacing: 15

                        property alias sliderValue: answerSlider.sliderValue

                        BilingualText {
                            id: leftText
                            textEn: questionModel.min_en
                            textFr: questionModel.min_fr
                        }

                        BilingualText {
                            id: rightText
                            textEn: questionModel.max_en
                            textFr: questionModel.max_fr
                        }

                        BilingualText {
                            id: questionText
                            textEn: subtitle_en || ""
                            textFr: subtitle_fr || ""
                        }

                        Label {
                            Layout.alignment: Qt.AlignLeft
                            text: questionText.text
                            visible: questionText.text
                            font {
                                family: "Red Hat Display"
                                weight: Font.Medium
                                pixelSize: 24
                                letterSpacing: 24 * 10 / 1000
                            }
                            color: invertedTheme ? Palette.lightBlack : Palette.white
                        }
                        WidgetAnswerSlider {
                            id: answerSlider

                            sliderValue: 35
                            textLeft: leftText.text
                            textRight: rightText.text
                            showNumber: false

                            onSliderMoved: {
                                thisPage.handleSliderMoved(model.index, intValue);
                            }
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

            readonly property int filter_AGE: 0
            readonly property int filter_CULTURE: 1
            readonly property int filter_GENDER: 2
            readonly property int filter_LANGUAGE: 3

            /**
             * Gathers the values from all widgets.
             * Retrieves the info from the service.
             * Triggers sending OSC to the dataviz.
             */
            function sendDatavizViewBy() {
                switch (filterHighlighted) {
                case filter_AGE:
                    // call get answer by age
                    var questionId = model.identifier;
                    var ethnicity = "all"; // FIXME load from widgets
                    var gender = "all"; // FIXME load from widgets
                    var timeAnswered = "all"; // FIXME load from widgets
                    window.userProfile.getAnswerByAge(questionId, ethnicity, gender, timeAnswered, function (err, answerByAge) {
                        if (err) {
                            console.log("Error calling getAnswerByAge(" + questionId + "," + ethnicity + "," + gender + "," + timeAnswered + "): " + err.message);
                        } else {
                            // Retrieve my answer and my age:
                            var myAnswer = window.userProfile.answers[questionId];
                            var myAge = window.userProfile.age; // FIXME: it might be -1
                            // answerByAge is a list of 20 values
                            console.log("show_one_answer_by_age(" + myAnswer + ", " + myAge + ", " + answerByAge + ")");
                            window.datavizManager.show_one_answer_by_age(myAnswer, myAge, answerByAge);
                        }
                    });
                    break;
                case filter_CULTURE:
                    // TODO window.userProfile. call get answer by culture
                    var questionId = model.identifier;
                    var ageFrom = -1; // FIXME load from widgets
                    var ageTo= -1;
                    var gender = "all"; // FIXME load from widgets
                    var timeAnswered = "all"; // FIXME load from widgets

                    window.userProfile.getAnswerByEthnicity(questionId,ageFrom,ageTo,gender,timeAnswered,function (err, answerByEthnicity) {
                        if (err) {
                            console.log("Error calling getAnswerByEthnicity(" + questionId + "," + ageFrom + "," + ageTo + "," + gender + "," + timeAnswered + "): " + err.message);
                        } else {
                            // Retrieve my answer and my age:
                            var myAnswer = window.userProfile.answers[questionId];
                            var myEthnicity = window.userProfile.ethnicity; // FIXME: it might be -1
                            console.log("show_one_answer_by_age(" + myAnswer + ", " + myEthnicity + ", " + answerByEthnicity + ")");
                            window.datavizManager.show_one_answer_by_ethnicity(myAnswer, myEthnicity, answerByEthnicity);
                        }
                    });
                    break;
                case filter_GENDER:
                    // TODO window.userProfile. call get answer by gender
                    var questionId = model.identifier;
                    var ageFrom = -1; // FIXME load from widgets
                    var ageTo= -1;
                    var ethnicity = "all"; // FIXME load from widgets
                    var timeAnswered = "all"; // FIXME load from widgets

                    window.userProfile.getAnswerByGender(questionId,ethnicity,ageTo,ageFrom,timeAnswered,function (err, answerByGender) {
                        if (err) {
                            console.log("Error calling getAnswerByGender(" + questionId +  "," + ethnicity + "," + ageTo + "," + ageFrom + "," + timeAnswered + "): " + err.message);
                        } else {
                            // Retrieve my answer and my age:
                            var myAnswer = window.userProfile.answers[questionId];
                            var myGender = window.userProfile.gender; // FIXME: it might be -1
                            // answerByAge is a list of 20 values
                            console.log("show_one_answer_by_age(" + myAnswer + ", " + myGender + ", " + answerByGender + ")");
                            window.datavizManager.show_one_answer_by_gender(myAnswer, myGender, answerByGender);
                        }
                    });
                    break;
                case filter_LANGUAGE:
                    // TODO  window.userProfile. call get answer by lang
                    var questionId = model.identifier;
                    var ageFrom = -1; // FIXME load from widgets
                    var ageTo= -1;
                    var gender = "all";
                    var ethnicity = "all"; // FIXME load from widgets
                    var timeAnswered = "all"; // FIXME load from widgets

                    window.userProfile.getUserAnswerByLanguage(questionId,ageFrom,ageTo,ethnicity,gender,timeAnswered,function (err, answerByLanguage) {
                        if (err) {
                            console.log("Error calling getUserAnswerByLanguage(" + questionId +  "," + ethnicity + "," + ageTo + "," + ageFrom + "," + gender + "," + timeAnswered + "): " + err.message);
                        } else {
                            // Retrieve my answer and my age:
                            var myAnswer = window.userProfile.answers[questionId];
                            var myLanguage = window.userProfile.language; // FIXME: it might be -1
                            // answerByAge is a list of 20 values
                            console.log("show_one_answer_by_language(" + myAnswer + ", " + myLanguage + ", " + answerByLanguage + ")");
                            window.datavizManager.show_one_answer_by_language(myAnswer, myLanguage, answerByLanguage);
                        }
                    });
                    break;
                default: // in case this is a multiple question pageAge
                    // TODO window.userProfile. call get answers

                    // TODO  window.userProfile. call get answer by lang
                    var questionId = getQuestionIdentifiers();
                    var ageFrom = -1; // FIXME load from widgets
                    var ageTo= -1;
                    var gender = "all";
                    var ethnicity = "all"; // FIXME load from widgets
                    var timeAnswered = "all"; // FIXME load from widgets

                    window.userProfile.getAnswers(questionId,ageFrom,ageTo,ethnicity,gender,timeAnswered,function (err, answers) {
                        if (err) {
                            console.log("Error calling getAnswers(" + questionId +  "," + ethnicity + "," + ageTo + "," + ageFrom + "," + gender + "," + timeAnswered + "): " + err.message);
                        } else {
                            // Retrieve my answer and my age:
                            var myAnswer = window.userProfile.answers[questionId];
                            // answerByAge is a list of 20 values
                            console.log("show_one_answer(" + myAnswer + "," + answers[questionId] + "");
                            var answersTitles=  window.datavizManager.makeTitleMineTheirs(questionId,myAnswer,answers)
                            window.datavizManager.view_answers(answersTitles);
                        }
                    });
                    break;
                }
            }

            onVisibleChanged: {
                if (visible) {
                    sendDatavizViewBy();
                }
            }

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
                            sendDatavizViewBy();
                        }
                        onValueToChanged: {
                            // TODO
                            sendDatavizViewBy();
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
                            // TODO: Whenever the current selection changes, call sendDatavizViewBy();
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
                            // TODO: Whenever the current selection changes, call sendDatavizViewBy();
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
                            // TODO: Whenever the current selection changes, call sendDatavizViewBy();
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
                                    onClicked: {
                                        subfilter.currentIndex = index;
                                        sendDatavizViewBy();
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
