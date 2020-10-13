import QtQuick 2.3
import QtTest 1.0
import "../mpop_kiosk" as MpopKiosk

TestCase {
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
                    incidence_drogue: false
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
                console.log("Did not set the user's gender, yet");
            }
            if (userTasks.setAge === false) {
                ret = false;
                console.log("Did not set the user's age, yet");
            }
            if (userTasks.answerQuestions.incidence_drogue === false) {
                ret = false;
                console.log("Did not answer question incidence_drogue, yet");
            }

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
        const userOneAnswers = {
            incidence_drogue: 99
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

        let totalTimeWait = 0;
        while (userIsAllSet(userOneTasks) === false) {
            wait(5); // ms
            totalTimeWait += 5;
            if (totalTimeWait >= 10000) {
                fail("It took too long");
            }
        }

        compare(userProfile.age, userOneAge, "age has been set");
        compare(userProfile.gender, userOneGender, "gender has been set");
        compare(userProfile.answers.hasOwnProperty("incidence_drogue") ? userProfile.answers.incidence_drogue : "", userOneAnswers.incidence_drogue, "Answer for incidence_drogue has been set.");
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
