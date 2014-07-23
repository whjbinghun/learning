#ifndef DEMO_H
#define DEMO_H

#include <QMainWindow>
#include <QWidget>
#include <QToolButton>
#include <QTcpSocket>
#include <string>
#include <QStandardItemModel>
#include <QPainter>
#include "mythread.h"
#include "../video_decode_lib/video_decode_api.h"
#include "qmutex.h"


#define FRAME_SIZE 640*480*4
#define FRAME_VI_SIZE 1920*1080*4

typedef enum
{
    IR_MONITOR_VER_NEGO_REQ = 0x0001,                /* 版本协商请求 */
    IR_MONITOR_LINK_AUTH_REQ = 0x0002,              /* 鉴权请求 */
    IR_MONITOR_LINK_HEART_REQ = 0x0003,             /* 心跳检测请求 */
    IR_MONITOR_LINK_REL_REQ = 0x0004,                   /* 连接释放请求 */
    IR_MONITOR_XML_DATA_REQ = 0x0005,                /* XML文件请求 */
    IR_MONITOR_VID_DATA_PLAY_REQ = 0x0006,       /* 视频数据播放请求 */
    IR_MONITOR_VID_DATA_STOP_REQ = 0x0007,      /* 视频数据停止请求 */
    IR_MONITOR_PLATFORM_CTRL_REQ = 0x0010,    /* 云台控制请求 */

    IR_MONITOR_VER_NEGO_RESP = IR_MONITOR_VER_NEGO_REQ + 0x8000,
    IR_MONITOR_LINK_AUTH_RESP = IR_MONITOR_LINK_AUTH_REQ + 0x8000,
    IR_MONITOR_LINK_HEART_RESP = IR_MONITOR_LINK_HEART_REQ + 0x8000,
    IR_MONITOR_LINK_REL_RESP = IR_MONITOR_LINK_REL_REQ + 0x8000,
    IR_MONITOR_XML_DATA_RESP = IR_MONITOR_XML_DATA_REQ + 0x8000,
    IR_MONITOR_VID_DATA_PLAY_RESP = IR_MONITOR_VID_DATA_PLAY_REQ + 0x8000,
    IR_MONITOR_VID_DATA_STOP_RESP = IR_MONITOR_VID_DATA_STOP_REQ + 0x8000,
    IR_MONITOR_PLATFORM_CTRL_RESP = IR_MONITOR_PLATFORM_CTRL_REQ + 0x8000,
    IR_MONITOR_MSG_TYPE_BUTT = 0xFFFF
} IR_MONITOR_MSG_TYPE_E;

//消息头
#pragma pack(push)
#pragma pack(1)
typedef struct {
    unsigned int msg_type;
    unsigned int total_len;//总长度
    unsigned int seq_id;//序列号
} MsgHeader;
#pragma pack(pop)

//xml请求  消息体
#pragma pack(push)
#pragma pack(1)
typedef struct {
    MsgHeader header;
    unsigned int client_id;//客户端ID
}XmlReq;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
//xml回复 消息体
typedef struct {
    MsgHeader header;
    unsigned char result;//1：发送成功   其他：发送失败
    unsigned char xml_buf[1024*10];//把设备信息输出为xml格式信息,就相当于设置了一个地址
}XmlResp;
#pragma pack(pop)

typedef MsgHeader HeartBeatReq;//心跳包
typedef MsgHeader HeartBeatResp;//心跳包回复

//播放请求 消息体
#pragma pack(push)
#pragma pack(1)
typedef struct {
    MsgHeader header;
    char client_id[128];
}VodPlayReq;//播放
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct {
    MsgHeader header;
    unsigned char result;
    char url_buf[1024];
}VodPlayResp;//回复播放 消息他
#pragma pack(pop)

//停止播放请求
#pragma pack(push)
#pragma pack(1)
typedef struct {
    MsgHeader header;
    char client_id[128];
}VodStopPlayReq;
#pragma pack(pop)

//回复停止播放
#pragma pack(push)
#pragma pack(1)
typedef struct {
    MsgHeader header;
    unsigned char result;
}VodStopPlayResp;
#pragma pack(pop)

//设备信息
typedef struct {
    int n_class;
    QString s_sn;
    QString s_type;
    QString s_ip;
    int n_port;
    QString s_mac;
}DeviceInfo;

//视频播放
typedef struct {
    unsigned int seq_id;//序列号
    int n_class;
}DevicePlayInfo;

//可见光播放的相关信息
typedef struct {
    unsigned int ui_data_len;
    int width;
    int height;
    int num;
    int den;
}VIPlayMessage;


