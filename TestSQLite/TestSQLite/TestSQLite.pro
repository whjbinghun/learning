#-------------------------------------------------
#
# Project created by QtCreator 2016-06-20T11:42:09
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TestSQLite
TEMPLATE = app


SOURCES += main.cpp\
        testsqlite.cpp \
    SuSqlite.cpp

HEADERS  += testsqlite.h \
    SuSqlite.h

FORMS    += testsqlite.ui
