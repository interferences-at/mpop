import QtQuick 2.0

/**
 * Controls mpop_dataviz by sending OSC messages to it.
 *
 * This is where we implement all OSC message sending.
 *
 * The actual OSC port and hostname to send to is set elsewhere.
 * (see environment variables and/or command-line options in main.cpp)
 */
Item {
    id: thisDatavizManager

    property var oscMessageSender: null // The oscSender instance.
    readonly property int datavizWindowIndex: 0 // You should leave this to 0
    // The translated titles
    readonly property var titles: {
        // Gender
        "male": { "en": "Male", "fr": "Homme" },
        "female": { "en": "Female", "fr": "Femme" },
        "other": { "en": "Other", "fr": "Autre" },
        // Ethnicity
        "quebecer": { "en": "Quebecer", "fr": "Québécoise" },
        "canadian": { "en": "Canadian", "fr": "Canadienne" },
        "american": { "en": "American", "fr": "Américaine" },
        "european": { "en": "European", "fr": "Européenne" },
        "native": { "en": "Native", "fr": "Autochtone" },
        "other": { "en": "Other", "fr": "Autre" },
        // Language
        "en": { "en": "English", "fr": "Anglais" },
        "fr": { "en": "French", "fr": "Français" },
        // Final Questions`
        "investir_education": { "en": "Education?", "fr": "L’éducation?" },
        "investir_sante_services_sociaux": {
            "en": "Health and Social Services?",
            "fr": "La santé et les services sociaux?" },
        "investir_emploi": {
            "en": "Job-related programs?",
            "fr": "Les programmes liés à l’emploi?" },

    }

    readonly property var modelEthnicities: ModelEthnicities {}
    readonly property var modelGenders: ModelGenders {}
    // The translated titles
    readonly property var titles: {
        // Gender
        "male": {
            "en": modelGenders.findQuestion("male").text_en,
            "fr": modelGenders.findQuestion("male").text_fr },
        "female": {
            "en": modelGenders.findQuestion("female").text_en,
            "fr": modelGenders.findQuestion("female").text_fr },
        "other": {
            "en": modelGenders.findQuestion("other").text_en,
            "fr": modelGenders.findQuestion("other").text_fr },
        // Ethnicity
        "quebecer": {
            "en": modelEthnicities.findQuestion("quebecer").text_en,
            "fr": modelEthnicities.findQuestion("quebecer").text_fr },
        "canadian": {
            "en": modelEthnicities.findQuestion("canadian").text_en,
            "fr": modelEthnicities.findQuestion("canadian").text_fr },
        "american": {
            "en": modelEthnicities.findQuestion("american").text_en,
            "fr": modelEthnicities.findQuestion("american").text_fr },
        "european": {
            "en": modelEthnicities.findQuestion("european").text_en,
            "fr": modelEthnicities.findQuestion("european").text_fr },
        "native": {
            "en": modelEthnicities.findQuestion("native").text_en,
            "fr": modelEthnicities.findQuestion("native").text_fr },
        "other": {
            "en": modelEthnicities.findQuestion("other").text_en,
            "fr": modelEthnicities.findQuestion("other").text_fr },
        // Language
        "en": { "en": "English", "fr": "Anglais" },
        "fr": { "en": "French", "fr": "Français" },
        // Final Questions
        "investir_education": {
            "en": modelQuestions.findQuestion("investir_education").subtitle_en,
            "fr": modelQuestions.findQuestion("investir_education").subtitle_fr },
        "investir_sante_services_sociaux": {
            "en": modelQuestions.findQuestion("investir_sante_services_sociaux").subtitle_en,
            "fr": modelQuestions.findQuestion("investir_sante_services_sociaux").subtitle_fr },
        "investir_emploi": {
            "en": modelQuestions.findQuestion("investir_emploi").subtitle_en,
            "fr": modelQuestions.findQuestion("investir_emploi").subtitle_fr },

    }

    /**
     * Shows my current answer.
     *
     * /my_answer
     *
     * @param value int One single value for my current answer.
     */
    function my_answer(value) {
        oscMessageSender.send(_makePath("my_answer"), [value]);
    }

    /**
     * Creates an object to pass in an Array to some other method.
     */
    function makeTitleTheirs(titleText, theirAnswer) {
        var ret = {
            title: titleText,
            theirs: theirAnswer
        };
        return ret;
    }


    /**
     * View an answer to a single question by gender.
     *
     * @param myRow int My row index.
     * @param myAnswer int Value for my answer.
     * @param titleTheirs Array List of objects created using makeTitleTheirs.
     * @see makeTitleTheirs
     */
    function show_one_answer_by_gender(myRow, myAnswer, titleTheirs) {
        var rowCount = titleTheirs.length;
        var args = [rowCount, myRow, myAnswer];
        for (var i = 0; i < rowCount; i ++) {
            var answer = titleTheirs[i];
            args.push(translatedTitle(answer.title));
            args.push(answer.theirs);
        }
        oscMessageSender.send(_makePath("view_answer_by_gender"), args);
    }


    /**
     * View an answer to a single question by ethnicity.
     *
     * @param myRow int My row index.
     * @param myAnswer int Value for my answer.
     * @param titleTheirs Array List of objects created using makeTitleTheirs.
     * @see makeTitleTheirs
     */
    function show_one_answer_by_ethnicity(myRow, myAnswer, titleTheirs) {
        var rowCount = window.userProfile.getObjectLength(titleTheirs);
        var args = [rowCount, myRow, myAnswer];
        for (var i = 0; i < rowCount; i ++) {
            var answer = titleTheirs[i];
            args.push(translatedTitle(answer.title));
            args.push(answer.theirs);
        }
        oscMessageSender.send(_makePath("view_answer_by_culture"), args);
    }


    /**
     * View an answer to a single question by language.
     *
     * @param myRow int My row index.
     * @param myAnswer int Value for my answer.
     * @param titleTheirs Array List of objects created using makeTitleTheirs.
     * @see makeTitleTheirs
     */
    function show_one_answer_by_language(myRow, myAnswer, titleTheirs) {
        var rowCount = titleTheirs.length;
        var args = [rowCount, myRow, myAnswer];
        for (var i = 0; i < rowCount; i ++) {
            var answer = titleTheirs[i];
            args.push(translatedTitle(answer.title));
            args.push(answer.theirs);
        }
        oscMessageSender.send(_makePath("view_answer_by_language"), args);
    }


    /**
     * Sets a screensaver parameter.
     *
     * /screensaver_set_param
     *
     * @param name string Name of the parameter to set.
     * @param value float Value for that parameter. (can be an int or float)
     */
    function screensaver_set_param(name, value) {
        var args = [name, value];
        oscMessageSender.send(_makePath("screensaver_set_param"), args);
    }


    /**
     * View one answer by age.
     *
     * /view_answer_by_age
     *
     * @param myAnswer int My answer
     * @param myAnswer int My age
     * @param values Array List of 20 values - one for each 5-year range.
     */
    function show_one_answer_by_age(myAnswer, myAge, values) {
        var args = [myAnswer, myAge];
        args = args.concat(values);
        oscMessageSender.send(_makePath("view_answer_by_age"), args);
    }

    /**
     * Shows my current answers.
     *
     * /my_answer
     *
     * @param value Array List of ints for my current answers. There should be between 2 and 5 answers.
     */
    function my_answers(values) {
        var path = _makePath("my_answers");
        _verb(path, values);
        oscMessageSender.send(path, values);
    }


    function _verb(path, args) {
        console.log("Send OSC: " + path + " " + args.toString());
    }


    /**
     * Go to the screensaver.
     */
    function goto_screensaver() {
        oscMessageSender.send(_makePath("goto_screensaver"), []);
    }


    /**
     * Creates an object to pass in an Array to view_answers.
     */
    function makeTitleMineTheirs(titleText, myAnswer, theirAnswer) {
        var ret = {
            title: titleText,
            mine: myAnswer,
            theirs: theirAnswer
        };
        return ret;
    }


    /**
     * View the answers for a multiple-question page.
     *
     * @param values Array List of answers, created with makeTitleMineTheirs.
     */
    function view_answers(values) {
        var numAnswers = values.length;
        var args = [numAnswers];
        for (var i = 0; i < numAnswers; i ++) {
            var answer = values[i];
            args.push(translatedTitle(answer.title));
            args.push(answer.mine);
            args.push(answer.theirs);
        }
        oscMessageSender.send(_makePath("view_answers"), args);
    }


    /**
     * Show all answers.
     * @param values Array List of 25 int values.
     */
    function all_answers(values) {
        oscMessageSender.send(_makePath("all_results"), values);
    }

    /**
     * Set Davaviz language
     * @param language identifier
     */
    function set_dataviz_language(lang) {
        oscMessageSender.send(_makePath("language"), [lang]);
    }


    /**
     * Prepares a OSC path.
     * @param suffix String that starts with a slash.
     */
    function _makePath(suffix) {
        var stringStartingWithSlash = suffix;
        if (suffix.charAt(0) !== "/") {
            stringStartingWithSlash = "/" + suffix;
        }
        var ret = "/dataviz/" + datavizWindowIndex + stringStartingWithSlash;
        return ret;
    }

    /**
     * Translate titles
     * @param title key
     */
    function translatedTitle(title) {
        var language = window.lang.toString()
        if (language === '') {
            lang = 'en'; // Just in case
        }

        return titles[title][language];
    }
}
