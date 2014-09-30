/********************************************************************************
** Form generated from reading UI file 'drawlinecircle.ui'
**
** Created by: Qt User Interface Compiler version 5.2.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DRAWLINECIRCLE_H
#define UI_DRAWLINECIRCLE_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QSlider>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>
#include "irframe.h"
#include "irslider.h"

QT_BEGIN_NAMESPACE

class Ui_DrawLineCircle
{
public:
    QWidget *centralWidget;
    IrFrame *id_ir_frame;
    IrSlider *id_ir_slider;
    QSlider *id_slider;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *DrawLineCircle)
    {
        if (DrawLineCircle->objectName().isEmpty())
            DrawLineCircle->setObjectName(QStringLiteral("DrawLineCircle"));
        DrawLineCircle->resize(979, 637);
        centralWidget = new QWidget(DrawLineCircle);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        id_ir_frame = new IrFrame(centralWidget);
        id_ir_frame->setObjectName(QStringLiteral("id_ir_frame"));
        id_ir_frame->setGeometry(QRect(60, 40, 841, 491));
        id_ir_frame->setFrameShape(QFrame::Box);
        id_ir_frame->setFrameShadow(QFrame::Raised);
        id_ir_slider = new IrSlider(id_ir_frame);
        id_ir_slider->setObjectName(QStringLiteral("id_ir_slider"));
        id_ir_slider->setGeometry(QRect(110, 330, 400, 20));
        id_ir_slider->setMaximum(100);
        id_ir_slider->setSliderPosition(0);
        id_ir_slider->setOrientation(Qt::Horizontal);
        id_slider = new QSlider(id_ir_frame);
        id_slider->setObjectName(QStringLiteral("id_slider"));
        id_slider->setGeometry(QRect(110, 390, 400, 20));
        id_slider->setOrientation(Qt::Horizontal);
        DrawLineCircle->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(DrawLineCircle);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 979, 23));
        DrawLineCircle->setMenuBar(menuBar);
        mainToolBar = new QToolBar(DrawLineCircle);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        DrawLineCircle->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(DrawLineCircle);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        DrawLineCircle->setStatusBar(statusBar);

        retranslateUi(DrawLineCircle);

        QMetaObject::connectSlotsByName(DrawLineCircle);
    } // setupUi

    void retranslateUi(QMainWindow *DrawLineCircle)
    {
        DrawLineCircle->setWindowTitle(QApplication::translate("DrawLineCircle", "DrawLineCircle", 0));
    } // retranslateUi

};

namespace Ui {
    class DrawLineCircle: public Ui_DrawLineCircle {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DRAWLINECIRCLE_H
