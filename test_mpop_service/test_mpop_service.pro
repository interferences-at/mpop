# Compiles the tests for the mpop_service

TEMPLATE = app

QT += testlib
QT += websockets
QT += sql
QT += core
QT -= gui

CONFIG += sdk_no_version_check
CONFIG += c++11
CONFIG += console
CONFIG -= app_bundle

TARGET = test_mpop_service

SOURCES += \
    ../mpop_service/facade.cpp \
    ../mpop_service/mpopservice.cpp \
    ../mpop_service/notification.cpp \
    ../mpop_service/request.cpp \
    ../mpop_service/response.cpp \
    main.cpp \
    TestMpopService.cpp

HEADERS += \
    ../mpop_service/config.h \
    ../mpop_service/facade.h \
    ../mpop_service/mpopservice.h \
    ../mpop_service/notification.h \
    ../mpop_service/request.h \
    ../mpop_service/response.h \
    TestMpopService.h

INCLUDEPATH += $$PWD/../mpop_service/
