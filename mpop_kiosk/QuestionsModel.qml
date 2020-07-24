import QtQuick 2.0

ListModel {
    id: questionsModel

    function _find(model, criteria) {
        for (var i = 0; i < model.count; ++i) {
            if (criteria(model.get(i))) {
                return model.get(i);
            }
        }
        return null;
    }
    function findQuestion(question_identifier) {
        return _find(questionsModel, function(item) {
            return item.identifier === question_identifier;
        });
    }

    ListElement {
        identifier: "incidence_drogue"
        question_fr: "D’après vous, quelle est l’incidence du marché de la drogue sur les infractions criminelles?"
        question_en: "TODO"
        min_fr: "Forte"
        max_fr: "Faible"
        min_en: "TODO"
        max_en: "TODO"
        db_row_id: 1
    }
    ListElement {
        identifier: "decriminalisation_crimes_non_violents"
        question_fr: "Êtes-vous favorable à une plus grande décriminalisation de certains méfaits comme les crimes non violents, la consommation de drogues et les infractions liées à la prostitution afin de diminuer la population carcérale?"
        question_en: "TODO"
        min_fr: "TODO"
        max_fr: "TODO"
        min_en: "TODO"
        max_en: "TODO"
        db_row_id: 2
    }
    ListElement {
        identifier: "systeme_bureaucrate"
        question_fr: "Croyez-vous que le système judiciaire est engorgé par la bureaucratie?"
        question_en: "TODO"
        min_fr: "TODO"
        max_fr: "TODO"
        min_en: "TODO"
        max_en: "TODO"
        db_row_id: 3
    }
    ListElement {
        identifier: "equitable_victimes"
        question_fr: "Trouvez-vous que le système judiciaire et carcéral sont équitables envers les victimes?"
        question_en: "TODO"
        min_fr: "Inéquitable"
        max_fr: "Équitable"
        min_en: "Unfair"
        max_en: "Fair"
        db_row_id: 4
    }
    ListElement {
        identifier: "equitable_vulnerables"
        question_fr: "Trouvez-vous que le système judiciaire et carcéral sont équitables envers les personnes vulnérables ou marginalisées?"
        question_en: "TODO"
        min_fr: "Inéquitable"
        max_fr: "Équitable"
        min_en: "Unfair"
        max_en: "Fair"
        db_row_id: 5
    }
    ListElement {
        identifier: "equitable_jeunes_contrevenants"
        question_fr: "Trouvez-vous que le système judiciaire et carcéral sont équitables envers les jeunes contrevenants?"
        question_en: "TODO"
        min_fr: "Inéquitable"
        max_fr: "Équitable"
        min_en: "Unfair"
        max_en: "Fair"
        db_row_id: 6
    }
    ListElement {
        identifier: "equitable_riches"
        question_fr: "Trouvez-vous que le système judiciaire et carcéral sont équitables envers les gens fortunés?"
        question_en: "TODO"
        min_fr: "Inéquitable"
        max_fr: "Équitable"
        min_en: "Unfair"
        max_en: "Fair"
        db_row_id: 7
    }
    ListElement {
        identifier: "equitable_minorites_culturelles"
        question_fr: "Trouvez-vous que le système judiciaire et carcéral sont équitables envers les minorités culturelles?"
        question_en: "TODO"
        min_fr: "Inéquitable"
        max_fr: "Équitable"
        min_en: "Unfair"
        max_en: "Fair"
        db_row_id: 8
    }
    ListElement {
        identifier: "confiance_systeme"
        question_fr: "Avez-vous confiance dans le système judiciaire?"
        question_en: "TODO"
        min_fr: "Aucune confiance"
        max_fr: "Confiance"
        min_en: "TODO"
        max_en: "TODO"
        db_row_id: 9
    }
    ListElement {
        identifier: "interner"
        question_fr: "Croyez-vous qu’il est juste d’interner plutôt que d’emprisonner des individus déclarés non criminellement responsables pour cause de troubles mentaux?"
        question_en: "TODO"
        min_fr: "Injuste"
        max_fr: "Juste"
        min_en: "TODO"
        max_en: "TODO"
        db_row_id: 10
    }
    ListElement {
        identifier: "peine_plus_severes"
        question_fr: "Croyez-vous que des peines plus sévères réduiraient le taux de criminalité?"
        question_en: "TODO"
        min_fr: "Non"
        max_fr: "Oui"
        min_en: "TODO"
        max_en: "TODO"
        db_row_id: 11
    }
    ListElement {
        identifier: "taux_recidive"
        question_fr: "Croyez-vous que le taux de récidive est élevé?"
        question_en: "TODO"
        min_fr: "Bas"
        max_fr: "Élevé"
        min_en: "TODO"
        max_en: "TODO"
        db_row_id: 12
    }
    ListElement {
        identifier: "bon_traitement"
        question_fr: "Estimez-vous qu’aujourd’hui les détenus sont bien traités?"
        question_en: "TODO"
        min_fr: "Maltraités"
        max_fr: "Bien traités"
        min_en: "TODO"
        max_en: "TODO"
        db_row_id: 13
    }
    ListElement {
        identifier: "confinement_solitaire"
        question_fr: "Pensez-vous que le confinement solitaire à sa place dans les prisons québécoises?"
        question_en: "TODO"
        min_fr: "Non"
        max_fr: "Oui"
        min_en: "TODO"
        max_en: "TODO"
        db_row_id: 14
    }
    ListElement {
        identifier: "soins_physiques"
        question_fr: "Croyez-vous que les soins médicaux sont adéquats en prison? (santé physique)"
        question_en: "TODO"
        min_fr: "TODO"
        max_fr: "TODO"
        min_en: "TODO"
        max_en: "TODO"
        db_row_id: 15
    }
    ListElement {
        identifier: "soins_mentaux"
        question_fr: "Croyez-vous que les soins médicaux sont adéquats en prison? (santé mentale)"
        question_en: "TODO"
        min_fr: "TODO"
        max_fr: "TODO"
        min_en: "TODO"
        max_en: "TODO"
        db_row_id: 16
    }
    ListElement {
        identifier: "ressources_reinsertion"
        question_fr: "Croyez-vous que les détenus ont les ressources adéquates pour entamer leur réinsertion sociale?"
        question_en: "TODO"
        min_fr: "TODO"
        max_fr: "TODO"
        min_en: "TODO"
        max_en: "TODO"
        db_row_id: 17
    }
    ListElement {
        identifier: "benefice_justice_reparatrice"
        question_fr: "Quel est le principal bénéfice de la justice réparatrice ?"
        question_en: "TODO"
        min_fr: "la réhabilitation des criminels"
        max_fr: "la guérison des victimes"
        min_en: "TODO"
        max_en: "TODO"
        db_row_id: 18
    }
    ListElement {
        identifier: "developper_alternatives_prison"
        question_fr: "Trouvez-vous que la société gagnerait à développer plus d’alternatives à la prison?"
        question_en: "TODO"
        min_fr: "TODO"
        max_fr: "TODO"
        min_en: "TODO"
        max_en: "TODO"
        db_row_id: 19
    }
    ListElement {
        identifier: "investir_education"
        question_fr: "Croyez-vous que de meilleurs investissements dans les institutions suivantes sont importantes dans la réduction du taux de criminalité? - L’éducation"
        question_en: "TODO"
        min_fr: "TODO"
        max_fr: "L’éducation"
        min_en: "TODO"
        max_en: "TODO"
        db_row_id: 20
    }
    ListElement {
        identifier: "investir_sante_services_sociaux"
        question_fr: "Croyez-vous que de meilleurs investissements dans les institutions suivantes sont importantes dans la réduction du taux de criminalité? - La santé et de services sociaux"
        question_en: "TODO"
        min_fr: "TODO"
        max_fr: "La santé et de services sociaux"
        min_en: "TODO"
        max_en: "TODO"
        db_row_id: 21
    }
    ListElement {
        identifier: "investir_emploi"
        question_fr: "Croyez-vous que de meilleurs investissements dans les institutions suivantes sont importantes dans la réduction du taux de criminalité? - Différents programmes liés à l’emploi"
        question_en: "TODO"
        min_fr: "TODO"
        max_fr: "Différents programmes liés à l’emploi"
        min_en: "TODO"
        max_en: "TODO"
        db_row_id: 22
    }
    ListElement {
        identifier: "importance_assurance_protection_population"
        question_fr: "Selon vous, quelle est l’importance des objectifs suivants du système carcéral - Assurer la sécurité et la protection de la population"
        question_en: "TODO"
        min_fr: "TODO"
        max_fr: "Assurer la sécurité et la protection de la population"
        min_en: "TODO"
        max_en: "TODO"
        db_row_id: 23
    }
    ListElement {
        identifier: "importance_repondre_actes"
        question_fr: "Selon vous, quelle est l’importance des objectifs suivants du système carcéral - Veiller à ce que les délinquants répondent de leurs actes"
        question_en: "TODO"
        min_fr: "TODO"
        max_fr: "Veiller à ce que les délinquants répondent de leurs actes"
        min_en: "TODO"
        max_en: "TODO"
        db_row_id: 24
    }
    ListElement {
        identifier: "importance_soutien_rehabilitation"
        question_fr: "Selon vous, quelle est l’importance des objectifs suivants du système carcéral - Offrir le soutien nécessaire pour assurer la réhabilitation réussie des délinquants"
        question_en: "TODO"
        min_fr: "TODO"
        max_fr: "Offrir le soutien nécessaire pour assurer la réhabilitation réussie des délinquants"
        min_en: "TODO"
        max_en: "TODO"
        db_row_id: 25
    }
}

