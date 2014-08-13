#-------------------------------------------------
#
# Project created by QtCreator 2014-08-13T11:43:59
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DialogTitleBar
TEMPLATE = app


SOURCES += main.cpp\
        dialogtitlebar.cpp \
    titlebar.cpp \
    logindialog.cpp \
    cfgfileinfo.cpp

HEADERS  += dialogtitlebar.h \
    titlebar.h \
    logindialog.h \
    cfgfileinfo.h

FORMS    += dialogtitlebar.ui

RESOURCES += \
    image.qrc
