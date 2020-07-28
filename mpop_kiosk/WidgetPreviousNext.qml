import QtQuick 2.0
import QtQuick.Layouts 1.0
import QtQuick.Controls 2.0

RowLayout {
    Button {
        text: "<-"
        onClicked: {
            thisPage.previousButtonClicked();
        }
    }
    Button {
        text: "->"
        onClicked: {
            thisPage.nextButtonClicked();
        }
    }
}
