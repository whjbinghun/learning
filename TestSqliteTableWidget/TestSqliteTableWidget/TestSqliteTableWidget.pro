#-------------------------------------------------
#
# Project created by QtCreator 2016-06-21T17:40:22
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = wndSqliteTableWidget
TEMPLATE = app


SOURCES += main.cpp\
        wndsqlite.cpp \
    SuSqlite.cpp

HEADERS  += wndsqlite.h \
    SuSqlite.h

FORMS    += wndsqlite.ui
