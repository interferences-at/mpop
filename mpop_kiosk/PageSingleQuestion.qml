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
    property string titleText: ""
    property var questionIdentifier: null
    property string questionText: ""
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

            RowLayout{
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignCenter | Qt.AlignCenter
            RoundButton {
                id: button
                icon.source: "uparrow.svg"
                Layout.fillHeight: true
                Layout.fillWidth: true
                Layout.maximumWidth: 80
                Layout.maximumHeight: 80

                onClicked: {
                    sliderWidgetVisibility = true
                    buttonsVisibility = false
                    ageVisualization = false
                }
            }
            Button{
               Layout.alignment: Qt.AlignHCenter
               text: qsTr("Data visualization")
               background: none
               font.pixelSize: 30
               onClicked: {
                   buttonsVisibility = true
                   sliderWidgetVisibility  = false
                   ageVisualization = false
               }
            }
        }
            Label {
                Layout.alignment: Qt.AlignCenter
                text: qsTr(titleText)
                font.pixelSize: 36
            }
            Label {
                Layout.alignment: Qt.AlignCenter
                text: qsTr(questionText)
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
                text: qsTr("Please select a basic viewing setting")
                font.pixelSize: 30
                visible: buttonsVisibility
            }

            RowLayout{
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                spacing: 24
                visible: buttonsVisibility
                Button {
                    text: qsTr("Age")
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
                Button {
                   text: qsTr("Genre")
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
                Button {
                   text: qsTr("Culture")
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
                Button {
                   text: qsTr("Language")
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

            RowLayout{
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignRight | Qt.AlignRight
                visible: buttonsVisibility
                RoundButton {
                    text: qsTr("Left")
                    //icon.source: "leftarrow.svg"
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    Layout.maximumWidth: 80
                    Layout.maximumHeight: 80

                    onClicked: {
                        sliderWidgetVisibility = true
                        buttonsVisibility = false
                        ageVisualization = false
                    }
                }
                RoundButton {
                    text: qsTr("Right")
                    //icon.source: "rightarrow.svg"
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    Layout.maximumWidth: 80
                    Layout.maximumHeight: 80

                    onClicked: {
                        sliderWidgetVisibility = false
                        buttonsVisibility = false
                        ageVisualization = true
                    }
                }

            }

            Label {
                Layout.alignment: Qt.AlignLeft
                text: qsTr("Visualization by age of respondents")
                font.pixelSize: 24
                visible: ageVisualization
                leftPadding: 35
            }
            Label {
                Layout.alignment: Qt.AlignLeft
                text: qsTr("You can select the following filters to discover new correlations")
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
                    text: qsTr("Culture")
                    background: Rectangle {
                       color: "#000"
                       implicitWidth: 130
                       implicitHeight: 55
                   }
                   onClicked: {

                   }
                }
                Button {
                    text: qsTr("All")
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
                   text: qsTr("Quebecois")
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
                   text: qsTr("Canadian")
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
                   text: qsTr("Native")
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
                   text: qsTr("American")
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
                   text: qsTr("European")
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
                   text: qsTr("Other")
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
                    text: qsTr("Genre")
                    background: Rectangle {
                       color: "#000"
                       implicitWidth: 130
                       implicitHeight: 55
                   }
                   onClicked: {

                   }
                }
                Button {
                    text: qsTr("All")
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
                   text: qsTr("Women")
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
                   text: qsTr("Men")
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
                   text: qsTr("Other")
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
                    text: qsTr("Time")
                    background: Rectangle {
                       color: "#000"
                       implicitWidth: 130
                       implicitHeight: 55
                   }
                   onClicked: {

                   }
                }
                Button {
                    text: qsTr("All")
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
                   text: qsTr("Today")
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
                   text: qsTr("This Year")
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
                   text: qsTr("From the beginning")
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

            RowLayout{
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignRight | Qt.AlignRight
                visible: ageVisualization
                RoundButton {
                    text: qsTr("Left")
                    //icon.source: "leftarrow.svg"
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    Layout.maximumWidth: 80
                    Layout.maximumHeight: 80

                    onClicked: {
                        sliderWidgetVisibility = false
                        buttonsVisibility = true
                        ageVisualization = false
                    }
                }
                RoundButton {
                    text: qsTr("Right")
                    //icon.source: "rightarrow.svg"
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    Layout.maximumWidth: 80
                    Layout.maximumHeight: 80

                    onClicked: {
                        //sliderWidgetVisibility = false
                        //buttonsVisibility = false
                        //ageVisualization = true
                    }
                }

            }


        }

    }

    // FIXME: Should the WidgetPreviousNext be part of the main.qml instead?
    /*WidgetPreviousNext {
        onNextButtonClicked: thisPage.nextButtonClicked()
        onPreviousButtonClicked: thisPage.previousButtonClicked()
    }*/

    // TODO: Sub-page: Choose Single Question Mode
    // TODO: Sub-page: Dataviz Single Question By Age
    // TODO: Sub-page: Dataviz Single Question By Gender
    // TODO: Sub-page: Dataviz Single Question By Ethnicity
}
