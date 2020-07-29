import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtQuick.Window 2.11
import Screensaver 1.0

/**
 * Main window of this application
 * We follow the standard QML conventions: https://doc.qt.io/qt-5/qml-codingconventions.html
 */
ApplicationWindow {
    id: window

    property string lastRfidRead: ""
    property string lastMessageReceived: ""

    function handleMessageReceived(oscPath, oscArguments) {
        console.log("(QML) Received OSC: " + oscPath + " " + oscArguments);
        lastMessageReceived = oscPath + " " + oscArguments;
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

    visible: true
    width: 1920
    height: 1080
    title: qsTr("MPOP Kiosk")


    Connections {
        target: rfidReader
        onLastRfidReadChanged: {
            lastRfidRead = rfidReader.lastRfidRead;
            console.log("(QML) Last RFID read: " + lastRfidRead);
        }
        onTagRead: {
            console.log("(QML) RFID read: " + tag);
        }
    }


    Connections {
        target: oscReceiver

        onMessageReceived: {
            handleMessageReceived(oscAddress, message);
        }
    }

    UserProfile {
        id: userProfile
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
            /**
             * The main model that contains all the questions.
             */
            model: ModelQuestions {
                id: modelQuestions
            }

            delegate: WidgetQuestionButton {
                questionName: question_fr
                height: parent.height / parent.count
                spacing: 0
            }
        }

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
                            text: "Ceci est une question."
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
                        WidgetPreviousNext {
                            Layout.alignment: Qt.AlignRight
                            Layout.fillWidth: false
                            Layout.fillHeight: false
                        }
                    }


                }
            }

            // This is a code screensaver
            // TODO: Put this code below to the right trigger
            StackLayout {
                id: screensaverLayout
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.margins: 0

                Rectangle {
                    width: parent.width
                    height: parent.height
                    color: "black"
                    visible: true

                    Screensaver {
                        id: screensaver
                        anchors.fill: parent
                    }
                }
            }

            // End of the StackLayout

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
}
