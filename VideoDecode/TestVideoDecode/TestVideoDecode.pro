#-------------------------------------------------
#
# Project created by QtCreator 2014-04-30T10:41:19
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TestVideoDecode
TEMPLATE = app

LIBS += -L"$$_PRO_FILE_PWD_/../video_decode_lib/"
LIBS += -lVideoConvert

SOURCES += main.cpp\
           mainwindow.cpp \

HEADERS  += mainwindow.h \
                 ../video_decode_lib/video_decode_api.h \

FORMS    += mainwindow.ui
