#-------------------------------------------------
#
# Project created by QtCreator 2016-11-09T10:12:36
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CategoryWidget
TEMPLATE = app


SOURCES += main.cpp\
        CategoryWidget.cpp \
    CategoryListView.cpp

HEADERS  += CategoryWidget.h \
    CategoryListView.h \
    structData.h \
    enumData.h

FORMS    += CategoryWidget.ui

RESOURCES += \
    image.qrc
