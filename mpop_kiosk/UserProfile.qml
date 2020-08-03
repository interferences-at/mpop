import QtQuick 2.0
import QtWebSockets 1.0

/**
 * Informations about the current user.
 */
Item {
    // constants
    readonly property int const_NUM_QUESTIONS: 999 // FIXME
    readonly property int const_INVALID_NUMBER: -1

    // properties
    // Connection with the mpop_service:
    property int service_port_number: 3333
    property string service_host: "0.0.0.0"

    // user's profile:
    property string gender: null
    property string culture: null
    property int age: const_INVALID_NUMBER
    property string language: null
    property var answers: []
    property string rfidTag: null
    property int userId: const_INVALID_NUMBER


    // functions

    // TODO
    function resetUser(cb) {
        gender = null;
        culture = null;
        age = const_INVALID_NUMBER;
        language = null;
        answers = [];
        userId = const_INVALID_NUMBER
        rfidTag = null

        for (var i = 0; i < const_NUM_QUESTIONS; i ++) {
            answers[i] = null;
        }
        cb();
    }

    // TODO
    function getUserGender(cb) {

    }

    // TODO
    function getUserLanguage(cb) {

    }

    // TODO
    function setUserGender(cb, value) {

    }

    // TODO
    function setUserLanguage(cb, value) {

    }

    // TODO
    function setUserAge(cb, value) {

    }

    // TODO
    function getUserAge(cb) {

    }

    // TODO
    function setUserAnswer(cb, question_identifier, value) {

    }

    // TODO
    function getUserAnswer(cb, question_identifier) {

    }

    // TODO
    function setRfidTag(cb, tag) {

    }

    // TODO
    function getRfidTag(cb) {

    }

    // TODO
    function getUserId(cb) {

    }

    /**
     * Client websocket to communicate with the mpop_service.
     */

    Timer {
        id: timer

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

    WebSocket {
        id: websocket

        // Maximum int value in QML
        readonly property int max_INT: 2147483647
        property var responseCallbacks: ({})

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

            responseCallbacks[callId] = cb;

            var strToSend = JSON.stringify(request);

            if (websocket.status === WebSocket.Open) {
                websocket.sendTextMessage(strToSend);
                console.log("string sent :", '"' + strToSend + '"');
            } else {
                console.log("Cannot call remote method: no websocket connection.");
            }
        }

        // TODO: Periodically clear all callbacks
        function clearAllCallback() {
            responseCallbacks = ({});
        }

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
                    responseCallbacks[key](result);
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
            timer.running = true;
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
                sendPing(function cb(result) {
                    console.log("Got answer from ping: " + result);
                });
            } else if (websocket.status === WebSocket.Closed) {
                // pass
            }
        }
    }
}
