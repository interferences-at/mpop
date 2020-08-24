#pragma once

#include <QObject>
#include <QtTest/QtTest>
#include <QSharedPointer>
#include "mpopservice.h"
#include "facade.h"

class TestMpopService: public QObject
{
    Q_OBJECT

private slots:
    // called before the first test function is executed
    void init_TestCase();

    // test methods for the utility methods of the Facade class:
    void test_01_toBoolean();


    // test method for Get Average of Answers  for Multiple Question Ids:
    void test_15_getAnswers();

    // test methods for the Request and Response classes:
    void test_02_requestParams();
    void test_03_response();
    void test_04_error_response();

    // test methods for the demographic questions:
    void test_10_getOrCreateUser();
    void test_12_setUserLanguage();
    void test_13_setUserGender();
    void test_14_setUserEthnicity();

    // test methods for the survey question for a single visitor:
    void test_20_setUserAnswer();
    void test_21_getUserAnswers();

    // TODO: test to free a tag

    // test methods for the survey questions, for multiple visitors: (for the dataviz)
    // TODO

    // called after the last test function was executed
    void cleanupTestCase();

private:
    QSharedPointer<Facade> facade;
    bool is_mysql_supported;

    void removeDatabaseTestEntries();
};
