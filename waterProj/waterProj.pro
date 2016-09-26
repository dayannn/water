#-------------------------------------------------
#
# Project created by QtCreator 2016-07-05T11:16:00
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = waterProj
TEMPLATE = app

CONFIG += c++11

SOURCES += main.cpp\
        mainwindow.cpp \
    facade.cpp \
    transformmanager.cpp \
    scenemanager.cpp \
    drawmanager.cpp \
    loadmanager.cpp \
    compositeobject.cpp \
    camera.cpp \
    carcasmodelpainter.cpp \
    filestreamimp.cpp \
    stream.cpp \
    scene.cpp \
    canvas.cpp \
    paintwidget.cpp \
    transformer.cpp \
    matrix.cpp \
    polygonalmodelpainter.cpp \
    geometry.cpp \
    model.cpp \
    watergrid.cpp

HEADERS  += mainwindow.h \
    facade.h \
    baseexception.h \
    basecommand.h \
    transformmanager.h \
    scenemanager.h \
    drawmanager.h \
    loadmanager.h \
    baseobject.h \
    compositeobject.h \
    camera.h \
    basescene.h \
    basecanvas.h \
    basepainter.h \
    carcasmodelpainter.h \
    exceptions.h \
    streamimp.h \
    filestreamimp.h \
    stream.h \
    scene.h \
    canvas.h \
    paintwidget.h \
    transformer.h \
    matrix.h \
    geometry.h \
    polygonalmodelpainter.h \
    model.h \
    watergrid.h

FORMS    += mainwindow.ui

DISTFILES += \
    classes_diagram.qmodel

# Profiler flags
#CONFIG += DEBUG
#QMAKE_CXXFLAGS_DEBUG += -pg
#QMAKE_CXXFLAGS_MT_DBG += -pg
#QMAKE_LFLAGS_DEBUG += -pg
