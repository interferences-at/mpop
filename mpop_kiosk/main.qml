import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtQuick.Window 2.11
import QtGraphicalEffects 1.0
import Screensaver 1.0

/**
 * Main window of this application
 * We follow the standard QML conventions: https://doc.qt.io/qt-5/qml-codingconventions.html
 */
ApplicationWindow {
    id: window

    property string lastRfidRead: ""
    property string lastMessageReceived: ""
    property alias lang: userProfile.language // All the BilingualText items watch this value
    property alias rfidTag: userProfile.rfidTag
    property bool invertedTheme: true         // used to differentiate light from dark mode (might get turned to string to account for accent-bg variant)

    // Those aliases are accessed directly via the global window item:
    property alias userProfile: userProfile
    property alias datavizManager: datavizManager

    readonly property string const_KIOSK_MODE_ENTRY: "entrance"
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

    background: Rectangle {
        color: Palette.lightBlack
    }

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
            screensaver.visible = true;
        }
    }


    /**
     * Communicates with the kiosk_service via JSON-RPC 2.0 to INSERT/UPDATE/SELECT in the MySQL database.
     */
    UserProfile {
        id: userProfile

        function goToDemographicQuestions() {
            mainStackLayout.currentIndex = mainStackLayout.index_DEMOGRAPHIC_QUESTIONS;
            demographicQuestionsStackLayout.currentIndex = demographicQuestionsStackLayout.index_MY_LANGUAGE;

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

        onUserAnswersUpdated: {
            // Update the value of each slider according to the answer of the visitor for that slider:
            var numQuestions = modelQuestions.rowCount;
            for(var i = 0; i < numQuestions; i ++) {
                var item = modelQuestions.get(i);
                var identifier = item.identifier;

                // retrieve it in the property of the UserProfile
                if (answers.hasOwnProperty(identifier)) {
                    // TODO var theQuestionPage =
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
     * This is where we implement all OSC message sending.
     */
    DatavizManager {
        id: datavizManager

        oscMessageSender: oscSender
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
            id: screensaverrectangle

            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.margins: 0
            color: "black"

            Timer {
                id: hideTextTimer

                interval: 3 * 1000
                repeat: false
                running: false
                triggeredOnStart: false
                onTriggered: {
                    console.log("hideTextTimer triggered");
                    clickedMessage.visible = false;
                }
            }

            Screensaver {
                id: screensaver
                anchors.fill: parent
            }

            Label {
                id: clickedMessage

                text: "Scannez votre puce pour commencer.\nScan your tag to start."
                color: "#fff"
                font.weight: Font.Bold
                visible: false
                font.pointSize: 30
                anchors.horizontalCenter: parent.horizontalCenter
                topPadding: 300
                leftPadding: 150
            }

            // touch area of your root component
            MouseArea {
                anchors.fill: parent
                propagateComposedEvents: true
                onClicked: {
                    console.log("Screensaver touched");
                    clickedMessage.visible = true;
                    hideTextTimer.start();
                    idleTimer.running = false;
                    idleTimer.running = true;
                }
            }
        }

        /**
         * Section with the demographic question of the entry kiosk.
         *
         * This is our main two-column layout.
         */
        RowLayout {

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

                readonly property int index_MY_LANGUAGE: 0
                readonly property int index_MY_GENDER: 1
                readonly property int index_MY_ETHNICITY: 2
                readonly property int index_MY_AGE: 3
                readonly property int index_ENJOY_YOUR_VISIT: 4

                PageEntrance {
                    id: pageLanguage
                    sideLabel: BilingualText {
                        textFr: "Choisir une langue"
                        textEn: "Choose a language"
                    }
                    model: ListModel {
                        ListElement {
                            language_identifier: "fr"
                            text: "Français"
                        }
                        ListElement {
                            language_identifier: "en"
                            text: "English"
                        }
                    }

                    onChoiceClicked: {
                        console.log("onLanguageChosen " + index)
                        userProfile.setUserLanguage(userProfile.userId, model.get(index).language_identifier, function(err) {
                            if (err) {
                                console.log(err.message);
                            } else {
                                // Success. Nothing to do.
                                // The userProfile.language property should be updated.
                            }
                        });
                    }
                }

                // Select your gender
                PageEntrance {
                    id: pageGender
                    sideLabel: BilingualText {
                        textEn: "You are..."
                        textFr: "Vous êtes..."
                    }
                    model: ModelGenders {}

                    onChoiceClicked: {
                        console.log("onGenderChosen " + index)
                        userProfile.setUserGender(userProfile.userId, model.get(index).identifier, function (err) {
                            if (err)
                                console.log(err.message);
                        });
                    }
                }

                // Select your ethnicity
                PageEntrance {
                    id: pageEthnicity
                    sideLabel: BilingualText {
                        textEn: "To which nation do you identify the most?"
                        textFr: "À quelle nationvous identifiez-vousle plus ?"
                    }
                    model: ModelEthnicities {}

                    onChoiceClicked: {
                        console.log("onEthnicityChosen " + index)
                        userProfile.setUserEthnicity(userProfile.userId, model.get(index).identifier, function (err) {
                            if (err) {
                                console.log(err.message);
                            }
                        });
                    }
                }

                // Select your age
                PageEntrance {
                    id: pageAge
                    sideLabel: BilingualText {
                        textEn: "How old are you?"
                        textFr: "Quel âge avez-vous?"
                    }
                    model: 120

                    onChoiceClicked: {
                        console.log("onAgeChosen " + index);
                        userProfile.setUserAge(userProfile.userId, index, function (err) {
                            if (err) {
                                console.log(err.message);
                            }
                        });
                    }
                }

                // Enjoy your visit (only shown in the entry kiosk)
                ColumnLayout {
                    Label {
                        Layout.leftMargin: 40
                        Layout.topMargin: 100

                        BilingualText {
                            id: textThankYou
                            textEn: "Thank you so much!\nYou can now\nstart your visit."
                            textFr: "Merci beaucoup!\nVous pouvez maintenant\ncommencer votre visite."
                        }
                        text: textThankYou.text

                        font {
                            pixelSize: 57
                            capitalization: Font.AllUppercase
                        }
                    }
                }
            }

            WidgetPreviousNext {
                Layout.alignment: Qt.AlignBottom
                Layout.rightMargin: 25
                Layout.bottomMargin: 30
                visible: demographicQuestionsStackLayout.currentIndex !== demographicQuestionsStackLayout.count - 1
                showPrevButton: demographicQuestionsStackLayout.currentIndex > 0

                onNextButtonClicked: {
                    if (demographicQuestionsStackLayout.currentIndex === demographicQuestionsStackLayout.count - 2) {
                        console.log(kioskConfig.kiosk_mode);
                        // if this is the entry kiosk, show the "enjoy your visit" page.
                        // if this is the center kiosk, go to the questions
                        if (kioskConfig.kiosk_mode !== window.const_KIOSK_MODE_ENTRY) {
                            // kiosk_mode is central:
                            mainStackLayout.nextPage();
                            // prevent changing page
                            return;
                        }
                    }

                    // change demographic question page
                    demographicQuestionsStackLayout.nextPage();
                }

                onPreviousButtonClicked: {
                    demographicQuestionsStackLayout.previousPage();
                }
            }
        }


        /**
         * Section with the questions.
         *
         * This is our main two-column layout.
         */
        RowLayout {
            id: questionsContainer
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.margins: 0
            spacing: 0

            function setCurrentPage(index) {
                // format pageNumberText from index
                var pageNumberText = ('00' + (index + 1)).slice(-2);

                // propagate to subcomponents
                questionsStackLayout.currentIndex = index;
                pageButtonsListView.currentIndex = index;
                currentPageNumberLabel.text = pageNumberText;
            }

            /**
             * List of buttons to access each question page.
             */
            Rectangle {
                Layout.preferredWidth: 80
                Layout.fillHeight: true
                color: Palette.white
                border.color: Palette.lightBlack
                Layout.topMargin: -1
                Layout.bottomMargin: -1
                Layout.leftMargin: -1

                ColumnLayout {
                    anchors.fill: parent

                    // "Questions" label
                    Label {
                        Layout.fillWidth: true
                        Layout.preferredHeight: 170
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        text: "Questions"
                        font {
                            pixelSize: 20
                            capitalization: Font.AllUppercase
                        }
                        color: Palette.lightBlack
                        rotation: -90
                        transformOrigin: Item.Center
                    }

                    ListView {
                        id: pageButtonsListView

                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        orientation: Qt.Vertical

                        // There are 15 pages, and that should not change.
                        // oops!
                        // should instead directly feed from the total amount of questions
                        // to prevent having to change this every time the question count changes
                        // this model is convoluted and unnecessary!
                        model: ModelPageButtons {
                            id: pageButtonsModel
                        }

                        // Let's draw each button:
                        delegate: WidgetQuestionButton {
                            text: ("00" + (index + 1)).slice(-2) // Property of the items in the list model this ListView uses.
                            highlighted: index === pageButtonsListView.currentIndex // Property of the items in the list model this ListView uses.
                            afterCurrent: index > pageButtonsListView.currentIndex

                            onClicked: {
                                questionsContainer.setCurrentPage(index);
                            }
                        }
                    }

                    WidgetQuestionButton {
                        text: "Toutes"
                        font.pixelSize: 11
                        font.letterSpacing: 11 * 25 / 1000
                        Layout.fillWidth: true
                    }
                }
            }

            /**
             * Displays the current page number.
             */
            Item {
                Layout.fillWidth: true
                Layout.fillHeight: true

                WidgetGoToDataviz {
                    anchors.top: parent.top

                    // automatically checks if current page has dataviz enabled
                    toggled: questionsStackLayout.children[questionsStackLayout.currentIndex].datavizIndex !== 0
                    onClicked: questionsStackLayout.toggleDataviz()
                }

                RowLayout {
                    anchors.fill: parent
                    anchors.topMargin: 80
                    spacing: 0

                    Label {
                        Layout.fillHeight: true
                        Layout.alignment: Qt.AlignTop
                        Layout.preferredWidth: 295
                        topPadding: 55
                        horizontalAlignment: Text.AlignHCenter

                        id: currentPageNumberLabel
                        text: "01" // Changed dynamically
                        font.capitalization: Font.AllUppercase
                        color: "#ffffff"
                        font {
                            pixelSize: 206
                        }

                        background: Item {}

                        // border
                        Rectangle {
                            anchors.right: parent.right
                            height: parent.height
                            width: 1
                            color: "#fff"
                        }
                    }

                    // Outline around the questions
                    // Contents
                    StackLayout {
                        id: questionsStackLayout

                        readonly property int index_FIRST_QUESTION: 0

                        currentIndex: 0
                        Layout.fillHeight: true
                        Layout.preferredWidth: 1415

                        onCurrentIndexChanged: {
                            children[currentIndex].loadAnswersForCurrentVisitor();
                        }

                        /**
                         * Go to the dataviz mode, or leaves it - for the current page.
                         */
                        function toggleDataviz() {
                            children[currentIndex].toggleDataviz();
                        }

                        // The pages for single and multiple questions:
                        // TODO: wrap in Repeater and feed with model data

                        // Page 01 - single
                        PageQuestion {
                            modelQuestions: modelQuestions
                            questionIdentifiers: ["incidence_drogue"]
                            datavizSender: datavizManager
                            serviceClient: userProfile
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                        }

                        // Page 02 - single
                        PageQuestion {
                            modelQuestions: modelQuestions
                            questionIdentifiers: ["decriminalisation_crimes_non_violents"]
                            datavizSender: datavizManager
                            serviceClient: userProfile
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                        }

                        // Page 03 - single
                        PageQuestion {
                            modelQuestions: modelQuestions
                            questionIdentifiers: ["systeme_bureaucrate"]
                            datavizSender: datavizManager
                            serviceClient: userProfile
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                        }

                        // Page 04 - multiple
                        PageQuestion {
                            // FIXME: the main question text should be common (most often) to all questions in a multiple-question page:
                            questionIdentifiers: ["equitable_victimes", "equitable_vulnerables", "equitable_jeunes_contrevenants", "equitable_riches", "equitable_minorites_culturelles"]
                            datavizSender: datavizManager
                            serviceClient: userProfile
                            modelQuestions: modelQuestions // The whole model with all questions.
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                        }

                        // Page 05 - single
                        PageQuestion {
                            modelQuestions: modelQuestions
                            questionIdentifiers: ["confiance_systeme"]
                            datavizSender: datavizManager
                            serviceClient: userProfile
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                        }

                        // Page 06 - single
                        PageQuestion {
                            modelQuestions: modelQuestions
                            questionIdentifiers: ["interner"]
                            datavizSender: datavizManager
                            serviceClient: userProfile
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                        }

                        // Page 07 - single
                        PageQuestion {
                            modelQuestions: modelQuestions
                            questionIdentifiers: ["peine_plus_severes"]
                            datavizSender: datavizManager
                            serviceClient: userProfile
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                        }

                        // Page 08 - single
                        PageQuestion {
                            modelQuestions: modelQuestions
                            questionIdentifiers: ["taux_recidive"]
                            datavizSender: datavizManager
                            serviceClient: userProfile
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                        }

                        // Page 09 - single
                        PageQuestion {
                            modelQuestions: modelQuestions
                            questionIdentifiers: ["bon_traitement"]
                            datavizSender: datavizManager
                            serviceClient: userProfile
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                        }

                        // Page 10 - single
                        PageQuestion {
                            modelQuestions: modelQuestions
                            questionIdentifiers: ["confinement_solitaire"]
                            datavizSender: datavizManager
                            serviceClient: userProfile
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                        }

                        // Page 11 - multiple
                        PageQuestion {
                            modelQuestions: modelQuestions
                            questionIdentifiers: ["soins_physiques", "soins_mentaux"]
                            datavizSender: datavizManager
                            serviceClient: userProfile
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                        }

                        // Page 12 - single
                        PageQuestion {
                            modelQuestions: modelQuestions
                            questionIdentifiers: ["ressources_reinsertion"]
                            datavizSender: datavizManager
                            serviceClient: userProfile
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                        }

                        // Page 13 - single
                        PageQuestion {
                            modelQuestions: modelQuestions
                            questionIdentifiers: ["benefice_justice_reparatrice"]
                            datavizSender: datavizManager
                            serviceClient: userProfile
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                        }

                        // Page 14 - single
                        PageQuestion {
                            modelQuestions: modelQuestions
                            questionIdentifiers: ["developper_alternatives_prison"]
                            datavizSender: datavizManager
                            serviceClient: userProfile
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                        }

                        // Page 15 - multiple
                        PageQuestion {
                            modelQuestions: modelQuestions
                            questionIdentifiers: ["investir_education", "investir_sante_services_sociaux", "investir_emploi"]
                            datavizSender: datavizManager
                            serviceClient: userProfile
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                        }
                    }

                    Item {
                        Layout.fillHeight: true
                        Layout.preferredWidth: 130
                        z: 10
                        Layout.topMargin: -80

                        // border
                        Rectangle {
                            anchors.left: parent.left
                            height: parent.height
                            width: 1
                            color: "#fff"
                        }

                        ColumnLayout {
                            width: parent.width
                            height: parent.height
                            spacing: 0

                            ColumnLayout {
                                Layout.alignment: Qt.AlignTop | Qt.AlignHCenter
                                Layout.topMargin: 30
                                spacing: 5

                                Item {
                                    Layout.preferredWidth: 80
                                    Layout.preferredHeight: 80

                                    RoundButton {
                                        id: closeAppButton

                                        implicitWidth: 80
                                        implicitHeight: 80

                                        background: Rectangle {
                                            radius: 40
                                            color: closeAppButton.down ? Palette.accent : Palette.white
                                        }

                                        Image {
                                            anchors.centerIn: parent
                                            source: "qrc:/cross.svg"
                                        }
                                    }

                                    DropShadow {
                                        anchors.fill: closeAppButton
                                        horizontalOffset: 2
                                        verticalOffset: 4
                                        radius: 4
                                        samples: 9
                                        color: "#66000000"
                                        source: closeAppButton
                                    }
                                }

                                Label {
                                    Layout.maximumWidth: 80
                                    text: "Quitter le questionnaire"
                                    wrapMode: Text.WordWrap
                                    horizontalAlignment: Text.AlignHCenter
                                    color: "#ffffff"
                                    font {
                                        pixelSize: 11
                                        letterSpacing: 11 * 25 / 1000
                                        capitalization: Font.AllUppercase
                                    }
                                    Layout.alignment: Qt.AlignHCenter
                                }
                            }

                            WidgetPreviousNext {
                                Layout.alignment: Qt.AlignBottom | Qt.AlignHCenter
                                Layout.bottomMargin: 30

                                readonly property int num_PAGES: 15

                                showPrevButton: questionsStackLayout.currentIndex > 0

                                onNextButtonClicked: {
                                    var i = questionsStackLayout.currentIndex + 1;
                                    if (i === num_PAGES) {
                                        mainStackLayout.currentIndex = mainStackLayout.index_EXIT_SECTION;
                                    } else {
                                        questionsContainer.setCurrentPage(i);
                                    }
                                }
                                onPreviousButtonClicked: questionsContainer.setCurrentPage(questionsStackLayout.currentIndex - 1)
                            }
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
                    text: textMerci.text
                    font.capitalization: Font.AllUppercase

                    BilingualText {
                        id: textMerci
                        textEn: "Thanks a lot."
                        textFr: "Merci beaucoup."
                    }
                }
                Label {
                    text: textGiveYouKeyBack.text
                    font.capitalization: Font.AllUppercase

                    BilingualText {
                        id: textGiveYouKeyBack
                        textEn: "Don't forget\nto give your key back."
                        textFr: "N'oubliez pas\nde remettre votre clé."
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
