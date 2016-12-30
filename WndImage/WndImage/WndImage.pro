#-------------------------------------------------
#
# Project created by QtCreator 2014-12-03T17:54:14
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = WndImage
TEMPLATE = app


SOURCES += main.cpp\
        wndimage.cpp \
    StandbyAdvertWidget.cpp

HEADERS  += wndimage.h \
    StandbyAdvertWidget.h

FORMS    += wndimage.ui \
    StandbyAdvertWidget.ui

RESOURCES += \
    images.qrc
