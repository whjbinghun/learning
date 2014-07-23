#ifndef NETCONTROL_H
#define NETCONTROL_H
#include "netcapture.h"
#include <QTimer>
#include <QDomNodeList>
#include <QMap>
#include <QMutex>
#include <QList>
#include <netcaptureir.h>
#include <netcapturegroup.h>

#define CONN_ALIVE_SECONDS 180

typedef enum
{
    IR_MONITOR_VER_NEGO_REQ = 0x0001, /* 版本协商请求 */
    IR_MONITOR_LINK_AUTH_REQ = 0x0002, /* 鉴权请求 */
    IR_MONITOR_LINK_HEART_REQ = 0x0003, /* 心跳检测请求 */
    IR_MONITOR_LINK_REL_REQ = 0x0004, /* 连接释放请求 */
    IR_MONITOR_XML_DATA_REQ = 0x0005, /* XML文件请求 */
    IR_MONITOR_VID_DATA_PLAY_REQ = 0x0006, /* 视频数据播放请求 */
    IR_MONITOR_VID_DATA_STOP_REQ = 0x0007, /* 视频数据停止请求 */
    IR_MONITOR_REPLAY_DATA_REQ = 0x0008, /* 回放数据控制请求 */
    IR_MONITOR_PLATFORM_CTRL_REQ = 0x0010, /* 云台控制请求 */
    IR_MONITOR_GROUP_CFG_ADD_REQ = 0x0020, /* 配置信息新增请求 */
    IR_MONITOR_GROUP_CFG_DEL_REQ = 0x0021, /* 配置信息删除请求 */
    IR_MONITOR_GROUP_CFG_CHG_REQ = 0x0022, /* 配置信息变更请求 */
    IR_MONITOR_VER_NEGO_RESP = IR_MONITOR_VER_NEGO_REQ + 0x8000,
    IR_MONITOR_LINK_AUTH_RESP = IR_MONITOR_LINK_AUTH_REQ + 0x8000,
    IR_MONITOR_LINK_HEART_RESP = IR_MONITOR_LINK_HEART_REQ + 0x8000,
    IR_MONITOR_LINK_REL_RESP = IR_MONITOR_LINK_REL_REQ + 0x8000,
    IR_MONITOR_XML_DATA_RESP = IR_MONITOR_XML_DATA_REQ + 0x8000,
    IR_MONITOR_VID_DATA_PLAY_RESP = IR_MONITOR_VID_DATA_PLAY_REQ + 0x8000,
    IR_MONITOR_VID_DATA_STOP_RESP = IR_MONITOR_VID_DATA_STOP_REQ + 0x8000,
    IR_MONITOR_REPLAY_DATA_RESP          = IR_MONITOR_REPLAY_DATA_REQ + 0x8000,
    IR_MONITOR_PLATFORM_CTRL_RESP = IR_MONITOR_PLATFORM_CTRL_REQ + 0x8000,
    IR_MONITOR_GROUP_CFG_ADD_RESP = IR_MONITOR_GROUP_CFG_ADD_REQ + 0x8000,
    IR_MONITOR_GROUP_CFG_DEL_RESP = IR_MONITOR_GROUP_CFG_DEL_REQ + 0x8000,
    IR_MONITOR_GROUP_CFG_CHG_RESP = IR_MONITOR_GROUP_CFG_CHG_REQ + 0x8000,
    IR_MONITOR_MSG_TYPE_BUTT = 0xFFFF
}IR_MONITOR_MSG_TYPE_E;

typedef enum
{
    IR_MONITOR_PLATFORM_CTRL_CMD_SET = 1,
    IR_MONITOR_PLATFORM_CTRL_CMD_CALL_BACK = 2,
    IR_MONITOR_PLATFORM_CTRL_CMD_CRUISE = 3,
    IR_MONITOR_PLATFORM_CTRL_CMD_UP = 4,
    IR_MONITOR_PLATFORM_CTRL_CMD_DOWN = 5,
    IR_MONITOR_PLATFORM_CTRL_CMD_LEFT = 6,
    IR_MONITOR_PLATFORM_CTRL_CMD_RIGHT = 7,
    IR_MONITOR_PLATFORM_CTRL_CMD_BUTT = 0xFFFFFFFF
}IR_MONITOR_PLATFORM_CTRL_CMD_E;

