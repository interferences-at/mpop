import QtQuick 2.0

/**
 * Model for the gender selection.
 */
ListModel {
    ListElement {
        identifier: "male"
        name_fr: "Homme"
        name_en: "Male"
    }
    ListElement {
        identifier: "female"
        name_fr: "Femmee"
        name_en: "Female"
    }
    ListElement {
        identifier: "other"
        name_fr: "Autre"
        name_en: "Other"
    }
}
