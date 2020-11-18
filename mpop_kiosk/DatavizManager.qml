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

    readonly property var modelEthnicities: ModelEthnicities {}
    readonly property var modelGenders: ModelGenders {}

    function getTextForGender(gender) {
        var language = window.lang;
        var ret = gender;
        if (language === 'en') {
            ret = modelGenders.findQuestion(gender).text_en;
        } else {
            ret = modelGenders.findQuestion(gender).text_fr;
        }
        return ret;
    }

    function getTextForEthnicity(key) {
        var language = window.lang;
        var ret = key;
        if (language === 'en') {
            ret = modelEthnicities.findQuestion(key).text_en;
        } else {
            ret = modelEthnicities.findQuestion(key).text_fr;
        }
        return ret;
    }

    function getTextForQuestion(key) {
        var language = window.lang;
        var ret = key;
        /*
          FIXME: modelQuestions.findQuestion doesn't work for subquestions.
        if (language === 'en') {
            ret = modelQuestions.findQuestion(key).text_en;
            console.log("find question " + key + " " + ret);
        } else {
            ret = modelQuestions.findQuestion(key).text_fr;
        }
        */

        var subtitle_fr = "";
        var subtitle_en = "";
        if (key === "equitable_victimes") {
            subtitle_fr = "les victimes";
            subtitle_en = "victims";
        } else if (key ===  "equitable_vulnerables") {
            subtitle_fr = "les personnes marginalisées";
            subtitle_en = "marginalized people";
        } else if (key ===  "equitable_jeunes_contrevenants") {
            subtitle_fr = "les jeunes contrevenants";
            subtitle_en = "young offenders";
        } else if (key ===  "equitable_riches") {
            subtitle_fr = "les gens fortunés";
            subtitle_en = "wealthy people";
        } else if (key === "equitable_minorites_culturelles") {
            subtitle_fr = "les minorités visibles ou culturelles";
            subtitle_en = "visible or cultural minorities";
        } else if (key ===  "soins_physiques") {
            subtitle_fr = "Santé physique";
            subtitle_en = "Physical health";
        } else if (key ===  "soins_mentaux") {
            subtitle_fr =  "Santé mentale";
            subtitle_en = "Mental health";
        } else if (key === "investir_education") {
            subtitle_fr = "L’éducation?";
            subtitle_en = "Education?";
        } else if (key ===  "investir_sante_services_sociaux") {
            subtitle_fr = "La santé et les services sociaux?";
            subtitle_en = "Health and Social Services?";
        } else if (key ===  "investir_emploi") {
            subtitle_fr = "Les programmes liés à l’emploi?";
            subtitle_en =  "Job-related programs?";
        }
        if (language === "fr") {
            ret = subtitle_fr;
        } else {
            ret = subtitle_en;
        }

        return ret;
    }

    function getTextForLanguage(key) {
        var language = window.lang;
        var results = {
            "en": { "en": "English", "fr": "Anglais" },
            "fr": { "en": "French", "fr": "Français" }
        };
        var ret = key;
        ret = results[language][key];
        return ret;
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
            args.push(getTextForGender(answer.title));
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
            args.push(getTextForEthnicity(answer.title));
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
            args.push(getTextForLanguage(answer.title));
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
     * /my_answers <multiple args>
     *
     * @param value Array List of ints for my current answers. There should be between 2 and 5 answers.
     */
    function my_answers(values) {
        var path = _makePath("my_answers");
        _verb(path, values);
        oscMessageSender.send(path, values);
    }


    function _verb(path, args) {
        // console.log("Send OSC: " + path + " " + args.toString());
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
        // num answers
        var args = [numAnswers];
        for (var i = 0; i < numAnswers; i ++) {
            var answer = values[i];
            // text, my answer, their answer
            args.push(getTextForQuestion(answer.title));
            args.push(answer.mine);
            args.push(answer.theirs);
        }
        // console.log(_makePath("view_answers") + " " + args);
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
}
