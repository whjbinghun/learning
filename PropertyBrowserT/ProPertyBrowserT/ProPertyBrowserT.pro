#-------------------------------------------------
#
# Project created by QtCreator 2014-07-16T19:45:17
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

DEPENDPATH += .
INCLUDEPATH += .
include(./src/qtpropertybrowser.pri)

TARGET = ProPertyBrowserT
TEMPLATE = app


SOURCES += main.cpp\
        propertybrowsert.cpp \
    propertybrowsera.cpp \
    propertydockwidget.cpp

HEADERS  += propertybrowsert.h \
    propertybrowsera.h \
    propertydockwidget.h

FORMS    += propertybrowsert.ui
