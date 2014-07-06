/********************************************************************************
** Form generated from reading UI file 'drawtitlebar.ui'
**
** Created by: Qt User Interface Compiler version 5.2.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DRAWTITLEBAR_H
#define UI_DRAWTITLEBAR_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_DrawTitleBar
{
public:
    QWidget *centralWidget;

    void setupUi(QMainWindow *DrawTitleBar)
    {
        if (DrawTitleBar->objectName().isEmpty())
            DrawTitleBar->setObjectName(QStringLiteral("DrawTitleBar"));
        DrawTitleBar->resize(850, 600);
        DrawTitleBar->setMinimumSize(QSize(850, 600));
        centralWidget = new QWidget(DrawTitleBar);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        DrawTitleBar->setCentralWidget(centralWidget);

        retranslateUi(DrawTitleBar);

        QMetaObject::connectSlotsByName(DrawTitleBar);
    } // setupUi

    void retranslateUi(QMainWindow *DrawTitleBar)
    {
        DrawTitleBar->setWindowTitle(QApplication::translate("DrawTitleBar", "DrawTitleBar", 0));
    } // retranslateUi

};

namespace Ui {
    class DrawTitleBar: public Ui_DrawTitleBar {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DRAWTITLEBAR_H
