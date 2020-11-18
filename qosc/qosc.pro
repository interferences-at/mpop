# FIXME: this .pro file is unused.
# It was difficult to build this library on Windows

TEMPLATE = lib

VERSION = 1.0.0

# TARGET = QOSC

QT -= gui
QT += core
QT += network

CONFIG -= app_bundle
CONFIG += create_prl
CONFIG += c++11
CONFIG += staticlib
CONFIG += sdk_no_version_check # disable warning on macOS
CONFIG += debug_and_release
CONFIG += build_all
CONFIG -= qtquickcompiler

CONFIG(debug, debug|release) {
    TARGET = qosc_debug_binary
} else {
    TARGET = qosc_release_binary
}

DEFINES += QOSC_LIBRARY

SOURCES += \
    oscsender.cpp \
    oscreceiver.cpp \
    contrib/oscpack/OscOutboundPacketStream.cpp \
    contrib/oscpack/OscPrintReceivedElements.cpp \
    contrib/oscpack/OscReceivedElements.cpp \
    contrib/oscpack/OscTypes.cpp

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    oscsender.h \
    oscreceiver.h \
    contrib/oscpack/MessageMappingOscPacketListener.h \
    contrib/oscpack/OscException.h \
    contrib/oscpack/OscHostEndianness.h \
    contrib/oscpack/OscOutboundPacketStream.h \
    contrib/oscpack/OscPacketListener.h \
    contrib/oscpack/OscPrintReceivedElements.h \
    contrib/oscpack/OscReceivedElements.h \
    contrib/oscpack/OscTypes.h

