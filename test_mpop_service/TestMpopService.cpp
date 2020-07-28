#include "TestMpopService.h"
#include "mpopservice.h"

// called before the first test function is executed
void TestMpopService::init_TestCase()
{
    Config config;
    MPopService::load_config_from_env_vars(config);
    // FIXME: Will attempt to connect to a MySQL database
    // Relies on it.
    this->facade = QSharedPointer<Facade>(new Facade(config));

    // TODO: Perhaps clean all users that we've created in previous tests, as well as all their answers
}


// called after the last test function was executed
void TestMpopService::cleanupTestCase()
{
    // No need to delete the MPopService
    // Though, once the destructor is called, the MySQL connection will close.
}


void TestMpopService::test_toBoolean()
{
    // Expected to be true:
    QCOMPARE(MPopService::toBoolean("TRUE"), true);
    QCOMPARE(MPopService::toBoolean("true"), true);
    QCOMPARE(MPopService::toBoolean("True"), true);
    QCOMPARE(MPopService::toBoolean("1"), true);

    // Expected to be false:
    QCOMPARE(MPopService::toBoolean("FALSE"), false);
    QCOMPARE(MPopService::toBoolean("false"), false);
    QCOMPARE(MPopService::toBoolean("False"), false);
    QCOMPARE(MPopService::toBoolean("0"), false);

    // Invalid values should return false:
    QCOMPARE(MPopService::toBoolean("qweqweqwe"), false);
    QCOMPARE(MPopService::toBoolean("????"), false);
    QCOMPARE(MPopService::toBoolean("987"), false);
    QCOMPARE(MPopService::toBoolean("!!!!!!"), false);
}

void TestMpopService::test_getOrCreateUser() {
    static const QString TEST_RFID_TAG = "test_RFID_tag_0000";
    int userId = this->facade->getOrCreateUser(TEST_RFID_TAG);

    // Check that our user has the same ID as before:
    int userIdAgain = this->facade->getOrCreateUser(TEST_RFID_TAG);
    QCOMPARE(userIdAgain, userId);
}


void TestMpopService::test_getUserAnswers() {
    static const QString TEST_RFID_TAG = "test_RFID_tag_0001";
    static const QString TEST_QUESTION_ID = "confiance_systeme";
    int userId = this->facade->getOrCreateUser(TEST_RFID_TAG);

    // Set an answer, and get it:
    int value = 99;
    this->facade->setUserAnswer(userId, TEST_QUESTION_ID, value);
    QMap<QString, int> answers = this->facade->getUserAnswers(userId);
    int actual = answers[TEST_QUESTION_ID];
    QCOMPARE(actual, value);
}
