#include "TestMpopService.h"
#include "mpopservice.h"
#include "response.h"
#include "request.h"
#include <QDebug>


// called before the first test function is executed
void TestMpopService::init_TestCase()
{
    // Load the configuration options from the env vars
    Config config;
    MPopService::load_config_from_env_vars(config);

    // Setup the facade
    this->facade = QSharedPointer<Facade>(new Facade(config));

    // Check if MySQL is supported
    this->is_mysql_supported = this->facade->isDatabaseReady();

    this->removeDatabaseTestEntries();
}

void TestMpopService::removeDatabaseTestEntries() {
    // Removes the test entries in the database, if they exist

    if (this->is_mysql_supported) {
        QList<QString> rfidTagsToDelete;

        // Developers who create more mock RFID tags within unit tests should
        // add them to this list:
        rfidTagsToDelete.push_back("test_RFID_tag_0000");
        rfidTagsToDelete.push_back("test_RFID_tag_0001");
        rfidTagsToDelete.push_back("test_RFID_tag_0002");
        rfidTagsToDelete.push_back("test_RFID_tag_0003");
        rfidTagsToDelete.push_back("test_RFID_tag_0004");
        rfidTagsToDelete.push_back("test_RFID_tag_0005");
        rfidTagsToDelete.push_back("test_RFID_tag_0006");
        rfidTagsToDelete.push_back("test_RFID_tag_0007");
        rfidTagsToDelete.push_back("test_RFID_tag_0008");
        rfidTagsToDelete.push_back("test_RFID_tag_0009");
        rfidTagsToDelete.push_back("test_RFID_tag_00010");
        rfidTagsToDelete.push_back("test_RFID_tag_00011");
        this->facade->deleteTagsVisitorsAndTheirAnswers(rfidTagsToDelete);
    }
}

// called after the last test function was executed
void TestMpopService::cleanupTestCase()
{
    this->removeDatabaseTestEntries();

    // No need to delete the MPopService
    // Though, once the destructor is called, the MySQL connection will close.
}

void TestMpopService::test_01_toBoolean()
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

