#-------------------------------------------------
#
# Project created by QtCreator 2013-09-03T14:44:07
#
#-------------------------------------------------

QT       += core gui network multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = myTcpClient
TEMPLATE = app


SOURCES += main.cpp\
        frmmain.cpp

HEADERS  += frmmain.h \
    myhelper.h

FORMS    += frmmain.ui

MOC_DIR=temp/moc
RCC_DIR=temp/rcc
UI_DIR=temp/ui
OBJECTS_DIR=temp/obj
DESTDIR=bin
