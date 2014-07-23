#-------------------------------------------------
#
# Project created by QtCreator 2014-04-11T18:21:17
#
#-------------------------------------------------

QT       += core gui network
QT       += xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Demo2
TEMPLATE = app

LIBS += -L"$$_PRO_FILE_PWD_/../video_decode_lib/"
LIBS += "$$_PRO_FILE_PWD_/../video_decode_lib/VideoDecode.dll"
#LIBS += -lVideoConvert
LIBS += -lVideoDecode

SOURCES += main.cpp\
                    demo.cpp

INCLUDEPATH += ../video_decode_lib

HEADERS  += demo.h \
            ../video_decode_lib/video_decode_api.h

FORMS    += demo.ui

#INCLUDEPATH += ../include
#LIBS += -L"$$_PRO_FILE_PWD_/../av_libs/"
#LIBS += "$$_PRO_FILE_PWD_/../av_libs/avformat.lib"
#LIBS += "$$_PRO_FILE_PWD_/../av_libs/avcodec.lib"
#LIBS += "$$_PRO_FILE_PWD_/../av_libs/avdevice.lib"
#LIBS += "$$_PRO_FILE_PWD_/../av_libs/avfilter.lib"
#LIBS += "$$_PRO_FILE_PWD_/../av_libs/avutil.lib"
#LIBS += "$$_PRO_FILE_PWD_/../av_libs/swresample.lib"
#LIBS += "$$_PRO_FILE_PWD_/../av_libs/swscale.lib"