typedef struct {
    unsigned char* vi_buf;
    int len;
}VIPlayBuf;

//云台控制请求  消息体
#pragma pack(push)
#pragma pack(1)
typedef struct {
    MsgHeader header;
    unsigned int play_from_zi;//云台控制字
    unsigned int play_from_num;//云台控制字值
}PlayFormReq;
#pragma pack(pop)

//云台控制回复  消息体
#pragma pack(push)
#pragma pack(1)
typedef struct {
    MsgHeader header;
    unsigned char result;
}PlayFormResq;
#pragma pack(pop)

typedef enum
{
    IR_MONITOR_PLATFORM_CTRL_CMD_SET         = 1,  //设置
    IR_MONITOR_PLATFORM_CTRL_CMD_CALL_BACK   = 2,  //召回
    IR_MONITOR_PLATFORM_CTRL_CMD_CRUISE      = 3,  //巡航
    IR_MONITOR_PLATFORM_CTRL_CMD_UP          = 4,  //上
    IR_MONITOR_PLATFORM_CTRL_CMD_DOWN        = 5,  //下
    IR_MONITOR_PLATFORM_CTRL_CMD_LEFT        = 6,  //左
    IR_MONITOR_PLATFORM_CTRL_CMD_RIGHT       = 7,  //右

    IR_MONITOR_PLATFORM_CTRL_CMD_BUTT        = 0xFFFFFFFF
}IR_MONITOR_PLATFORM_CTRL_CMD_E;

namespace Ui {
class Demo;
}

class Demo : public QMainWindow
{
    Q_OBJECT

public:
    explicit Demo(QWidget *parent = 0);
    ~Demo();

    void xml_reply(char *buf, int length);
    void parse(QString file_name);

    void callback_ir_decode(unsigned char *puc_out_buf, unsigned int ui_data_len, int width, int height);
    void callback_vi_decode(unsigned char *puc_out_buf, unsigned int ui_data_len, int width, int height);

private:
    void init_tool_bar();
    void init_mouse();
    void init_connect();
    void init_vi_show();
    void init_ir_show();
    void init_xml_buf();
    void init_timer();
    void init_device_list();

    void resizeEvent( QResizeEvent * event ) ;
    void paintEvent(QPaintEvent *event) ;

    void send_xml_request();
    void play_reply(char *buf, int length);
    void stop_play_reply(char *buf) ;
    void receiver_play_message(char *url, int n_class);

public slots:
    void connect_server();            //连接服务器
    void connect_sucessful();       //链接成功
    void receiver_message();

    void send_heart_request();    //发送心跳检测请求
    void connect_play( QString str_device_sn, int n_class );
    void disconnect_play( QString str_device_sn, int n_class );

    void receiver_ir_play_message();    //红外播放视频流
    void receiver_vi_play_message();   //可见光播放视频流

    void set_vi_play();
    void update_show();

private slots:
    void play_show();
    void back_show();

    void displayError(QAbstractSocket::SocketError);  //显示错误
    void on_id_listview_device_doubleClicked(const QModelIndex &index);

public:
    Ui::Demo *ui;

private:
    QToolButton *mp_button_new;
    QToolButton *mp_button_open;
    QStandardItemModel *mp_standard_item_model;

    QString mstr_ip;
    int mn_port;

    QTcpSocket *mp_tcp_client;
    QTcpSocket *mp_ir_tcp_client;
    QTcpSocket *mp_vi_tcp_client;
    QString ms_message;  //存放从服务器接收到的字符串

    quint16 mn_blockSize;  //存放文件的大小信息

    enum {
        YF_Disconnected = 0,
        YF_Connected = 1,
        YF_Connecting = 2,
        YF_Disconnecting = 3
    };

    QImage m_ir_image;//红外
    char * m_ir_buf;
    quint64 m_ir_buf_len;
    int mn_ir_connect_status;       //.红外连接状态

    char *m_vi_buf;
    quint64 m_vi_buf_len;
    QImage m_vi_image;//可见光
    int mn_vi_connect_status;       //.可见光连接状态

    QStringList m_seq_id_list;
    QStringList m_seq_id_class_list;            //视频播放
    QStringList m_stop_seq_id_class_list;   //停止播放

    VIPlayMessage m_vi_play_message;
    QList<VIPlayBuf> m_vi_play_buf;

    YHANDLE  mh_vi_video_decode;
    YHANDLE  mh_ir_video_decode;
    //IRPlayMessage m_ir_play_message;

    int mi_file_flag;
    char *mp_xml_buf;

    QMutex m_mutex_ir, m_mutex_vi;
};

#endif // DEMO_H




