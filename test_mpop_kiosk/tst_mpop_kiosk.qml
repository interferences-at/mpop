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

    MpopKiosk.UserProfile {
        id: userProfile

        service_port_number: config.service_port_number
        service_host: config.service_host
        is_verbose: config.is_verbose
    }

    TestConfigParser {
        id: config
    }
}
