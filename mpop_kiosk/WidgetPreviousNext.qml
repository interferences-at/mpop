import QtQuick 2.0
import QtQuick.Layouts 1.0
import QtQuick.Controls 2.0
import QtQuick.Controls 2.15

ColumnLayout {
    id: thisWidget
    Layout.topMargin : 300

    signal previousButtonClicked()
    signal nextButtonClicked()

    // TODO: Improve the look of these buttons

    RoundButton {
        id: button
        //text: "\u2191"
        icon.source: "uparrow.svg"
        Layout.fillHeight: true
        Layout.fillWidth: true
        Layout.maximumWidth: 80
        Layout.maximumHeight: 80

        onClicked: {
            thisWidget.previousButtonClicked();
        }

    }

    Label {
        id: label1
        text: qsTr("Previous")
        font.capitalization: Font.AllUppercase
        color: "#ffffff"
        font.pixelSize: 12
        anchors.topMargin: 5
        Layout.bottomMargin: 30
        leftPadding : 10
    }


    RoundButton {
        id: downbutton
        icon.source: "downarrow.svg"
        Layout.fillHeight: true
        Layout.fillWidth: true
        Layout.maximumWidth: 80
        Layout.maximumHeight: 80
        onClicked: {
            thisWidget.nextButtonClicked();
        }
    }
    Label {
        text: qsTr("next")
        font.capitalization: Font.AllUppercase
        color: "#ffffff"
        font.pixelSize: 12
        anchors.topMargin: 5
        leftPadding : 22
    }
}
