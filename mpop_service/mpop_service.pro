QT += websockets
QT += sql
QT += core
# QT -= gui

TARGET = mpop_service
CONFIG += console
CONFIG -= app_bundle
CONFIG += c++11

TEMPLATE = app

SOURCES += \
    Logger.cpp \
    facade.cpp \
    main.cpp \
    mpopservice.cpp \
    notification.cpp \
    request.cpp \
    response.cpp

HEADERS += \
    Logger.h \
    config.h \
    facade.h \
    missingparametererror.h \
    mpopservice.h \
    notification.h \
    request.h \
    response.h \
    sqlerror.h

# EXAMPLE_FILES += exampleclient.html

INSTALLS += TARGET
