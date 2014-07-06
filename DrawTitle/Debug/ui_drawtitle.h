/********************************************************************************
** Form generated from reading UI file 'drawtitle.ui'
**
** Created by: Qt User Interface Compiler version 5.2.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DRAWTITLE_H
#define UI_DRAWTITLE_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_DrawTitle
{
public:
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *DrawTitle)
    {
        if (DrawTitle->objectName().isEmpty())
            DrawTitle->setObjectName(QStringLiteral("DrawTitle"));
        DrawTitle->setWindowModality(Qt::NonModal);
        DrawTitle->resize(400, 300);
        centralWidget = new QWidget(DrawTitle);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        DrawTitle->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(DrawTitle);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        DrawTitle->setStatusBar(statusBar);

        retranslateUi(DrawTitle);

        QMetaObject::connectSlotsByName(DrawTitle);
    } // setupUi

    void retranslateUi(QMainWindow *DrawTitle)
    {
        DrawTitle->setWindowTitle(QApplication::translate("DrawTitle", "DrawTitle", 0));
    } // retranslateUi

};

namespace Ui {
    class DrawTitle: public Ui_DrawTitle {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DRAWTITLE_H
