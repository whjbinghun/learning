/********************************************************************************
** Form generated from reading UI file 'viframergb.ui'
**
** Created by: Qt User Interface Compiler version 5.2.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VIFRAMERGB_H
#define UI_VIFRAMERGB_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ViFrameRGB
{
public:
    QWidget *centralWidget;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *ViFrameRGB)
    {
        if (ViFrameRGB->objectName().isEmpty())
            ViFrameRGB->setObjectName(QStringLiteral("ViFrameRGB"));
        ViFrameRGB->resize(400, 300);
        centralWidget = new QWidget(ViFrameRGB);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        ViFrameRGB->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(ViFrameRGB);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 400, 20));
        ViFrameRGB->setMenuBar(menuBar);
        mainToolBar = new QToolBar(ViFrameRGB);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        ViFrameRGB->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(ViFrameRGB);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        ViFrameRGB->setStatusBar(statusBar);

        retranslateUi(ViFrameRGB);

        QMetaObject::connectSlotsByName(ViFrameRGB);
    } // setupUi

    void retranslateUi(QMainWindow *ViFrameRGB)
    {
        ViFrameRGB->setWindowTitle(QApplication::translate("ViFrameRGB", "ViFrameRGB", 0));
    } // retranslateUi

};

namespace Ui {
    class ViFrameRGB: public Ui_ViFrameRGB {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VIFRAMERGB_H
