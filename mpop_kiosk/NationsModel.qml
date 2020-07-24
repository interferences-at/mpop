import QtQuick 2.0

ListModel {
    id: nationsModel

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
    function findQuestion(nation_identifier) {
        return _find(nationsModel, function(item) {
            return item.identifier === nation_identifier;
        });
    }

    /**
     * We keep them in the same order as in the SQL migration script.
     */
    ListElement {
        identifier: "quebecer"
        text_fr: "Québécoise"
        text_en: "Quebecer"
        db_row_id: 1
    }
    ListElement {
        identifier: "canadian"
        text_fr: "canadian"
        text_en: "canadian"
        db_row_id: 2
    }
    ListElement {
        identifier: "american"
        text_fr: "Américaine"
        text_en: "American"
        db_row_id: 3
    }
    ListElement {
        identifier: "european"
        text_fr: "Européenne"
        text_en: "European"
        db_row_id: 4
    }
    ListElement {
        identifier: "native"
        text_fr: "Autochtone"
        text_en: "Native"
        db_row_id: 5
    }
    ListElement {
        identifier: "other"
        text_fr: "Autre"
        text_en: "Other"
        db_row_id: 6
    }
}

