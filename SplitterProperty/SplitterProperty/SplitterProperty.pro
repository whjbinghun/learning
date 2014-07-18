#-------------------------------------------------
#
# Project created by QtCreator 2014-07-18T09:26:15
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

DEPENDPATH += .
INCLUDEPATH += .
include(./src/qtpropertybrowser.pri)

TARGET = SplitterProperty
TEMPLATE = app


SOURCES += main.cpp\
        splitterproperty.cpp \
    propertybrowsera.cpp \
    wndchart.cpp \
    wndpropertybrowser.cpp \
    wndtempana.cpp

HEADERS  += splitterproperty.h \
    propertybrowsera.h \
    wndchart.h \
    wndpropertybrowser.h \
    wndtempana.h

FORMS    += splitterproperty.ui
