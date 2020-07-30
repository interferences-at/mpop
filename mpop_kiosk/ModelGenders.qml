import QtQuick 2.0

/**
 * Model for the gender selection.
 */
ListModel {
    id: thisModel

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
        name_fr: "Femmee"
        name_en: "Female"
    }
    ListElement {
        identifier: "other"
        name_fr: "Autre"
        name_en: "Other"
    }
}
