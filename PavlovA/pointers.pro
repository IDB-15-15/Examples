QT += core
QT -= gui

TARGET = pointers
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    figures.cpp

HEADERS += \
    figures.h

QMAKE_CXXFLAGS += -std=gnu++14
