import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

/**
 * Slider to select my age.
 */
ColumnLayout {
    // Provides signal onSliderValueChanged

    property string textLeft: ""
    property string textRight: ""
    property bool showNumber: false
    property alias sliderValue: slider0.value
    property real sliderFrom: 0
    property real sliderTo: 120
    property real sliderStepSize: 1
    property real leftAndRightMargin: 100

    Slider {
        id: slider0

        Layout.alignment: Qt.AlignCenter
        Layout.fillWidth: true
        Layout.leftMargin: leftAndRightMargin
        Layout.rightMargin: leftAndRightMargin

        value: 50 // Changed from outside.
        from: sliderFrom
        to: sliderTo
        stepSize: sliderStepSize
        snapMode: Slider.SnapAlways
        // TODO: Customize the size of the background line and handle
        // background.implicitHeight: 100
        // handle.implicitHeight: 100
    }

    RowLayout {
        Layout.fillWidth: true
        Layout.leftMargin: leftAndRightMargin
        Layout.rightMargin: leftAndRightMargin

        Label {
            text: textLeft
            Layout.fillWidth: false
            //Layout.alignment: Qt.Alignleft
            font.pixelSize: 12
        }
        ColumnLayout {
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignCenter

            // FIXME: The value label should be in the center
            Label {
                text: slider0.value
                visible: showNumber
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignCenter
                font.pixelSize: 12
            }
        }

        Label {
            text: textRight
            Layout.fillWidth: false
            Layout.alignment: Qt.AlignRight
            font.pixelSize: 12
        }
    }
}
