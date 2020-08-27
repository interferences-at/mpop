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
        text_fr: "Homme"
        text_en: "Male"
    }
    ListElement {
        identifier: "female"
        text_fr: "Femme"
        text_en: "Female"
    }
    ListElement {
        identifier: "other"
        text_fr: "Autre"
        text_en: "Other"
    }
}
