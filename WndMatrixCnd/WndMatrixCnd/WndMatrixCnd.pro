#-------------------------------------------------
#
# Project created by QtCreator 2015-03-19T09:59:46
#
#-------------------------------------------------

QT       += core gui
QT       += network
QT       += xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = WndMatrixCnd
TEMPLATE = app

DESTDIR = "$$_PRO_FILE_PWD_/../../distribute/win32-mingw/"

LIBS += -L"$$_PRO_FILE_PWD_/public/monitor-client-libs/"
LIBS += -L"$$_PRO_FILE_PWD_/public/video_decode_lib/"
LIBS += -L"$$_PRO_FILE_PWD_/public/ms_preset_temp_analysis/"
LIBS += -L"$$_PRO_FILE_PWD_/public/alarm_judge/"

INCLUDEPATH += .

INCLUDEPATH += "$$_PRO_FILE_PWD_/public/video_decode_lib/"
INCLUDEPATH += "$$_PRO_FILE_PWD_/public/monitor-client-libs/include"
INCLUDEPATH += "$$_PRO_FILE_PWD_/public/monitor-client-libs/include/ms-protocol"
INCLUDEPATH += "$$_PRO_FILE_PWD_/public/monitor-client-libs/include/logiclayer"
INCLUDEPATH += "$$_PRO_FILE_PWD_/public/alarm_judge"

# libs 链接库是需要考虑链接库的顺序

CONFIG(debug, debug|release) {
    TARGET = PresetSettingd
    LIBS += -llogiclayerd
    LIBS += -lmsprotocold
    LIBS += -lmsnetd
    LIBS += -lmslogd
} else {
    TARGET = PresetSetting
    LIBS += -llogiclayer
    LIBS += -lmsprotocol
    LIBS += -lmsnet
    LIBS += -lmslog
}

LIBS += -lVideoDecode
LIBS += -lAlarmJudge


SOURCES += main.cpp\
        wndmatrixcnd.cpp \
    wndmatrixalarmlist.cpp \
    wndcolor.cpp \
    alarmcfg.cpp \
    dlgalarmcondition.cpp \
    wndirpreview.cpp \
    ir_ana_label.cpp \
    deletelabel.cpp \
    ir_matrix_analysis_button.cpp \
    matrixfactory.cpp \
    matrixwidget.cpp \
    mscclienttype.cpp \
    common-ui/dlgalarmconfig.cpp

HEADERS  += wndmatrixcnd.h \
    wndmatrixalarmlist.h \
    wndcolor.h \
    dlgalarmcondition.h \
    alarmcfg.h \
    wndirpreview.h \
    ir_ana_label.h \
    deletelabel.h \
    ir_matrix_analysis_button.h \
    matrixfactory.h \
    matrixwidget.h \
    mscclienttype.h \
    common-ui/dlgalarmconfig.h

FORMS    += wndmatrixcnd.ui \
    dlgalarmcondition.ui \
    matrixwidget.ui \
    common-ui/dlgalarmconfig.ui
