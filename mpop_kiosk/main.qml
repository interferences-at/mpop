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
    property alias lang: userProfile.language

    readonly property string const_KIOSK_MODE_ENTRY: "entry"
    readonly property string const_KIOSK_MODE_CENTRAL: "central"
    readonly property string const_KIOSK_MODE_EXIT: "exit"

    /**
     * Handles the incoming OSC messages.
     */
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

    /**
     * Quits this application.
     */
    function quitThisApp() {
        Qt.quit();
    }

    // Callback for when we send the RFID request.
    function setFakeRfidTagCb(err) {
        if (err) {
            console.log(err);
        } else {
            console.log("Got a successful response for setFakeRfidTag.");
        }
    }

    /**
     * Shows or hides the debug view.
     */
    function toggleDebugView() {
        mainStackLayout.currentIndex = mainStackLayout.index_OSC_DEBUG;;
    }

    // assigning properties
    visible: true
    width: 1920
    height: 1080
    title: textWindowTitle.text

    Component.onCompleted: {
        if (kioskConfig.is_fullscreen) {
            console.log("Turning on fullscreen mode");
            visibility = Window.FullScreen;
        }

        // TODO: Show/hide sections according the kiosk_mode we are in.
        console.log("Kiosk mode is " + kioskConfig.kiosk_mode);
    }

    BilingualText {
        id: textWindowTitle
        textEn: "MPOP Kiosk"
        textFr: "Le kiosque MPOP"
        language: lang
    }

    /**
     * Handles the signals from the RFID serial reader.
     */
    Connections {
        target: rfidReader
        onLastRfidReadChanged: {
            lastRfidRead = rfidReader.lastRfidRead;
            console.log("(QML) Last RFID read: " + lastRfidRead);
            userProfile.setRfidTag(lastRfidRead);
        }
        onTagRead: {
            console.log("(QML) RFID read: " + tag);
        }
    }

    /**
     * Handles the signals from the OSC receiver.
     */
    Connections {
        target: oscReceiver

        onMessageReceived: {
            handleMessageReceived(oscAddress, message);
        }
    }

    Timer {
        id: idleTimer

        interval: 500 // TODO: change this
        running: true
        repeat: true
        onTriggered: {
            // TODO: detect clicks, and time how time elapsed since last click
            // if idle for too long, go to screensaver
        }
    }


    /**
     * Communicates with the kiosk_service via JSON-RPC 2.0 to INSERT/UPDATE/SELECT in the MySQL database.
     */
    UserProfile {
        id: userProfile

        function goToDemographicQuestions() {
            mainStackLayout.currentIndex = mainStackLayout.index_DEMOGRAPHIC_QUESTIONS;
            demographicQuestionsStackLayout.currentIndex = demographicQuestionsStackLayout.index_FIRST_PAGE;

            // Set the state for every model according to the answers of the visitor:
            if (userProfile.gender === userProfile.const_INVALID_STRING) {
                pageGender.setHightlighted(-1);
            } else {
                pageGender.setHightlighted(-1); // TODO
            }
        }

        function goToSurveyQuestions() {
            mainStackLayout.currentIndex = mainStackLayout.index_SURVEY_QUESTIONS;
            questionsStackLayout.currentIndex = questionsStackLayout.index_FIRST_QUESTION;

        }

        service_port_number: kioskConfig.service_port_number
        service_host: kioskConfig.service_host
        is_verbose: kioskConfig.is_verbose

        onLanguageChanged: {
            console.log("Language changed: " + language);
        }

        onUserIdChanged: {
            if (userId === const_INVALID_NUMBER) {
                // go back to screensaver.
                console.log("Error: invalid userId");
                mainStackLayout.currentIndex = mainStackLayout.index_SCREENSAVER;
            } else {
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
                    // TODO
                    console.log("TODO: go to exit kiosk mode");
                }
            }
        }
    }

    // Keyboard shortcuts:
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

    Shortcut {
        sequence: "PgDown"
        onActivated: mainStackLayout.nextPage()
    }

    Shortcut {
        sequence: "PgUp"
        onActivated: mainStackLayout.previousPage()
    }

    // handle number key press event

    Shortcut {
        sequence: "0"
        onActivated: userProfile.setFakeRfidTag(0, setFakeRfidTagCb)
    }

    Shortcut {
        sequence: "1"
        onActivated: userProfile.setFakeRfidTag(1, setFakeRfidTagCb)
    }

    Shortcut {
        sequence: "2"
        onActivated: userProfile.setFakeRfidTag(2, setFakeRfidTagCb)
    }

    Shortcut {
        sequence: "3"
        onActivated: userProfile.setFakeRfidTag(3, setFakeRfidTagCb)
    }

    Shortcut {
        sequence: "4"
        onActivated: userProfile.setFakeRfidTag(4, setFakeRfidTagCb)
    }

    Shortcut {
        sequence: "5"
        onActivated: userProfile.setFakeRfidTag(5, setFakeRfidTagCb)
    }

    Shortcut {
        sequence: "6"
        onActivated: userProfile.setFakeRfidTag(6, setFakeRfidTagCb)
    }

    Shortcut {
        sequence: "7"
        onActivated: userProfile.setFakeRfidTag(7, setFakeRfidTagCb)
    }

    Shortcut {
        sequence: "8"
        onActivated: userProfile.setFakeRfidTag(8, setFakeRfidTagCb)
    }

    Shortcut {
        sequence: "9"
        onActivated: userProfile.setFakeRfidTag(9, setFakeRfidTagCb)
    }


    /**
     * The main model that contains all the questions.
     */
    ModelQuestions {
        id: modelQuestions
    }

    /**
     * Main StackLayout
     */
    StackLayout {
        id: mainStackLayout

        readonly property int index_SCREENSAVER: 0
        readonly property int index_DEMOGRAPHIC_QUESTIONS: 1
        readonly property int index_SURVEY_QUESTIONS: 2
        readonly property int index_EXIT_SECTION: 3
        readonly property int index_OSC_DEBUG: 4

        function nextPage() {
            mainStackLayout.currentIndex = (mainStackLayout.currentIndex + 1) % mainStackLayout.count;
        }

        function previousPage() {
            mainStackLayout.currentIndex = (mainStackLayout.count + mainStackLayout.currentIndex - 1) % mainStackLayout.count;
        }

        currentIndex: index_SCREENSAVER
        anchors.fill: parent

        /**
         * The Screensaver shows floating bars.
         */
        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.margins: 0
            color: "black"

            Screensaver {
                id: screensaver
                anchors.fill: parent
            }
        }

        /**
         * Section with the demographic question of the entry kiosk.
         *
         * This is our main two-column layout.
         */
        StackLayout {
            id: demographicQuestionsStackLayout

            /**
             * Go to the previous page.
             */
            function previousPage() {
                demographicQuestionsStackLayout.currentIndex -= 1
            }

            /**
             * Go to the next page.
             */
            function nextPage() {
                demographicQuestionsStackLayout.currentIndex += 1
            }

            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.margins: 0

            readonly property int index_FIRST_PAGE: 0
            readonly property int index_MY_LANGUAGE: 0
            readonly property int index_MY_GENDER: 1
            readonly property int index_MY_ETHNICITY: 2
            readonly property int index_MY_AGE: 3
            readonly property int index_ENJOY_YOUR_VISIT: 4

            PageLanguage {
                id: pageLanguage

                onLanguageChosen: {
                    console.log("onLanguageChosen " + value);
                    userProfile.setUserLanguage(userProfile.userId, value, function (err) {
                        if (err) {
                            console.log(err.message);
                        } else {
                            languageSwitcher.language = value;
                        }
                    });
                }
                onPreviousButtonClicked: {
                    // TODO
                    // FIXME: there should be no previous button here.
                }
                onNextButtonClicked: {
                    demographicQuestionsStackLayout.nextPage();
                }
            }

            // Select your gender
            PageGender {
                id: pageGender

                onGenderChosen: {
                    console.log("onGenderChosen " + value);
                    userProfile.setUserGender(userProfile.userId, value, function (err) {
                        if (err) {
                            console.log(err.message);
                        }
                    });
                }
                onPreviousButtonClicked: {
                    demographicQuestionsStackLayout.previousPage();
                }
                onNextButtonClicked: {
                    demographicQuestionsStackLayout.nextPage();
                }
            }

            // Select your ethnicity
            PageEthnicity {
                id: pageEthnicity

                onEthnicityChosen: {
                    console.log("onEthnicityChosen " + value);
                    userProfile.setUserEthnicity(userProfile.userId, value, function (err) {
                        if (err) {
                            console.log(err.message);
                        }
                    });
                }
                onPreviousButtonClicked: {
                    demographicQuestionsStackLayout.previousPage();
                }
                onNextButtonClicked: {
                    demographicQuestionsStackLayout.nextPage();
                }
            }

            // Select your age
            PageAge {
                id: pageAge

                onAgeChosen: {
                    console.log("onAgeChosen " + value);
                    userProfile.setUserAge(userProfile.userId, value, function (err) {
                        if (err) {
                            console.log(err.message);
                        }
                    });
                }
                onPreviousButtonClicked: {
                    demographicQuestionsStackLayout.previousPage();
                }
                onNextButtonClicked: {
                    // TODO
                    // if this is the entry kiosk, show the "enjoy your visit" page.
                    // if this is the center kiosk, go to the questions
                    if (kioskConfig.kiosk_mode == "entry") {
                        demographicQuestionsStackLayout.nextPage();
                        // TODO: after a short delay, go to screensaver.
                    } else {
                        // kiosk_mode should be central:
                        mainStackLayout.nextPage();
                    }
                }
            }

            // Enjoy your visit (only shown in the entry kiosk)
            ColumnLayout {
                Label {
                    BilingualText {
                        id: textThankYou
                        language: lang
                        textEn: "Thank you so much!"
                        textFr: "Merci beaucoup!"
                    }
                    text: textThankYou.text
                    font.capitalization: Font.AllUppercase
                }
                Label {
                    BilingualText {
                        id: textStartYourVisit
                        language: lang
                        textEn: "You can now\nstart your visit!"
                        textFr: "Vous pouvez maintenant\ncommencer votre visite."
                    }
                    text: textStartYourVisit.text
                    font.capitalization: Font.AllUppercase
                }
            }
        }

        /**
         * Section with the questions.
         *
         * This is our main two-column layout.
         */
        RowLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.margins: 0
            spacing: 6

            /**
             * List of buttons to access each question page.
             */
            ListView {
                id: pageButtonsListView

                function setCurrentPage(index, pageNumberText) {
                    questionsStackLayout.currentIndex = index; // index of the item in the model
                    currentPageNumberLabel.text = pageNumberText;
                    pageButtonsModel.highlightButton(pageNumberText);
                }

                Component.onCompleted: {
                    setCurrentPage(0, "01"); // Initial value
                }

                Layout.margins: 0
                Layout.fillWidth: false
                Layout.fillHeight: true
                orientation: Qt.Vertical
                width: currentItem.width

                // There are 15 pages, and that should not change.
                model: ModelPageButtons {
                    id: pageButtonsModel
                }

                // Let's draw each button:
                delegate: WidgetQuestionButton {
                    buttonTitle: pageNumber // Property of the items in the list model this ListView uses.
                    // FIXME: Perhaps we should fine-tune the height of these buttons
                    height: parent.height / parent.count
                    spacing: 0
                    highlighted: isHighlighted // Property of the items in the list model this ListView uses.

                    onButtonClicked: {
                        pageButtonsListView.setCurrentPage(index, pageNumber);
                    }
                }
            }
            /**
             * Displays the current page number.
             */
            RowLayout {
                Rectangle {
                    Layout.minimumWidth: 80
                    Layout.minimumHeight: 700
                    Layout.leftMargin: 20
                    Layout.rightMargin: 20
                    color: "#000"

                    Label {
                        id: currentPageNumberLabel
                        text: "01" // Changed dynamically
                        Layout.alignment: Qt.AlignCenter
                        font.capitalization: Font.AllUppercase
                        color: "#ffffff"
                        font.bold : true
                        font.pixelSize: 75
                        visible: sliderWidgetVisibility
                    }
                }
            }

            RowLayout {
                // Outline around the questions
                Rectangle{
                    Layout.minimumWidth: 1000
                    Layout.minimumHeight: 800
                    color: "#000"
                    border.color: "grey"
                    border.width: 5

                    // Contents
                    StackLayout {
                        id: questionsStackLayout

                        readonly property int index_FIRST_QUESTION: 0

                        currentIndex: 0
                        Layout.fillWidth: true
                        Layout.fillHeight: true

                        // The pages for single and multiple questions:

                        // Page 01
                        PageSingleQuestion {
                            modelQuestions: modelQuestions
                            questionIdentifier: "incidence_drogue"
                            datavizSender: oscSender
                            serviceClient: userProfile
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                        }

                        // TODO: page 02 single decriminalisation_crimes_non_violents


                        // TODO page 03 single systeme_bureaucrate

                        // page 03 (multiple)
                        PageMultipleQuestion {
                            // FIXME: the main question text should be common (most often) to all questions in a multiple-question page:
                            questionIdentifiers: ["equitable_victimes", "equitable_vulnerables", "equitable_jeunes_contrevenants", "equitable_riches", "equitable_minorites_culturelles"]
                            datavizSender: oscSender
                            serviceClient: userProfile
                            modelQuestions: modelQuestions
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            mainQuestionText: modelQuestions.findQuestion('equitable_victimes').question_fr;
                        }

                        // TODO: remaining questions
                    }


                }

                WidgetPreviousNext {
                    readonly property int num_PAGES: 15

                    onNextButtonClicked: {
                        var i = questionsStackLayout.currentIndex;
                        if (i === num_PAGES) {
                            mainStackLayout.currentIndex = mainStackLayout.index_EXIT_SECTION;
                        } else {
                            i += 1;
                            questionsStackLayout.currentIndex = i;
                        }
                    }
                    onPreviousButtonClicked: {
                        var i = questionsStackLayout.currentIndex;
                        if (i === 0) {
                            console.log("Cannot go further up");
                            console.log("FIXME: hide the up button if page is 0");
                        } else {
                            i -= 1;
                            questionsStackLayout.currentIndex = i;
                        }
                    }
                }
            }
        }

        // Exit section:
        StackLayout {
            id: exitSection

            readonly property int index_LAST_QUESTIONS: 0

            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.margins: 0

            PageFinalQuestion {
                // TODO
            }

            ColumnLayout {
                Label {
                    BilingualText {
                        id: textMerci
                        language: lang
                        textEn: "Thanks a lot."
                        textFr: "Merci beaucoup."
                    }
                    text: textMerci.text
                    font.capitalization: Font.AllUppercase
                }
                Label {
                    BilingualText {
                        id: textGiveYouKeyBack
                        language: lang
                        textEn: "Don't forget\nto give your key back."
                        textFr: "N'oubliez pas\nde remettre votre clé."
                    }
                    text: textGiveYouKeyBack.text
                    font.capitalization: Font.AllUppercase
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
