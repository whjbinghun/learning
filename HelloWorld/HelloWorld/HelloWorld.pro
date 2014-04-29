#-------------------------------------------------
#
# Project created by QtCreator 2014-03-25T17:03:28
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = HelloWorld
TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++0x


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h \
    newspaper.h \
    reader.h
