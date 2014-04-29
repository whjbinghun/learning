/********************************************************************************
** Form generated from reading UI file 'landdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.2.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LANDDIALOG_H
#define UI_LANDDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>

QT_BEGIN_NAMESPACE

class Ui_LandDialog
{
public:
    QPushButton *id_land;
    QPushButton *id_cancel;
    QLabel *id_ip;
    QLabel *id_port;
    QLineEdit *id_ip_edit;
    QLineEdit *id_port_edit;
    QTextEdit *id_text_edit;

    void setupUi(QDialog *LandDialog)
    {
        if (LandDialog->objectName().isEmpty())
            LandDialog->setObjectName(QStringLiteral("LandDialog"));
        LandDialog->resize(400, 300);
        id_land = new QPushButton(LandDialog);
        id_land->setObjectName(QStringLiteral("id_land"));
        id_land->setGeometry(QRect(40, 240, 80, 23));
        id_cancel = new QPushButton(LandDialog);
        id_cancel->setObjectName(QStringLiteral("id_cancel"));
        id_cancel->setGeometry(QRect(190, 240, 80, 23));
        id_ip = new QLabel(LandDialog);
        id_ip->setObjectName(QStringLiteral("id_ip"));
        id_ip->setGeometry(QRect(50, 70, 57, 15));
        id_port = new QLabel(LandDialog);
        id_port->setObjectName(QStringLiteral("id_port"));
        id_port->setGeometry(QRect(50, 130, 57, 15));
        id_ip_edit = new QLineEdit(LandDialog);
        id_ip_edit->setObjectName(QStringLiteral("id_ip_edit"));
        id_ip_edit->setGeometry(QRect(120, 60, 171, 23));
        id_port_edit = new QLineEdit(LandDialog);
        id_port_edit->setObjectName(QStringLiteral("id_port_edit"));
        id_port_edit->setGeometry(QRect(120, 120, 171, 23));
        id_text_edit = new QTextEdit(LandDialog);
        id_text_edit->setObjectName(QStringLiteral("id_text_edit"));
        id_text_edit->setGeometry(QRect(40, 160, 311, 70));

        retranslateUi(LandDialog);

        QMetaObject::connectSlotsByName(LandDialog);
    } // setupUi

    void retranslateUi(QDialog *LandDialog)
    {
        LandDialog->setWindowTitle(QApplication::translate("LandDialog", "Dialog", 0));
        id_land->setText(QApplication::translate("LandDialog", "\347\231\273\351\231\206", 0));
        id_cancel->setText(QApplication::translate("LandDialog", "\345\217\226\346\266\210", 0));
        id_ip->setText(QApplication::translate("LandDialog", "IP\357\274\232", 0));
        id_port->setText(QApplication::translate("LandDialog", "Port\357\274\232", 0));
    } // retranslateUi

};

namespace Ui {
    class LandDialog: public Ui_LandDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LANDDIALOG_H
