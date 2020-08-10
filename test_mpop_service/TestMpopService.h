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

    // test functions for the Facade class:
    void test_01_toBoolean();

    // test functions for the Request class:
    void test_02_requestParams();

    // test functions for the demographic questions:
    void test_10_getOrCreateUser();
    void test_12_getUserLanguage();
    void test_13_getUserGender();
    void test_14_getUserEthnicity();

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
};
