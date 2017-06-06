#-------------------------------------------------
#
# Project created by QtCreator 2017-05-25T00:00:11
#
#-------------------------------------------------

QT       += core gui opengl testlib

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = viewer
TEMPLATE = app

LIBS += -lopengl32

include(triangulation3d/Triangulation/Triangulation.pri)

SOURCES += main.cpp\
        widget.cpp \
    window.cpp \
    widgettests.cpp \
    camera.cpp

HEADERS  += widget.h \
    window.h \
    widgettests.h \
    camera.h

RESOURCES += \
    resources.qrc
