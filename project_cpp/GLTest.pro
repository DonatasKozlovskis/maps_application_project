#-------------------------------------------------
#
# Project created by QtCreator 2013-11-25T17:00:02
#
#-------------------------------------------------

QT       += core gui opengl sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GLTest
TEMPLATE = app


SOURCES += main.cpp  mainwindow.cpp glwidget.cpp \
    map.cpp \
    road.cpp \
    node.cpp dijkstra.cpp poi.cpp patch.cpp

HEADERS  += mainwindow.h glwidget.h \
    map.h \
    road.h \
    node.h dijkstra.h poi.h patch.h

FORMS    += mainwindow.ui
