import QtQuick 2.0
import QtWebSockets 1.0

/**
 * Informations about the current user.
 */
Item {
    property string gender: null
    property string culture: null
    property int age: null
    property string language: null
    property var answers: []
    property string rfidTag: null
    property int userId: null

    readonly property int const_NUM_QUESTIONS:

    function resetUser() {
        gender = null;
        culture = null;
        age = null;
        language = null;
        answers = [];
        userId = null
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
        url: "ws://1.1.1.1:3333" // TODO: Make it configurable
        onTextMessageReceived: {
            messageBox.text = messageBox.text + "\nReceived message: " + message
        }
        onStatusChanged:
        {
            if (socket.status === WebSocket.Error) {
                console.log("Error: " + websocket.errorString)
            } else if (websocket.status === WebSocket.Open) {
                websocket.sendTextMessage("Hello World")
            } else if (socket.status === WebSocket.Closed) {
                messageBox.text += "\nSocket closed"
            }
            active: false
        }
    }

}
