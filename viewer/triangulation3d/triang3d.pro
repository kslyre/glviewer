QT += core
QT += gui testlib

CONFIG += c++11

TARGET = triang3d
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

include(Triangulation/Triangulation.pri)

SOURCES += main.cpp \
    triangulationtests.cpp

HEADERS += \
    triangulationtests.h
