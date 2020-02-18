QT += websockets

TARGET = mpop_service
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += \
    main.cpp \
    mpopservice.cpp

HEADERS += \
    mpopservice.h

# EXAMPLE_FILES += exampleclient.html

INSTALLS += TARGET
