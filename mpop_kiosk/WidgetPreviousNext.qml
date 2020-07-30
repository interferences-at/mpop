import QtQuick 2.0
import QtQuick.Layouts 1.0
import QtQuick.Controls 2.0

RowLayout {
    id: thisWidget

    signal previousButtonClicked()
    signal nextButtonClicked()

    // TODO: Improve the look of these buttons
    Button {
        text: "<-"
        onClicked: {
            thisWidget.previousButtonClicked();
        }
    }

    Button {
        text: "->"
        onClicked: {
            thisWidget.nextButtonClicked();
        }
    }
}
