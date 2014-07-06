#include "landdialog.h"
#include "ui_landdialog.h"
#include <QDebug>

LandDialog::LandDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LandDialog)
    , mstr_ip("")
    , mn_port(0)
    , mp_tcp_client(0)
{
    ui->setupUi(this);
    //在头文件中定义指针，必须初始化
    mp_tcp_client = new QTcpSocket(this);

    connect(mp_tcp_client, SIGNAL(connected()), this, SLOT(connect_sucessful()));
    connect(mp_tcp_client,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(displayError(QAbstractSocket::SocketError)));
    connect(mp_tcp_client, SIGNAL(readyRead()), this, SLOT(readMessage()));

}

LandDialog::~LandDialog()
{


    delete ui;
}

void LandDialog::on_id_land_clicked()
{
    qDebug()<<ui->id_ip_edit->text()<<ui->id_port_edit->text().toInt();
    mstr_ip = ui->id_ip_edit->text();
    mn_port = ui->id_port_edit->text().toInt();
    //链接服务器
    mp_tcp_client->connectToHost(ui->id_ip_edit->text(),ui->id_port_edit->text().toInt());
    //等待链接时间
    if (mp_tcp_client->waitForConnected(1000))
    {

    }


    //newConnect(); //请求连接
    //1.验证成功，跳转到主窗口
    //accept();
    //2.验证失败，报错，不跳转
}

void LandDialog::on_id_cancel_clicked()
{
    //断开服务器链接
    mp_tcp_client->disconnectFromHost();
    mp_tcp_client->waitForDisconnected(1000);
    if (mp_tcp_client->state() == QAbstractSocket::UnconnectedState || mp_tcp_client->waitForDisconnected(1000))
    {
        //ui->btnConnect->setText("连接");
        //ui->txtReceive->append("断开连接成功");
        //ui->btnSend->setEnabled(true);
        qDebug()<<"True";
    }
    close();
}

void LandDialog::newConnect()
{

    mn_blockSize = 0; //初始化其为0

    mp_tcp_client->abort(); //取消已有的连接

    //mp_tcp_client->connectToHost(ui->id_ip_edit->text(), ui->id_port_edit->text().toInt());

    //连接到主机，这里从界面获取主机地址和端口号

}

void LandDialog::readMessage()
{

    QDataStream in(mp_tcp_client);

    in.setVersion(QDataStream::Qt_5_2);

    //设置数据流版本，这里要和服务器端相同

    if(mn_blockSize==0) //如果是刚开始接收数据

    {

        //判断接收的数据是否有两字节，也就是文件的大小信息

        //如果有则保存到blockSize变量中，没有则返回，继续接收数据

        if(mp_tcp_client->bytesAvailable() < (int)sizeof(quint16)) return;

        in >> mn_blockSize;

    }

    if(mp_tcp_client->bytesAvailable() < mn_blockSize) return;

    //如果没有得到全部的数据，则返回，继续接收数据

    in >> ms_message;

    //将接收到的数据存放到变量中

    //ui->messageLabel->setText(ms_message);
    ui->id_text_edit->setText(ms_message);
    //显示接收到的数据

}

void LandDialog::connect_sucessful() {
    //1.验证成功，跳转到主窗口
    //accept();
}

void LandDialog::displayError(QAbstractSocket::SocketError)
{

    qDebug() << mp_tcp_client->errorString(); //输出错误信息

}
