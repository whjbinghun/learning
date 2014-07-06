/********************************************************************************
** Form generated from reading UI file 'pictureshow.ui'
**
** Created by: Qt User Interface Compiler version 5.2.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PICTURESHOW_H
#define UI_PICTURESHOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>
#include <picpreview.h>
#include "picpreconfig.h"

QT_BEGIN_NAMESPACE

class Ui_PictureShow
{
public:
    QWidget *centralWidget;
    PicPreConfig *id_widget;
    PicPreview *id_frame;
    QFrame *id_move_frame;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *PictureShow)
    {
        if (PictureShow->objectName().isEmpty())
            PictureShow->setObjectName(QStringLiteral("PictureShow"));
        PictureShow->resize(848, 547);
        centralWidget = new QWidget(PictureShow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        id_widget = new PicPreConfig(centralWidget);
        id_widget->setObjectName(QStringLiteral("id_widget"));
        id_widget->setGeometry(QRect(10, 10, 801, 461));
        id_frame = new PicPreview(id_widget);
        id_frame->setObjectName(QStringLiteral("id_frame"));
        id_frame->setGeometry(QRect(220, 290, 561, 161));
        id_move_frame = new QFrame(id_widget);
        id_move_frame->setObjectName(QStringLiteral("id_move_frame"));
        id_move_frame->setGeometry(QRect(50, 40, 221, 161));
        id_move_frame->setFrameShape(QFrame::StyledPanel);
        id_move_frame->setFrameShadow(QFrame::Raised);
        PictureShow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(PictureShow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 848, 23));
        PictureShow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(PictureShow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        PictureShow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(PictureShow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        PictureShow->setStatusBar(statusBar);

        retranslateUi(PictureShow);

        QMetaObject::connectSlotsByName(PictureShow);
    } // setupUi

    void retranslateUi(QMainWindow *PictureShow)
    {
        PictureShow->setWindowTitle(QApplication::translate("PictureShow", "PictureShow", 0));
    } // retranslateUi

};

namespace Ui {
    class PictureShow: public Ui_PictureShow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PICTURESHOW_H