void TestMpopService:: test_15_getAnswers() {

    // Set demographic questions for multiple user
    // Set multiple answers for multiple user
    // check avg value of each question is ok.

    if (! this->is_mysql_supported) {
        QSKIP("This test requires MySQL");
    }

    // User Questions
    static const QString TEST_QUESTION_01_ID = "incidence_drogue";
    static const QString TEST_QUESTION_02_ID = "decriminalisation_crimes_non_violents";
    static const QString TEST_QUESTION_03_ID = "systeme_bureaucrate";
    static const QString TEST_QUESTION_04_ID = "equitable_victimes";

    // User Answers
    static const int TEST_QUESTION_01_VALUE = 25;
    static const int TEST_QUESTION_02_VALUE = 15;
    static const int TEST_QUESTION_03_VALUE = 10;
    static const int TEST_QUESTION_04_VALUE = 90;

    // Calculated Avg. for three User
    static const int Test_QUESTION_01_AVG = 25;
    static const int Test_QUESTION_02_AVG = 15;
    static const int Test_QUESTION_03_AVG = 10;
    static const int Test_QUESTION_04_AVG = 90;

    // declaration of RFIDs
    static const QString TEST_RFID_TAG_1 = "test_RFID_tag_0006";
    static const QString TEST_RFID_TAG_2 = "test_RFID_tag_0007";
    static const QString TEST_RFID_TAG_3 = "test_RFID_tag_0008";

    // declaration of language for users
    static const QString TEST_USER_1_LANGUAGE ="fr";
    static const QString TEST_USER_2_LANGUAGE ="en";
    static const QString TEST_USER_3_LANGUAGE ="en";

    // declaration of gender for users
    static const QString TEST_USER_1_GENDER = "male";
    static const QString TEST_USER_2_GENDER = "female";
    static const QString TEST_USER_3_GENDER = "male";

    // declaration of Ethnicity for users
    static const QString TEST_USER_1_Ethnicity = "quebecer";
    static const QString TEST_USER_2_Ethnicity = "canadian";
    static const QString TEST_USER_3_Ethnicity = "quebecer";

    // Get or create user id.
    int user_1 = this->facade->getOrCreateUser(TEST_RFID_TAG_1);
    int user_2 = this->facade->getOrCreateUser(TEST_RFID_TAG_2);
    int user_3 = this->facade->getOrCreateUser(TEST_RFID_TAG_3);

    // set users Languages
    bool okSetLanguage_1 = this->facade->setUserLanguage(user_1, TEST_USER_1_LANGUAGE);
    QCOMPARE(okSetLanguage_1, true);
    bool okSetLanguage_2 = this->facade->setUserLanguage(user_2, TEST_USER_2_LANGUAGE);
    QCOMPARE(okSetLanguage_2, true);
    bool okSetLanguage_3 = this->facade->setUserLanguage(user_3, TEST_USER_3_LANGUAGE);
    QCOMPARE(okSetLanguage_3, true);

    // set users Gender
    bool okSetGender_1 = this->facade->setUserGender(user_1, TEST_USER_1_GENDER);
    QCOMPARE(okSetGender_1, true);
    bool okSetGender_2 = this->facade->setUserGender(user_2, TEST_USER_2_GENDER);
    QCOMPARE(okSetGender_2, true);
    bool okSetGender_3 = this->facade->setUserGender(user_3, TEST_USER_3_GENDER);
    QCOMPARE(okSetGender_3, true);

    // set users Ethnicity
    bool okSetEthnicity_1 = this->facade->setUserEthnicity(user_1, TEST_USER_1_Ethnicity);
    QCOMPARE(okSetEthnicity_1, true);
    bool okSetEthnicity_2 = this->facade->setUserEthnicity(user_2, TEST_USER_2_Ethnicity);
    QCOMPARE(okSetEthnicity_2, true);
    bool okSetEthnicity_3 = this->facade->setUserEthnicity(user_3, TEST_USER_3_Ethnicity);
    QCOMPARE(okSetEthnicity_3, true);

    // set Users Answers
    this->facade->setUserAnswer(user_1, TEST_QUESTION_01_ID, TEST_QUESTION_01_VALUE);
    this->facade->setUserAnswer(user_1, TEST_QUESTION_02_ID, TEST_QUESTION_02_VALUE);
    this->facade->setUserAnswer(user_1, TEST_QUESTION_03_ID, TEST_QUESTION_03_VALUE);
    this->facade->setUserAnswer(user_1, TEST_QUESTION_04_ID, TEST_QUESTION_04_VALUE);

    this->facade->setUserAnswer(user_2, TEST_QUESTION_01_ID, TEST_QUESTION_01_VALUE);
    this->facade->setUserAnswer(user_2, TEST_QUESTION_02_ID, TEST_QUESTION_02_VALUE);
    this->facade->setUserAnswer(user_2, TEST_QUESTION_03_ID, TEST_QUESTION_03_VALUE);
    this->facade->setUserAnswer(user_2, TEST_QUESTION_04_ID, TEST_QUESTION_04_VALUE);

    this->facade->setUserAnswer(user_3, TEST_QUESTION_01_ID, TEST_QUESTION_01_VALUE);
    this->facade->setUserAnswer(user_3, TEST_QUESTION_02_ID, TEST_QUESTION_02_VALUE);
    this->facade->setUserAnswer(user_3, TEST_QUESTION_03_ID, TEST_QUESTION_03_VALUE);
    this->facade->setUserAnswer(user_3, TEST_QUESTION_04_ID, TEST_QUESTION_04_VALUE);


    QList<QString> questionList;
    questionList.insert(0,TEST_QUESTION_01_ID);
    questionList.insert(1,TEST_QUESTION_02_ID);
    questionList.insert(2,TEST_QUESTION_03_ID);
    questionList.insert(3,TEST_QUESTION_04_ID);


    qDebug() << "Questions List" << questionList;

    QMap<QString, int> answers = this->facade->getAnswers(questionList,-1,-1,"all","all","all");

    QCOMPARE(answers[TEST_QUESTION_01_ID], Test_QUESTION_01_AVG);
    QCOMPARE(answers[TEST_QUESTION_02_ID], Test_QUESTION_02_AVG);
    QCOMPARE(answers[TEST_QUESTION_03_ID], Test_QUESTION_03_AVG);
    QCOMPARE(answers[TEST_QUESTION_04_ID], Test_QUESTION_04_AVG);

}

