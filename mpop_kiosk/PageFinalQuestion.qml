import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

/**
 * Page that contains final question.
 */
RowLayout {
    id: thisPage

    Layout.fillWidth: true
    Layout.fillHeight: true

    property var model
    property alias showPrevButton: widgetPreviousNext.showPrevButton

    signal finalQuestionChosen(string value)
    signal previousClicked()
    signal nextClicked()

    function handleSliderMoved(index, value) {
        // TODO: connect to JSON-RPC API
        console.log("final question slider moved:", index, value);
    }

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

    // Navigation
    Rectangle {
        Layout.fillHeight: true
        Layout.preferredWidth: 130
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

                onPreviousButtonClicked: previousClicked()
                onNextButtonClicked: nextClicked()
            }
        }
    }
}


