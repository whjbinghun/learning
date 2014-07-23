#-------------------------------------------------
#
# Project created by QtCreator 2014-05-15T09:12:44
#
#-------------------------------------------------

QT       += core gui xml network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = VodPlayInterface
TEMPLATE = app

LIBS += -L"$$_PRO_FILE_PWD_/../video_decode_lib/"
LIBS += -lVideoDecode

SOURCES += main.cpp\
        vodplayinterface.cpp \
    titlebar.cpp \
    netcontrol.cpp \
    netcapture.cpp \
    netcaptureir.cpp \
    log-handler.cpp \
    wndpreview.cpp \
    wndconfig.cpp \
    netcapturegroup.cpp\
    picpreview.cpp \
    picpreviewpan.cpp \
    picpreviewgroup.cpp \
    dlgconfigprs.cpp \
    wndvideoplayback.cpp \
    wndirdisplay.cpp \
    wndvidisplay.cpp \
    threadplayback.cpp

HEADERS  += vodplayinterface.h \
    titlebar.h \
    netcontrol.h \
    netcapture.h \
    netcaptureir.h \
    ../include/video_decode_api.h \
    log-handler.h \
    wndpreview.h \
    wndconfig.h \
    netcapturegroup.h \
    picpreview.h \
    picpreviewpan.h \
    picpreviewgroup.h \
    wndcommon.h \
    dlgconfigprs.h \
    wndvideoplayback.h \
    wndirdisplay.h \
    wndvidisplay.h \
    threadplayback.h

FORMS    += vodplayinterface.ui \
    DlgConfigProgress.ui

INCLUDEPATH += ../include

