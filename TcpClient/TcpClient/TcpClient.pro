#-------------------------------------------------
#
# Project created by QtCreator 2014-04-16T17:56:56
#
#-------------------------------------------------

QT       += core gui network
QT       += xml


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TcpClient
TEMPLATE = app


SOURCES += main.cpp\
        tcpclient.cpp \

HEADERS  += tcpclient.h \

FORMS    += tcpclient.ui \
    landdialog.ui
