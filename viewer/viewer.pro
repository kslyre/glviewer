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
    camera.cpp \
    raytrace.cpp \
    modelfactory.cpp \
    bvh.cpp \
    model.cpp \
    vbo.cpp \
    derivable.cpp \
    functors.cpp \
    optimizations.cpp \
    kdtree.cpp

HEADERS  += widget.h \
    window.h \
    widgettests.h \
    camera.h \
    raytrace.h \
    modelfactory.h \
    bvh.h \
    model.h \
    vbo.h \
    derivable.h \
    functors.h \
    optimizations.h \
    kdtree.h

RESOURCES += \
    resources.qrc
