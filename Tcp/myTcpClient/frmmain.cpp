#include "frmmain.h"
#include "ui_frmmain.h"
#include "myhelper.h"

frmMain::frmMain(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::frmMain)
{
    ui->setupUi(this);

    FormHelper::FormInCenter(this);
    FormHelper::FormNotResize(this);    

    this->Init();
}

frmMain::~frmMain()
{
    delete ui;
}

void frmMain::Init()
{
    ui->btnSend->setEnabled(false);
    ui->txtReceive->setReadOnly(true);
    tcpClient=new QTcpSocket(this);
    tcpClient->abort();//取消原有连接
    connect(tcpClient,SIGNAL(readyRead()),this,SLOT(ReadData()));
    connect(tcpClient,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(ReadError(QAbstractSocket::SocketError)));
}

void frmMain::ReadData()
{
    QByteArray buffer=tcpClient->readAll();
    if (!buffer.isEmpty())
    {
        if (ui->ckHexReceive->isChecked())
        {
            ui->txtReceive->append("接收数据:"+myHelper::ByteArrayToHexStr(buffer));
        }
        else
        {
            ui->txtReceive->append("接收数据:"+buffer);
        }
    }
}

void frmMain::ReadError(QAbstractSocket::SocketError)
{
    tcpClient->disconnectFromHost();
    ui->btnConnect->setText("连接");
    ui->txtReceive->append(tr("连接服务器失败,原因:%1").arg(tcpClient->errorString()));
}

void frmMain::on_btnConnect_clicked()
{

    if (ui->btnConnect->text()=="连接")
    {
        //链接服务器
        tcpClient->connectToHost(ui->txtIP->text(),ui->txtPort->text().toInt());
        //等待链接时间
        if (tcpClient->waitForConnected(1000))
        {
            ui->btnConnect->setText("断开");
            ui->txtReceive->append("连接服务器成功");
            ui->btnSend->setEnabled(true);
        }
    }
    else
    {
        //断开服务器链接
        tcpClient->disconnectFromHost();
        tcpClient->waitForDisconnected(1000);
        if (tcpClient->state() == QAbstractSocket::UnconnectedState || tcpClient->waitForDisconnected(1000))
        {
            ui->btnConnect->setText("连接");
            ui->txtReceive->append("断开连接成功");
            ui->btnSend->setEnabled(true);
        }
    }
}

void frmMain::on_btnClear_clicked()
{
    ui->txtReceive->clear();
}

void frmMain::on_btnSend_clicked()
{
    QString data=ui->txtSend->text();

    if (data!="")
    {
        if (ui->ckHexSend->isChecked())
        {
            tcpClient->write(myHelper::HexStrToByteArray(data));
        }
        else
        {
            tcpClient->write(data.toAscii());
        }
    }
}
