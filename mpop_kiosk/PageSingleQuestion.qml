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
    property string lang: ""

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

    BilingualText {
        id: textDataVisualization
        language: thisPage.lang
        textFr: "Visualisation de données"
        textEn: "Data visualization"
    }

    /**
     * StackLayout to switch from answering the question to data viz.
     */
    StackLayout {
        id: questionDatavizStackLayout

        currentIndex: 0

        ColumnLayout{

            //layout for header of the page
            RowLayout{
                Layout.alignment: Qt.AlignCenter | Qt.AlignCenter

                Rectangle {
                    color: '#000'
                    Layout.fillWidth: true
                    Layout.preferredHeight: 60
                    Layout.preferredWidth: 600

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
                        text: textDataVisualization.text
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
                    text: titleText
                    font.pixelSize: 36
                }
                Label {
                    Layout.alignment: Qt.AlignCenter
                    text: questionText
                    font.pixelSize: 30
                    bottomPadding: 30
                    leftPadding: 60
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
                Layout.leftMargin: 400
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

            ColumnLayout {
                Layout.leftMargin: 30
                spacing: 30

                Repeater {
                    // model
                    model: ListModel {
                        ListElement {
                            sectionTitle: qsTr("Culture")
                            filters: [
                                ListElement { label: qsTr("All") },
                                ListElement { label: qsTr("Quebecois") },
                                ListElement { label: qsTr("Canadian") },
                                ListElement { label: qsTr("Native") },
                                ListElement { label: qsTr("American") },
                                ListElement { label: qsTr("European") },
                                ListElement { label: qsTr("Other") }
                            ]
                        }

                        ListElement {
                            sectionTitle: qsTr("Genre")
                            filters: [
                                ListElement { label: qsTr("All") },
                                ListElement { label: qsTr("Male") },
                                ListElement { label: qsTr("Female") },
                                ListElement { label: qsTr("Other") }
                            ]
                        }

                        ListElement {
                            sectionTitle: qsTr("Time")
                            filters: [
                                ListElement { label: qsTr("All") },
                                ListElement { label: qsTr("Today") },
                                ListElement { label: qsTr("This Year") },
                                ListElement { label: qsTr("From the beginning") }
                            ]
                        }
                    }

                    // display
                    ColumnLayout {
                        Layout.fillWidth: true
                        Layout.alignment: Qt.AlignLeft

                        spacing: 15
                        visible: ageVisualization

                        Text {
                            text: sectionTitle
                            color: "#fff"
                            font {
                                family: "Trim SemiBold"
                                weight: Font.DemiBold
                                pixelSize: 11
                                letterSpacing: 11 * 25 / 1000
                                capitalization: Font.AllUppercase
                            }
                        }

                        GridLayout {
                            Layout.leftMargin: -10
                            columnSpacing: 10

                            Repeater {
                                model: filters
                                WidgetFilterButton { text: label }
                            }
                        }
                    }
                }
            }



            RowLayout {
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
        if (btnid === "ageBtn") {
            ageHighlighted = true;
            genreHighlighted = false;
            cultureHighlighted = false;
            languageHighlighted = false;
        } else if(btnid === "genreBtn") {
            ageHighlighted = false;
            genreHighlighted = true;
            cultureHighlighted = false;
            languageHighlighted = false;
        } else if(btnid === "cultureBtn") {
            ageHighlighted = false;
            genreHighlighted = false;
            cultureHighlighted = true;
            languageHighlighted = false;
        } else if(btnid === "langBtn"){
            ageHighlighted = false;
            genreHighlighted = false;
            cultureHighlighted = false;
            languageHighlighted = true;
        }
    }

    // TODO: Sub-page: Choose Single Question Mode
    // TODO: Sub-page: Dataviz Single Question By Age
    // TODO: Sub-page: Dataviz Single Question By Gender
    // TODO: Sub-page: Dataviz Single Question By Ethnicity
}
