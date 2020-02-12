import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtQuick.Window 2.11

// Main window of this application
ApplicationWindow {
    id: window

    property string lastRfidRead: ""
    property string lastMessageReceived: ""

    visible: true
    width: 1920
    height: 1080
    title: qsTr("MPOP Kiosk")

    function handleMessageReceived(oscPath, oscArguments) {
        console.log("QML-Received OSC: " + oscPath + " " + oscArguments);
        lastMessageReceived = oscPath + " " + oscArguments;
    }

    Connections {
        target: rfidReader
        onLastRfidReadChanged: {
            lastRfidRead = rfidReader.lastRfidRead;
            console.log("Last RFID read: " + lastRfidRead);
        }
    }

    Connections {
        target: oscReceiver

        onMessageReceived: {
            handleMessageReceived(oscAddress, message);
        }
    }

    /**
     * Toggles the fullscreen state of the main window.
     */
    function toggleFullscreen() {
        if (visibility === Window.FullScreen) {
            visibility = Window.AutomaticVisibility;
        } else {
            visibility = Window.FullScreen;
        }
    }

    function quitThisApp() {
        Qt.quit();
    }

    function toggleDebugView() {
        stackLayout0.currentIndex = (stackLayout0.currentIndex + 1) % 2;
    }

    // Shortcuts:
    Shortcut {
        sequence: "Esc"
        onActivated: toggleFullscreen()
    }

    Shortcut {
        sequence: "Ctrl+Q"
        onActivated: quitThisApp()
    }
    Shortcut {
        sequence: "Tab"
        onActivated: toggleDebugView()
    }

    // Main two-column layout
    RowLayout {
        anchors.fill: parent
        spacing: 6

        // List of questions (buttons)
        ListView {
            Layout.margins: 0
            Layout.fillWidth: false
            Layout.fillHeight: true
            orientation: Qt.Vertical
            width: currentItem.width
            model: questionModel
            delegate: QuestionButton {
                questionName: name
                height: parent.height / parent.count
                spacing: 0
            }
        }
        //            }

        // Contents
        StackLayout {
            id: stackLayout0
            currentIndex: 0
            Layout.fillWidth: true
            Layout.fillHeight: true

            ColumnLayout {
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.margins: 0

                StackLayout {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    Layout.margins: 0

                    ColumnLayout {
                        Layout.fillWidth: true
                        Layout.fillHeight: true

                        Button {
                            Layout.alignment: Qt.AlignRight
                            text: "X"
                            font.pixelSize: 24
                        }

                        Label {
                            Layout.alignment: Qt.AlignCenter
                            text: "Question 01"
                            font.pixelSize: 36
                        }
                        Label {
                            Layout.alignment: Qt.AlignCenter
                            text: "Combien de bière par semaine buvez-vous ?"
                            font.pixelSize: 36
                        }
                        AnswerSlider {
                            Layout.fillWidth: true
                            sliderValue: 50
                            textLeft: "Peu"
                            textRight: "Beaucoup"
                            showNumber: false
                        }
                        Label {
                            Layout.alignment: Qt.AlignCenter
                            text: "Sous-titre pour le slider."
                            font.pixelSize: 24
                        }
                        RowLayout {
                            Layout.alignment: Qt.AlignRight
                            Layout.fillWidth: false
                            Layout.fillHeight: false

                            Button {
                                text: "<-"
                            }
                            Button {
                                text: "->"
                            }
                        }
                    }


                }
            }

            // OSC debug layout:
            ColumnLayout {
                RowLayout {
                    SpinBox {
                        id: someInt
                        value: 2
                    }
                    Slider {
                        id: someDouble
                        value: 3.14159
                        from: 0.0
                        to: 5.0
                    }
                    TextField {
                        id: someText
                        text: "hello"
                    }
                }

                Button {
                    text: "Send OSC"
                    onClicked: {
                        oscSender.send("/hello", [someInt.value, someDouble.value, someText.text]);
                    }
                }

                RowLayout {
                    Label {
                        text: "Received:"
                    }
                    Label {
                        text: lastMessageReceived
                    }
                }
            }
        }
    }

    ListModel {
        id: questionModel
        ListElement {
            name: "Question 01"
        }
        ListElement {
            name: "Question 02"
        }
        ListElement {
            name: "Question 03"
        }
        ListElement {
            name: "Question 04"
        }
        ListElement {
            name: "Question 05"
        }
        ListElement {
            name: "Question 06"
        }
        ListElement {
            name: "Question 07"
        }
        ListElement {
            name: "Question 08"
        }
        ListElement {
            name: "Question 09"
        }
        ListElement {
            name: "Question 10"
        }
        ListElement {
            name: "Question 11"
        }
        ListElement {
            name: "Question 12"
        }
        ListElement {
            name: "Question 13"
        }
        ListElement {
            name: "Question 14"
        }
        ListElement {
            name: "Question 15"
        }
        ListElement {
            name: "Question 16"
        }
        ListElement {
            name: "Question 17"
        }
        ListElement {
            name: "Question 18"
        }
        ListElement {
            name: "Question 19"
        }
        ListElement {
            name: "Question 20"
        }
        ListElement {
            name: "Question 21"
        }
        ListElement {
            name: "Question 22"
        }
        ListElement {
            name: "Question 23"
        }
        ListElement {
            name: "Question 24"
        }
        ListElement {
            name: "Question 25"
        }
    }
}
