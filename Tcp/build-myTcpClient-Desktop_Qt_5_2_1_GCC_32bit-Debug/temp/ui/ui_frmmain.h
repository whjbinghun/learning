/********************************************************************************
** Form generated from reading UI file 'frmmain.ui'
**
** Created by: Qt User Interface Compiler version 5.2.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FRMMAIN_H
#define UI_FRMMAIN_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_frmMain
{
public:
    QGroupBox *groupBox;
    QCheckBox *ckHexReceive;
    QLabel *label;
    QPushButton *btnConnect;
    QLineEdit *txtIP;
    QCheckBox *ckHexSend;
    QLineEdit *txtPort;
    QLabel *label_2;
    QGroupBox *groupBox_2;
    QTextEdit *txtReceive;
    QPushButton *btnSend;
    QLineEdit *txtSend;
    QPushButton *btnClear;

    void setupUi(QWidget *frmMain)
    {
        if (frmMain->objectName().isEmpty())
            frmMain->setObjectName(QStringLiteral("frmMain"));
        frmMain->resize(589, 529);
        frmMain->setStyleSheet(QStringLiteral(""));
        groupBox = new QGroupBox(frmMain);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(10, 470, 571, 51));
        ckHexReceive = new QCheckBox(groupBox);
        ckHexReceive->setObjectName(QStringLiteral("ckHexReceive"));
        ckHexReceive->setGeometry(QRect(120, 10, 111, 31));
        label = new QLabel(groupBox);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(230, 10, 31, 31));
        btnConnect = new QPushButton(groupBox);
        btnConnect->setObjectName(QStringLiteral("btnConnect"));
        btnConnect->setGeometry(QRect(480, 10, 81, 30));
        btnConnect->setCursor(QCursor(Qt::PointingHandCursor));
        txtIP = new QLineEdit(groupBox);
        txtIP->setObjectName(QStringLiteral("txtIP"));
        txtIP->setGeometry(QRect(260, 10, 111, 30));
        ckHexSend = new QCheckBox(groupBox);
        ckHexSend->setObjectName(QStringLiteral("ckHexSend"));
        ckHexSend->setGeometry(QRect(10, 10, 111, 31));
        txtPort = new QLineEdit(groupBox);
        txtPort->setObjectName(QStringLiteral("txtPort"));
        txtPort->setGeometry(QRect(420, 10, 51, 30));
        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(380, 10, 31, 31));
        groupBox_2 = new QGroupBox(frmMain);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        groupBox_2->setGeometry(QRect(10, 10, 571, 451));
        txtReceive = new QTextEdit(groupBox_2);
        txtReceive->setObjectName(QStringLiteral("txtReceive"));
        txtReceive->setGeometry(QRect(10, 10, 551, 391));
        txtReceive->setStyleSheet(QStringLiteral(""));
        txtReceive->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        btnSend = new QPushButton(groupBox_2);
        btnSend->setObjectName(QStringLiteral("btnSend"));
        btnSend->setGeometry(QRect(380, 410, 91, 30));
        btnSend->setCursor(QCursor(Qt::PointingHandCursor));
        txtSend = new QLineEdit(groupBox_2);
        txtSend->setObjectName(QStringLiteral("txtSend"));
        txtSend->setGeometry(QRect(10, 410, 361, 30));
        btnClear = new QPushButton(groupBox_2);
        btnClear->setObjectName(QStringLiteral("btnClear"));
        btnClear->setGeometry(QRect(480, 410, 81, 30));
        btnClear->setCursor(QCursor(Qt::PointingHandCursor));
        QWidget::setTabOrder(txtIP, txtPort);
        QWidget::setTabOrder(txtPort, btnConnect);
        QWidget::setTabOrder(btnConnect, btnClear);
        QWidget::setTabOrder(btnClear, ckHexSend);
        QWidget::setTabOrder(ckHexSend, ckHexReceive);
        QWidget::setTabOrder(ckHexReceive, txtSend);
        QWidget::setTabOrder(txtSend, btnSend);
        QWidget::setTabOrder(btnSend, txtReceive);

        retranslateUi(frmMain);

        QMetaObject::connectSlotsByName(frmMain);
    } // setupUi

    void retranslateUi(QWidget *frmMain)
    {
        frmMain->setWindowTitle(QApplication::translate("frmMain", "Tcp\351\200\232\344\277\241\345\256\242\346\210\267\347\253\257", 0));
        groupBox->setTitle(QString());
        ckHexReceive->setText(QApplication::translate("frmMain", "16\350\277\233\345\210\266\346\216\245\346\224\266", 0));
        label->setText(QApplication::translate("frmMain", "IP\357\274\232", 0));
        btnConnect->setText(QApplication::translate("frmMain", "\350\277\236\346\216\245", 0));
        txtIP->setText(QApplication::translate("frmMain", "192.168.1.187", 0));
        ckHexSend->setText(QApplication::translate("frmMain", "16\350\277\233\345\210\266\345\217\221\351\200\201", 0));
        txtPort->setText(QApplication::translate("frmMain", "60000", 0));
        label_2->setText(QApplication::translate("frmMain", "\347\253\257\345\217\243\357\274\232", 0));
        groupBox_2->setTitle(QString());
        btnSend->setText(QApplication::translate("frmMain", "\345\217\221\351\200\201", 0));
        btnClear->setText(QApplication::translate("frmMain", "\346\270\205\347\251\272", 0));
    } // retranslateUi

};

namespace Ui {
    class frmMain: public Ui_frmMain {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FRMMAIN_H
