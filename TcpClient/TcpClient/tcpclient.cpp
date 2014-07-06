#include "tcpclient.h"
#include "ui_tcpclient.h"
#include <QDebug>
#include <QHostAddress>
#include <QFile>
#include <QMessageBox>
#include <QDomDocument>

#include<netinet/in.h>


static  unsigned int uint_seq_id = 0;

TcpClient::TcpClient(QString str_ip, int n_port, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::TcpClient)
  , mstr_ip(str_ip)
  , mn_port(n_port)
{
    ui->setupUi(this);


    ui->id_ip_edit->setText(mstr_ip);
    //整数转换成字符串
    ui->id_port_edit->setText(QString::number(mn_port));
    qDebug()<<mstr_ip<<mn_port;
}

TcpClient::TcpClient(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::TcpClient)
  ,mp_tcp_client(0)
{
    ui->setupUi(this);
    //在头文件中定义指针，必须初始化
    mp_tcp_client = new QTcpSocket(this);
    mn_start_stop_play = 0;
    newConnect();

    //服务器链接成功
    connect(mp_tcp_client, SIGNAL(connected()), this, SLOT(connect_sucessful()));
    connect(mp_tcp_client,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(displayError(QAbstractSocket::SocketError)));
    //接收服务器发送的数据
    connect(mp_tcp_client, SIGNAL(readyRead()), this, SLOT(receiver_message()));

    //点击列表，发送播放请求
    //connect(id_list_view,SIGNAL(clicked()),this, SLOT(send_play));

}


TcpClient::~TcpClient()
{
    delete ui;
}


void TcpClient::newConnect()
{

    mn_blockSize = 0; //初始化其为0
    mp_tcp_client->abort(); //取消已有的连接
    //mp_tcp_client->connectToHost("192.168.0.107", 6666);
    //mp_tcp_client->connectToHost(QHostAddress::LocalHost,(quint16)6666);
    mp_tcp_client->connectToHost("192.168.0.116", 5555);
    //连接到主机，这里从界面获取主机地址和端口号

}

void TcpClient::readMessage()
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

    qDebug()<<"11111"<<ms_message;
    //将接收到的数据存放到变量中

    //ui->messageLabel->setText(ms_message);
    ui->id_text_edit->setText(ms_message);
    //显示接收到的数据

}

void TcpClient::connect_sucessful() {
    qDebug()<<"链接成功";
    //1.验证成功，跳转到主窗口
    //accept();

    //向服务器发送xml请求
    send_xml_request();

}

//向服务器发送xml请求
void TcpClient::send_xml_request() {
    XmlReq xml_req;
    //htonl是主机字节序到网络字节序的转换
    xml_req.header.msg_type = htonl(IR_MONITOR_XML_DATA_REQ);
    //应答消息流水号
    xml_req.header.seq_id = htonl(uint_seq_id++);
    xml_req.client_id =htonl(1);
    int n_xml_length = sizeof(xml_req);
    xml_req.header.total_len = htonl(n_xml_length);
    qDebug()<<xml_req.header.msg_type<< xml_req.header.seq_id<<xml_req.client_id<<xml_req.header.total_len<<n_xml_length;
    char buf[1024];

    memcpy(buf, &xml_req, n_xml_length);

    mp_tcp_client->write(buf, n_xml_length);
}

void TcpClient::itemClicked(QModelIndex index) {
    if(mn_start_stop_play == 0){
        play_request(index);
        mn_start_stop_play = 1;
    }else {
        stop_play_request(index);
        mn_start_stop_play = 0;
    }

}

//播放视频 请求
void TcpClient::play_request(QModelIndex index) {
    //获取listview的内容
    QString str_tmp = ( ( QStandardItemModel *) ui->id_listview->model() )->item( index.row() )->text();
    VodPlayReq vod_play_req;

    vod_play_req.header.msg_type = IR_MONITOR_VID_DATA_PLAY_REQ;
    vod_play_req.header.seq_id = uint_seq_id++;
    //设备名
    vod_play_req.client_id = str_tmp.toLatin1().data();

    vod_play_req.header.total_len = sizeof(vod_play_req.header)+sizeof(vod_play_req.client_id);

    qDebug()<<vod_play_req.header.msg_type<< vod_play_req.header.seq_id<<vod_play_req.client_id<<vod_play_req.header.total_len;
    //char buf[1024];

    //memcpy(&buf, &xml_req, xml_req.header.total_len);

    mp_tcp_client->write((char *)&vod_play_req,vod_play_req.header.total_len);

}

