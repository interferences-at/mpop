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
    property bool ageHighlighted: false
    property bool genreHighlighted: false
    property bool cultureHighlighted: false
    property bool languageHighlighted: false
    property bool buttonTextHighlight: true

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

            //layout for header of the page
            RowLayout{
                Layout.alignment: Qt.AlignCenter | Qt.AlignCenter

                Rectangle {
                    color: 'black'
                    Layout.fillWidth: true
                    Layout.preferredHeight: 60
                    Layout.preferredWidth: 300

                    Button {
                        id: button
                        icon.source: "uparrow.svg"
                        anchors.right: datavisual.left;
                        background: Rectangle {
                            color: "#000"
                            implicitWidth: 60
                            implicitHeight: 40
                        }

                        onClicked: {
                            sliderWidgetVisibility = true
                            buttonsVisibility = false
                            ageVisualization = false
                        }
                    }
                    Button{
                        id:datavisual
                        anchors.centerIn: parent
                        text: qsTr("Data visualization")
                        background: Rectangle {
                            color: "#000"
                            implicitWidth: 60
                            implicitHeight: 40
                        }
                        font.pixelSize: 30

                        onClicked: {
                            buttonsVisibility = true
                            sliderWidgetVisibility  = false
                            ageVisualization = false
                        }
                    }
                    Button {
                        id:button2
                        icon.source: "uparrow.svg"
                        anchors.left: datavisual.right;
                        background: Rectangle {
                            color: "#000"
                            implicitWidth: 60
                            implicitHeight: 40
                        }
                        onClicked: {
                            sliderWidgetVisibility = true
                            buttonsVisibility = false
                            ageVisualization = false

                        }
                    }
                    RoundButton {
                        anchors.left: button2.right;
                        anchors.leftMargin: 250;
                        text: "X"
                        font.bold : true
                        font.pixelSize: 20

                        padding : 20
                    }
                }
            }

            ColumnLayout{
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
                    width: 300
                    sliderValue: 50
                    textLeft: minText
                    textRight: maxText
                    showNumber: false
                    visible: sliderWidgetVisibility
                }

                Rectangle {
                    width: 600
                    height: 80
                    Layout.leftMargin: 250
                    visible: buttonsVisibility
                    color: "black"

                    Label {
                        Layout.alignment: Qt.AlignCenter
                        text: qsTr("Please select a basic viewing setting")
                        font.pixelSize: 30
                        visible: buttonsVisibility
                    }
                }
            }

            RowLayout{
                id: root
                Layout.fillWidth: true
                Layout.fillHeight: true
                //Layout.margins: 0
                Layout.leftMargin: 170
                //Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                spacing: 24
                visible: buttonsVisibility
                Button {
                    id: ageBtn
                    text: qsTr("Age")
                    highlighted : ageHighlighted
                    background: Rectangle {
                       color: "#000"
                       implicitWidth: 140
                       implicitHeight: 55
                       border.color: ageHighlighted ? "#FF0000" : "#fff"
                       border.width: ageHighlighted ? 2 : 1
                       radius: 2
                   }
                   onClicked: {
                        highlighSelectedButton("ageBtn")
                   }
                }
                Button {
                   id: genreBtn
                   text: qsTr("Genre")
                   highlighted: genreHighlighted
                   background: Rectangle {
                      color: "#000"
                      implicitWidth: 140
                      implicitHeight: 55
                      border.color: genreHighlighted ? "#FF0000" : "#fff"
                      border.width: genreHighlighted ? 2 : 1
                      radius: 2
                   }
                   onClicked: {
                        highlighSelectedButton("genreBtn")
                   }
                }
                Button {
                   id : cultureBtn
                   text: qsTr("Culture")
                   highlighted: cultureHighlighted
                   background: Rectangle {
                      color: "#000"
                      implicitWidth: 140
                      implicitHeight: 55
                      border.color: cultureHighlighted ? "#FF0000" : "#fff"
                      border.width: cultureHighlighted ? 2 : 1
                      radius: 2
                   }
                   onClicked: {
                        highlighSelectedButton("cultureBtn")
                   }
                }
                Button {
                   id: langBtn
                   text: qsTr("Language")
                   highlighted: languageHighlighted
                   background: Rectangle {
                      color: "#000"
                      implicitWidth: 140
                      implicitHeight: 55
                      border.color: languageHighlighted ? "#FF0000" : "#fff"
                      border.width: languageHighlighted ? 2 : 1
                      radius: 2
                   }
                   onClicked: {
                       highlighSelectedButton("langBtn")
                   }
                }

            }

            RowLayout{
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignCenter | Qt.AlignCenter
                Layout.topMargin: 50
                Layout.leftMargin: 550
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
                Layout.leftMargin: 30
                Layout.topMargin: 10
                visible: ageVisualization
                leftPadding: 35
            }
            Label {
                Layout.alignment: Qt.AlignLeft
                text: qsTr("You can select the following filters to discover new correlations")
                font.pixelSize: 24
                Layout.leftMargin: 30
                visible: ageVisualization
                leftPadding: 35
                bottomPadding: 30
            }


            RowLayout{
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignLeft | Qt.AlignLeft
                Layout.leftMargin: 30
                spacing: 24
                visible: ageVisualization

                Button {
                    id: culturebtn
                    text: qsTr("Culture")
                    background: Rectangle {
<<<<<<< HEAD
                       color: "#000"
                       implicitWidth: 130
                       implicitHeight: 55
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
                        border.color = "red"
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
=======
                        color: "#000"
                        implicitWidth: 130
                        implicitHeight: 55
                    }
                    onClicked: {

                    }
>>>>>>> 60a66dc... layout for the single question
                }

                GridLayout {
                    anchors.fill: parent
                    columns: 5
                    anchors.leftMargin: 155
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
            }

            RowLayout{

                Layout.fillWidth: true
                Layout.alignment: Qt.AlignLeft | Qt.AlignLeft
                Layout.leftMargin: 30
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
                Layout.leftMargin: 30
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
                Layout.alignment: Qt.AlignCenter | Qt.AlignCenter
                Layout.topMargin: 30
                Layout.leftMargin: 500
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
            }


        }

    }

    function highlighSelectedButton(btnid){
        if(btnid === "ageBtn"){
            ageHighlighted      = true
            genreHighlighted    = false
            cultureHighlighted  = false
            languageHighlighted = false
         }else if(btnid === "genreBtn"){
            ageHighlighted      = false
            genreHighlighted    = true
            cultureHighlighted  = false
            languageHighlighted = false
         }else if(btnid === "cultureBtn"){
            ageHighlighted      = false
            genreHighlighted    = false
            cultureHighlighted  = true
            languageHighlighted = false
         }else if(btnid === "langBtn"){
            ageHighlighted      = false
            genreHighlighted    = false
            cultureHighlighted  = false
            languageHighlighted = true
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
