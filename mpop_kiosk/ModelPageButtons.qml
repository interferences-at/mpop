import QtQuick 2.0

/**
 * Model for the 15 buttons on the right of the screen, to choose the current question page.
 */
ListModel {

    id: thisModel

    function highlightButton(pageNumberStr) {
        function criteria(item) {
            return item.pageNumber === pageNumberStr;
        }

        for (var i = 0; i < thisModel.count; ++i) {
            if (criteria(thisModel.get(i))) {
                thisModel.get(i).isHighlighted = true;
            } else {
                thisModel.get(i).isHighlighted = false;
            }
        }
        return null;
    }

    ListElement {
        pageNumber: "01"
        isHighlighted: false
    }
    ListElement {
        pageNumber: "02"
        isHighlighted: false
    }
    ListElement {
        pageNumber: "03"
        isHighlighted: false
    }
    ListElement {
        pageNumber: "04"
        isHighlighted: false
    }
    ListElement {
        pageNumber: "05"
        isHighlighted: false
    }
    ListElement {
        pageNumber: "06"
        isHighlighted: false
    }
    ListElement {
        pageNumber: "07"
        isHighlighted: false
    }
    ListElement {
        pageNumber: "08"
        isHighlighted: false
    }
    ListElement {
        pageNumber: "09"
        isHighlighted: false
    }
    ListElement {
        pageNumber: "10"
        isHighlighted: false
    }
    ListElement {
        pageNumber: "11"
        isHighlighted: false
    }
    ListElement {
        pageNumber: "12"
        isHighlighted: false
    }
    ListElement {
        pageNumber: "13"
        isHighlighted: false
    }
    ListElement {
        pageNumber: "14"
        isHighlighted: false
    }
    ListElement {
        pageNumber: "15"
        isHighlighted: false
    }
}
