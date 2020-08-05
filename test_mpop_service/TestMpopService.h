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

    // test functions:
    void test_toBoolean();
    void test_getOrCreateUser();
    void test_getUserAnswers();
    void test_getUserLanguage();
    void test_getUserGender();
    void test_getUserNation();
    // called after the last test function was executed
    void cleanupTestCase();

private:
    QSharedPointer<Facade> facade;
};
