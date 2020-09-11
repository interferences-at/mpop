import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtGraphicalEffects 1.0

/**
 * Slider to select an age range.
 */
ColumnLayout {
    // Provides signals onSliderFromChanged and onSliderToChanged

    property string textLeft: ""
    property string textRight: ""
    property bool showNumbers: false
    property real valueFrom: 0 // Minimum possible value
    property real valueTo: 120 // Maximum possible value
    property real sliderFrom: 0 // Position of the slider
    property real sliderTo: 120 // Position of the slider
    property real sliderStepSize: 1
    property bool fullWidth: false
    property color colorPressed: window.invertedTheme ? Palette.white : Palette.lightBlack
    property color colorNotPressed: window.invertedTheme ? Palette.white : Palette.lightBlack

    Layout.maximumWidth: fullWidth ? Number.POSITIVE_INFINITY : 920
    spacing: 0

    Item {
        Layout.fillWidth: true
        height: childrenRect.height
        Layout.bottomMargin: 5
        visible: showNumbers

        Label {
            id: numberLabel0
            text: slider0.first.value
            font.pixelSize: 20
            color: Palette.white

            transform: Translate {
                x: (slider0.first.visualPosition * (slider0.availableWidth - slider0.first.implicitHandleWidth)) + (slider0.first.implicitHandleWidth / 2) - (numberLabel0.paintedWidth / 2)
            }
        }

        Label {
            id: numberLabel1
            text: slider0.second.value
            font.pixelSize: 20
            color: Palette.white

            transform: Translate {
                x: (slider0.second.visualPosition * (slider0.availableWidth - slider0.second.implicitHandleWidth)) + (slider0.second.implicitHandleWidth / 2) - (numberLabel1.paintedWidth / 2)
            }
        }
    }

    RangeSlider {
        id: slider0
        Layout.fillWidth: true
        first.value: valueFrom
        second.value: valueTo
        from: sliderFrom
        to: sliderTo
        stepSize: sliderStepSize
        snapMode: Slider.SnapAlways
        padding: 0

        background: Rectangle {
            y: slider0.availableHeight / 2 - height / 2
            implicitWidth: 200
            implicitHeight: 4
            width: slider0.availableWidth
            height: implicitHeight
            radius: 2
            color: window.invertedTheme ? Palette.lightGrey : Palette.mediumGrey

            Rectangle {
                x: slider0.first.visualPosition * parent.width
                width: slider0.second.visualPosition * parent.width - x
                height: parent.height
                color: window.invertedTheme ? Palette.lightBlack : Palette.white
                radius: 2
            }
        }

        first.handle: Rectangle {
            x: slider0.first.visualPosition * (slider0.availableWidth - width)
            y: slider0.availableHeight / 2 - height / 2
            implicitWidth: 26
            implicitHeight: 26
            radius: 13
            color: slider0.first.pressed ? colorPressed : colorNotPressed
            border.color: window.invertedTheme ? Palette.lightGrey : Palette.mediumGrey
        }

        second.handle: Rectangle {
            x: slider0.second.visualPosition * (slider0.availableWidth - width)
            y: slider0.availableHeight / 2 - height / 2
            implicitWidth: 26
            implicitHeight: 26
            radius: 13
            color: slider0.second.pressed ? colorPressed : colorNotPressed
            border.color: window.invertedTheme ? Palette.lightGrey : Palette.mediumGrey
        }

        first.onMoved: resetIdleTimer()
        second.onMoved: resetIdleTimer()
    }

    RowLayout {
        Layout.fillWidth: true
        Layout.topMargin: 10

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
