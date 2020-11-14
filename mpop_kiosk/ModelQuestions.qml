import QtQuick 2.0

/**
 * Model for all the questions.
 *
 * Contains the full text of each question in French and English.
 * Also contains the text to display near each end of the slider.
 * All questions are answered with a slider whose int value ranges from 0 to 100.
 * We also store the MySQL `id` field for each row. (those are hard-coded in the SQL migration script, so that it's easier and faster)
 * The questions won't change much, anyways.
 *
 * Some pages contains more than one (1) question, but handling those is not done here. It's done in the layout.s
 */
ListModel {
    id: thisModel

    function _find(model, criteria) {
        for (var i = 0; i < model.count; ++i) {
            if (criteria(model.get(i))) {
                return model.get(i);
            } else {
                if (model.get(i).subquestions) {
                    var subquestions = model.get(i).subquestions;
                    for (var j = 0; j < subquestions.count; ++j) {
                        if (criteria(subquestions.get(j))) {
                            return subquestions.get(j);
                        }
                    }
                }
            }
        }
        return null;
    }

    /**
     * Finds a question given its identifier.
     * @param string question_identifier
     * @return ListElements
     */
    function findQuestion(question_identifier) {
        return _find(thisModel, function(item) {
            return item.identifier === question_identifier;
        });
    }

    /**
     * Here is the list of questions.
     * They are ordered in the same initial way as the questions in the layout.
     */

    // Page 1
    // Single question:
    ListElement {
        identifier: "incidence_drogue"
        question_fr: "D’après vous, quel pourcentage des infractions criminelles est lié à la drogue?"
        question_en: "In your opinion, what percentage of criminal offenses are drug-related?"
        min_fr: "0%"
        max_fr: "100%"
        min_en: "0%"
        max_en: "100%"
        subtitle_fr: ""
        subtitle_en: ""
        db_row_id: 1
    }

    // Page 2
    // Single question:
    ListElement {
        identifier: "decriminalisation_crimes_non_violents"
        question_fr: "Afin de diminuer la population carcérale, devrait-on décriminaliser certains méfaits (p. ex. : crimes non violents, consommation de drogues, infractions liées à la prostitution)?"
        question_en: "In order to reduce the prison population, should certain misdemeanours be decriminalized (e.g. non-violent crimes, drug abuse, offenses related to prostitution)?"
        min_fr: "Jamais"
        max_fr: "Tout à fait"
        min_en: "Never"
        max_en: "Absolutely"
        subtitle_fr: ""
        subtitle_en: ""
        db_row_id: 2
    }

    // Page 3
    // Single question:
    ListElement {
        identifier: "systeme_bureaucrate"
        question_fr: "Croyez-vous que le système judiciaire est engorgé à cause de la bureaucratie?"
        question_en: "Do you think that the justice system is overloaded due to bureaucracy?"
        min_fr: "Pas du tout"
        max_fr: "Tout à fait"
        min_en: "Not at all"
        max_en: "Absolutely"
        subtitle_fr: ""
        subtitle_en: ""
        db_row_id: 3
    }

    // Page 4
    // Group of 5:
    ListElement {
        question_fr: "Trouvez-vous que le système judiciaire est juste envers…?"
        question_en: "Do you think the justice system is fair towards…?"
        min_fr: "Pas du tout"
        max_fr: "Tout à fait"
        min_en: "Not at all"
        max_en: "Absolutely"

        subquestions: [
            ListElement {
                identifier: "equitable_victimes"
                subtitle_fr: "les victimes"
                subtitle_en: "victims"
                db_row_id: 4
            },

            ListElement {
                identifier: "equitable_vulnerables"
                subtitle_fr: "les personnes marginalisées"
                subtitle_en: "marginalized people"
                db_row_id: 5
            },

            ListElement {
                identifier: "equitable_jeunes_contrevenants"
                subtitle_fr: "les jeunes contrevenants"
                subtitle_en: "young offenders"
                db_row_id: 6
            },

            ListElement {
                identifier: "equitable_riches"
                subtitle_fr: "les gens fortunés"
                subtitle_en: "wealthy people"
                db_row_id: 7
            },

            ListElement {
                identifier: "equitable_minorites_culturelles"
                subtitle_fr: "les minorités visibles ou culturelles"
                subtitle_en: "visible or cultural minorities"
                db_row_id: 8
            }
        ]
    }

    // Page 5
    // Single question:
    ListElement {
        identifier: "confiance_systeme"
        question_fr: "Avez-vous confiance dans le système judiciaire?"
        question_en: "Do you have confidence in the justice system?"
        min_fr: "Pas du tout"
        max_fr: "Tout à fait"
        min_en: "Not at all"
        max_en: "Absolutely"
        subtitle_fr: ""
        subtitle_en: ""
        db_row_id: 9
    }

    // Page 6
    // Single question:
    ListElement {
        identifier: "interner"
        question_fr: "Les personnes déclarées non criminellement responsables pour cause de troubles mentaux devraient-elles être :"
        question_en: "Should persons found not criminally responsible on account of mental disorder be:"
        min_fr: "Internées"
        max_fr: "Emprisonnées"
        min_en: "Interned"
        max_en: "Imprisoned"
        subtitle_fr: ""
        subtitle_en: ""
        db_row_id: 10
    }

    // Page 7
    // Single question:
    ListElement {
        identifier: "peine_plus_severes"
        question_fr: "Selon vous, quel est l’impact de peines plus sévères sur le taux de criminalité?"
        question_en: "In your opinion, what impact would more severe sentencing have on the rate of criminal activity?"
        min_fr: "Réduction"
        max_fr: "Augmentation"
        min_en: "Reduction"
        max_en: "Increase"
        subtitle_fr: ""
        subtitle_en: ""
        db_row_id: 11
    }

    // Page 8
    // Single question:
    ListElement {
        identifier: "taux_recidive"
        question_fr: "Au Québec, le taux de récidive est-il :"
        question_en: "In Québec, the recidivism rate is:"
        min_fr: "Faible"
        max_fr: "Élevé"
        min_en: "Low"
        max_en: "High"
        subtitle_fr: ""
        subtitle_en: ""
        db_row_id: 12
    }

    // Page 9
    // Single question:
    ListElement {
        identifier: "bon_traitement"
        question_fr: "Estimez-vous que les détenus québécois sont :"
        question_en: "Do you think that prisoners in Québec are:"
        min_fr: "Maltraités"
        max_fr: "Dorlotés"
        min_en: "Mistreated"
        max_en: "Spoilt"
        subtitle_fr: ""
        subtitle_en: ""
        db_row_id: 13
    }

    // Page 10
    // Single question:
    ListElement {
        identifier: "confinement_solitaire"
        question_fr: "Le confinement solitaire (le trou) a-t-il sa place dans les prisons?"
        question_en: "Does solitary confinement (the hole) have a place in prisons?"
        min_fr: "Pas du tout"
        max_fr: "Tout à fait"
        min_en: "Not at all"
        max_en: "Absolutely"
        subtitle_fr: ""
        subtitle_en: ""
        db_row_id: 14
    }

    // Page 11
    // Group of 2:
    ListElement {
        question_fr: "Croyez-vous que les soins médicaux sont adéquats en prison?"
        question_en: "Do you think that medical care in prison is adequate?"
        min_fr: "Pas du tout"
        max_fr: "Tout à fait"
        min_en: "Not at all"
        max_en: "Absolutely"

        subquestions: [
            ListElement {
                identifier: "soins_physiques"
                subtitle_fr: "Santé physique"
                subtitle_en: "Physical health"
                db_row_id: 15
            },
            ListElement {
                identifier: "soins_mentaux"
                subtitle_fr: "Santé mentale"
                subtitle_en: "Mental health"
                db_row_id: 16
            }
        ]
    }

    // Page 12
    // Single question:
    ListElement {
        identifier: "ressources_reinsertion"
        question_fr: "Croyez-vous que les prisons fournissent à leurs détenus les ressources nécessaires à leur réinsertion sociale?"
        question_en: "Do you think prisons supply the necessary resources to prisoners so that they can reintegrate into society?"
        min_fr: "Pas du tout"
        max_fr: "Tout à fait"
        min_en: "Not at all"
        max_en: "Absolutely"
        subtitle_fr: ""
        subtitle_en: ""
        db_row_id: 17
    }

    // Page 13
    // Single question:
    ListElement {
        identifier: "benefice_justice_reparatrice"
        question_fr: "Quel est le principal bénéfice de la justice réparatrice :"
        question_en: "What is the main benefit of restorative justice:"
        min_fr: "La réhabilitation des criminels"
        max_fr: "La guérison des victimes"
        min_en: "Rehabilitation of criminals"
        max_en: "Healing for victims"
        subtitle_fr: ""
        subtitle_en: ""
        db_row_id: 18
    }

    // Page 14
    // Single question:
    ListElement {
        identifier: "developper_alternatives_prison"
        question_fr: "Trouvez-vous que le gouvernement devrait développer d’autres solutions de rechange à la prison?"
        question_en: "Do you think that the government should develop some alternative solutions to the prison system?"
        min_fr: "Pas du tout"
        max_fr: "Tout à fait"
        min_en: "Not at all"
        max_en: "Absolutely"
        subtitle_fr: ""
        subtitle_en: ""
        db_row_id: 19
    }

    // Page 15
    // Group of 3:
    ListElement {
        question_fr: "Pour réduire le taux de criminalité, à quelle hauteur devrait-on investir dans…?"
        question_en: "To reduce the rate of criminal activity, to what degree should we invest in…?"
        min_fr: "Rien"
        max_fr: "Beaucoup"
        min_en: "Nothing"
        max_en: "A lot"

        subquestions: [
            ListElement {
                identifier: "investir_education"
                subtitle_fr: "L’éducation?"
                subtitle_en: "Education?"
                db_row_id: 20
            },
            ListElement {
                identifier: "investir_sante_services_sociaux"
                subtitle_fr: "La santé et les services sociaux?"
                subtitle_en: "Health and Social Services?"
                db_row_id: 21
            },
            ListElement {
                identifier: "investir_emploi"
                subtitle_fr: "Les programmes liés à l’emploi?"
                subtitle_en: "Job-related programs?"
                db_row_id: 22
            }
        ]
    }
}
