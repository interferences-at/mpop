import QtQuick 2.0
import QtQuick.Layouts 1.0
import QtQuick.Controls 2.4

ColumnLayout {
    id: thisWidget

    Layout.alignment: Qt.AlignBottom | Qt.AlignRight
    Layout.bottomMargin: 35
    Layout.rightMargin: 25
    spacing: 25

    property bool showPrevButton: true
    property bool showNextButton: true

    signal previousButtonClicked()
    signal nextButtonClicked()

    BilingualText {
        id : textPrevious
        textEn: "Previous"
        textFr: "Précédent"
    }

    BilingualText {
        id : textNext
        textEn: "Next"
        textFr: "Suivante"
    }

    ColumnLayout {
        visible: showPrevButton
        spacing: 5

        RoundButton {
            id: button
            Layout.preferredWidth: 80
            Layout.preferredHeight: 80

            Image {
                anchors.centerIn: parent
                source: "qrc:/arrow.svg"
            }

            background: Rectangle {
                radius: 40
                color: "#fff"
            }

            onClicked: {
                thisWidget.previousButtonClicked();
            }
        }

        Label {
            id: label1
            text: textPrevious.text
            color: "#ffffff"
            font {
                pixelSize: 11
                letterSpacing: 11 * 25 / 1000
                capitalization: Font.AllUppercase
            }
            Layout.alignment: Qt.AlignHCenter
        }
    }

    ColumnLayout {
        visible: showNextButton
        spacing: 5

        RoundButton {
            id: downbutton
            Layout.preferredWidth: 80
            Layout.preferredHeight: 80

            background: Rectangle {
                radius: 40
                color: "#fff"
            }

            Image {
                anchors.centerIn: parent
                source: "qrc:/arrow.svg"
            }

            transform: Rotation {
                angle: 180
                origin.x: downbutton.width / 2
                origin.y: downbutton.height / 2
            }

            onClicked: {
                thisWidget.nextButtonClicked();
            }
        }

        Label {
            text: textNext.text
            color: "#ffffff"
            font {
                pixelSize: 11
                letterSpacing: 11 * 25 / 1000
                capitalization: Font.AllUppercase
            }
            Layout.alignment: Qt.AlignHCenter
        }
    }
}
