#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QMainWindow>
#include <QTcpSocket>
#include <string>
#include <QStandardItemModel>

using namespace std;

typedef enum
{
    IR_MONITOR_VER_NEGO_REQ = 0x0001, /* 版本协商请求 */

    IR_MONITOR_LINK_AUTH_REQ = 0x0002, /* 鉴权请求 */

    IR_MONITOR_LINK_HEART_REQ = 0x0003, /* 心跳检测请求 */

    IR_MONITOR_LINK_REL_REQ = 0x0004, /* 连接释放请求 */

    IR_MONITOR_XML_DATA_REQ = 0x0005, /* XML文件请求 */

    IR_MONITOR_VID_DATA_PLAY_REQ = 0x0006, /* 视频数据播放请求 */

    IR_MONITOR_VID_DATA_STOP_REQ = 0x0007, /* 视频数据停止请求 */

    IR_MONITOR_VER_NEGO_RESP = IR_MONITOR_VER_NEGO_REQ + 0x8000,

    IR_MONITOR_LINK_AUTH_RESP = IR_MONITOR_LINK_AUTH_REQ + 0x8000,

    IR_MONITOR_LINK_HEART_RESP = IR_MONITOR_LINK_HEART_REQ + 0x8000,

    IR_MONITOR_LINK_REL_RESP = IR_MONITOR_LINK_REL_REQ + 0x8000,

    IR_MONITOR_XML_DATA_RESP = IR_MONITOR_XML_DATA_REQ + 0x8000,

    IR_MONITOR_VID_DATA_PLAY_RESP = IR_MONITOR_VID_DATA_PLAY_REQ + 0x8000,

    IR_MONITOR_VID_DATA_STOP_RESP = IR_MONITOR_VID_DATA_STOP_REQ + 0x8000,

    IR_MONITOR_MSG_TYPE_BUTT = 0xFFFF

}IR_MONITOR_MSG_TYPE_E;

//消息头
typedef struct {
    unsigned int msg_type;
    unsigned int total_len;//总长度
    unsigned int seq_id;//序列号
}MsgHeader;

//xml请求  消息体
typedef struct {
    MsgHeader header;
    unsigned int client_id;//客户端ID
}XmlReq;

#pragma pack(push)
#pragma pack(1)
//xml回复 消息体
typedef struct {
    MsgHeader header;
    unsigned char result;//1：发送成功   其他：发送失败
    //unsigned char xml_str[0];//把设备信息输出为xml格式信息,就相当于设置了一个地址
}XmlResp;
#pragma pack(pop)

typedef MsgHeader HeartBeatReq;//心跳包
typedef MsgHeader HeartBeatResp;//心跳包回复

//播放请求 消息体
typedef struct {
    MsgHeader header;
    char *client_id;
}VodPlayReq;//播放

#pragma pack(push)
#pragma pack(1)
typedef struct {
    MsgHeader header;
    unsigned char result;
    quint8 url_str[0];
}VodPlayResp;//回复播放 消息他
#pragma pack(pop)

//停止播放请求
typedef struct {
    MsgHeader header;
    char *client_id;
}VodStopPlayReq;

//回复停止播放
#pragma pack(push)
#pragma pack(1)
typedef struct {
    MsgHeader header;
    unsigned char result;
    quint8 url_str[0];
}VodStopPlayResp;
#pragma pack(pop)

namespace Ui {
class TcpClient;
}

class TcpClient : public QMainWindow
{
    Q_OBJECT

public:
    explicit TcpClient(QString str_ip, int n_port, QWidget *parent = 0);
    explicit TcpClient(QWidget *parent = 0);
    ~TcpClient();

    void xml_reply(char *buf, int length);
    void parse(QString file_name);
private:
    void send_xml_request();
    void play_reply(char *buf, int length);
    void stop_play_reply(char *buf, int length) ;

public slots:
    void receiver_message();
    //发送视频数据请求
    void send_play() ;
    //发送心跳检测请求
    void send_heart();
    void itemClicked(QModelIndex index);
    void play_request(QModelIndex index);
    void stop_play_request(QModelIndex index);

private slots:
    void newConnect(); //连接服务器
    void readMessage();  //接收数据
    void connect_sucessful();//链接成功
    void displayError(QAbstractSocket::SocketError);  //显示错误

private:
    Ui::TcpClient *ui;

private:
    QString mstr_ip;
    int mn_port;

    QTcpSocket *mp_tcp_client;
    QString ms_message;  //存放从服务器接收到的字符串
    quint8 mu8_message[512];//512字节
    quint16 mn_blockSize;  //存放文件的大小信息
    QStandardItemModel *standardItemModel;
    int mn_start_stop_play;

};

#endif // TCPCLIENT_H
