#ifndef LANDDIALOG_H
#define LANDDIALOG_H

#include <QDialog>
#include <QTcpSocket>
#include <QtNetwork>

namespace Ui {
class LandDialog;
}

class LandDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LandDialog(QWidget *parent = 0);
    ~LandDialog();

public:
    QString get_ip() { return mstr_ip; }
    int get_port() { return mn_port; }

private slots:
    void on_id_land_clicked();
    void on_id_cancel_clicked();

private slots:
    void newConnect(); //连接服务器
    void readMessage();  //接收数据
    void connect_sucessful();//链接成功
    void displayError(QAbstractSocket::SocketError);  //显示错误

private:
    Ui::LandDialog *ui;
    QTcpSocket *mp_tcp_client;
    QString ms_message;  //存放从服务器接收到的字符串
    quint16 mn_blockSize;  //存放文件的大小信息

    QString mstr_ip;
    int mn_port;
};

#endif // LANDDIALOG_H
