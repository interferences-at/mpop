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
        textFr: "Précédente"
    }

    BilingualText {
        id : textNext
        textEn: "Next"
        textFr: "Suivante"
    }

    Repeater {
        model: 2

        WidgetIconButton {
            showButton: model.index ? showNextButton : showPrevButton
            iconRotation: model.index * 180
            labelText: (model.index ? textNext : textPrevious).text
            Layout.alignment: Qt.AlignHCenter

            onClicked: thisWidget[model.index ? "nextButtonClicked" : "previousButtonClicked"]()
        }
    }
}
