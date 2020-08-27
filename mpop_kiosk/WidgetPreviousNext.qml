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
    // property int pageNumber?
    // property int pageCount?

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

    Repeater {
        model: 2

        ColumnLayout {
            enabled: model.index ? showNextButton : showPrevButton
            opacity: model.index ? showNextButton : showPrevButton
            spacing: 5

            RoundButton {
                id: button

                Layout.preferredWidth: 80
                Layout.preferredHeight: 80

                background: Rectangle {
                    radius: 40
                    color: button.down ? Palette.accent : Palette.white
                }

                Image {
                    anchors.centerIn: parent
                    source: "qrc:/arrow.svg"
                }

                transform: Rotation {
                    angle: model.index * 180
                    origin.x: button.width / 2
                    origin.y: button.height / 2
                }

                onClicked: thisWidget[model.index ? "nextButtonClicked" : "previousButtonClicked"]();
            }

            Label {
                text: (model.index ? textNext : textPrevious).text
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
}
