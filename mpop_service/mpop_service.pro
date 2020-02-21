QT += websockets
QT += sql
QT += core

TARGET = mpop_service
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += \
    facade.cpp \
    main.cpp \
    mpopservice.cpp \
    notification.cpp \
    request.cpp \
    response.cpp

HEADERS += \
    facade.h \
    mpopservice.h \
    notification.h \
    request.h \
    response.h

# EXAMPLE_FILES += exampleclient.html

INSTALLS += TARGET