void TestMpopService::test_10_getOrCreateUser() {
    // Create a user
    // chech it exists.
    // then try to create it again, and it should return its id.

    if (! this->is_mysql_supported) {
        QSKIP("This test requires MySQL");
    }
    static const QString TEST_RFID_TAG = "test_RFID_tag_0000";
    int userId = this->facade->getOrCreateUser(TEST_RFID_TAG);

    // Check that our user has the same ID as before:
    int userIdAgain = this->facade->getOrCreateUser(TEST_RFID_TAG);
    QCOMPARE(userIdAgain, userId);
}

void TestMpopService::test_21_getUserAnswers() {
    // Get/set a single answer for a user.

    if (! this->is_mysql_supported) {
        QSKIP("This test requires MySQL");
    }
    static const QString TEST_RFID_TAG = "test_RFID_tag_0001";
    static const QString TEST_QUESTION_ID = "confiance_systeme";
    int userId = this->facade->getOrCreateUser(TEST_RFID_TAG);

    // Set an answer, and get it:
    int value = 99;
    this->facade->setUserAnswer(userId, TEST_QUESTION_ID, value);
    QMap<QString, int> answers = this->facade->getUserAnswers(userId);
    QVariant actual = answers[TEST_QUESTION_ID];
    QCOMPARE(actual, value);

}

void TestMpopService::test_12_setUserLanguage() {
    // Set/get the language of a user.

    if (! this->is_mysql_supported) {
        QSKIP("This test requires MySQL");
    }
    static const QString TEST_RFID_TAG = "test_RFID_tag_0002";
    static const QString TEST_USER_LANGUAGE = "fr";
    static const QString KEY_LANGUAGE = "language";
    int userId = this->facade->getOrCreateUser(TEST_RFID_TAG);

    // Should be initially null:
    qDebug() << "Check its initial state";
    QMap<QString, QVariant> userInfo1 = this->facade->getUserInfo(userId);
    QVariant userLanguage1 = userInfo1[KEY_LANGUAGE];
    QString str_userLanguage = userLanguage1.toString();
    QCOMPARE(str_userLanguage, QString());

    // Should match what we set it to:
    qDebug() << "Check its new set state";
    bool okSetLanguage = this->facade->setUserLanguage(userId, TEST_USER_LANGUAGE);
    QCOMPARE(okSetLanguage, true);
    QMap<QString, QVariant> userInfo2 = this->facade->getUserInfo(userId);
    QVariant userLanguage = userInfo2[KEY_LANGUAGE];
    QCOMPARE(userLanguage, TEST_USER_LANGUAGE);
}

void TestMpopService::test_13_setUserGender() {
    // Set/get the gender of a user.

    if (! this->is_mysql_supported) {
        QSKIP("This test requires MySQL");
    }
    static const QString TEST_RFID_TAG = "test_RFID_tag_0003";
    static const QString TEST_USER_GENDER = "male";
    static const QString KEY_GENDER = "gender";
    int userId = this->facade->getOrCreateUser(TEST_RFID_TAG);

    // Should be initially null:
    qDebug() << "Check its initial state";
    QMap<QString, QVariant> userInfo1 = this->facade->getUserInfo(userId);
    QVariant userGender1 = userInfo1[KEY_GENDER];
    QString str_userGender = userGender1.toString();
    QCOMPARE(QString(), str_userGender);

    // Should match what we set it to:
    qDebug() << "Check its new set state";
    this->facade->setUserGender(userId, TEST_USER_GENDER);
    QMap<QString, QVariant> userInfo2 = this->facade->getUserInfo(userId);
    QVariant userGender = userInfo2[KEY_GENDER];
    QCOMPARE(TEST_USER_GENDER, userGender);
}

void TestMpopService::test_14_setUserEthnicity() {
    // Set/get the ethnicity of a user.
    // First check it's unset. (or null)

    if (! this->is_mysql_supported) {
        QSKIP("This test requires MySQL");
    }
    static const QString TEST_RFID_TAG = "test_RFID_tag_0004";
    static const QString TEST_USER_ETHNICITY = "quebecer";
    static const QString KEY_ETHNICITY = "ethnicity";
    int userId = this->facade->getOrCreateUser(TEST_RFID_TAG);

    // Should be initially null:
    qDebug() << "Check its initial state";
    QMap<QString, QVariant> userInfo1 = this->facade->getUserInfo(userId);
    QVariant userEthnicity1 = userInfo1[KEY_ETHNICITY];
    int int_userEthnicity = userEthnicity1.toInt();
    QCOMPARE(int_userEthnicity, int());

    // Should match what we set it to:
    qDebug() << "Check its new set state";
    this->facade->setUserEthnicity(userId, TEST_USER_ETHNICITY);
    QMap<QString, QVariant> userInfo2 = this->facade->getUserInfo(userId);
    QVariant userEthnicity2 = userInfo2[KEY_ETHNICITY];
    QCOMPARE(TEST_USER_ETHNICITY, userEthnicity2);
}

