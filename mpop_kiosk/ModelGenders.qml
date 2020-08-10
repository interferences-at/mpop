import QtQuick 2.0

/**
 * Model for the gender selection.
 */
ListModel {
    id: thisModel

    readonly property int index_MALE: 0
    readonly property int index_FEMALE: 1
    readonly property int index_OTHER: 2

    function _find(model, criteria) {
        for (var i = 0; i < model.count; ++i) {
            if (criteria(model.get(i))) {
                return model.get(i);
            }
        }
        return null;
    }

    /**
     * Returns a ListElement given its identifier field.
     */
    function findQuestion(gender_identifier) {
        return _find(thisModel, function(item) {
            return item.identifier === gender_identifier;
        });
    }

    ListElement {
        identifier: "male"
        name_fr: "Homme"
        name_en: "Male"
    }
    ListElement {
        identifier: "female"
        name_fr: "Femme"
        name_en: "Female"
    }
    ListElement {
        identifier: "other"
        name_fr: "Autre"
        name_en: "Other"
    }
    ListElement {
        identifier: ""
        name_fr: ""
        name_en: ""
    }
    ListElement {
        identifier: ""
        name_fr: ""
        name_en: ""
    }
    ListElement {
        identifier: ""
        name_fr: ""
        name_en: ""
    }
    ListElement {
        identifier: ""
        name_fr: ""
        name_en: ""
    }
    ListElement {
        identifier: ""
        name_fr: ""
        name_en: ""
    }
    ListElement {
        identifier: ""
        name_fr: ""
        name_en: ""
    }
}
