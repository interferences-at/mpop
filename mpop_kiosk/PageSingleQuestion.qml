import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

/**
 * Page that contains a single question.
 */
ColumnLayout {
    id: thisPage

    property var modelQuestions: null
    property var serviceClient: null
    property var datavizSender: null
    property string minText: ""
    property string maxText: ""
    property string titleText: "Question 01"
    property var questionIdentifier: null
    property string questionText: null
    property bool sliderWidgetVisibility: true
    property bool buttonsVisibility: false
    property bool ageVisualization: false

    signal nextButtonClicked()
    signal previousButtonClicked()

    Component.onCompleted: {
        var item = modelQuestions.findQuestion(questionIdentifier);

        var current_lang = "fr"; // TODO: check what current language is
        if (current_lang === "fr") {
            questionText = item.question_fr;
            minText = item.min_fr;
            maxText = item.max_fr;
        } else {
            questionText = item.question_en;
            minText = item.min_en;
            maxText = item.max_en;
        }

        // TODO: Retrieve value for user from service and populate the slider, if set.
    }
    StackLayout{
        currentIndex: 0
        ColumnLayout{
            Button{
               Layout.alignment: Qt.AlignHCenter
               text: "Data visualization"
               background: none
               font.pixelSize: 30
               onClicked: {
                   buttonsVisibility = true
                   sliderWidgetVisibility  = false
               }
            }
            Label {
                Layout.alignment: Qt.AlignCenter
                text: titleText
                font.pixelSize: 36
            }
            Label {
                Layout.alignment: Qt.AlignCenter
                text: questionText
                font.pixelSize: 30
                bottomPadding: 30
            }

            WidgetAnswerSlider {
                Layout.fillWidth: true
                sliderValue: 50
                textLeft: minText
                textRight: maxText
                showNumber: false
                visible: sliderWidgetVisibility
            }

            Label {
                Layout.alignment: Qt.AlignCenter
                text: 'Please select a basic viewing setting'
                font.pixelSize: 30
                visible: buttonsVisibility
            }

            RowLayout{
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                spacing: 24
                visible: buttonsVisibility
                Button {
                    text: "Age"
                    background: Rectangle {
                       color: "#000"
                       implicitWidth: 140
                       implicitHeight: 55
                       border.color: "#fff"
                       border.width: 1
                       radius: 2
                   }
                   onClicked: {
                       buttonsVisibility = false
                       sliderWidgetVisibility  = false
                       ageVisualization = true
                   }
                }
                Button{
                   text: "Genre"
                   background: Rectangle {
                      color: "#000"
                      implicitWidth: 140
                      implicitHeight: 55
                      border.color: "#fff"
                      border.width: 1
                      radius: 2
                   }
                   onClicked: {
                       buttonsVisibility = false
                       sliderWidgetVisibility  = false
                       ageVisualization = true
                   }
                }

                Button{
                   text: "Culture"
                   background: Rectangle {
                      color: "#000"
                      implicitWidth: 140
                      implicitHeight: 55
                      border.color: "#fff"
                      border.width: 1
                      radius: 2
                   }
                   onClicked: {
                       buttonsVisibility = false
                       sliderWidgetVisibility  = false
                       ageVisualization = true
                   }
                }
                Button{
                   text: "Language"
                   background: Rectangle {
                      color: "#000"
                      implicitWidth: 140
                      implicitHeight: 55
                      border.color: "#fff"
                      border.width: 1
                      radius: 2
                   }
                   onClicked: {
                       buttonsVisibility = false
                       sliderWidgetVisibility  = false
                       ageVisualization = true
                   }
                }
            }

            Label {
                Layout.alignment: Qt.AlignLeft
                text: 'Visualization by age of respondents'
                font.pixelSize: 24
                visible: ageVisualization
                leftPadding: 35
            }
            Label {
                Layout.alignment: Qt.AlignLeft
                text: 'You can select the following filters to discover new correlations'
                font.pixelSize: 24
                visible: ageVisualization
                leftPadding: 35
                bottomPadding: 30
            }

            RowLayout{

                Layout.fillWidth: true
                Layout.alignment: Qt.AlignLeft | Qt.AlignLeft
                spacing: 24
                visible: ageVisualization
                Button {
                    text: "Culture"
                    background: Rectangle {
                       color: "#000"
                       implicitWidth: 130
                       implicitHeight: 55
                   }
                   onClicked: {

                   }
                }
                Button {
                    text: "Tous"
                    background: Rectangle {
                       color: "#000"
                       implicitWidth: 130
                       implicitHeight: 55
                       border.color: "#fff"
                       border.width: 1
                       radius: 2
                   }
                   onClicked: {

                   }
                }
                Button{
                   text: "Québécoise"
                   background: Rectangle {
                      color: "#000"
                      implicitWidth: 130
                      implicitHeight: 55
                      border.color: "#fff"
                      border.width: 1
                      radius: 2
                   }
                   onClicked: {

                   }
                }

                Button{
                   text: "Canadienne"
                   background: Rectangle {
                      color: "#000"
                      implicitWidth: 130
                      implicitHeight: 55
                      border.color: "#fff"
                      border.width: 1
                      radius: 2
                   }
                   onClicked: {

                   }
                }
                Button{
                   text: "Autochtone"
                   background: Rectangle {
                      color: "#000"
                      implicitWidth: 130
                      implicitHeight: 55
                      border.color: "#fff"
                      border.width: 1
                      radius: 2
                   }
                   onClicked: {

                   }
                }
                Button{
                   text: "Américaine"
                   background: Rectangle {
                      color: "#000"
                      implicitWidth: 130
                      implicitHeight: 55
                      border.color: "#fff"
                      border.width: 1
                      radius: 2
                   }
                   onClicked: {

                   }
                }
                Button{
                   text: "Européenne"
                   background: Rectangle {
                      color: "#000"
                      implicitWidth: 130
                      implicitHeight: 55
                      border.color: "#fff"
                      border.width: 1
                      radius: 2
                   }
                   onClicked: {

                   }
                }
                Button{
                   text: "Autre"
                   background: Rectangle {
                      color: "#000"
                      implicitWidth: 130
                      implicitHeight: 55
                      border.color: "#fff"
                      border.width: 1
                      radius: 2
                   }
                   onClicked: {

                   }
                }
            }

            RowLayout{

                Layout.fillWidth: true
                Layout.alignment: Qt.AlignLeft | Qt.AlignLeft
                spacing: 24
                visible: ageVisualization
                Button {
                    text: "Genre"
                    background: Rectangle {
                       color: "#000"
                       implicitWidth: 130
                       implicitHeight: 55
                   }
                   onClicked: {

                   }
                }
                Button {
                    text: "Tous"
                    background: Rectangle {
                       color: "#000"
                       implicitWidth: 130
                       implicitHeight: 55
                       border.color: "#fff"
                       border.width: 1
                       radius: 2
                   }
                   onClicked: {

                   }
                }
                Button{
                   text: "Femme"
                   background: Rectangle {
                      color: "#000"
                      implicitWidth: 130
                      implicitHeight: 55
                      border.color: "#fff"
                      border.width: 1
                      radius: 2
                   }
                   onClicked: {

                   }
                }

                Button{
                   text: "Homme"
                   background: Rectangle {
                      color: "#000"
                      implicitWidth: 130
                      implicitHeight: 55
                      border.color: "#fff"
                      border.width: 1
                      radius: 2
                   }
                   onClicked: {

                   }
                }

                Button{
                   text: "Autre"
                   background: Rectangle {
                      color: "#000"
                      implicitWidth: 130
                      implicitHeight: 55
                      border.color: "#fff"
                      border.width: 1
                      radius: 2
                   }
                   onClicked: {

                   }
                }
            }

            RowLayout{

                Layout.fillWidth: true
                Layout.alignment: Qt.AlignLeft | Qt.AlignLeft
                spacing: 24
                visible: ageVisualization
                Button {
                    text: "Temps"
                    background: Rectangle {
                       color: "#000"
                       implicitWidth: 130
                       implicitHeight: 55
                   }
                   onClicked: {

                   }
                }
                Button {
                    text: "Tous"
                    background: Rectangle {
                       color: "#000"
                       implicitWidth: 130
                       implicitHeight: 55
                       border.color: "#fff"
                       border.width: 1
                       radius: 2
                   }
                   onClicked: {

                   }
                }
                Button{
                   text: "Aujourd'hui"
                   background: Rectangle {
                      color: "#000"
                      implicitWidth: 130
                      implicitHeight: 55
                      border.color: "#fff"
                      border.width: 1
                      radius: 2
                   }
                   onClicked: {

                   }
                }

                Button{
                   text: "Cette année"
                   background: Rectangle {
                      color: "#000"
                      implicitWidth: 130
                      implicitHeight: 55
                      border.color: "#fff"
                      border.width: 1
                      radius: 2
                   }
                   onClicked: {

                   }
                }

                Button{
                   text: "Depuis le début"
                   background: Rectangle {
                      color: "#000"
                      implicitWidth: 140
                      implicitHeight: 55
                      border.color: "#fff"
                      border.width: 1
                      radius: 2
                   }
                   onClicked: {

                   }
                }
            }
        }

    }

    // FIXME: Should the WidgetPreviousNext be part of the main.qml instead?
    WidgetPreviousNext {
        onNextButtonClicked: thisPage.nextButtonClicked()
        onPreviousButtonClicked: thisPage.previousButtonClicked()
    }

    // TODO: Sub-page: Choose Single Question Mode
    // TODO: Sub-page: Dataviz Single Question By Age
    // TODO: Sub-page: Dataviz Single Question By Gender
    // TODO: Sub-page: Dataviz Single Question By Ethnicity
}
