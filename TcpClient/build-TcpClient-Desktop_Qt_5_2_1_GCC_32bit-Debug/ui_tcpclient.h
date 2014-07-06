/********************************************************************************
** Form generated from reading UI file 'tcpclient.ui'
**
** Created by: Qt User Interface Compiler version 5.2.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TCPCLIENT_H
#define UI_TCPCLIENT_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TcpClient
{
public:
    QWidget *centralWidget;
    QLineEdit *id_ip_edit;
    QLineEdit *id_port_edit;
    QTextEdit *id_text_edit;
    QListView *id_listview;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *TcpClient)
    {
        if (TcpClient->objectName().isEmpty())
            TcpClient->setObjectName(QStringLiteral("TcpClient"));
        TcpClient->resize(569, 426);
        centralWidget = new QWidget(TcpClient);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        id_ip_edit = new QLineEdit(centralWidget);
        id_ip_edit->setObjectName(QStringLiteral("id_ip_edit"));
        id_ip_edit->setGeometry(QRect(310, 50, 113, 23));
        id_port_edit = new QLineEdit(centralWidget);
        id_port_edit->setObjectName(QStringLiteral("id_port_edit"));
        id_port_edit->setGeometry(QRect(310, 110, 113, 23));
        id_text_edit = new QTextEdit(centralWidget);
        id_text_edit->setObjectName(QStringLiteral("id_text_edit"));
        id_text_edit->setGeometry(QRect(210, 160, 341, 191));
        id_listview = new QListView(centralWidget);
        id_listview->setObjectName(QStringLiteral("id_listview"));
        id_listview->setGeometry(QRect(10, 10, 141, 321));
        id_listview->setMouseTracking(true);
        TcpClient->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(TcpClient);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 569, 20));
        TcpClient->setMenuBar(menuBar);
        mainToolBar = new QToolBar(TcpClient);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        TcpClient->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(TcpClient);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        TcpClient->setStatusBar(statusBar);

        retranslateUi(TcpClient);

        QMetaObject::connectSlotsByName(TcpClient);
    } // setupUi

    void retranslateUi(QMainWindow *TcpClient)
    {
        TcpClient->setWindowTitle(QApplication::translate("TcpClient", "TcpClient", 0));
    } // retranslateUi

};

namespace Ui {
    class TcpClient: public Ui_TcpClient {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TCPCLIENT_H
