/********************************************************************************
** Form generated from reading UI file 'DlgConfigProgress.ui'
**
** Created by: Qt User Interface Compiler version 5.2.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DLGCONFIGPROGRESS_H
#define UI_DLGCONFIGPROGRESS_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QProgressBar>

QT_BEGIN_NAMESPACE

class Ui_DlgConfigProgress
{
public:
    QProgressBar *id_progress_bar;
    QLabel *id_label_title;

    void setupUi(QDialog *DlgConfigProgress)
    {
        if (DlgConfigProgress->objectName().isEmpty())
            DlgConfigProgress->setObjectName(QStringLiteral("DlgConfigProgress"));
        DlgConfigProgress->resize(391, 62);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(DlgConfigProgress->sizePolicy().hasHeightForWidth());
        DlgConfigProgress->setSizePolicy(sizePolicy);
        DlgConfigProgress->setMinimumSize(QSize(391, 62));
        DlgConfigProgress->setMaximumSize(QSize(391, 62));
        id_progress_bar = new QProgressBar(DlgConfigProgress);
        id_progress_bar->setObjectName(QStringLiteral("id_progress_bar"));
        id_progress_bar->setGeometry(QRect(10, 30, 371, 23));
        id_progress_bar->setValue(24);
        id_label_title = new QLabel(DlgConfigProgress);
        id_label_title->setObjectName(QStringLiteral("id_label_title"));
        id_label_title->setGeometry(QRect(10, 10, 361, 16));
        QFont font;
        font.setBold(true);
        font.setWeight(75);
        id_label_title->setFont(font);
        id_label_title->setFrameShape(QFrame::NoFrame);

        retranslateUi(DlgConfigProgress);

        QMetaObject::connectSlotsByName(DlgConfigProgress);
    } // setupUi

    void retranslateUi(QDialog *DlgConfigProgress)
    {
        DlgConfigProgress->setWindowTitle(QApplication::translate("DlgConfigProgress", "\346\255\243\345\234\250\344\277\235\345\255\230", 0));
        id_label_title->setText(QApplication::translate("DlgConfigProgress", "\350\257\264\346\230\216\344\277\241\346\201\257", 0));
    } // retranslateUi

};

namespace Ui {
    class DlgConfigProgress: public Ui_DlgConfigProgress {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DLGCONFIGPROGRESS_H