void TcpClient::stop_play_request(QModelIndex index) {
    //获取listview的内容
    QString str_tmp = ( ( QStandardItemModel *) ui->id_listview->model() )->item( index.row() )->text();
    VodStopPlayReq vod_stop_play_req;

    vod_stop_play_req.header.msg_type = IR_MONITOR_VID_DATA_PLAY_REQ;
    vod_stop_play_req.header.seq_id = uint_seq_id++;
    //设备名
    vod_stop_play_req.client_id = str_tmp.toLatin1().data();

    vod_stop_play_req.header.total_len = sizeof(vod_stop_play_req.header)+sizeof(vod_stop_play_req.client_id);

    qDebug()<<vod_stop_play_req.header.msg_type<< vod_stop_play_req.header.seq_id<<vod_stop_play_req.client_id<<vod_stop_play_req.header.total_len;


    //char buf[1024];

    //memcpy(&buf, &xml_req, xml_req.header.total_len);

    mp_tcp_client->write((char *)&vod_stop_play_req, vod_stop_play_req.header.total_len);
}

//发送视频数据请求
void TcpClient::send_play() {

}

//发送心跳检测请求
void TcpClient::send_heart() {
   /* HeartBeatReq heart_beat_req;
    //htonl是主机字节序到网络字节序的转换
    heart_beat_req.msg_type = htonl(IR_MONITOR_XML_DATA_REQ);
    //应答消息流水号
    xheart_beat_req.seq_id = htonl(uint_seq_id++);
    heart_beat_req.client_id =htonl(1);
    int n_xml_length = sizeof(xml_req);
    heart_beat_req.total_len = htonl(n_xml_length);
    qDebug()<<xml_req.header.msg_type<< xml_req.header.seq_id<<xml_req.client_id<<xml_req.header.total_len<<n_xml_length;
    char buf[1024];

    memcpy(buf, &xml_req, n_xml_length);

    mp_tcp_client->write(buf, n_xml_length);*/
}



//接收 消息
void TcpClient::receiver_message() {
    while(mp_tcp_client->bytesAvailable()>0)
    {
        int length = mp_tcp_client->bytesAvailable();

        char buf[1024];
        //读取服务器的数据+
        mp_tcp_client->read(buf, length);

        XmlResp xml_resp;


        memcpy(&xml_resp.header,buf,sizeof(xml_resp.header));
        unsigned int msg_type;
        msg_type = ntohl(xml_resp.header.msg_type);//32773
        switch (msg_type) {
        case IR_MONITOR_VER_NEGO_RESP :
            break;

        case IR_MONITOR_LINK_AUTH_RESP :
            break;

        case IR_MONITOR_LINK_HEART_RESP :
            break;

        case IR_MONITOR_LINK_REL_RESP :
            break;
        case IR_MONITOR_XML_DATA_RESP :  //xml 回复
            xml_reply(buf,length);
            break;

        case IR_MONITOR_VID_DATA_PLAY_RESP : //数据播放回复
            play_reply(buf, length);
            break;

        case IR_MONITOR_VID_DATA_STOP_RESP : //数据停止回
            stop_play_reply(buf, length);
            break;

        case IR_MONITOR_MSG_TYPE_BUTT :
            break;
        default:
            break;
        }

    }



}

/*void TcpClient::sendMessage() {

    QByteArray block; //用于暂存我们要发送的数据

    QDataStream out(&block,QIODevice::WriteOnly);

    //使用数据流写入数据

    out.setVersion(QDataStream::Qt_5_2);


    //设置数据流的版本，客户端和服务器端使用的版本要相同

    out<<(quint16) 0;

    out<<(const char *)“hello Tcp!!!”;

    out.device()->seek(0);

    out<<(quint16) (block.size() – sizeof(quint16));

    QTcpSocket *clientConnection = tcpServer->nextPendingConnection();

    //我们获取已经建立的连接的子套接字

    connect(clientConnection,SIGNAL(disconnected()),clientConnection,

            SLOT(deleteLater()));

    clientConnection->write(block);

    clientConnection->disconnectFromHost();

    ui->statusLabel->setText(“send message successful!!!”);

    //发送数据成功后，显示提示

}*/

void TcpClient::displayError(QAbstractSocket::SocketError)
{

    qDebug() << mp_tcp_client->errorString(); //输出错误信息

}

void TcpClient::xml_reply(char * buf, int length){
    //创建文件
    QString s_file_name = "../TcpClient/monitor.xml";
    QFile file(s_file_name);
    //如果文件不存在，那就创建文件
    if(file.exists()==false) {
        file.open(QIODevice::WriteOnly);
    }
    //file.open(QIODevice::WriteOnly);

    XmlResp xml_resp;

    //记录buf读取位置
    int n_pos=0;
    n_pos = sizeof(xml_resp.header);
    memcpy(&xml_resp.result,buf+n_pos,sizeof(xml_resp.result));
    if(xml_resp.result == 1) {
        n_pos = n_pos + sizeof(xml_resp.result);
        //memcpy(&xml_resp.xml_str,buf+13,length-13 );
        //写入文件
        QTextStream out(&file);
        file.open(QIODevice::WriteOnly);
        out << buf+13 << endl;
        qDebug()<<buf+13;
        //解析xml文件
        parse(s_file_name);
    }else { //如果获取xml文件失败，则重新发送xml请求
        send_xml_request();
    }
}

//播放视频回复
void TcpClient::play_reply(char * buf, int length) {
    VodPlayResp vod_play_resp;
    int n_pos=0;
    n_pos = sizeof(vod_play_resp.header);
    memcpy(&vod_play_resp.result,buf+n_pos,sizeof(vod_play_resp.result));
    if(vod_play_resp.result == 1) {
        n_pos = n_pos + sizeof(vod_play_resp.result);
        memcpy(&vod_play_resp.url_str,buf+13,length-13 );


    }else { //重新发送播放请求
        //play_request();
    }

}

void TcpClient::stop_play_reply(char * buf, int length) {
    VodStopPlayResp vod_stop_play_resp;
    int n_pos=0;
    n_pos = sizeof(vod_stop_play_resp.header);
    memcpy(&vod_stop_play_resp.result,buf+n_pos,sizeof(vod_stop_play_resp.result));
    if(vod_stop_play_resp.result == 1) {
        n_pos = n_pos + sizeof(vod_stop_play_resp.result);
        memcpy(&vod_stop_play_resp.url_str, buf+13, length-13 );


    }else { //重新发送播放请求
        //play_request();
    }
}

//对xml文件的解析
void TcpClient::parse(QString file_name)
{
    if(file_name.isEmpty())
        return;

    //打开文件
    QFile file(file_name);
    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox::information(NULL, QString("title"), QString("open error!"));

        return;
    }

    QDomDocument document;
    QString error;
    int row = 0, column = 0;
    //setContent：设置内容
    if(!document.setContent(&file, false, &error, &row, &column))
    {
        //解析文件失败
        QMessageBox::information(NULL, QString("title"), QString("parse file failed at line row and column") + QString::number(row, 10) + QString(",") + QString::number(column, 10));

        return;
    }

    if(document.isNull())
    {
        QMessageBox::information(NULL, QString("title"), QString("document is null!"));

        return;
    }

    QDomElement root = document.documentElement();

    //root_tag_name为persons
    QString root_tag_name = root.tagName();
    if(root.hasAttribute("name"))
    {
        //name为Qt
        QString name = root.attributeNode("name").value();
    }

    //获取id="1"的节点
    QDomElement person = root.firstChildElement();
    if(person.isNull())
        return;

    QString person_tag_name = person.tagName();

    //id为1
    QString id = person.attributeNode("id").value();

    //获取子节点，数目为2
    QDomNodeList list = root.childNodes();
    int count = list.count();
    qDebug()<<"11111"<<count;

    //listview 列表
    QStringList s_list;

    for(int i=0; i<count;i++)
    {
        QDomNode dom_node = list.item(i);
        QDomElement element = dom_node.toElement();

        //获取id值，等价
        QString id_1 = element.attributeNode("id").value();
        QString id_2 = element.attribute("id");
        qDebug()<<id_1<<id_2;

        //获取子节点，数目为4，包括：name、age、email、website
        QDomNodeList child_list = element.childNodes();
        int child_count = child_list.count();
        qDebug()<<"22222"<<child_count;
        for(int j=0; j<child_count;j++)
        {
            QDomNode child_dom_node = child_list.item(j);
            QDomElement child_element = child_dom_node.toElement();
            QString child_tag_name = child_element.tagName();
            QString child__tag_value = child_element.text();
            qDebug()<<child_tag_name<<child__tag_value;
            if(child_tag_name=="sn"){
                s_list.append(child__tag_value);
            }
        }
    }

    standardItemModel = new QStandardItemModel(this);
    int nCount = s_list.size();
    qDebug()<<nCount;
    for(int i = 0; i < nCount; i++)
    {
        QString string = static_cast<QString>(s_list.at(i));
        QStandardItem *item = new QStandardItem(string);
        if(i % 2 == 1)
        {
            QLinearGradient linearGrad(QPointF(0, 0), QPointF(200, 200));
            linearGrad.setColorAt(0, Qt::darkGreen);
            linearGrad.setColorAt(1, Qt::yellow);
            QBrush brush(linearGrad);
            item->setBackground(brush);
        }
        standardItemModel->appendRow(item);
    }
    ui->id_listview->setModel(standardItemModel);
    //ui->id_listview->setFixedSize(200,300);
   // mn_play_start_stop=0;
    connect(ui->id_listview,SIGNAL(doubleclicked(QModelIndex)),this,SLOT(itemClicked(QModelIndex)));
}




