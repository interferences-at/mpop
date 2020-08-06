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
    // constants
    readonly property int const_NUM_QUESTIONS: 999 // FIXME
    readonly property int const_INVALID_NUMBER: -1

    // properties
    // Connection with the mpop_service:
    // These default value will be overriden:
    property int service_port_number: 3333
    property string service_host: "0.0.0.0"
    property bool is_verbose: false

    // user's profile:
    property string gender: null
    property string ethnicity: null
    property int age: const_INVALID_NUMBER
    property string language: null
    property var answers: []
    property string rfidTag: null
    property int userId: const_INVALID_NUMBER
    property bool sent_user_info_to_service: false
    property bool did_read_user_info_from_service: false


    // functions

    /**
     * Resets all the properties that are relevant to the current user.
     */
    function resetUser(cb) {
        gender = null;
        ethnicity = null;
        age = const_INVALID_NUMBER;
        language = null;
        answers = [];
        userId = const_INVALID_NUMBER
        rfidTag = null

        for (var i = 0; i < const_NUM_QUESTIONS; i ++) {
            answers[i] = null;
        }
        cb(null);
    }

    // TODO
    function getUserGender(cb) {

    }

    // TODO
    function getUserLanguage(cb) {

    }

    // TODO
    function setUserGender(value, cb) {

    }

    // TODO
    function setUserLanguage(value, cb) {

    }

    // TODO
    function getUserEthnicity(cb) {

    }

    // TODO
    function setUserEthnicity(value, cb) {

    }

    // TODO
    function setUserAge(value, cb) {

    }

    // TODO
    function getUserAge(cb) {

    }

    // TODO
    function setUserAnswer(question_identifier, value, cb) {

    }

    // TODO
    function getUserAnswer(question_identifier, cb) {

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
                        userId = user_id;
                        _populateUserInfo(userId, function (err3) {
                            if (err3) {
                                console.log(err3); // let's no pass this error upstream
                                cb(null); // done (even if an error occured)
                            } else {
                                cb(null); // done
                            }
                        });
                    }
                });
            }
        });
    }

    /**
     * Retrieves all user info that we have from the service.
     * and populates the variables here.
     */
    function _populateUserInfo(userId, cb) {
        if (userId === const_INVALID_NUMBER) {
            cb(new Error("Currently no user id."));
        } else {
            callRemoteMethod("getUserInfo", [userId], function (err1, userInfo) {
                if (err1) {
                    cb(err1);
                } else {
                    language = userInfo.language;
                    gender = userInfo.gender;
                    age = userInfo.age;
                    ethnicity = userInfo.ethnicity;
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
        property var responseCallbacks: ({})

        /**
         * @return int
         */
        function generateRequestId() {
            currentRequestId = (currentRequestId + 1) % max_INT;
            var ret = currentRequestId;
            return ret;
        }

        function callRemoteMethod(methodName, params, cb) {
            var callId = generateRequestId();
            var request = {
                method: methodName,
                params: params,
                id: callId
            };

            // Store the callback and the callId, so that we check later if we have received an answer,
            // or handle it right away when we get the response.
            responseCallbacks[callId] = cb;

            // FIXME: it seems like the JSON.stringify function adds double quotes around integers.
            var strToSend = JSON.stringify(request);

            if (websocket.status === WebSocket.Open) {
                websocket.sendTextMessage(strToSend);
                console.log("string sent :", '"' + strToSend + '"');
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

        function _callCallbackForKeyIfFound(callId, result) {
            var foundCb = false;
            console.log("_callCallbackForKeyIfFound " + JSON.stringify(result));
            for (var key in responseCallbacks) {
                if (key === callId) {
                    console.log("Found registered callback for " + callId);
                    // Calling the callback:
                    var cb = responseCallbacks[key];
                    cb(null, result);
                    // Removing the callback:
                    delete responseCallbacks[key];
                    foundCb = true;
                    break;
                }
            }
            if (! foundCb) {
                console.log("_callCallbackForKeyIfFound: Cound not find a callback for " + callId);
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
            console.log("onTextMessageReceived: " + message);
            //messageBox.text = messageBox.text + "\nReceived message: " + message;
            var data = JSON.parse(message);
            var method = (data.hasOwnProperty("method")) ? data.method : "";
            var result = (data.hasOwnProperty("result")) ? data.result : "";
            var params = (data.hasOwnProperty("params")) ? data.params : "";
            var callId = data.id;
            // TODO: if it's a result, check for call id, otherwise, it must be a method call.

            console.log("JSON-RPC: " + JSON.stringify(data));

            _callCallbackForKeyIfFound(callId, result);
        }

        onStatusChanged:
        {
            var stringStatus = statusToString(websocket.status);
            console.log("websocket status changed to " + stringStatus);
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
