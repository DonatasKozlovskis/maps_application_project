#-------------------------------------------------
#
# Project created by QtCreator 2013-11-24T01:44:57
#
#-------------------------------------------------

QT       += core gui opengl sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SEProjectDraft
TEMPLATE = app

INCLUDEPATH += "..\..\eigen"

SOURCES += main.cpp\
    mapexplorer.cpp \
    road.cpp \
    node.cpp \
    map.cpp \
    mapglwidget.cpp \
    dijkstra.cpp patch.cpp poi.cpp \
    poiwidget.cpp \
    poicontainer.cpp

HEADERS  += mapexplorer.h \
    road.h \
    node.h \
    map.h \
    mapglwidget.h \
    dijkstra.h \
    patch.h \
    poi.h \
    poiwidget.h \
    poicontainer.h

FORMS    += mapexplorer.ui \
    poiwidget.ui

RESOURCES += \
    maps.qrc