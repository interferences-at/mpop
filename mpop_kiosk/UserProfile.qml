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
    property string gender: null
    property string culture: null
    property int age: const_INVALID_NUMBER
    property string language: null
    property var answers: []
    property string rfidTag: null
    property int userId: const_INVALID_NUMBER


    // functions

    // TODO
    function resetUser() {
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
    }

    // TODO
    function getUserGender() {

    }

    // TODO
    function getUserLanguage() {

    }

    // TODO
    function setUserGender(value) {

    }

    // TODO
    function setUserLanguage(value) {

    }

    // TODO
    function setUserAge(value) {

    }

    // TODO
    function getUserAge() {

    }

    // TODO
    function setUserAnswer(question_identifier, value) {

    }

    // TODO
    function getUserAnswer(question_identifier) {

    }

    // TODO
    function setRfidTag(tag) {

    }

    // TODO
    function getRfidTag() {

    }

    // TODO
    function getUserId() {

    }

    /**
     * Client websocket to communicate with the mpop_service.
     */
    WebSocket {
        id: websocket

        function sendPing() {
            var request = {
                method: "ping",
                params: [],
                id: 2
            };
            var strToSend = JSON.stringify(request);
            if (websocket.status === WebSocket.Open) {
                websocket.sendTextMessage(strToSend);
                console.log("string sent :", '"' + strToSend + '"');
            }
        }

        url: "ws://127.0.0.1:3333" // TODO: Make it configurable
        active: true

        onTextMessageReceived: {
            //messageBox.text = messageBox.text + "\nReceived message: " + message;
            var data = JSON.parse(message);
            var method = data.method;
            var params = data.params;
            var callId = data.id;
            console.log(data);
            if (method === "pong") {
                console.log("got pong");
            }
            // TODO else if (method == ...)
        }
        onStatusChanged:
        {
            console.log("websocket status change" + websocket.status);
            if (websocket.status === WebSocket.Error) {
                console.log("Error: " + websocket.errorString)
            } else if (websocket.status === WebSocket.Open) {
                sendPing();
            } else if (websocket.status === WebSocket.Closed) {
                // messageBox.text += "\nSocket closed"'
                console.log("websocket connection closed");
                active = false; // close the websocket
            }
        }
    }
}
