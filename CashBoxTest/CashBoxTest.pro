#-------------------------------------------------
#
# Project created by QtCreator 2016-12-05T16:10:01
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CashBoxTest
TEMPLATE = app

LIBS += -L"$$_PRO_FILE_PWD_/DLL_utility/"
LIBS += -lpos_ad_dll
LIBS += -lPOSDLL
LIBS += -lWinDrawer

SOURCES += main.cpp\
        CashBoxTest.cpp

HEADERS  += CashBoxTest.h

FORMS    += CashBoxTest.ui
