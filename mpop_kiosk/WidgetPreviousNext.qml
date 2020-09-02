import QtQuick 2.0
import QtQuick.Layouts 1.0
import QtQuick.Controls 2.4
import QtGraphicalEffects 1.0

ColumnLayout {
    id: thisWidget

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

            Item {
                Layout.preferredWidth: 80
                Layout.preferredHeight: 80

                RoundButton {
                    id: button
                    implicitWidth: 80
                    implicitHeight: 80

                    background: Rectangle {
                        radius: 40
                        color: button.down ? Palette.accent : Palette.white
                    }

                    Image {
                        id: arrowIcon
                        anchors.centerIn: parent
                        source: "qrc:/arrow.svg"

                        transform: Rotation {
                            angle: model.index * 180
                            origin.x: arrowIcon.width / 2
                            origin.y: arrowIcon.height / 2
                        }
                    }

                    onClicked: thisWidget[model.index ? "nextButtonClicked" : "previousButtonClicked"]();
                }

                DropShadow {
                    anchors.fill: button
                    horizontalOffset: 1
                    verticalOffset: 3
                    radius: 5
                    samples: 11
                    color: "#66000000"
                    source: button
                }
            }

            Label {
                text: (model.index ? textNext : textPrevious).text
                color: invertedTheme ? Palette.lightBlack : Palette.white
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
