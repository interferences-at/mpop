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

    function show_one_answer_by_gender() {
        // TODO
    }

    function show_one_answer_by_ethnicity() {
        var args = [];
        // TODO
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
        oscMessageSender.send(_makePath("my_answer"), values);
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
    function makeAnswer(text, myAnswer, theirAnswer) {
        var ret = {
            text: "",
            mine: myAnswer,
            theirs: theirAnswer
        };
        return ret;
    }


    /**
     * View the answers for a multiple-question page.
     *
     * @param values Array List of answers, created with makeAnswer.
     */
    function view_answers(values) {
        var numAnswers = values.length;
        var args = [numAnswers];
        for (var i = 0; i < numAnswers; i ++) {
            var answer = values[i];
            args.push(answer.text);
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
