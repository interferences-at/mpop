QT += testlib
QT += core
QT -= gui

CONFIG += sdk_no_version_check
CONFIG += c++11

TARGET = test_mpop_kiosk
CONFIG += console
CONFIG -= app_bundle
CONFIG += warn_on qmltestcase
TEMPLATE = app

SOURCES += main.cpp

HEADERS +=

INCLUDEPATH += $$PWD/../mpop_kiosk/

# LIBS += -L$$PWD/../mpop_kiosk -lmpop_kiosk

DISTFILES += \
    TestConfigParser.qml \
    tst_mpop_kiosk.qml \
    tst_tests_work.qml