void TestMpopService::test_02_requestParams() {
    // Creates a request from a JSON string
    // And check that its positional params are ok.
    // TODO: test named params

    static const QString EXPECTED_RFID_TAG = "test_RFID_tag_doesntexist";
    static const int EXPECTED_REQUEST_ID = 9999;
    static const QString EXPECTED_METHOD_NAME = "getOrCreateUser";
    static const QString request_getOrCreateUser_01 =
        "{"
            "\"id\": 9999, "
            "\"method\":\"getOrCreateUser\", "
            "\"params\": ["
                "\"test_RFID_tag_doesntexist\""
            "]"
        "}";
     Request request = Request::fromString(request_getOrCreateUser_01);
     QString requestToString = request.toString();
     qDebug() << "request_getOrCreateUser_01:" << request_getOrCreateUser_01;
     qDebug() << "requestToString:" << requestToString;

     QString method = request.method;
     QCOMPARE(EXPECTED_METHOD_NAME, method);

     QString param_1 = request.getParamByPosition(0).toString();
     QCOMPARE(EXPECTED_RFID_TAG, param_1);

     int request_id = request.intId;
     QCOMPARE(EXPECTED_REQUEST_ID, request_id);
}

void TestMpopService::test_03_response() {
    // Checks that a response can copy the id from a request,
    // so that clients can know which of their request to associate them to.
    // We could test the other properties, but it's mostly
    // TODO: test the id that can either be a string or an int.

    static const int EXPECTED_RESPONSE_ID = 9999;
    static const QString request_getOrCreateUser_01 =
        "{"
            "\"id\": 9999, "
            "\"method\":\"getOrCreateUser\", "
            "\"params\": ["
                "\"test_RFID_tag_doesntexist\""
            "]"
        "}";
    Request request = Request::fromString(request_getOrCreateUser_01);
    Response response;
    response.copyIdFromRequest(request);

    int response_id = response.intId;
    QCOMPARE(EXPECTED_RESPONSE_ID, response_id);
}

void TestMpopService::test_04_error_response() {
    // Checks that a response that contains an error ends up in a JSON string
    // that reflects that error, its error number and its message.
    static const int EXPECTED_RESPONSE_ID = 9999;
    static const int ERROR_CODE = 99;
    static const QString EXPECTED_ERROR_STRING = "{\"error\":{\"code\":99,\"data\":null,\"message\":\"This is an error message.\"},\"id\":9999}";

    Response response;
    response.idType = Response::NUMBER_ID;
    response.intId = EXPECTED_RESPONSE_ID;
    response.error.message = "This is an error message.";
    response.error.code = ERROR_CODE;
    // we could also check the data member of the error, but we don't use it for now.

    QCOMPARE(response.NUMBER_ID, response.idType);

    int response_id = response.intId;
    QCOMPARE(EXPECTED_RESPONSE_ID, response_id);

    // We test the toString method:
    QString errorResponseString = response.toString();
    qDebug() << errorResponseString;

    // TODO: Also test the fromString method

    // make sure the string matches what it should be
    QCOMPARE(errorResponseString, EXPECTED_ERROR_STRING);
}

