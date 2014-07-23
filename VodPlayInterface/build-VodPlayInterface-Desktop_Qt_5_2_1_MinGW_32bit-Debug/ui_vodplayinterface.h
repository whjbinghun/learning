/********************************************************************************
** Form generated from reading UI file 'vodplayinterface.ui'
**
** Created by: Qt User Interface Compiler version 5.2.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VODPLAYINTERFACE_H
#define UI_VODPLAYINTERFACE_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDateEdit>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTimeEdit>
#include <QtWidgets/QWidget>
#include <picpreview.h>
#include <picpreviewpan.h>
#include <wndirdisplay.h>
#include <wndpreview.h>
#include <wndvideoplayback.h>
#include <wndvidisplay.h>
#include "picpreviewgroup.h"
#include "wndconfig.h"

QT_BEGIN_NAMESPACE

class Ui_VodPlayInterface
{
public:
    QWidget *centralWidget;
    QStackedWidget *stackedWidget;
    QWidget *id_monitor_interface;
    WndPreview *id_monitor_frame_2;
    QFrame *id_monitor_frame_4;
    QFrame *id_monitor_frame_6;
    QFrame *id_monitor_frame_5;
    QFrame *id_monitor_frame_3;
    QFrame *id_monitor_frame_7;
    QFrame *id_monitor_frame_8;
    WndPreview *id_monitor_frame_1;
    WndConfig *id_config_interface;
    QLabel *id_config_label_ir;
    QLabel *id_config_label_vi;
    QLabel *id_config_label_pan;
    PicPreview *id_config_frame_ir;
    PicPreview *id_config_frame_vi;
    PicPreviewPan *id_config_frame_pan;
    PicPreviewGroup *id_config_frame_group;
    QPushButton *id_config_save;
    QPushButton *id_config_change;
    QLabel *id_config_label_move;
    QPushButton *id_config_btn_save;
    QWidget *id_analyse_interface;
    WndIRDisplay *id_analyse_frame_ir;
    WndVIDisplay *id_analyse_frame_vi;
    QTabWidget *tabWidget;
    QWidget *tab;
    QWidget *tab_2;
    QGroupBox *id_analyse_group_pan;
    QPushButton *id_analyse_btn_rightup;
    QPushButton *id_analyse_btn_right;
    QPushButton *id_analyse_btn_down;
    QLineEdit *id_analyse_lineedit_preset;
    QPushButton *id_analyse_btn_recall;
    QPushButton *id_analyse_btn_left;
    QPushButton *id_analyse_btn_up;
    QPushButton *id_analyse_btn_rightdown;
    QPushButton *id_analyse_btn_leftup;
    QPushButton *id_analyse_btn_leftdown;
    QPushButton *id_analyse_btn_set;
    QLabel *id_analyse_label_preset;
    QGroupBox *id_analyse_group_connect_status;
    QTableWidget *id_analyse_widget_connect_status;
    WndVideoPlayback *id_back_interface;
    WndIRDisplay *id_back_frame_ir;
    WndVIDisplay *id_back_frame_vi;
    QSlider *id_back_slider_play;
    QPushButton *id_back_btn_start;
    QPushButton *id_back_btn_replay;
    QTabWidget *tabWidget_2;
    QWidget *tab_3;
    QWidget *tab_4;
    QGroupBox *id_back_group_select;
    QTimeEdit *id_back_time_end;
    QDateEdit *id_back_date_start;
    QLabel *id_back_label_endtime;
    QDateEdit *id_back_date_end;
    QLabel *id_back_label_device;
    QLabel *id_back_label_starttime;
    QTimeEdit *id_back_time_start;
    QComboBox *id_back_combobox_device;
    QGroupBox *id_back_group_connect_status;
    QTableWidget *id_back_widget_connect_status;
    QPushButton *id_back_btn_stop;
    QPushButton *id_back_btn_first_frame;
    QPushButton *id_back_btn_pre_frame;
    QPushButton *id_back_btn_next_frame;
    QPushButton *id_back_btn_last_frame;
    QPushButton *id_back_btn_play_speed;

    void setupUi(QMainWindow *VodPlayInterface)
    {
        if (VodPlayInterface->objectName().isEmpty())
            VodPlayInterface->setObjectName(QStringLiteral("VodPlayInterface"));
        VodPlayInterface->resize(1122, 761);
        VodPlayInterface->setMouseTracking(true);
        centralWidget = new QWidget(VodPlayInterface);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        centralWidget->setMouseTracking(true);
        stackedWidget = new QStackedWidget(centralWidget);
        stackedWidget->setObjectName(QStringLiteral("stackedWidget"));
        stackedWidget->setGeometry(QRect(30, 20, 1071, 751));
        id_monitor_interface = new QWidget();
        id_monitor_interface->setObjectName(QStringLiteral("id_monitor_interface"));
        id_monitor_interface->setMouseTracking(true);
        id_monitor_frame_2 = new WndPreview(id_monitor_interface);
        id_monitor_frame_2->setObjectName(QStringLiteral("id_monitor_frame_2"));
        id_monitor_frame_2->setGeometry(QRect(510, 10, 501, 161));
        id_monitor_frame_2->setMouseTracking(true);
        id_monitor_frame_2->setFrameShape(QFrame::Box);
        id_monitor_frame_2->setFrameShadow(QFrame::Raised);
        id_monitor_frame_4 = new QFrame(id_monitor_interface);
        id_monitor_frame_4->setObjectName(QStringLiteral("id_monitor_frame_4"));
        id_monitor_frame_4->setGeometry(QRect(510, 170, 501, 181));
        id_monitor_frame_4->setMouseTracking(true);
        id_monitor_frame_4->setFrameShape(QFrame::Box);
        id_monitor_frame_4->setFrameShadow(QFrame::Raised);
        id_monitor_frame_6 = new QFrame(id_monitor_interface);
        id_monitor_frame_6->setObjectName(QStringLiteral("id_monitor_frame_6"));
        id_monitor_frame_6->setGeometry(QRect(510, 350, 501, 161));
        id_monitor_frame_6->setMouseTracking(true);
        id_monitor_frame_6->setFrameShape(QFrame::Box);
        id_monitor_frame_6->setFrameShadow(QFrame::Raised);
        id_monitor_frame_5 = new QFrame(id_monitor_interface);
        id_monitor_frame_5->setObjectName(QStringLiteral("id_monitor_frame_5"));
        id_monitor_frame_5->setGeometry(QRect(10, 350, 501, 161));
        id_monitor_frame_5->setMouseTracking(true);
        id_monitor_frame_5->setFrameShape(QFrame::Box);
        id_monitor_frame_5->setFrameShadow(QFrame::Raised);
        id_monitor_frame_3 = new QFrame(id_monitor_interface);
        id_monitor_frame_3->setObjectName(QStringLiteral("id_monitor_frame_3"));
        id_monitor_frame_3->setGeometry(QRect(10, 170, 501, 181));
        id_monitor_frame_3->setMouseTracking(true);
        id_monitor_frame_3->setFrameShape(QFrame::Box);
        id_monitor_frame_3->setFrameShadow(QFrame::Raised);
        id_monitor_frame_7 = new QFrame(id_monitor_interface);
        id_monitor_frame_7->setObjectName(QStringLiteral("id_monitor_frame_7"));
        id_monitor_frame_7->setGeometry(QRect(10, 510, 501, 161));
        id_monitor_frame_7->setMouseTracking(true);
        id_monitor_frame_7->setFrameShape(QFrame::Box);
        id_monitor_frame_7->setFrameShadow(QFrame::Raised);
        id_monitor_frame_8 = new QFrame(id_monitor_interface);
        id_monitor_frame_8->setObjectName(QStringLiteral("id_monitor_frame_8"));
        id_monitor_frame_8->setGeometry(QRect(510, 510, 501, 161));
        id_monitor_frame_8->setMouseTracking(true);
        id_monitor_frame_8->setFrameShape(QFrame::Box);
        id_monitor_frame_8->setFrameShadow(QFrame::Raised);
        id_monitor_frame_1 = new WndPreview(id_monitor_interface);
        id_monitor_frame_1->setObjectName(QStringLiteral("id_monitor_frame_1"));
        id_monitor_frame_1->setGeometry(QRect(10, 10, 501, 161));
        id_monitor_frame_1->setMouseTracking(true);
        id_monitor_frame_1->setFrameShape(QFrame::Box);
        id_monitor_frame_1->setFrameShadow(QFrame::Raised);
        stackedWidget->addWidget(id_monitor_interface);
        id_config_interface = new WndConfig();
        id_config_interface->setObjectName(QStringLiteral("id_config_interface"));
        id_config_interface->setMouseTracking(true);
        id_config_label_ir = new QLabel(id_config_interface);
        id_config_label_ir->setObjectName(QStringLiteral("id_config_label_ir"));
        id_config_label_ir->setGeometry(QRect(320, 0, 54, 12));
        id_config_label_vi = new QLabel(id_config_interface);
        id_config_label_vi->setObjectName(QStringLiteral("id_config_label_vi"));
        id_config_label_vi->setGeometry(QRect(320, 250, 61, 16));
        id_config_label_pan = new QLabel(id_config_interface);
        id_config_label_pan->setObjectName(QStringLiteral("id_config_label_pan"));
        id_config_label_pan->setGeometry(QRect(320, 490, 54, 12));
        id_config_frame_ir = new PicPreview(id_config_interface);
        id_config_frame_ir->setObjectName(QStringLiteral("id_config_frame_ir"));
        id_config_frame_ir->setGeometry(QRect(320, 20, 721, 221));
        id_config_frame_vi = new PicPreview(id_config_interface);
        id_config_frame_vi->setObjectName(QStringLiteral("id_config_frame_vi"));
        id_config_frame_vi->setGeometry(QRect(320, 260, 721, 221));
        id_config_frame_pan = new PicPreviewPan(id_config_interface);
        id_config_frame_pan->setObjectName(QStringLiteral("id_config_frame_pan"));
        id_config_frame_pan->setGeometry(QRect(320, 510, 721, 221));
        id_config_frame_group = new PicPreviewGroup(id_config_interface);
        id_config_frame_group->setObjectName(QStringLiteral("id_config_frame_group"));
        id_config_frame_group->setGeometry(QRect(10, 20, 291, 671));
        id_config_save = new QPushButton(id_config_frame_group);
        id_config_save->setObjectName(QStringLiteral("id_config_save"));
        id_config_save->setGeometry(QRect(0, 370, 290, 23));
        id_config_change = new QPushButton(id_config_frame_group);
        id_config_change->setObjectName(QStringLiteral("id_config_change"));
        id_config_change->setGeometry(QRect(0, 400, 290, 23));
        id_config_label_move = new QLabel(id_config_interface);
        id_config_label_move->setObjectName(QStringLiteral("id_config_label_move"));
        id_config_label_move->setGeometry(QRect(240, -10, 91, 51));
        id_config_label_move->setAcceptDrops(false);
        id_config_btn_save = new QPushButton(id_config_interface);
        id_config_btn_save->setObjectName(QStringLiteral("id_config_btn_save"));
        id_config_btn_save->setGeometry(QRect(10, 700, 291, 31));
        stackedWidget->addWidget(id_config_interface);
        id_analyse_interface = new QWidget();
        id_analyse_interface->setObjectName(QStringLiteral("id_analyse_interface"));
        id_analyse_interface->setMouseTracking(true);
        id_analyse_frame_ir = new WndIRDisplay(id_analyse_interface);
        id_analyse_frame_ir->setObjectName(QStringLiteral("id_analyse_frame_ir"));
        id_analyse_frame_ir->setGeometry(QRect(280, 50, 411, 281));
        id_analyse_frame_ir->setFrameShape(QFrame::StyledPanel);
        id_analyse_frame_ir->setFrameShadow(QFrame::Raised);
        id_analyse_frame_vi = new WndVIDisplay(id_analyse_interface);
        id_analyse_frame_vi->setObjectName(QStringLiteral("id_analyse_frame_vi"));
        id_analyse_frame_vi->setGeometry(QRect(700, 50, 361, 281));
        id_analyse_frame_vi->setFrameShape(QFrame::StyledPanel);
        id_analyse_frame_vi->setFrameShadow(QFrame::Raised);
        tabWidget = new QTabWidget(id_analyse_interface);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        tabWidget->setGeometry(QRect(280, 350, 781, 381));
        tabWidget->setTabPosition(QTabWidget::South);
        tabWidget->setTabShape(QTabWidget::Rounded);
        tabWidget->setElideMode(Qt::ElideRight);
        tabWidget->setDocumentMode(false);
        tabWidget->setTabsClosable(false);
        tabWidget->setMovable(false);
        tab = new QWidget();
        tab->setObjectName(QStringLiteral("tab"));
        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QStringLiteral("tab_2"));
        tabWidget->addTab(tab_2, QString());
        id_analyse_group_pan = new QGroupBox(id_analyse_interface);
        id_analyse_group_pan->setObjectName(QStringLiteral("id_analyse_group_pan"));
        id_analyse_group_pan->setGeometry(QRect(10, 50, 240, 240));
        id_analyse_btn_rightup = new QPushButton(id_analyse_group_pan);
        id_analyse_btn_rightup->setObjectName(QStringLiteral("id_analyse_btn_rightup"));
        id_analyse_btn_rightup->setGeometry(QRect(160, 20, 61, 34));
        id_analyse_btn_right = new QPushButton(id_analyse_group_pan);
        id_analyse_btn_right->setObjectName(QStringLiteral("id_analyse_btn_right"));
        id_analyse_btn_right->setGeometry(QRect(160, 64, 61, 34));
        id_analyse_btn_down = new QPushButton(id_analyse_group_pan);
        id_analyse_btn_down->setObjectName(QStringLiteral("id_analyse_btn_down"));
        id_analyse_btn_down->setGeometry(QRect(90, 108, 61, 34));
        id_analyse_lineedit_preset = new QLineEdit(id_analyse_group_pan);
        id_analyse_lineedit_preset->setObjectName(QStringLiteral("id_analyse_lineedit_preset"));
        id_analyse_lineedit_preset->setGeometry(QRect(130, 152, 90, 30));
        id_analyse_btn_recall = new QPushButton(id_analyse_group_pan);
        id_analyse_btn_recall->setObjectName(QStringLiteral("id_analyse_btn_recall"));
        id_analyse_btn_recall->setGeometry(QRect(130, 196, 90, 34));
        id_analyse_btn_left = new QPushButton(id_analyse_group_pan);
        id_analyse_btn_left->setObjectName(QStringLiteral("id_analyse_btn_left"));
        id_analyse_btn_left->setGeometry(QRect(20, 64, 61, 34));
        id_analyse_btn_up = new QPushButton(id_analyse_group_pan);
        id_analyse_btn_up->setObjectName(QStringLiteral("id_analyse_btn_up"));
        id_analyse_btn_up->setGeometry(QRect(90, 20, 61, 34));
        id_analyse_btn_rightdown = new QPushButton(id_analyse_group_pan);
        id_analyse_btn_rightdown->setObjectName(QStringLiteral("id_analyse_btn_rightdown"));
        id_analyse_btn_rightdown->setGeometry(QRect(160, 108, 61, 34));
        id_analyse_btn_leftup = new QPushButton(id_analyse_group_pan);
        id_analyse_btn_leftup->setObjectName(QStringLiteral("id_analyse_btn_leftup"));
        id_analyse_btn_leftup->setGeometry(QRect(20, 20, 61, 34));
        id_analyse_btn_leftdown = new QPushButton(id_analyse_group_pan);
        id_analyse_btn_leftdown->setObjectName(QStringLiteral("id_analyse_btn_leftdown"));
        id_analyse_btn_leftdown->setGeometry(QRect(20, 108, 61, 34));
        id_analyse_btn_set = new QPushButton(id_analyse_group_pan);
        id_analyse_btn_set->setObjectName(QStringLiteral("id_analyse_btn_set"));
        id_analyse_btn_set->setGeometry(QRect(20, 196, 90, 34));
        id_analyse_label_preset = new QLabel(id_analyse_group_pan);
        id_analyse_label_preset->setObjectName(QStringLiteral("id_analyse_label_preset"));
        id_analyse_label_preset->setGeometry(QRect(20, 150, 90, 34));
        QFont font;
        font.setPointSize(10);
        id_analyse_label_preset->setFont(font);
        id_analyse_group_connect_status = new QGroupBox(id_analyse_interface);
        id_analyse_group_connect_status->setObjectName(QStringLiteral("id_analyse_group_connect_status"));
        id_analyse_group_connect_status->setGeometry(QRect(10, 300, 240, 431));
        id_analyse_widget_connect_status = new QTableWidget(id_analyse_group_connect_status);
        if (id_analyse_widget_connect_status->columnCount() < 2)
            id_analyse_widget_connect_status->setColumnCount(2);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        id_analyse_widget_connect_status->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        id_analyse_widget_connect_status->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        id_analyse_widget_connect_status->setObjectName(QStringLiteral("id_analyse_widget_connect_status"));
        id_analyse_widget_connect_status->setGeometry(QRect(10, 20, 220, 401));
        stackedWidget->addWidget(id_analyse_interface);
        id_back_interface = new WndVideoPlayback();
        id_back_interface->setObjectName(QStringLiteral("id_back_interface"));
        id_back_interface->setMouseTracking(true);
        id_back_frame_ir = new WndIRDisplay(id_back_interface);
        id_back_frame_ir->setObjectName(QStringLiteral("id_back_frame_ir"));
        id_back_frame_ir->setGeometry(QRect(250, 30, 391, 261));
        id_back_frame_ir->setFrameShape(QFrame::StyledPanel);
        id_back_frame_ir->setFrameShadow(QFrame::Raised);
        id_back_frame_vi = new WndVIDisplay(id_back_interface);
        id_back_frame_vi->setObjectName(QStringLiteral("id_back_frame_vi"));
        id_back_frame_vi->setGeometry(QRect(650, 30, 391, 261));
        id_back_frame_vi->setFrameShape(QFrame::StyledPanel);
        id_back_frame_vi->setFrameShadow(QFrame::Raised);
        id_back_slider_play = new QSlider(id_back_interface);
        id_back_slider_play->setObjectName(QStringLiteral("id_back_slider_play"));
        id_back_slider_play->setGeometry(QRect(390, 290, 561, 20));
        id_back_slider_play->setOrientation(Qt::Horizontal);
        id_back_btn_start = new QPushButton(id_back_interface);
        id_back_btn_start->setObjectName(QStringLiteral("id_back_btn_start"));
        id_back_btn_start->setGeometry(QRect(250, 290, 21, 20));
        id_back_btn_replay = new QPushButton(id_back_interface);
        id_back_btn_replay->setObjectName(QStringLiteral("id_back_btn_replay"));
        id_back_btn_replay->setGeometry(QRect(959, 290, 81, 20));
        tabWidget_2 = new QTabWidget(id_back_interface);
        tabWidget_2->setObjectName(QStringLiteral("tabWidget_2"));
        tabWidget_2->setGeometry(QRect(250, 330, 791, 341));
        tabWidget_2->setTabPosition(QTabWidget::South);
        tab_3 = new QWidget();
        tab_3->setObjectName(QStringLiteral("tab_3"));
        tabWidget_2->addTab(tab_3, QString());
        tab_4 = new QWidget();
        tab_4->setObjectName(QStringLiteral("tab_4"));
        tabWidget_2->addTab(tab_4, QString());
        id_back_group_select = new QGroupBox(id_back_interface);
        id_back_group_select->setObjectName(QStringLiteral("id_back_group_select"));
        id_back_group_select->setGeometry(QRect(10, 30, 240, 240));
        id_back_time_end = new QTimeEdit(id_back_group_select);
        id_back_time_end->setObjectName(QStringLiteral("id_back_time_end"));
        id_back_time_end->setGeometry(QRect(90, 190, 130, 30));
        id_back_date_start = new QDateEdit(id_back_group_select);
        id_back_date_start->setObjectName(QStringLiteral("id_back_date_start"));
        id_back_date_start->setGeometry(QRect(90, 70, 130, 30));
        id_back_label_endtime = new QLabel(id_back_group_select);
        id_back_label_endtime->setObjectName(QStringLiteral("id_back_label_endtime"));
        id_back_label_endtime->setGeometry(QRect(20, 150, 60, 30));
        id_back_label_endtime->setFont(font);
        id_back_date_end = new QDateEdit(id_back_group_select);
        id_back_date_end->setObjectName(QStringLiteral("id_back_date_end"));
        id_back_date_end->setGeometry(QRect(90, 150, 130, 30));
        id_back_label_device = new QLabel(id_back_group_select);
        id_back_label_device->setObjectName(QStringLiteral("id_back_label_device"));
        id_back_label_device->setGeometry(QRect(20, 30, 60, 30));
        id_back_label_device->setFont(font);
        id_back_label_starttime = new QLabel(id_back_group_select);
        id_back_label_starttime->setObjectName(QStringLiteral("id_back_label_starttime"));
        id_back_label_starttime->setGeometry(QRect(20, 70, 60, 30));
        id_back_label_starttime->setFont(font);
        id_back_time_start = new QTimeEdit(id_back_group_select);
        id_back_time_start->setObjectName(QStringLiteral("id_back_time_start"));
        id_back_time_start->setGeometry(QRect(90, 109, 130, 30));
        id_back_combobox_device = new QComboBox(id_back_group_select);
        id_back_combobox_device->setObjectName(QStringLiteral("id_back_combobox_device"));
        id_back_combobox_device->setGeometry(QRect(90, 30, 130, 30));
        id_back_group_connect_status = new QGroupBox(id_back_interface);
        id_back_group_connect_status->setObjectName(QStringLiteral("id_back_group_connect_status"));
        id_back_group_connect_status->setGeometry(QRect(0, 280, 240, 391));
        id_back_widget_connect_status = new QTableWidget(id_back_group_connect_status);
        if (id_back_widget_connect_status->columnCount() < 2)
            id_back_widget_connect_status->setColumnCount(2);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        id_back_widget_connect_status->setHorizontalHeaderItem(0, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        id_back_widget_connect_status->setHorizontalHeaderItem(1, __qtablewidgetitem3);
        id_back_widget_connect_status->setObjectName(QStringLiteral("id_back_widget_connect_status"));
        id_back_widget_connect_status->setGeometry(QRect(10, 20, 220, 361));
        id_back_btn_stop = new QPushButton(id_back_interface);
        id_back_btn_stop->setObjectName(QStringLiteral("id_back_btn_stop"));
        id_back_btn_stop->setGeometry(QRect(270, 290, 21, 20));
        id_back_btn_first_frame = new QPushButton(id_back_interface);
        id_back_btn_first_frame->setObjectName(QStringLiteral("id_back_btn_first_frame"));
        id_back_btn_first_frame->setGeometry(QRect(290, 290, 21, 20));
        id_back_btn_pre_frame = new QPushButton(id_back_interface);
        id_back_btn_pre_frame->setObjectName(QStringLiteral("id_back_btn_pre_frame"));
        id_back_btn_pre_frame->setGeometry(QRect(310, 290, 21, 20));
        id_back_btn_next_frame = new QPushButton(id_back_interface);
        id_back_btn_next_frame->setObjectName(QStringLiteral("id_back_btn_next_frame"));
        id_back_btn_next_frame->setGeometry(QRect(330, 290, 21, 20));
        id_back_btn_last_frame = new QPushButton(id_back_interface);
        id_back_btn_last_frame->setObjectName(QStringLiteral("id_back_btn_last_frame"));
        id_back_btn_last_frame->setGeometry(QRect(350, 290, 21, 20));
        id_back_btn_play_speed = new QPushButton(id_back_interface);
        id_back_btn_play_speed->setObjectName(QStringLiteral("id_back_btn_play_speed"));
        id_back_btn_play_speed->setGeometry(QRect(370, 290, 21, 20));
        stackedWidget->addWidget(id_back_interface);
        VodPlayInterface->setCentralWidget(centralWidget);

        retranslateUi(VodPlayInterface);

        stackedWidget->setCurrentIndex(2);
        tabWidget->setCurrentIndex(0);
        tabWidget_2->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(VodPlayInterface);
    } // setupUi

    void retranslateUi(QMainWindow *VodPlayInterface)
    {
        VodPlayInterface->setWindowTitle(QApplication::translate("VodPlayInterface", "VodPlayInterface", 0));
        id_config_label_ir->setText(QApplication::translate("VodPlayInterface", "\347\272\242\345\244\226\350\256\276\345\244\207", 0));
        id_config_label_vi->setText(QApplication::translate("VodPlayInterface", "\345\217\257\350\247\201\345\205\211\350\256\276\345\244\207", 0));
        id_config_label_pan->setText(QApplication::translate("VodPlayInterface", "\344\272\221\345\217\260\350\256\276\345\244\207", 0));
        id_config_save->setText(QApplication::translate("VodPlayInterface", "\344\277\235\345\255\230", 0));
        id_config_change->setText(QApplication::translate("VodPlayInterface", "\344\277\256\346\224\271", 0));
        id_config_label_move->setText(QApplication::translate("VodPlayInterface", "\347\247\273\345\212\250\346\230\276\347\244\272\346\225\210\346\236\234\347\252\227\345\217\243", 0));
        id_config_btn_save->setText(QApplication::translate("VodPlayInterface", "\344\277\235\345\255\230", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("VodPlayInterface", "Tab 1", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("VodPlayInterface", "Tab 2", 0));
        id_analyse_group_pan->setTitle(QApplication::translate("VodPlayInterface", "\344\272\221\345\217\260\346\216\247\345\210\266", 0));
        id_analyse_btn_rightup->setText(QApplication::translate("VodPlayInterface", "\345\217\263\344\270\212", 0));
        id_analyse_btn_right->setText(QApplication::translate("VodPlayInterface", "\345\217\263", 0));
        id_analyse_btn_down->setText(QApplication::translate("VodPlayInterface", "\344\270\213", 0));
        id_analyse_lineedit_preset->setText(QApplication::translate("VodPlayInterface", "1", 0));
        id_analyse_btn_recall->setText(QApplication::translate("VodPlayInterface", "\346\213\233\345\233\236", 0));
        id_analyse_btn_left->setText(QApplication::translate("VodPlayInterface", "\345\267\246", 0));
        id_analyse_btn_up->setText(QApplication::translate("VodPlayInterface", "\344\270\212", 0));
        id_analyse_btn_rightdown->setText(QApplication::translate("VodPlayInterface", "\345\217\263\344\270\213", 0));
        id_analyse_btn_leftup->setText(QApplication::translate("VodPlayInterface", "\345\267\246\344\270\212", 0));
        id_analyse_btn_leftdown->setText(QApplication::translate("VodPlayInterface", "\345\267\246\344\270\213", 0));
        id_analyse_btn_set->setText(QApplication::translate("VodPlayInterface", "\350\256\276\347\275\256", 0));
        id_analyse_label_preset->setText(QApplication::translate("VodPlayInterface", "\351\242\204\347\275\256\347\202\271\357\274\232", 0));
        id_analyse_group_connect_status->setTitle(QApplication::translate("VodPlayInterface", "\350\277\236\346\216\245\347\212\266\346\200\201", 0));
        QTableWidgetItem *___qtablewidgetitem = id_analyse_widget_connect_status->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("VodPlayInterface", "\346\227\266\351\227\264", 0));
        QTableWidgetItem *___qtablewidgetitem1 = id_analyse_widget_connect_status->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("VodPlayInterface", "\344\277\241\346\201\257\345\206\205\345\256\271", 0));
        id_back_btn_start->setText(QApplication::translate("VodPlayInterface", "\342\226\266", 0));
        id_back_btn_replay->setText(QApplication::translate("VodPlayInterface", "\345\210\267\346\226\260", 0));
        tabWidget_2->setTabText(tabWidget_2->indexOf(tab_3), QApplication::translate("VodPlayInterface", "Tab 1", 0));
        tabWidget_2->setTabText(tabWidget_2->indexOf(tab_4), QApplication::translate("VodPlayInterface", "Tab 2", 0));
        id_back_group_select->setTitle(QApplication::translate("VodPlayInterface", "\351\200\211\346\213\251", 0));
        id_back_label_endtime->setText(QApplication::translate("VodPlayInterface", "\347\273\223\346\235\237\346\227\266\351\227\264\357\274\232", 0));
        id_back_label_device->setText(QApplication::translate("VodPlayInterface", "\350\256\276\345\244\207\347\202\271\357\274\232", 0));
        id_back_label_starttime->setText(QApplication::translate("VodPlayInterface", "\345\274\200\345\247\213\346\227\266\351\227\264\357\274\232", 0));
        id_back_group_connect_status->setTitle(QApplication::translate("VodPlayInterface", "\350\277\236\346\216\245\347\212\266\346\200\201", 0));
        QTableWidgetItem *___qtablewidgetitem2 = id_back_widget_connect_status->horizontalHeaderItem(0);
        ___qtablewidgetitem2->setText(QApplication::translate("VodPlayInterface", "\346\227\266\351\227\264", 0));
        QTableWidgetItem *___qtablewidgetitem3 = id_back_widget_connect_status->horizontalHeaderItem(1);
        ___qtablewidgetitem3->setText(QApplication::translate("VodPlayInterface", "\344\277\241\346\201\257\345\206\205\345\256\271", 0));
        id_back_btn_stop->setText(QApplication::translate("VodPlayInterface", "ST", 0));
        id_back_btn_first_frame->setText(QApplication::translate("VodPlayInterface", "|<", 0));
        id_back_btn_pre_frame->setText(QApplication::translate("VodPlayInterface", "<", 0));
        id_back_btn_next_frame->setText(QApplication::translate("VodPlayInterface", ">", 0));
        id_back_btn_last_frame->setText(QApplication::translate("VodPlayInterface", ">|", 0));
        id_back_btn_play_speed->setText(QApplication::translate("VodPlayInterface", ">>", 0));
    } // retranslateUi

};

namespace Ui {
    class VodPlayInterface: public Ui_VodPlayInterface {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VODPLAYINTERFACE_H