typedef enum
{
    IR_MONITOR_REPLAY_SETUP = 1,
    IR_MONITOR_REPLAY_PLAY = 2,
    IR_MONITOR_REPLAY_PAUSE = 3,
    IR_MONITOR_REPLAY_RESUME = 4,
    IR_MONITOR_REPLAY_STOP = 5,

    IR_MONITOR_REPLAY_SETUP_RESP = 0x1000 + IR_MONITOR_REPLAY_SETUP,
    IR_MONITOR_REPLAY_PLAY_RESP = 0x1000 + IR_MONITOR_REPLAY_PLAY,
    IR_MONITOR_REPLAY_PAUSE_RESP = 0x1000 + IR_MONITOR_REPLAY_PAUSE,
    IR_MONITOR_REPLAY_RESUME_RESP = 0x1000 + IR_MONITOR_REPLAY_RESUME,
    IR_MONITOR_REPLAY_STOP_RESP = 0x1000 + IR_MONITOR_REPLAY_STOP,

    IR_MONITOR_REPLAY_BUTT         = 0xFFFFFFFF
}IR_MONITOR_REPLAY_CMD_E;

typedef enum {
    DEVICE_VISIBLELIGHT = 0,
    DEVICE_INFRARED = 1
}DEVICE_CLASS;

typedef struct _tagDeviceInfo{
    int n_class;
    int n_port;
    QString str_ip;
    QString str_sn;
    QString str_type;
    QString str_mac;
    _tagDeviceInfo()
        : n_class(0), n_port(0)
        , str_ip(""), str_sn("")
        , str_type(""), str_mac(""){}
}DeviceInfo, *PDeviceInfo;

typedef struct _tagDeviceInfoSimple{
    int n_class;
    int n_group_num;
    QString str_sn;
    _tagDeviceInfoSimple()
        : n_class(0), n_group_num(0), str_sn(""){}
}DeviceInfoSimple;

typedef struct _tagDeviceGroup{
    DeviceInfo dev_ir;
    DeviceInfo dev_vi;
} DeviceGroup;

#pragma pack(push)
#pragma pack(1)
typedef struct {
    unsigned int msg_type;
    unsigned int total_len;
    unsigned int seq_id;
}MsgHeader;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct {
    MsgHeader header;
    unsigned int client_id;
}XmlReq;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct {
    MsgHeader header;
    unsigned char result;
    char xml_buf[1024*10];
}XmlResp;
#pragma pack(pop)

typedef MsgHeader HeartBeatReq;
typedef MsgHeader HeartBeatResp;

#pragma pack(push)
#pragma pack(1)
typedef struct {
    MsgHeader header;
    char device_sn[128];
}VodPlayReq;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct {
    MsgHeader header;
    unsigned char result;
    char url_buf[1024];
}VodPlayResp;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct {
    MsgHeader header;
    char device_sn[128];
}VodStopReq;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct {
    MsgHeader header;
    unsigned char result;
}VodStopResp;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct {
    MsgHeader header;
    unsigned int cmd;
    unsigned int val;
}PlatformCtrlReq;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct {
    MsgHeader header;
    unsigned char result;
}PlatformCtrlResp;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct {
    MsgHeader header;
    unsigned int num;
    char members[1024];
}GroupCfgAddReq;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct {
    MsgHeader header;
    unsigned char result;
    unsigned int id;
}GroupCfgAddResp;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct {
    MsgHeader header;
    unsigned int id;
}GroupCfgDelReq;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct {
    MsgHeader header;
    unsigned char result;
}GroupCfgDelResp;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct {
    MsgHeader header;
    unsigned int id;
    unsigned int num;
    char members[1024];
}GroupCfgChgReq;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct {
    MsgHeader header;
    unsigned char result;
}GroupCfgChgResp;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct {
    MsgHeader header;
    unsigned int n_group_num;
    unsigned int n_ctrl_cmd;
    unsigned int n_session;
    unsigned int n_play_speed;
    char sz_time[28];
}PlaybackControlReq;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct {
    MsgHeader header;
    unsigned char result;
    unsigned int n_ctrl_cmd;
    unsigned int n_session;
    char members[1024];
}PlaybackControlResp;
#pragma pack(pop)

