import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

/**
 * Page that contains a single question.
 */
Column {

    id: thisPage

    function getIndexIsQuestions() {
        return datavizIndex === index_QUESTIONS;
    }

    function goToIndexQuestion() {
        datavizIndex = index_QUESTIONS;
    }

    function goToIndexDataviz() {
        if (hasMultipleQuestions) {
            datavizIndex = index_CHOOSE_MULTIPLE;
        }
        else {
            datavizIndex = index_CHOOSE_SINGLE;
        }
    }

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


    /**
     * Retrieves the question identifiers.
     *
     * @ret the question identifire if its single type or list of identifires  for mutiple type.
     */
    function getQuestionIdentifiers() {
        var ret = [];
        for (var i = 0; i < numberOfQuestions; i ++) {
            var key = model.subquestions ? model.subquestions.get(i).identifier : model.identifier;
            ret.push(key);
        }
        return ret;
    }

    /**
     * Retrieves the list of questions answers.
     *
     * @param questionsIds is List of question ids.
     * @ret the answers of question identifires and set -1 in case user not answered the question.
     */
    function getQuestionsAnswers(questionsIds) {
        var ret = [];
        var answer ;
        for (var i = 0; i < questionsIds.length; i ++){
            if(window.userProfile.answers.hasOwnProperty(questionsIds[i])){
                answer = window.userProfile.answers[questionsIds[i]];
            }
            else{
                 answer=-1;
             }
            ret.push(answer);
        }
        return ret;
    }

    /**
     * Retrieves the object list of titles, myanswers and their answers avg.
     *
     * @param myAnswers is List of user answers.
     * @param answers is list of users answers average.
     * @ret is object of titles, myanswer and their answers.
     */
    function makeMultipleQuestionTitle(myAnswers, answers){
        var ret = [];

        var titleSize= window.userProfile.getObjectLength(answers);
        var objectPropertiesNames = Object.getOwnPropertyNames(answers);
        for (var i = 0; i < titleSize; i ++) {
            var answerTitle = window.datavizManager.makeTitleMineTheirs(objectPropertiesNames[i],myAnswers[i],answers[objectPropertiesNames[i]]);
            ret.push(answerTitle);
        }
        return ret;
    }

    /**
     * Retrieves the object list of titles and their answers avg.
     *
     * @param titleAnswers is list of users answers average.
     * @ret is object of titles and their answers.
     */
    function packAnswersTitle(titleAnswers){
        var ret = [];

        var titleSize= window.userProfile.getObjectLength(titleAnswers);
        var objectPropertiesNames = Object.getOwnPropertyNames(titleAnswers);
        for (var i = 0; i < titleSize; i ++) {
           var answerTitle = window.datavizManager.makeTitleTheirs(objectPropertiesNames[i],titleAnswers[objectPropertiesNames[i]])
           ret.push(answerTitle);
        }

        return ret;
    }

    function resetToDefaultAnswer() {
        for (var i = 0; i < numberOfQuestions; i ++) {
            var value = 50; // default
            sliderRepeater.itemAt(i).sliderValue = value;
        }
    }

    /**
     * Called when the answer changes their answer for a question using a slider.
     */
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

        currentAnswers[sliderIndex] = value;

        console.log("Has Multiple Question :" + hasMultipleQuestions);
        if (hasMultipleQuestions) {
            sendDatavizShowMyAnswers(currentAnswers);
        } else {
            sendDatavizShowMyAnswer(value);
        }
    }

    /**
     * Sends OSC to the dataviz to show my one (1) answer.
     */
    function sendDatavizShowMyAnswer(currentValue) {
        window.datavizManager.my_answer(currentValue);
    }

    /**
     * Sends OSC to the dataviz to show my multiple answers.
     */
    function sendDatavizShowMyAnswers(currentValues) {
        window.datavizManager.my_answers(currentValues);
    }

    /**
     * Show/hide the dataviz section.
     */
    function toggleDataviz() {
        if (datavizIndex === index_QUESTIONS) {
            goToIndexDataviz();
        } else {
            goToIndexQuestion();
        }
    }

    // property var modelQuestions: null
    property var questionModel: model
    property string titleText: ""
    property int numberOfQuestions: subquestions ? subquestions.rowCount() : 1 // [1,5]
    property bool hasMultipleQuestions: numberOfQuestions > 1
    property var questionIdentifiers: []
    property var currentAnswers: []
    property int filterHighlighted: -1
    property bool buttonTextHighlight: true

    // properties so that we can read what are the current values for each filter
    property string currentFilterValueForEthnicity: "all"
    property string currentFilterValueForGender: "all"
    property string currentFilterValueForTime: "all"
    property int currentFilterValueAgeFrom: 0
    property int currentFilterValueAgeTo: 100

    // Current sub-page index in the dataviz for this question page
    property alias datavizIndex: questionDatavizStackLayout.currentIndex

    // Index of each sub-page:
    readonly property int index_QUESTIONS: 0
    readonly property int index_CHOOSE_SINGLE: 1
    readonly property int index_CHOOSE_MULTIPLE: 2

    Component.onCompleted: {
        loadAnswersForCurrentVisitor();

        for (var i = 0; i < numberOfQuestions; i ++) {
            var default_value_for_sliders = 50;
            currentAnswers.push(default_value_for_sliders);
        }
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

        // XXX: this stack layout is not used at all.
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
            id: multiFilterSelection
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

                var questionId = model.identifier;
                var ageFrom = currentFilterValueAgeFrom;
                var ageTo = currentFilterValueAgeTo;
                var gender = currentFilterValueForGender;
                var ethnicity = currentFilterValueForEthnicity;
                var timeAnswered = currentFilterValueForTime;

                switch (filterHighlighted) {
                case filter_AGE:
                    // call get answer by age
                    window.userProfile.getAnswerByAge(questionId, ethnicity, gender, timeAnswered, function (err, answerByAge) {
                        if (err) {
                            console.log("Error calling getAnswerByAge(" + questionId + "," + ethnicity + "," + gender + "," + timeAnswered + "): " + err.message);
                        } else {
                            // Retrieve my answer and my age:
                            var myAnswer = window.userProfile.getMyAnswer(questionId);
                            var myAge = window.userProfile.age;
                            // answerByAge is a list of 20 values
                            console.log("show_one_answer_by_age(" + myAnswer + ", " + myAge + ", " + answerByAge + ")");
                            window.datavizManager.show_one_answer_by_age(myAnswer, myAge, answerByAge);
                        }
                    });
                    break;
                case filter_CULTURE:
                    // call get answer by culture
                    window.userProfile.getAnswerByEthnicity(questionId, ageFrom, ageTo, gender, timeAnswered, function (err, answerByEthnicity) {
                        if (err) {
                            console.log("Error calling getAnswerByEthnicity(" + questionId + "," + ageFrom + "," + ageTo + "," + gender + "," + timeAnswered + "): " + err.message);
                        } else {
                            // Retrieve my answer and my Ethnicity:
                            var myAnswer = window.userProfile.getMyAnswer(questionId);
                            var myEthnicity = window.userProfile.getMyEthnicity(answerByEthnicity);
                            console.log("show_one_answer_by_ethnicity(" + myAnswer + ", " + myEthnicity + ", " +  JSON.stringify(answerByEthnicity)  + ")");
                            // pack the title and their answers.
                            var theirTitles= packAnswersTitle(answerByEthnicity);
                            window.datavizManager.show_one_answer_by_ethnicity(myEthnicity, myAnswer, theirTitles);
                        }
                    });
                    break;
                case filter_GENDER:
                    // call get answer by gender
                    window.userProfile.getAnswerByGender(questionId,ethnicity,ageTo,ageFrom,timeAnswered,function (err, answerByGender) {
                        if (err) {
                            console.log("Error calling getAnswerByGender(" + questionId +  "," + ethnicity + "," + ageTo + "," + ageFrom + "," + timeAnswered + "): " + err.message);
                        } else {
                            // Retrieve my answer and my age:
                            var myAnswer = window.userProfile.getMyAnswer(questionId);
                            var myGender = window.userProfile.getMyGender(answerByGender);
                            // answerByGender is a list of 3 values
                            console.log("show_one_answer_by_Gender(" + myAnswer + ", " + myGender + ", " + answerByGender + ")");
                            // pack the title and their answers.
                            var theirTitles= packAnswersTitle(answerByGender);
                            window.datavizManager.show_one_answer_by_gender(myGender, myAnswer, theirTitles);
                        }
                    });
                    break;
                case filter_LANGUAGE:
                    // call get answer by language
                    window.userProfile.getUserAnswerByLanguage(questionId,ageFrom,ageTo,ethnicity,gender,timeAnswered,function (err, answerByLanguage) {
                        if (err) {
                            console.log("Error calling getUserAnswerByLanguage(" + questionId +  "," + ethnicity + "," + ageTo + "," + ageFrom + "," + gender + "," + timeAnswered + "): " + err.message);
                        } else {
                            // Retrieve my answer and my age:
                            var myAnswer = window.userProfile.getMyAnswer(questionId);
                            var myLanguage = window.userProfile.getMyLanguage(answerByLanguage);
                            // answerByLanguage is a list of 2 values
                            console.log("show_one_answer_by_language(" + myAnswer + ", " + myLanguage + ", " + answerByLanguage + ")");
                            // pack the title and their answers.
                            var theirTitles= packAnswersTitle(answerByLanguage);
                            window.datavizManager.show_one_answer_by_language(myLanguage, myAnswer, theirTitles);
                        }
                    });
                    break;
                default: // in case this is a multiple question pageAge
                    var questionIds = getQuestionIdentifiers();
                    // call get answers
                    window.userProfile.getAnswers(questionIds, ageFrom, ageTo, ethnicity, gender, timeAnswered, function (err, answers) {
                        if (err) {
                            console.log("Error calling getAnswers(" + questionIds +  "," + ethnicity + "," + ageTo + "," + ageFrom + "," + gender + "," + timeAnswered + "): " + err.message);
                        } else {
                            // Retrieve my answer
                            var myAnswers = getQuestionsAnswers(questionIds);
                            console.log("view_answers " + myAnswers + "," + JSON.stringify(answers) + "");
                            // pack the title, my answers and their answers.
                            var answersTitles = makeMultipleQuestionTitle(myAnswers, answers);
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
                            currentFilterValueAgeFrom = valueFrom;
                            multiFilterSelection.sendDatavizViewBy();
                        }
                        onValueToChanged: {
                            currentFilterValueAgeTo = valueTo;
                            multiFilterSelection.sendDatavizViewBy();
                        }
                    }
                }

                /**
                 * We use a repeater for the filter buttons.
                 *
                 * There is a model, and we repeat
                 */
                Repeater {
                    id: filterRepeater

                    // Model for the filter buttons. (choose culture, gender, time, etc.)
                    model: ListModel {
                        ListElement {
                            sectionTitleEn: "Culture"
                            sectionTitleFr: "Culture"
                            sectionId: "culture"
                            // TODO: Whenever the current selection changes, call sendDatavizViewBy();
                            filters: [
                                // FIXME: This model might be a duplicate
                                ListElement { textEn: "All"; textFr: "Tous"; filterValue: "all"  },
                                ListElement { textEn: "Quebecois"; textFr: "Québécoise"; filterValue: "quebecer" },
                                ListElement { textEn: "Canadian"; textFr: "Canadienne"; filterValue: "canadian" },
                                ListElement { textEn: "Native"; textFr: "Autochtone"; filterValue: "native" },
                                ListElement { textEn: "American"; textFr: "Américaine"; filterValue: "american" },
                                ListElement { textEn: "European"; textFr: "Européenne"; filterValue: "european" },
                                ListElement { textEn: "Other"; textFr: "Autre"; filterValue: "other" }
                            ]
                        }

                        ListElement {
                            sectionTitleEn: "Gender"
                            sectionTitleFr: "Genre"
                            sectionId: "gender"
                            // TODO: Whenever the current selection changes, call sendDatavizViewBy();
                            filters: [
                                // FIXME: This model might be a duplicate
                                ListElement { textEn: "All"; textFr: "Tous"; filterValue: "all" },
                                ListElement { textEn: "Male"; textFr: "Homme"; filterValue: "male" },
                                ListElement { textEn: "Female"; textFr: "Femme"; filterValue: "female" },
                                ListElement { textEn: "Other"; textFr: "Autre"; filterValue: "other" }
                            ]
                        }

                        ListElement {
                            sectionTitleEn: "Time"
                            sectionTitleFr: "Temps"
                            sectionId: "time"
                            // TODO: Whenever the current selection changes, call sendDatavizViewBy();
                            filters: [
                                // FIXME: This model might be a duplicate
                                ListElement { textEn: "All"; textFr: "Tous"; filterValue: "all" },
                                ListElement { textEn: "Today"; textFr: "Aujourd'hui"; filterValue: "today" },
                                ListElement { textEn: "This year"; textFr: "Cette année"; filterValue: "this_year" },
                                ListElement { textEn: "From the beginning"; textFr: "Depuis le début"; filterValue: "all" } // FIXME: This button is a duplicate of "All"
                            ]
                        }
                    }

                    // Layout for each row of filter buttons.
                    ColumnLayout {
                        id: subfilter

                        property int currentIndex: 0 // To highlight the currently selected button.
                        // reset index when RFID tag changes
                        property variant resetter: window.rfidTag
                        onResetterChanged: currentIndex = 0

                        Layout.fillWidth: true
                        spacing: 15
                        visible: filterHighlighted !== (model.index + 1) || model.index === (filterRepeater.count - 1)

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
                                        switch (sectionId) {
                                        case "time":
                                            currentFilterValueForTime = model.filterValue;
                                            break;
                                        case "gender":
                                            currentFilterValueForGender = model.filterValue;
                                            break;
                                        case "culture":
                                            currentFilterValueForEthnicity = model.filterValue;
                                            break;
                                        }

                                        subfilter.currentIndex = index; // Sets this button as highlighted.
                                        multiFilterSelection.sendDatavizViewBy();
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
            visible: datavizIndex > 1 && !hasMultipleQuestions

            Item { Layout.fillWidth: true }

            // Button to go back to the question main question page.
            WidgetIconButton {
                Layout.leftMargin: 30
                Layout.rightMargin: 30
                Layout.topMargin: 50
                Layout.alignment: Qt.AlignRight

                BilingualText {
                    id: goBackLabel

                    textEn: "Back"
                    textFr: "Retour"
                }

                buttonSize: 50
                iconRotation: -90
                labelText: goBackLabel.text
                onClicked: {
                    if (hasMultipleQuestions) {
                        datavizIndex = index_QUESTIONS;
                    } else {
                        datavizIndex --;
                    }
                }
            }
        }
    }
}
