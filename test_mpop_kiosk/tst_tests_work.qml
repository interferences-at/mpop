import QtQuick 2.3
import QtTest 1.0

// Simple example of a test.
TestCase {
    name: "MathTests"

    function test_math() {
        compare(2 + 2, 4, "2 + 2 = 4")
    }

// XXX: The following test should fail:
//    function test_fail() {
//        compare(2 + 2, 5, "2 + 2 = 5")
//    }
}
