QT += testlib
QT += core
QT -= gui

CONFIG += sdk_no_version_check
CONFIG += c++11

TARGET = test_mpop_kiosk
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    TestMpopKiosk.cpp

HEADERS += \
    TestMpopKiosk.h

INCLUDEPATH += $$PWD/../mpop_kiosk/

# LIBS += -L$$PWD/../mpop_kiosk -lmpop_kiosk

