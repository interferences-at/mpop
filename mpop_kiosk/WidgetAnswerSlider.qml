import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

/**
 * Slider to answer a question.
 */
ColumnLayout {
    // Provides signal onSliderValueChanged

    property string textLeft: ""
    property string textRight: ""
    property bool showNumber: false
    property real sliderValue: 50
    property real sliderFrom: 0
    property real sliderTo: 100
    property real sliderStepSize: 1
    property bool fullWidth: false

    Layout.maximumWidth: fullWidth ? Number.POSITIVE_INFINITY : 920
    spacing: 0

    Label {
        id: numberLabel
        text: slider0.value
        visible: showNumber
        font.pixelSize: 20

        transform: Translate {
            x: (slider0.visualPosition * (slider0.availableWidth - slider0.implicitHandleWidth)) + (slider0.implicitHandleWidth / 2) - (numberLabel.paintedWidth / 2)
        }
    }

    Slider {
        id: slider0
        Layout.fillWidth: true
        value: sliderValue
        from: sliderFrom
        to: sliderTo
        stepSize: sliderStepSize
        snapMode: Slider.SnapAlways
        padding: 0

//        background: Rectangle {
//            implicitHeight: 10
//            width: slider0.availableWidth
//            height: implicitHeight
//            color: Palette.lightGrey

//            Rectangle {
//                width: slider0.visualPosition * parent.width
//                height: parent.height
//                color: Palette.lightBlack
//            }
//        }

        // TODO: handle
    }

    RowLayout {
        Layout.fillWidth: true

        Label {
            text: textLeft
            font {
                pixelSize: 11
                capitalization: Font.AllUppercase
                letterSpacing: 11 * 25 / 1000
            }
        }

        Item { Layout.fillWidth: true }

        Label {
            text: textRight
            Layout.alignment: Qt.AlignRight
            font {
                pixelSize: 11
                capitalization: Font.AllUppercase
                letterSpacing: 11 * 25 / 1000
            }
        }
    }
}
