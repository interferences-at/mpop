import QtQuick 2.0

ListModel {
    // Final questions
    // Group of 3:
    ListElement {
        question_fr: "Selon vous, à quoi sert la prison?"
        question_en: "In your opinion, what is the purpose of prison?"
        min_fr: "Aucunement"
        max_fr: "Principalement"
        min_en: "Not at all"
        max_en: "Primarily"

        subquestions: [
            ListElement {
                identifier: "importance_assurance_protection_population"
                subtitle_fr: "À assurer la sécurité de la population"
                subtitle_en: "To ensure the population’s safety"
                db_row_id: 23
            },
            ListElement {
                identifier: "importance_repondre_actes"
                subtitle_fr: "À punir les criminels"
                subtitle_en: "To punish criminals"
                db_row_id: 24
            },
            ListElement {
                identifier: "importance_soutien_rehabilitation"
                subtitle_fr: "À soutenir la réhabilitation des délinquants"
                subtitle_en: "To assist the rehabilitation of offenders"
                db_row_id: 25
            }

        ]
    }
}
