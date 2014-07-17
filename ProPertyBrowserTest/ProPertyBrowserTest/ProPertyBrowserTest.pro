#-------------------------------------------------
#
# Project created by QtCreator 2014-07-15T15:58:16
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

DEPENDPATH += .
INCLUDEPATH += .
include(./src/qtpropertybrowser.pri)

TARGET = ProPertyBrowserTest
TEMPLATE = app


SOURCES += main.cpp\
        propertybrowsertest.cpp

HEADERS  += propertybrowsertest.h

FORMS    += propertybrowsertest.ui
