#-------------------------------------------------
#
# Project created by QtCreator 2016-11-20T13:48:35
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = WidgetTest
TEMPLATE = app


SOURCES += main.cpp\
        WidgetTest.cpp \
    FirstWidget.cpp \
    SecondWidget.cpp \
    FirstDialog.cpp \
    SecondDialog.cpp

HEADERS  += WidgetTest.h \
    FirstWidget.h \
    SecondWidget.h \
    FirstDialog.h \
    SecondDialog.h

FORMS    += WidgetTest.ui \
    FirstDialog.ui \
    SecondDialog.ui
