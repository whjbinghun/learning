#-------------------------------------------------
#
# Project created by QtCreator 2014-03-14T10:38:16
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TestNetCaptureEx
TEMPLATE = app

OBJECTS_DIR += ../../temp/TestNetCaptureEx #.o

INCLUDEPATH += ../YFNetCaptureEx
INCLUDEPATH += ../YFDrawShape
INCLUDEPATH += ../YFFormatAnaStr

win32{
    LIBS += ..\bin\release\YFIRPlayerEx.lib
    LIBS += ..\bin\release\YFDrawShape.lib
    LIBS += ..\bin\release\YFFormatAnaStr.lib
}

mac{
    INCLUDEPATH += /usr/local/Cellar/boost/1.52.0/include
    LIBS += -L/usr/local/Cellar/boost/1.52.0/lib
    LIBS += -lboost_thread-mt -lboost_system-mt
}

LIBS += -L"$$_PRO_FILE_PWD_/../libs/"
LIBS += -L"$$_PRO_FILE_PWD_/../../../ms-net/distribute/libs/"

unix:LIBS += -ljson -lYFCalcTemp -lYFCodec -lYFFormatAnaStr -lYFFileIdentify -lYFLog -lboost_filesystem
unix:LIBS += -lYFIRImaging -lYFDrawShape -lYFTime -lYFIR -lYFNetCaptureEx -lYFCorrect -lYFADTrend
unix:LIBS += -lms-net

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}

SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui
