#include "tcpserver.h"
#include "ui_tcpserver.h"

TcpServer::TcpServer(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::TcpServer)
{
    ui->setupUi(this);
    mp_tcp_server = new QTcpServer(this);
    mn_blockSize = 0;

    //监听客户端
    if(!mp_tcp_server->listen(QHostAddress::LocalHost,6666))

    {  //监听本地主机的6666端口，如果出错就输出错误信息，并关闭

        qDebug() <<"11111"<< mp_tcp_server->errorString();

        close();

    }

    connect(mp_tcp_server,SIGNAL(newConnection()),this,SLOT(sendMessage()));
    //connect(mp_tcp_server->,SIGNAL(readyRead()), this, SLOT(read_message()));

    //connect(mp_tcp_server, SIGNAL(revice))
    //connect(mp_tcp_server, SIGNAL(readyRead()), this, SLOT(read_message()));
    /*if (receivers(SIGNAL(valueChanged(QByteArray))) > 0) {
        QByteArray data;
        //get_the_value(&data);       // expensive operation
        //emit valueChanged(data);
    }*/
}



TcpServer::~TcpServer()
{
    delete ui;
}

void TcpServer::sendMessage() {

    QByteArray block; //用于暂存我们要发送的数据

    QDataStream out(&block,QIODevice::WriteOnly);

    //使用数据流写入数据

    out.setVersion(QDataStream::Qt_5_2);

    //设置数据流的版本，客户端和服务器端使用的版本要相同

    out<<(quint16) 0;

    out<<tr("hello Tcp2222!!!");

    out.device()->seek(0);

    out<<(quint16) (block.size() - sizeof(quint16));

    QTcpSocket *clientConnection = mp_tcp_server->nextPendingConnection();

    //我们获取已经建立的连接的子套接字

    connect(clientConnection,SIGNAL(disconnected()),clientConnection, SLOT(deleteLater()));

    clientConnection->write(block);

    //关闭客服端链接
    //clientConnection->disconnectFromHost();

    //ui->statusLabel->setText("send message successful!!!");

    //发送数据成功后，显示提示

}

void TcpServer::read_message() {
    //while(bytesAvailable()>0)
   //{
    //int length = bytesAvailable();
    int length = 16;

        char buf[1024];
        read(1, buf, length);

        XmlReq xml_req;


        memcpy(&xml_req.header.msg_type,buf,sizeof(xml_req.header.msg_type));

        int n_pos=0;
        n_pos = sizeof(xml_req.header.msg_type);
        memcpy(&xml_req.header.seq_id,buf+n_pos, sizeof(xml_req.header.seq_id));
        n_pos = n_pos + sizeof(xml_req.header.seq_id);
        memcpy(&xml_req.header.total_len, buf+n_pos, sizeof(xml_req.header.total_len));
        n_pos = n_pos + sizeof(xml_req.header.total_len);
        memcpy(&xml_req.client_id,buf+n_pos,sizeof(xml_req.client_id));
        //n_pos = n_pos + sizeof(xml_req.client_id);
        //memcpy(&xml_req.xml_str,buf+12,length-12 );
        qDebug()<<length<<xml_req.header.msg_type<<xml_req.header.seq_id<<xml_req.header.total_len<<xml_req.client_id;


    //}


    /*QByteArray block;

    QDataStream in(&block,QIODevice::ReadOnly);

    in.setVersion(QDataStream::Qt_5_2);



    //设置数据流版本，这里要和服务器端相同

    if(mn_blockSize==0) //如果是刚开始接收数据

    {

        //判断接收的数据是否有两字节，也就是文件的大小信息

        //如果有则保存到blockSize变量中，没有则返回，继续接收数据

        if(mp_tcp_server->bytesAvailable() < (int)sizeof(quint16)) return;

        in >> mn_blockSize;

    }

    if(mp_tcp_server->bytesAvailable() < mn_blockSize) return;

    //如果没有得到全部的数据，则返回，继续接收数据

    in >> ms_message;

    qDebug()<<"11111"<<ms_message;
    //将接收到的数据存放到变量中

    //ui->messageLabel->setText(ms_message);
    ui->statusLabel->setText(ms_message);
    //显示接收到的数据
    read_xml();

    QTcpSocket *clientConnection = mp_tcp_server->nextPendingConnection();
    IP=socket->peerAddress().tostring();//得到客户端IP
    qDebug()<<"tcp client IP";
    //我们获取已经建立的连接的子套接字

    connect(clientConnection,SIGNAL(disconnected()),clientConnection, SLOT(deleteLater()));

    clientConnection->read(block);*/
}

void TcpServer::read_xml() {

}

void TcpServer::incomingConnection(int socketDescriptor)
{
    /*TcpClientSocket *tcpClientSocket=new TcpClientSocket(this);
    connect(tcpClientSocket,SIGNAL(updateClients(QString,int)),this,SLOT(updateClients(QString,int)));
    connect(tcpClientSocket,SIGNAL(disconnected(int)),this,SLOT(slotDisconnected(int)));

    tcpClientSocket->setSocketDescriptor(socketDescriptor);

    tcpClientSocketList.append(tcpClientSocket);*/
    connect(mp_tcp_server,SIGNAL(readyRead()),this,SLOT(read_message()));
}
