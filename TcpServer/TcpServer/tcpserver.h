#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QMainWindow>
#include <QtNetwork>
#include <QTcpSocket>
#include <QTcpServer>
#include <QObject>


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
    quint8 xml_str[0];//把设备信息输出为xml格式信息
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
}VodPlayResp;//停止播放
#pragma pack(pop)

namespace Ui {
class TcpServer;
}

class TcpServer : public QMainWindow
{
    Q_OBJECT

public:
    explicit TcpServer(QWidget *parent = 0);
    ~TcpServer();
public:
    void read_xml();

private slots:
    void sendMessage();
    void read_message();

protected:
    void incomingConnection(int socketDescriptor);

private:
    Ui::TcpServer *ui;
    QTcpServer *mp_tcp_server;
    quint16 mn_blockSize;  //存放文件的大小信息
    QString ms_message;  //存放从客户端接收到的字符串
};

#endif // TCPSERVER_H
