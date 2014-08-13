#-------------------------------------------------
#
# Project created by QtCreator 2014-07-24T10:40:55
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = LoginInterface
TEMPLATE = app


SOURCES += main.cpp\
        logininterface.cpp \
    logindialog.cpp \
    cfgfileinfo.cpp \
    titlebar.cpp

HEADERS  += logininterface.h \
    logindialog.h \
    cfgfileinfo.h \
    titlebar.h

FORMS    += logininterface.ui

RESOURCES += \
    image.qrc
