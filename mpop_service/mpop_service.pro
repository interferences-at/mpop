QT += websockets

TARGET = mpop_service
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += \
    main.cpp \
    mpopservice.cpp \
    request.cpp \
    response.cpp

HEADERS += \
    mpopservice.h \
    request.h \
    response.h

# EXAMPLE_FILES += exampleclient.html

INSTALLS += TARGET
