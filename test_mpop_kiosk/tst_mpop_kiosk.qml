import QtQuick 2.3
import QtTest 1.0
import "../mpop_kiosk" as MpopKiosk

TestCase {

    property var rfidTagsToDelete: []

    name: "JsonRpcTests"

    function initTestCase() {
        // Parse the command-line options for this test suite
        config.parseCommandLineOptions();
        var max_wait = 3000; // ms
        var total_wait = 0; // ms
        var done = false;
        while (done === false) {
            wait(5); // ms
            total_wait += 5;
            if (userProfile.isConnected) {
                done = true;
            }
            else if (total_wait >= max_wait) {
                console.warn("Could not connect to the mpop service after " + total_wait + " ms");
                done = true;
            }
        }
    }

    function cleanupTestCase() {
        // TODO: erase the test users
    }

    function test_math() {
        compare(2 + 2, 4, "2 + 2 = 4")
    }

    /**
     * Sends ping to the service.
     */
    function test_ping() {
        if (userProfile.isConnected === false) {
            skip("This test requires a connection to the MPOP Service via JSON-RPC.");
            return;
        }

        // if (! userProfile.isConnected) skip();
        var done = false;
        userProfile.ping(function (err, result) {
            if (err) {
                console.log("Error calling ping(): " + err.message);
                fail("Error calling ping: " + err.message);
                done = true;
            } else {
                console.log("result is " + result);
                compare(result, "pong", "result = pong");
            }
            done = true;
        });
        while (done === false) {
            wait(5); // ms
        }
    }

    function test_multiple_answers() {
        if (userProfile.isConnected === false) {
            skip("This test requires a connection to the MPOP Service via JSON-RPC.");
            return;
        }

        const rfidTagOne = "CAFEBABE0001";
        const rfidTagTwo = "CAFEBABE0002";

        rfidTagsToDelete.push(rfidTagOne);
        rfidTagsToDelete.push(rfidTagTwo);

        let userOneOk = false;

        userProfile.setRfidTag(rfidTagOne, function (err, result) {
            if (err) {
                console.log("Error calling setRfidTag: " + err.message);
                fail("Error calling setRfidTag: " + err.message);
            } else {
                console.log("Success calling setRfidTag: " + result);
                userOneOk = true;
            }
        });

        while (userOneOk === false) {
            wait(5); // ms
        }

        /**
         * Creates a structure to store the tasks to set its answers and properties.
         */
        function createUserTasks() {
            var ret = {
                setGender: false,
                setAge: false,
                setLanguage: false,
                setEthnicity: false,
                answerQuestions: {
                    incidence_drogue: false,
                    equitable_victimes: false,
                    equitable_vulnerables: false,
                    equitable_jeunes_contrevenants: false,
                    equitable_riches: false,
                    equitable_minorites_culturelles: false
                }
            };
            return ret;
        }

        /**
         * Checks that a given user methods has all been called.
         */
        function userIsAllSet(userTasks) {
            let ret = true;
            if (userTasks.setGender === false) {
                ret = false;
                // console.log("Did not set the user's gender, yet");
            }
            if (userTasks.setAge === false) {
                ret = false;
                // console.log("Did not set the user's age, yet");
            }
            /*
            if (userTasks.answerQuestions.incidence_drogue === false) {
                ret = false;
                // console.log("Did not answer question incidence_drogue, yet");
            }
            if (userTasks.answerQuestions.equitable_victimes === false) {
                ret = false;
                // console.log("Did not answer question equitable_victimes, yet");
            }
            if (userTasks.answerQuestions.equitable_vulnerables === false) {
                ret = false;
                // console.log("Did not answer question equitable_vulnerables, yet");
            }
            if (userTasks.answerQuestions.equitable_jeunes_contrevenants === false) {
                ret = false;
                // console.log("Did not answer question equitable_jeunes_contrevenants, yet");
            }
            if (userTasks.answerQuestions.equitable_riches === false) {
                ret = false;
                // console.log("Did not answer question equitable_riches, yet");
            }
            if (userTasks.answerQuestions.equitable_minorites_culturelles === false) {
                ret = false;
                // console.log("Did not answer question equitable_minorites_culturelles, yet");
            }
            */

            if (ret) {
                console.log("We are all set.");
            }
            return ret;
        }

        // Tasks for our first user
        let userOneTasks = createUserTasks();

        // Properties of our first user
        const userOneGender = "other";
        const userOneAge = 21;
        const userOneEthnicity = "quebecer";
        const userOneLanguage = "fr";
        const userOneAnswers = {
            incidence_drogue: 99,
            equitable_victimes: 20,
            equitable_vulnerables: 20,
            equitable_jeunes_contrevenants: 20,
            equitable_riches: 20,
            equitable_minorites_culturelles: 20
        };

        userProfile.setUserGender(userProfile.userId, userOneGender, function (err) {
            if (err) {
                fail("Error calling setUserGender: " + err.message);
            } else {
                userOneTasks.setGender = true;
            }
        });

        userProfile.setUserAge(userProfile.userId, userOneAge, function (err) {
            if (err) {
                fail("Error calling setUserGender: " + err.message);
            } else {
                userOneTasks.setAge = true;
            }
        });

        userProfile.setUserAnswer(userProfile.userId, "incidence_drogue", userOneAnswers.incidence_drogue, function (err) {
            if (err) {
                fail("Error calling setUserAnswer: " + err.message);
            } else {
                userOneTasks.answerQuestions.incidence_drogue = true;
            }
        });

        userProfile.setUserAnswer(userProfile.userId, "equitable_victimes", userOneAnswers.equitable_victimes, function (err) {});
        userProfile.setUserAnswer(userProfile.userId, "equitable_vulnerables", userOneAnswers.equitable_vulnerables, function (err) {});
        userProfile.setUserAnswer(userProfile.userId, "equitable_jeunes_contrevenants", userOneAnswers.equitable_jeunes_contrevenants, function (err) {});
        userProfile.setUserAnswer(userProfile.userId, "equitable_riches", userOneAnswers.equitable_riches, function (err) {});
        userProfile.setUserAnswer(userProfile.userId, "equitable_minorites_culturelles", userOneAnswers.equitable_minorites_culturelles, function (err) {});

        let totalTimeWait = 0;
        while (userIsAllSet(userOneTasks) === false) {
            wait(5); // ms
            totalTimeWait += 5;
            if (totalTimeWait >= 10000) {
                console.log(JSON.stringify(userOneTasks));
                fail("It took too long - setting up user one");
            }
        }

        compare(userProfile.age, userOneAge, "age has been set");
        compare(userProfile.gender, userOneGender, "gender has been set");
        compare(userProfile.answers.hasOwnProperty("incidence_drogue") ? userProfile.answers.incidence_drogue : "", userOneAnswers.incidence_drogue, "Answer for incidence_drogue has been set.");
        compare(userProfile.answers.hasOwnProperty("equitable_victimes") ? userProfile.answers.equitable_victimes : "", userOneAnswers.equitable_victimes, "Answer for equitable_victimes has been set.");

        let userTwoOk = false;

        userProfile.setRfidTag(rfidTagTwo, function (err, result) {
            if (err) {
                console.log("Error calling setRfidTag: " + err.message);
                fail("Error calling setRfidTag: " + err.message);
            } else {
                console.log("Success calling setRfidTag: " + result);
                userTwoOk = true;
            }
        });

        while (userTwoOk === false) {
            wait(5); // ms
        }

        let userTwoTasks = createUserTasks();

        // Properties of our first user
        const userTwoGender = "female";
        const userTwoAge = 50;
        const userTwoEthnicity = "canadian";
        const userTwoLanguage = "en";
        const userTwoAnswers = {
            incidence_drogue: 28,
            equitable_victimes: 40,
            equitable_vulnerables: 40,
            equitable_jeunes_contrevenants: 40,
            equitable_riches: 40,
            equitable_minorites_culturelles: 40
        };

        userProfile.setUserGender(userProfile.userId, userTwoGender, function (err) {});
        userProfile.setUserAge(userProfile.userId, userTwoAge, function (err) {});
        userProfile.setUserAnswer(userProfile.userId, "incidence_drogue", userTwoAnswers.incidence_drogue, function (err) {});
        userProfile.setUserAnswer(userProfile.userId, "equitable_victimes", userTwoAnswers.equitable_victimes, function (err) {});
        userProfile.setUserAnswer(userProfile.userId, "equitable_vulnerables", userTwoAnswers.equitable_vulnerables, function (err) {});
        userProfile.setUserAnswer(userProfile.userId, "equitable_jeunes_contrevenants", userTwoAnswers.equitable_jeunes_contrevenants, function (err) {});
        userProfile.setUserAnswer(userProfile.userId, "equitable_riches", userTwoAnswers.equitable_riches, function (err) {});
        userProfile.setUserAnswer(userProfile.userId, "equitable_minorites_culturelles", userTwoAnswers.equitable_minorites_culturelles, function (err) {});

        let totalTimeWaitTwo = 0;
        while (totalTimeWaitTwo <= 1500) { // FIXME
            wait(5); // ms
            totalTimeWaitTwo += 5;
        }

        // Get answers by *:
        userProfile.getAnswerByAge("incidence_drogue", "all", "all", "all", function (err, answerByAge) {
            if (err) {
                console.log("Error calling getAnswerByAge(" + questionId + "," + ethnicity + "," + gender + "," + timeAnswered + "): " + err.message);
                fail("Error calling getAnswerByAge");
            } else {
                let expectedAnswersByAge = [
                        0, // [0,4]
                        0,
                        0,
                        0,
                        99,

                        0, // [25,29]
                        0,
                        0,
                        0,
                        0,

                        28, // [50,54]
                        0,
                        0,
                        0,
                        0,

                        0, // [75,79]
                        0,
                        0,
                        0,
                        0,
                    ];
                // FIXME: The following check fails:
                compare(answerByAge, expectedAnswersByAge, "expected answersByAge match");
            }
        });

        let answersToGet = [
                "equitable_victimes",
                "equitable_vulnerables",
                "equitable_jeunes_contrevenants",
                "equitable_riches",
                "equitable_minorites_culturelles"
            ];
        let ageFrom = 0;
        let ageTo = 100;
        let ethnicity = "all";
        let gender = "all";
        let timeAnswered = "all";

        userProfile.getAnswers(answersToGet, ageFrom, ageTo, ethnicity, gender, timeAnswered, function (err, allAnswers) {
            let expectedAnswers = {
                equitable_victimes: 20,
                equitable_vulnerables: 20,
                equitable_jeunes_contrevenants: 20,
                equitable_riches: 20,
                equitable_minorites_culturelles: 20
            };
            console.log("Expected: " + JSON.stringify(expectedAnswers));
            // FIXME: The following check fails:
            compare(allAnswers, expectedAnswers, "check that expected allAnswers match");
        });

        // TODO: test getAnswerByGender
        // TODO: test getAnswerByEthnicity
        // TODO: etc.
    }

    MpopKiosk.UserProfile {
        id: userProfile

        service_port_number: config.service_port_number
        service_host: config.service_host
        is_verbose: config.is_verbose
        thisIsInSystemTests: true
    }

    TestConfigParser {
        id: config
    }
}
