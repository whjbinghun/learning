#-------------------------------------------------
#
# Project created by QtCreator 2014-04-18T14:13:49
#
#-------------------------------------------------

QT       += core gui
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TcpServer
TEMPLATE = app


SOURCES += main.cpp\
        tcpserver.cpp

HEADERS  += tcpserver.h

FORMS    += tcpserver.ui