class NetControl : public NetCapture
{
public:
    NetControl( const char *s_server_ip, int i_server_port );
    ~NetControl();

public:
    int send_vod_play_req( const char *s_sn ,const DeviceInfo &device_info, const int &n_group_num );
    int send_vod_stop_req( const char *s_sn );
    void little_to_big( MsgHeader &head );
    void big_to_little( MsgHeader &head );
    void get_vods( QMap<quint64,NetCaptureVod*> &p_vods );
    NetCaptureGroup *get_group_connect( int n_index );

    bool get_group_info( int n_index, DeviceGroup &dev_group );
    void get_group_device( QMap< int, DeviceGroup > &map_group  );
    void get_no_group_ir_device( QList< DeviceInfo > &list_ir );
    void get_no_group_vi_device( QList< DeviceInfo > &list_vi );
    bool get_device_group_info( const QString &str_sn, int &n_group_num, int &n_class );

    void clear_device_group();
    bool analysis_xml( QString str_xml );
    void analysis_xml_device( QDomNodeList& doc_node_list, int n_group_num = -1 );

    void send_connect_group_device();
    int send_heart_tag();
    int send_update_xml();
    bool is_update_xml( int n_cmd );

    void clear_group_ctrl_req();
    void get_complete_group_req( int &n_complete, int &n_failure );
    int send_group_add_req( const QList<QString>& devices );
    int send_add_group( int n_sn_num, QString str_sns );
    int send_del_group( int n_group );
    int send_change_group( int n_group, int n_sn_num, QString str_sns );

    void set_group_resp_map( const int &n_seq, const int & n_result );
    void get_group_resp_map( const int &n_seq, int &n_result );

    void clear_playback_connect_req();
    void get_playback_complete_connect_req( const int &n_seq_cmd, int &n_result );
    int send_playback_connect( int n_group_num );
    int send_playback_play( int n_group_num, int n_speed, QString str_time_min = "", QString str_time_max = "" );
    int send_playback_pause( int n_group_num );
    int send_playback_disconnect( int n_group_num );
    int send_playback_cmd( int n_group_num, int n_replay_cmd, int n_session = 0 );

    int get_playback_group_num() { return mn_group_num_playback; }
    bool get_playback_connect() { return mb_connected_playback; }
    bool get_playback_pause() { return mb_pause_playback; }
    NetCaptureGroup *get_playback_group() { return mp_connect_playback; }


public slots:

public:
    virtual void on_receive();

private:
    void get_recv_buf();
    int parse_recv();
    int parse_xml( const MsgHeader &head );
    int parse_heartbeat( const MsgHeader &head );
    int parse_vod_play( const MsgHeader &head );
    int parse_vod_stop( const MsgHeader &head );
    int parse_group_cfg_add( const MsgHeader &head );
    int parse_group_cfg_del( const MsgHeader &head );
    int parse_group_cfg_chg( const MsgHeader &head );
    int parse_playback( const MsgHeader &head );
    int parse_playback_connect( char * );

    void play_vod( int i_no, int i_class, const char *s_ip, int i_port, QString str_sn );

private:
    time_t mi_latest_recv;
    quint32 mn_seq_cmd;
    QString mstr_xml_change;

    QMap< int, DeviceInfoSimple > m_map_device_reqs;      //.等待返回连接命令设备 int: 命令码
    QMap< int, int > m_map_group_ctrl_reqs;             //.等待返回组控制命令设备
    QMap< int, int > m_map_update_xml_reqs;             //.等待返回xml更新命令
    QMap< int, int > m_map_playback_connect_reqs;    //.等待返回回放控制连接和断开命令

    QMap< int, DeviceGroup > m_map_device_group;    //. 已经分组设备
    QList< DeviceInfo > m_list_device_ir_no_group;            //. 未分组设备
    QList< DeviceInfo > m_list_device_vi_no_group;            //. 未分组设备
    QMutex m_mutex_device_grp;

    QMap< int, NetCaptureGroup*> m_map_connect_videos;      //.连接组设备
    QMap<quint64,NetCaptureVod*> mo_vods;
    QMutex m_mutex_device_net;

    NetCaptureGroup *mp_connect_playback;
    int mn_group_num_playback;
    int mn_session_playback;
    bool mb_connected_playback;
    bool mb_pause_playback;
    QMutex m_mutex_device_palyback;

    char *mp_recv_buf;
    int mi_recv_len;
};

#endif // NETCONTROL_H