void TestMpopService::test_20_setUserAnswer() {
    // Set multiple answers for a user
    // And check they are ok.

    if (! this->is_mysql_supported) {
        QSKIP("This test requires MySQL");
    }

    static const QString TEST_QUESTION_01_ID = "incidence_drogue";
    static const QString TEST_QUESTION_02_ID = "decriminalisation_crimes_non_violents";
    static const QString TEST_QUESTION_03_ID = "systeme_bureaucrate";
    static const QString TEST_QUESTION_04_ID = "equitable_victimes";
    static const int TEST_QUESTION_01_VALUE = 23;
    static const int TEST_QUESTION_02_VALUE = 100;
    static const int TEST_QUESTION_03_VALUE = 0;
    static const int TEST_QUESTION_04_VALUE = 99;
    static const int TEST_QUESTION_01_UPDATE_VALUE = 33;
    static const int TEST_QUESTION_02_UPDATE_VALUE = 90;
    static const int TEST_QUESTION_03_UPDATE_VALUE = 10;
    static const int TEST_QUESTION_04_UPDATE_VALUE = 80;
    static const QString TEST_RFID_TAG = "test_RFID_tag_0005";

    int userId = this->facade->getOrCreateUser(TEST_RFID_TAG);

    // set User Answers
    this->facade->setUserAnswer(userId, TEST_QUESTION_01_ID, TEST_QUESTION_01_VALUE);
    this->facade->setUserAnswer(userId, TEST_QUESTION_02_ID, TEST_QUESTION_02_VALUE);
    this->facade->setUserAnswer(userId, TEST_QUESTION_03_ID, TEST_QUESTION_03_VALUE);
    this->facade->setUserAnswer(userId, TEST_QUESTION_04_ID, TEST_QUESTION_04_VALUE);

    QMap<QString, int> answers = this->facade->getUserAnswers(userId);

    QCOMPARE(answers[TEST_QUESTION_01_ID], TEST_QUESTION_01_VALUE);
    QCOMPARE(answers[TEST_QUESTION_02_ID], TEST_QUESTION_02_VALUE);
    QCOMPARE(answers[TEST_QUESTION_03_ID], TEST_QUESTION_03_VALUE);
    QCOMPARE(answers[TEST_QUESTION_04_ID], TEST_QUESTION_04_VALUE);

    // update user answers
    this->facade->setUserAnswer(userId, TEST_QUESTION_01_ID, TEST_QUESTION_01_UPDATE_VALUE);
    this->facade->setUserAnswer(userId, TEST_QUESTION_02_ID, TEST_QUESTION_02_UPDATE_VALUE);
    this->facade->setUserAnswer(userId, TEST_QUESTION_03_ID, TEST_QUESTION_03_UPDATE_VALUE);
    this->facade->setUserAnswer(userId, TEST_QUESTION_04_ID, TEST_QUESTION_04_UPDATE_VALUE);

    //retrive Updated Answers
    QMap<QString, int> answers1 = this->facade->getUserAnswers(userId);

    qDebug() << answers1;

    QCOMPARE(answers1[TEST_QUESTION_01_ID], TEST_QUESTION_01_UPDATE_VALUE);
    QCOMPARE(answers1[TEST_QUESTION_02_ID], TEST_QUESTION_02_UPDATE_VALUE);
    QCOMPARE(answers1[TEST_QUESTION_03_ID], TEST_QUESTION_03_UPDATE_VALUE);
    QCOMPARE(answers1[TEST_QUESTION_04_ID], TEST_QUESTION_04_UPDATE_VALUE);
}

void TestMpopService::test_22_getAnswerByAge() {

    if (! this->is_mysql_supported) {
        QSKIP("This test requires MySQL");
    }

    // User Question
    static const QString TEST_QUESTION_05_ID = "equitable_vulnerables";

    // User Answers
    static const int TEST_QUESTION_05_VALUE = 35;

    // Calculated Avg. for three User
    static const int Test_QUESTION_05_AVG = 35;

    // declaration of RFIDs
    static const QString TEST_RFID_TAG_1 = "test_RFID_tag_0009";

    // Get or create user id.
    int user_1 = this->facade->getOrCreateUser(TEST_RFID_TAG_1);

    // set Users Answers
    this->facade->setUserAnswer(user_1, TEST_QUESTION_05_ID, TEST_QUESTION_05_VALUE);


    QList<int> answers1 = this->facade->getAnswerByAge(TEST_QUESTION_05_ID,"all","all","all");

    qDebug() << "Questions List" << answers1;
    //QCOMPARE(answers1[TEST_QUESTION_05_ID], Test_QUESTION_05_AVG);

}

