import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtGraphicalEffects 1.0

/**
 * Slider to answer a question.
 */
ColumnLayout {
    // Provides signal onSliderValueChanged

    id: thisWidget

    signal sliderMoved(int intValue)

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
        Layout.bottomMargin: 10

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

        onMoved: {
            var intValue = Math.round(value);
            thisWidget.sliderMoved(intValue);
            resetIdleTimer();
        }

        background: Rectangle {
            width: slider0.availableWidth
            height: 10
            color: window.invertedTheme ? Palette.lightGrey : Palette.mediumGrey

            Rectangle {
                width: slider0.visualPosition * (slider0.availableWidth - handle.implicitWidth) + (handle.implicitWidth / 2)
                height: parent.height
                color: window.invertedTheme ? Palette.lightBlack : Palette.white
            }
        }

        handle: Rectangle {
            id: handle
            implicitWidth: 26
            implicitHeight: 26
            radius: 13
            color: window.invertedTheme ? Palette.lightBlack : Palette.white
            x: slider0.visualPosition * (slider0.availableWidth - implicitWidth)
            y: 5 - (implicitHeight / 2)
        }

        DropShadow {
            anchors.fill: handle
            horizontalOffset: 1
            verticalOffset: 2
            radius: 3
            samples: 7
            color: "#66000000"
            source: handle
        }
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
            color: invertedTheme ? Palette.lightBlack : Palette.white
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
            color: invertedTheme ? Palette.lightBlack : Palette.white
        }
    }
}
