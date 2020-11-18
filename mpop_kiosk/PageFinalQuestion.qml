import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtQuick.Shapes 1.0
import AnswersView 1.0

/**
 * Page that contains final question.
 */
RowLayout {
    id: thisPage

    Layout.fillWidth: true
    Layout.fillHeight: true

    spacing: 0

    property var model
    property alias showPrevButton: widgetPreviousNext.showPrevButton

    property var myAnswers: [0, 0, 0]
    property var theirAnswers: [0, 0, 0]

    signal finalQuestionChosen(string value)
    signal previousClicked()
    signal nextClicked()

    function handleSliderMoved(index, value) {
        // TODO: connect to JSON-RPC API
        console.log("final question slider moved:", index, value);

        // Update locale slider Array
        thisPage.myAnswers[index] = value;
        // update my answers values
        answersView.myAnswers = thisPage.myAnswers;
    }

    function setDatavizAnswers() {
        const questionIds = [model.subquestions.count]

        for (let i = 0; i < model.subquestions.count; i++) {
            questionIds[i] = model.subquestions.get(i).identifier
        }

        window.userProfile.getAnswers(questionIds, 0, 100, "all", "all", "all", function(error, allAnswers) {
            if (error) {
                console.log(error);
            } else {
                for (let i = 0; i < Object.keys(allAnswers).length; i++) {
                    const key = Object.keys(allAnswers)[i];
                    const value = allAnswers[key];

                    thisPage.theirAnswers[i] = value > 0 ? value : 0;
                }
                // Update their Answers
                answersView.theirAnswers = thisPage.theirAnswers;
            }
        });
    }

    StackLayout {
        id: finalQuestionStackLayout

        readonly property int index_SLIDERS: 0
        readonly property int index_DATAVIZ: 1

        currentIndex: index_SLIDERS

        Layout.fillWidth: true
        Layout.fillHeight: true

        RowLayout {

            ColumnLayout {
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.alignment: Qt.AlignTop

                BilingualText {
                    id: textFinalQuestion
                    textFr: model.question_fr
                    textEn: model.question_en
                }

                Label {
                    Layout.preferredWidth: 700
                    Layout.leftMargin: 40
                    Layout.topMargin: 80
                    padding: 0

                    text: textFinalQuestion.text
                    font {
                        pixelSize: 28
                        capitalization: Font.AllUppercase
                    }
                    wrapMode: Text.WordWrap
                }

                ColumnLayout {
                    Layout.topMargin: 70
                    Layout.leftMargin: 150

                    spacing: 40

                    Repeater {
                        model: thisPage.model.subquestions

                        ColumnLayout {
                            spacing: 15

                            property alias sliderValue: answerSlider.sliderValue

                            BilingualText {
                                id: leftText
                                textEn: thisPage.model.min_en
                                textFr: thisPage.model.min_fr
                            }

                            BilingualText {
                                id: rightText
                                textEn: thisPage.model.max_en
                                textFr: thisPage.model.max_fr
                            }

                            BilingualText {
                                id: questionText
                                textEn: subtitle_en
                                textFr: subtitle_fr
                            }

                            Label {
                                Layout.alignment: Qt.AlignLeft
                                text: questionText.text
                                visible: questionText.text
                                font {
                                    family: "Red Hat Display"
                                    weight: Font.Medium
                                    pixelSize: 18
                                    letterSpacing: 18 * 10 / 1000
                                }
                                color: invertedTheme ? Palette.lightBlack : Palette.white
                            }
                            WidgetAnswerSlider {
                                id: answerSlider

                                sliderValue: 35
                                textLeft: leftText.text
                                textRight: rightText.text
                                showNumber: false

                                Layout.maximumWidth: 615

                                onSliderMoved: {
                                    thisPage.handleSliderMoved(model.index, intValue);
                                }
                            }
                        }
                    }
                }


            }

            Item { Layout.fillWidth: true }
        }

        Rectangle {
            id: answersViewRectangle

            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.margins: 0
            color: "black"

            AnswersView {
                id: answersView
                anchors.fill: parent
                render: mainStackLayout.currentIndex === mainStackLayout.index_EXIT_SECTION &&
                        finalQuestionStackLayout.currentIndex === finalQuestionStackLayout.index_DATAVIZ
                theirAnswers: thisPage.theirAnswers
                myAnswers: thisPage.myAnswers
            }

            ColumnLayout {
                anchors.fill: parent

                Shape {
                    ShapePath {
                        strokeColor: "white"
                        strokeWidth: 2
                        startX: 76
                        startY: 50
                        PathLine { x: 76; y: 992 }
                    }
                }

                RowLayout {
                    Layout.fillWidth: true
                    Layout.alignment: Qt.AlignTop

                    ColumnLayout {
                        Layout.topMargin: 82
                        Layout.leftMargin: 95

                        spacing: 321

                        Repeater {
                            model: thisPage.model.subquestions

                            ColumnLayout {

                                BilingualText {
                                    id: titleText
                                    textEn: subtitle_en
                                    textFr: subtitle_fr
                                }

                                Label {
                                    Layout.alignment: Qt.AlignLeft
                                    text: titleText.text
                                    font {
                                        family: "Trim SemiBold"
                                        weight: Font.DemiBold
                                        pointSize: 14
                                        capitalization: Font.AllUppercase
                                    }
                                    color: Palette.white
                                }

                            }

                        }

                    }

                }


                RowLayout {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    Layout.preferredHeight: 78
                    Layout.preferredWidth: 1260
                    Layout.leftMargin: 95

                    Shape {
                        ShapePath {
                            strokeColor: "white"
                            strokeWidth: 2
                            PathLine { x: 960; y: 0 }
                        }
                    }


                    ColumnLayout {

                        BilingualText {
                            id: minLeftText
                            textEn: thisPage.model.min_en
                            textFr: thisPage.model.min_fr
                        }

                        Label {
                            Layout.fillWidth: true
                            Layout.alignment: Qt.AlignLeft
                            text: minLeftText.text
                            font {
                                family: "Trim SemiBold"
                                weight: Font.DemiBold
                                pointSize: 14
                                capitalization: Font.AllUppercase
                            }
                            color: Palette.white
                        }
                    }

                    ColumnLayout {

                        BilingualText {
                            id: maxRightText
                            textEn: thisPage.model.max_en
                            textFr: thisPage.model.max_fr
                        }

                        Label {
                            Layout.fillWidth: true
                            Layout.alignment: Qt.AlignRight
                            text: maxRightText.text
                            font {
                                family: "Trim SemiBold"
                                weight: Font.DemiBold
                                pointSize: 14
                                capitalization: Font.AllUppercase
                            }
                            color: Palette.white
                        }
                    }
                }

            }

        }

    }



    // Navigation
    Rectangle {
        Layout.fillHeight: true
        Layout.preferredWidth: 110
        Layout.margins: -1
        Layout.leftMargin: 0

        color: "transparent"
        border.color: Palette.white

        RowLayout {
            width: parent.width
            height: parent.height

            WidgetPreviousNext {
                id: widgetPreviousNext

                Layout.alignment: Qt.AlignBottom | Qt.AlignHCenter
                Layout.bottomMargin: 30

                onPreviousButtonClicked: {
                    if (finalQuestionStackLayout.currentIndex > 0) {
                        finalQuestionStackLayout.currentIndex = finalQuestionStackLayout.currentIndex - 1
                    } else {
                        previousClicked()
                    }
                }
                onNextButtonClicked: {
                    if (finalQuestionStackLayout.currentIndex < finalQuestionStackLayout.count - 1) {
                        // Retrieve data from service to populate the dataviz
                         setDatavizAnswers()
                        finalQuestionStackLayout.currentIndex = finalQuestionStackLayout.currentIndex + 1
                    } else {
                        nextClicked()
                    }
                }
            }
        }
    }
}
