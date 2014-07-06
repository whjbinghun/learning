/********************************************************************************
** Form generated from reading UI file 'rectimage.ui'
**
** Created by: Qt User Interface Compiler version 5.2.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RECTIMAGE_H
#define UI_RECTIMAGE_H

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

QT_BEGIN_NAMESPACE

class Ui_RectImage
{
public:
    QWidget *centralWidget;
    QFrame *id_frame;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *RectImage)
    {
        if (RectImage->objectName().isEmpty())
            RectImage->setObjectName(QStringLiteral("RectImage"));
        RectImage->resize(951, 666);
        centralWidget = new QWidget(RectImage);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        id_frame = new QFrame(centralWidget);
        id_frame->setObjectName(QStringLiteral("id_frame"));
        id_frame->setGeometry(QRect(190, 170, 561, 351));
        id_frame->setFrameShape(QFrame::StyledPanel);
        id_frame->setFrameShadow(QFrame::Raised);
        RectImage->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(RectImage);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 951, 20));
        RectImage->setMenuBar(menuBar);
        mainToolBar = new QToolBar(RectImage);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        RectImage->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(RectImage);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        RectImage->setStatusBar(statusBar);

        retranslateUi(RectImage);

        QMetaObject::connectSlotsByName(RectImage);
    } // setupUi

    void retranslateUi(QMainWindow *RectImage)
    {
        RectImage->setWindowTitle(QApplication::translate("RectImage", "RectImage", 0));
    } // retranslateUi

};

namespace Ui {
    class RectImage: public Ui_RectImage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RECTIMAGE_H
