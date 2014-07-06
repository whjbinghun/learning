/********************************************************************************
** Form generated from reading UI file 'testpixmap.ui'
**
** Created by: Qt User Interface Compiler version 5.2.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TESTPIXMAP_H
#define UI_TESTPIXMAP_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TestPixmap
{
public:
    QWidget *centralWidget;
    QLabel *label;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *TestPixmap)
    {
        if (TestPixmap->objectName().isEmpty())
            TestPixmap->setObjectName(QStringLiteral("TestPixmap"));
        TestPixmap->resize(400, 300);
        centralWidget = new QWidget(TestPixmap);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(110, 90, 121, 110));
        TestPixmap->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(TestPixmap);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 400, 23));
        TestPixmap->setMenuBar(menuBar);
        mainToolBar = new QToolBar(TestPixmap);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        TestPixmap->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(TestPixmap);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        TestPixmap->setStatusBar(statusBar);

        retranslateUi(TestPixmap);

        QMetaObject::connectSlotsByName(TestPixmap);
    } // setupUi

    void retranslateUi(QMainWindow *TestPixmap)
    {
        TestPixmap->setWindowTitle(QApplication::translate("TestPixmap", "TestPixmap", 0));
        label->setText(QApplication::translate("TestPixmap", "TextLabel", 0));
    } // retranslateUi

};

namespace Ui {
    class TestPixmap: public Ui_TestPixmap {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TESTPIXMAP_H
