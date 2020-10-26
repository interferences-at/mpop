import QtQuick 2.0
import QtWebSockets 1.0

/**
 * Informations about the current user.
 *
 * We follow the coding conventions of NodeJS for the callbacks in this file.
 * For callback functions:
 *
 * - Thefirst argument to a callback is an error object or null
 * - The second argument is the result, and there can be more than one.
 * - The function who take a callback always take it as their last argument
 */
Item {
    id: thisUserProfile

    signal userAnswersUpdated()

    // constants
    readonly property int const_NUM_QUESTIONS: 999 // FIXME
    readonly property int const_INVALID_NUMBER: -1
    readonly property string const_INVALID_STRING: ""

    // properties
    // Connection with the mpop_service:
    // These default value will be overriden:
    property int service_port_number: 3333
    property string service_host: "0.0.0.0"
    property bool is_verbose: false
    property bool isConnected: websocket.status === WebSocket.Open
    property bool thisIsInSystemTests: false

    // user's profile:
    property string gender: const_INVALID_STRING
    property string ethnicity: const_INVALID_STRING
    property int age: const_INVALID_NUMBER
    property string language: const_INVALID_STRING
    property var answers: ({}) // JavaScript object - for key-value pairs
    property string rfidTag: const_INVALID_STRING
    property int userId: const_INVALID_NUMBER
    property bool sent_user_info_to_service: false
    property bool did_read_user_info_from_service: false


    // functions

    /**
     * Resets all the properties that are relevant to the current user.
     */
    function resetUser(cb) {
        gender = const_INVALID_STRING;
        ethnicity = const_INVALID_STRING;
        age = const_INVALID_NUMBER;
        language = const_INVALID_STRING;
        answers = {};
        userId = const_INVALID_NUMBER
        rfidTag = const_INVALID_STRING

        for (var i = 0; i < const_NUM_QUESTIONS; i ++) {
            answers[i] = null;
        }
        cb(null);
    }

    /**
     * Sends a request to the mpop_service to set a user's gender, and store its value in a property here.
     * @param string value The gender to set.
     */
    function setUserGender(user_id, value, cb) {
        // TODO: validate arguments
        console.log("Calling setUserGender(" + user_id + "," + value + ")");
        websocket.callRemoteMethod("setUserGender", [user_id, value], function (err, user_id) {
            if (err) {
                console.log("Error calling setUserGender(" + user_id + "," + value + "): " + err.message);
                cb(err);
            } else {
                thisUserProfile.gender = value; // set the property
                cb(null); // call callback with no error
            }
        });
    }

    /**
     * Sends a request to the mpop_service to set a user's language, and store its value in a property here.
     * @param string value The language to set.
     */
    function setUserLanguage(user_id, value, cb) {
        // TODO: validate arguments
        console.log("Calling setUserLanguage(" + user_id + "," + value + ")");
        websocket.callRemoteMethod("setUserLanguage", [user_id, value], function (err, user_id) {
            if (err) {
                console.log("Error calling setUserLanguage(" + user_id + "," + value + "): " + err.message);
                cb(err);
            } else {
                thisUserProfile.language = value; // set the property
                cb(null); // call callback with no error
            }
        });
    }

    /**
     * Sends a request to the mpop_service to set a user's ethnicity, and store its value in a property here.
     * @param string value The ethnicity to set.
     */
    function setUserEthnicity(user_id, value, cb) {
        // TODO: validate arguments
        console.log("Calling setUserEthnicity(" + user_id + "," + value + ")");
        websocket.callRemoteMethod("setUserEthnicity", [user_id, value], function (err, user_id) {
            if (err) {
                console.log("Error calling setUserEthnicity(" + user_id + "," + value + "): " + err.message);
                cb(err);
            } else {
                thisUserProfile.ethnicity = value; // set the property
                cb(null); // call callback with no error
            }
        });
    }

    /**
     * Sends a request to the mpop_service to set a user's age, and store its value in a property here.
     * @param int value The age to set.
     */
    function setUserAge(user_id, value, cb) {
        // TODO: validate arguments
        console.log("Calling setUserAge(" + user_id + "," + value + ")");
        websocket.callRemoteMethod("setUserAge", [user_id, value], function (err, user_id) {
            if (err) {
                console.log("Error calling setUserAge(" + user_id + "," + value + "): " + err.message);
                cb(err);
            } else {
                thisUserProfile.age = value; // set the property
                cb(null); // call callback with no error
            }
        });
    }

    /**
     * Sends a request to the mpop_service to set a user's age, and store its value in a property here.
     * @param int value The age to set.
     */
    function setUserAnswer(user_id, question_identifier, value, cb) {
        // TODO: validate arguments
        console.log("Calling setUserAnswer(" + user_id + "," + question_identifier + "," + value + ")");
        websocket.callRemoteMethod("setUserAnswer", [user_id, question_identifier, value], function (err, user_id) {
            if (err) {
                console.log("Error calling setUserAnswer(" + user_id + "," + question_identifier + "," + value + "): " + err.message);
                cb(err);
            } else {
                thisUserProfile.answers[question_identifier] = value;
                cb(null); // call callback with no error
            }
        });
    }

    /**
     * Calls getAnswerByAge.
     * @param questionId string.
     * @param ethnicity string. (default: all)
     * @param gender string. (default: all)
     * @param timeAnswered string. (default: all)
     * @see Facade.cpp
     */
    function getAnswerByAge(questionId, ethnicity, gender, timeAnswered, cb) {
        // TODO: validate arguments
        console.log("Calling getAnswerByAge(" + questionId + "," + ethnicity + "," + gender + "," + timeAnswered + ")");
        websocket.callRemoteMethod("getAnswerByAge", [questionId, ethnicity, gender,timeAnswered], function (err, ansByAge) {
            if (err) {
                console.log("Error calling getAnswerByAge(" + questionId + "," + ethnicity + "," + gender + " ," + timeAnswered + "): " + err.message);
                cb(err);
            } else {
                // ansByAge is a list of 20 values
                cb(null, ansByAge); // call callback with no error
            }
        });
    }

    /**
     * Calls getAnswerByEthnicity.
     * @param questionId string.
     * @param ageTo int. (default: -1)
     * @param ageFrom int. (default: -1)
     * @param gender string. (default: all)
     * @param timeAnswered string. (default: all)
     * @see Facade.cpp
     */

    function getAnswerByEthnicity(questionId, ageFrom, ageTo, gender, timeAnswered, cb){
        // TODO: validate arguments
        console.log("Calling getAnswerByEthnicity(" + questionId + "," + ageFrom + "," + ageTo + "," + gender + "," + timeAnswered + ")");
        websocket.callRemoteMethod("getAnswerByEthnicity", [questionId, ageFrom, ageTo, gender,timeAnswered], function (err, ansByEthnicity) {
            if (err) {
                console.log("Error calling getAnswerByEthnicity(" + questionId + "," + ageFrom + "," + ageTo + "," + gender + "," + timeAnswered + "): " + err.message);
                cb(err);
            } else {
                // ansByEthnicity is a list of 6 values
                cb(null, ansByEthnicity); // call callback with no error
            }
        });
    }

    /**
     * Calls getAnswerByGender.
     * @param questionId string.
     * @param ethenicity string (default: all)
     * @param ageTo int. (default: -1)
     * @param ageFrom int. (default: -1)
     * @param timeAnswered string. (default: all)
     * @see Facade.cpp
     */
    function getAnswerByGender(questionId,ethenicity,ageTo,ageFrom,timeAnswered, cb){

        console.log("Calling getAnswerByGender(" + questionId + "," + ethenicity + "," + ageTo + "," + ageFrom + "," + timeAnswered + ")");
        websocket.callRemoteMethod("getAnswerByGender", [questionId, ethenicity,ageTo, ageFrom, timeAnswered], function (err, ansByGender) {
            if (err) {
                console.log("Error calling getAnswerByEthnicity(" + questionId + "," + ethenicity + "," + ageTo + "," + ageFrom + timeAnswered + "): " + err.message);
                cb(err);
            } else {
                // ansByGender is a list of 3 values
                cb(null, ansByGender); // call callback with no error
            }
        });
    }

    /**
     * Calls getUserAnswerByLanguage.
     * @param questionId string.
     * @param ethenicity string (default: all)
     * @param ageTo int. (default: -1)
     * @param ageFrom int. (default: -1)
     * @param gender string. (default: all)
     * @param timeAnswered string. (default: all)
     * @see Facade.cpp
     */
    function getUserAnswerByLanguage(questionId,ageFrom,ageTo,ethnicity,gender,timeAnswered,cb){

        console.log("Calling getUserAnswerByLanguage(" + questionId + "," + gender + "," + ethnicity + "," + ageTo + "," + ageFrom + "," + timeAnswered + ")");
        websocket.callRemoteMethod("getAnswerByLanguage", [questionId,ageFrom,ageTo,ethnicity,gender,timeAnswered], function (err, ansByLanguage) {
            if (err) {
                console.log("Error calling getAnswerByLanguage(" + questionId + "," + ethnicity + "," + ageTo + "," + ageFrom + timeAnswered + "): " + err.message);
                cb(err);
            } else {
                // ansByGender is a list of 3 values
                cb(null, ansByLanguage); // call callback with no error
            }
        });

    }



    /**
     * Calls getAnswers.
     * @param questionIds QList.
     * @param ageFrom int. (default: -1)
     * @param ageTo int. (default: -1)
     * @param ethnicity string. (default: all)
     * @param gender string. (default: all)
     * @param timeAnswered string. (default: all)
     * @see Facade.cpp
     */
    function getAnswers(questionIds, ageFrom, ageTo, ethnicity, gender, timeAnswered, callback) {
        console.log("Calling getAnswers(" + questionIds + ", " + ageFrom + "," + ageTo + "," + ethnicity + "," + gender + "," + timeAnswered + ")");
        var params = [questionIds, ageFrom, ageTo, ethnicity, gender, timeAnswered];
        websocket.callRemoteMethod("getAnswers", params, function (error, allAnswers) {
            if (error) {
                console.log("Error calling getAnswers(" + questionIds + ", " + ageFrom + "," + ageTo + "," + ethnicity + "," + gender + "," + timeAnswered + ")");
                callback(error);
            } else {
                callback(null, allAnswers);
            }
        });
    }

    /**
     * Sets the RFID tag.
     * Reads the info from the service, if any.
     * Populate the info here.
     *
     * @param tag string RFID tag to set it to.
     * @param cb Callback that expects no result.
     */
    function setRfidTag(tag, cb) {
        resetUser(function (err) {
            if (err) {
                cb(err);
            } else {
                rfidTag = tag;
                // call setOrCreateUser method from the service.
                websocket.callRemoteMethod("getOrCreateUser", [tag], function (err2, user_id) {
                    if (err2) {
                        cb(err2);
                    } else {
                        _populateUserInfo(user_id, function (err3) {
                            if (err3) {
                                console.log(err3); // let's no pass this error upstream
                                cb(null); // done (even if an error occured)
                            } else {
                                // Set the userId property, so that we can use it from anywhere
                                userId = user_id;
                                // TODO: also populate their answers
                                _populateUserAnswers(user_id, function (err4) {
                                    if (err4) {
                                        console.log(err4); // let's no pass this error upstream
                                        cb(null); // done (even if an error occured)
                                    } else {
                                        // TODO: also populate their answers
                                        cb(null); // Success. Call the callback with no error.
                                    }
                                });
                            }
                        });
                    }
                });
            }
        });
    }

    /**
     * Checks if the user has answered to the demographic questions.
     * @return boolean
     */
    function hasDemographicQuestionsAnswered() {
        var ret = true;
        if (userProfile.age == userProfile.const_INVALID_NUMBER) {
            ret = false;
        }
        else if (userProfile.gender == userProfile.const_INVALID_STRING) {
            ret = false;
        }
        else if (userProfile.ethnicity == userProfile.const_INVALID_STRING) {
            ret = false;
        }
        else if (userProfile.language == userProfile.const_INVALID_STRING) {
            ret = false;
        }
        return ret;
    }

    /**
     * Retrieves all user info that we have from the service.
     * and populates the variables here.
     */
    function _populateUserInfo(userId, cb) {
        if (userId === const_INVALID_NUMBER) {
            cb(new Error("Currently no user id."));
        } else {
            websocket.callRemoteMethod("getUserInfo", [userId], function (err1, userInfo) {
                if (err1) {
                    cb(err1);
                } else {
                    if (userInfo.language) {
                        language = userInfo.language;
                    }
                    if (userInfo.gender) {
                        gender = userInfo.gender;
                    }
                    if (userInfo.age) {
                        age = userInfo.age;
                    }
                    if (userInfo.ethnicity) {
                        ethnicity = userInfo.ethnicity;
                    }
                    // TODO: store rfidTag for user
                    // TODO: store userId for user

                    // check Kiosk Mode and demographic que. answered.
                    if (thisIsInSystemTests === false) {
                        _checkKioskMode();
                    }
                    cb(null); // done
                }
            });
        }
    }


    /**
     * Check Kiosk mode for central, Entry or final.
     * and also check if demographic questions are answered than skip to survey questions.
     *
     * FIXME: We should really not access functions and properties that belong to a parent from within a QML type.
     * This is so that QML types are reusable, no matter in which context we use them. (for example, in a system test suite)
     */
    function _checkKioskMode(){

        // Go to the demographic question if this is the entry kiosk
        if (kioskConfig.kiosk_mode == window.const_KIOSK_MODE_ENTRY) {
                goToDemographicQuestions();

        // Go to the survey questions if this is the central kiosk
        // But: if the user hasn't answered the demographic questions, send them there.
        } else if (kioskConfig.kiosk_mode == window.const_KIOSK_MODE_CENTRAL) {
            if (userProfile.hasDemographicQuestionsAnswered()) {
                goToSurveyQuestions();

            } else {
                goToDemographicQuestions();
            }

        // If this is the exit kiosk, send them to the final pages
        } else if (kioskConfig.kiosk_mode == window.const_KIOSK_MODE_EXIT) {
            goToFinalQuestions();
        }
    }

    /**
     * Retrieves all answers for a user - from the service -
     * and populates the variables here.
     *
     * @param cb Callback to call with an nullable error, and not other arguments.
     */
    function _populateUserAnswers(userId, cb) {
        if (userId === const_INVALID_NUMBER) {
            cb(new Error("Currently no user id."));
        } else {
            websocket.callRemoteMethod("getUserAnswers", [userId], function (err1, userAnswers) {
                if (err1) {
                    cb(err1);
                } else {
                    // QMap<QString, int>
                    for (var key in userAnswers) {
                        var answerValue = userAnswers[key];
                        console.log("Loading answer " + key + " = " + answerValue);
                        thisUserProfile.answers[key] = answerValue;
                    }

                    userAnswersUpdated();

                    cb(null); // done
                }
            });
        }
    }

    /**
     * Sets the current RFID tag to one within the range [0,9]
     * This is so that developers can easily test the system by pressing a number.
     *
     * The callback function should handle an error, but no result.
     */
    function setFakeRfidTag(index, cb) {
        var tagToSet = "";
        switch (index) {
        case 0:
            tagToSet = "DEADBEEF0000";
            break;
        case 1:
            tagToSet = "DEADBEEF0001";
            break;
        case 2:
            tagToSet = "DEADBEEF0002";
            break;
        case 3:
            tagToSet = "DEADBEEF0003";
            break;
        case 4:
            tagToSet = "DEADBEEF0004";
            break;
        case 5:
            tagToSet = "DEADBEEF0005";
            break;
        case 6:
            tagToSet = "DEADBEEF0006";
            break;
        case 7:
            tagToSet = "DEADBEEF0007";
            break;
        case 8:
            tagToSet = "DEADBEEF0008";
            break;
        case 9:
            tagToSet = "DEADBEEF0009";
            break;
        }
        setRfidTag(tagToSet, function (err) {
            if (err) {
                cb(err);
            } else {
                cb(null);
            }
        });
    }

    /**
     * Retrieves the current RFID tag.
     *
     * @param cb Callback whose result is the current RFID tag as a string.
     */
    function getRfidTag(cb) {
        if (rfidTag) {
            cb(null, rfidTag);
        } else {
            cb(new Error("Currently no RFID tag is set."));
        }
    }

    /**
     * Retrieves the current user id.
     *
     * @param cb Callback whose result is the current user id as an int.
     */
    function getUserId(cb) {
        if (userId === const_INVALID_NUMBER) {
            cb(new Error("Currently no user id."));
        } else {
            cb(null, userId);
        }
    }

    /**
     * Sends a ping. The service should answer with "pong".
     */
    function ping(cb) {
        websocket.sendPing(cb);
    }

    /**
     * Retrieves the current user answer for specific question.
     *
     * @param questionId which use to filter from answer for current user.
     * @ret user answer or -1 if not answered yet.
     */
    function getMyAnswer(questionId){

        var myAnswer =-1;

        if(window.userProfile.answers.hasOwnProperty(questionId)) {
            myAnswer= window.userProfile.answers[questionId];
        }
        return myAnswer;
    }


    /**
     * Retrieves the current user ethnicity row index.
     *
     * @param ethnicities list to filter current user ethnicity row index.
     * @ret ethnicity index or -1 if not answered.
     */
    function getMyEthnicity(ethnicities){

        var rawId =-1;
        var myEthnicity = ethnicity ;

        if(myEthnicity===const_INVALID_STRING) {
            rawId=-1;
        }
        else
        {
            var i=0 ;
            for(var key in  ethnicities)
            {
                if(key===myEthnicity){
                    rawId = i;
                }
                i++;
            }
        }
        return rawId;
    }

    /**
     * Retrieves the current user gender row index.
     *
     * @param genders list to filter current user gender row index.
     * @ret gender index or -1 if not answered.
     */
    function getMyGender(genders){

        var rawId =-1;
        var myGender = gender ;

        if(myGender===const_INVALID_STRING) {
            rawId=-1;
        }
        else
        {
            var i=0 ;
            for(var key in  genders)
            {
                if(key===myGender){
                    rawId = i;
                }
                i++;
            }
        }
        return rawId;
    }

    /**
     * Retrieves the current user language row index.
     *
     * @param languages list to filter current user language row index.
     * @ret language index or -1 if not answered.
     */
    function getMyLanguage(languages){

        var rawId =-1;
        var myLanguage = language ;

        if(myLanguage===const_INVALID_STRING) {
            rawId=-1;
        }
        else
        {
            var i=0 ;
            for(var key in  languages)
            {
                if(key===myLanguage){
                    rawId = i;
                }
                i++;
            }
        }
        return rawId;
    }

    /**
     * Retrieves the object list length.
     *
     * @param object list of key and value pair.
     * @ret length of object.
     */
    function getObjectLength(object){
        var i=0 ;
        for(var key in  object)
        {
            i++;
        }
        return i;
    }

    /**
     * Calls when user is ideal and remove all callback ids.
     *
     */
    function clearAllCallbackIds(){
       websocket.clearAllCallback();

    }


    /**
     * Makes the client periodically reconnect with the weboscket server, if necessary.
     */
    Timer {
        id: reconnectTimer

        interval: 5000 // ms
        running: false // started when the websocket is ready.
        repeat: true
        onTriggered: {
            // console.log("websocket reconnect timer.onTriggered");
            var reconnect = false;

            if (websocket.active === false) {
                reconnect = true;
            }

            var websocketStatus = websocket.status;
            if (websocketStatus === WebSocket.Open) {
                // pass
            } else if (websocketStatus === WebSocket.Closed) {
                reconnect = true;
            } else if (websocketStatus === WebSocket.Closing) {
                // pass
            } else if (websocketStatus === WebSocket.Connecting) {
                // pass
            } else if (websocketStatus === WebSocket.Error) {
                reconnect = true;
            }

            if (reconnect) {
                console.log("websocket not connected. Attempt to reconnect.");
                websocket.active = false; // important to set it to false first.
                websocket.active = true; // attempt to reconnect
            }
        }
    }

    /**
     * Client websocket to communicate with the mpop_service.
     */
    WebSocket {
        id: websocket

        // Maximum int value in QML
        readonly property int max_INT: 2147483647
        // Associative array whose keys are the callId for the JSON-RPC 2.0 requests
        // and the values are the callbacks to trigger when we get a response for them,
        // or if the timeout expires.
        // TODO: expire timeouts
        // Keys are strings, so we convert the numbers to strings with .toString()
        property var responseCallbacks: ({})

        /**
         * @return int
         */
        function generateRequestId() {
            currentRequestId = (currentRequestId + 1) % max_INT;
            var ret = currentRequestId;
            return ret;
        }

        /**
         * Calls a remote method of the mpop_service.
         *
         * Sends a JSON-RPC 2.0 via websockets.
         * Then, it stores a callback associated to a call id.
         * Call ids are integers, but we stores them as an associative array
         * whose keys are strings.
         * When we receive a response from the service, we check
         * if we have a callback for its id and call the callback with the result.
         *
         * @param string methodsName Name of the method of the mpop_service to call.
         * @param array params List of params to provide to the method.
         * @param callable cb Function to call with the result once we receive a response.
         *
         * The callbacks are called with an Error, if any, as the first argument, or with the result as the second argument.
         */
        function callRemoteMethod(methodName, params, cb) {
            var callId = generateRequestId();
            var request = {
                method: methodName,
                params: params,
                id: callId
            };

            // Store the callback and the callId, so that we check later if we have received an answer,
            // or handle it right away when we get the response.
            responseCallbacks[callId.toString()] = cb;

            // FIXME: it seems like the JSON.stringify function adds double quotes around integers.
            var strToSend = JSON.stringify(request);

            if (websocket.status === WebSocket.Open) {
                websocket.sendTextMessage(strToSend);
                // console.log("string sent :", '"' + strToSend + '"');
            } else {
                console.log("Cannot call remote method: no websocket connection.");
                cb(new Error("Cannot call remote method: no websocket connection."));
            }
        }

        // TODO: Periodically clear all callbacks
        function clearAllCallback() {
            for (var key in responseCallbacks) {
                var cb = responseCallbacks[key];
                cb(new Error("This callback has expired, and never got called."));
            }
            responseCallbacks = ({});
        }

        /**
         * Calls the ping method.
         */
        function sendPing(cb) {
            callRemoteMethod("ping", [], cb);
        }

        /**
         * @param Number callId
         */
        function _callCallbackForKeyIfFound(callId, result) {
            var foundCb = false;
            // console.log("_callCallbackForKeyIfFound " + JSON.stringify(result));
            for (var key in responseCallbacks) {
                // console.log("Check if callback " + key + " matches what we are looking for: " + callId.toString());
                // console.log("Types are " + (typeof key) + " and " + (typeof callId.toString()));
                if (key === callId.toString()) {
                    // console.log("Found registered callback for " + callId.toString());
                    // Calling the callback:
                    var cb = responseCallbacks[key];
                    // console.log("Calling the callback with result " + callId.toString() + " " + result);
                    cb(null, result);
                    // Removing the callback:
                    delete responseCallbacks[key];
                    foundCb = true;
                    break;
                }
            }
            if (! foundCb) {
                console.log("_callCallbackForKeyIfFound: Could not find a callback for " + callId);
            }
        }

        /**
         * Returns a string for a given WebSocket.status value.
         */
        function statusToString(websocketStatus) {
            if (websocketStatus === WebSocket.Open) {
                return "Open";
            } else if (websocketStatus === WebSocket.Closed) {
                return "Closed";
            } else if (websocketStatus === WebSocket.Closing) {
                return "Closing";
            } else if (websocketStatus === WebSocket.Connecting) {
                return "Connecting";
            } else if (websocketStatus === WebSocket.Error) {
                return "Error";
            }
        }

        property int currentRequestId: 0

        url: "ws://" + service_host + ":" + service_port_number
        active: true

        onActiveChanged: {
            console.log("websocket.active = " + active);
        }

        Component.onCompleted: {
            console.log("Websocket URL: " + url);
            reconnectTimer.running = true;
        }

        onTextMessageReceived: {
            // console.log("onTextMessageReceived: " + message);
            // messageBox.text = messageBox.text + "\nReceived message: " + message;
            var data = JSON.parse(message);
            var method = (data.hasOwnProperty("method")) ? data.method : "";
            var result = (data.hasOwnProperty("result")) ? data.result : "";
            var params = (data.hasOwnProperty("params")) ? data.params : "";
            var callId = data.id;
            // TODO: if it's a result, check for call id, otherwise, it must be a method call.

            // console.log("JSON-RPC: " + JSON.stringify(data));

            _callCallbackForKeyIfFound(callId, result);
        }

        onStatusChanged:
        {
            var stringStatus = statusToString(websocket.status);
            // console.log("websocket status changed to " + stringStatus);
            if (websocket.status === WebSocket.Error) {
                console.log("Error: " + websocket.errorString)
            } else if (websocket.status === WebSocket.Open) {
                console.log("CONNECTED to mpop_service via websockets.");
                sendPing(function cb(err, result) {
                    if (err) {
                        console.log("Error calling ping: " + err);
                    }
                    console.log("Got answer from ping: " + result);
                });
            } else if (websocket.status === WebSocket.Closed) {
                // pass
            }
        }
    }
}