void TestMpopService::test_23_getAnswerByEthnicity() {
    qDebug() << "test_23_getAnswerByEthnicity";
    // Set demographic questions for multiple user
    // Set multiple answers for multiple user
    // check avg value of each question is ok.

    if (! this->is_mysql_supported) {
        QSKIP("This test requires MySQL");
    }

    // User Questions
    static const QString TEST_QUESTION_06_ID = "equitable_jeunes_contrevenants";

    // User Answers
    static const int TEST_QUESTION_06_VALUE = 45;

    // Calculated Avg. for three User
    static const int Test_QUESTION_06_AVG = 45;

    // declaration of RFIDs
    static const QString TEST_RFID_TAG_1 = "test_RFID_tag_00010";

    // declaration of Ethnicity for users
    static const QString TEST_USER_1_Ethnicity = "quebecer";

    // Get or create user id.
    int user_1 = this->facade->getOrCreateUser(TEST_RFID_TAG_1);


    // set users Ethnicity
    bool okSetEthnicity_1 = this->facade->setUserEthnicity(user_1, TEST_USER_1_Ethnicity);
    QCOMPARE(okSetEthnicity_1, true);

    // set Users Answers
    this->facade->setUserAnswer(user_1, TEST_QUESTION_06_ID, TEST_QUESTION_06_VALUE);

    QMap<QString,int> answers = this->facade->getAnswerByEthnicity(TEST_QUESTION_06_ID,-1,-1,"all","all");
    qDebug() << answers;
    QCOMPARE(answers[TEST_USER_1_Ethnicity], Test_QUESTION_06_AVG);
}

void TestMpopService::test_24_getAnswerByGender() {
    qDebug() << "test_24_getAnswerByGender";

    if (! this->is_mysql_supported) {
        QSKIP("This test requires MySQL");
    }

    // User Questions
    static const QString TEST_QUESTION_07_ID = "equitable_riches";
    static const QString TEST_QUESTION_08_ID = "equitable_minorites_culturelles";
    static const QString TEST_QUESTION_09_ID = "confiance_systeme";

    // User Answers
    static const int TEST_QUESTION_07_VALUE = 55;
    static const int TEST_QUESTION_08_VALUE = 65;
    static const int TEST_QUESTION_09_VALUE = 67;


    // Calculated Avg. for three User
    static const int Test_QUESTION_07_AVG = 55;
    static const int Test_QUESTION_08_AVG = 65;
    static const int Test_QUESTION_09_AVG = 67;

    // declaration of RFIDs
    static const QString TEST_RFID_TAG_1 = "test_RFID_tag_00011";
    static const QString TEST_RFID_TAG_2 = "test_RFID_tag_00012";
    static const QString TEST_RFID_TAG_3 = "test_RFID_tag_00013";


    // declaration of gender for users
    static const QString TEST_USER_1_GENDER = "male";
    static const QString TEST_USER_2_GENDER = "female";
    static const QString TEST_USER_3_GENDER = "other";

    // Get or create user id.
    int user_1 = this->facade->getOrCreateUser(TEST_RFID_TAG_1);
    int user_2 = this->facade->getOrCreateUser(TEST_RFID_TAG_2);
    int user_3 = this->facade->getOrCreateUser(TEST_RFID_TAG_3);

    // set users Gender
    bool okSetGender_1 = this->facade->setUserGender(user_1, TEST_USER_1_GENDER);
    QCOMPARE(okSetGender_1, true);

    bool okSetGender_2 = this->facade->setUserGender(user_2, TEST_USER_2_GENDER);
    QCOMPARE(okSetGender_2, true);

    bool okSetGender_3 = this->facade->setUserGender(user_3, TEST_USER_3_GENDER);
    QCOMPARE(okSetGender_3, true);

    // set Users Answers
    this->facade->setUserAnswer(user_1, TEST_QUESTION_07_ID, TEST_QUESTION_07_VALUE);
    this->facade->setUserAnswer(user_2, TEST_QUESTION_08_ID, TEST_QUESTION_08_VALUE);
    this->facade->setUserAnswer(user_3, TEST_QUESTION_09_ID, TEST_QUESTION_09_VALUE);

    QMap<QString,int> answers = this->facade->getAnswerByGender(TEST_QUESTION_07_ID,"all",-1,-1,"all");
    QMap<QString,int> answers1 = this->facade->getAnswerByGender(TEST_QUESTION_08_ID,"all",-1,-1,"all");
    QMap<QString,int> answers2 = this->facade->getAnswerByGender(TEST_QUESTION_09_ID,"all",-1,-1,"all");

    //Multiple User with Multiple answers
    QCOMPARE(answers[TEST_USER_1_GENDER], Test_QUESTION_07_AVG);
    QCOMPARE(answers1[TEST_USER_2_GENDER], Test_QUESTION_08_AVG);
    QCOMPARE(answers2[TEST_USER_3_GENDER], Test_QUESTION_09_AVG);


}



