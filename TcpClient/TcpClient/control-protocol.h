#ifndef CONTROLPROTOCOL_H
#define CONTROLPROTOCOL_H

#include <pthread.h>
#include <string>

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

typedef struct {
    unsigned int msg_type;
    unsigned int total_len;
    unsigned int seq_id;
}MsgHeader;

typedef struct {
    MsgHeader header;
    unsigned int client_id;
}XmlReq;

typedef struct {
    MsgHeader header;
    unsigned char result;
    string xml_str;
}XmlResp;

typedef MsgHeader HeartBeatReq;
typedef MsgHeader HeartBeatResp;

typedef struct {
    MsgHeader header;
    unsigned int client_id;
}VodPlayReq;

typedef struct {
    MsgHeader header;
    unsigned char result;
    string url_str;
}VodPlayResp;

typedef struct {
    MsgHeader header;
    unsigned int client_id;
}VodStopReq;

typedef struct {
    MsgHeader header;
    unsigned char result;
}VodStopResp;

typedef union {
    XmlReq xmlreq;
    HeartBeatReq heartbeatreq;
    VodPlayReq vodplayreq;
    VodStopReq vodstopreq;
}ReqUnion;

typedef struct {
    ReqUnion req;
    unsigned int req_type;
}CtrlReq;

class Controller
{
public:
    Controller( int i_port );
    ~Controller();

    int init();
    int fini();

public:
    int pack_vod_stop_resp( VodStopReq vod_play_req );
    int parse_vod_stop_req( char *data, int len, VodStopReq &vod_stop_req);
    int pack_vod_play_resp( VodPlayReq vod_play_req );
    int parse_vod_play_req( char *data, int len, VodPlayReq &vod_play_req );
    int pack_heart_resp( HeartBeatReq heartbeat_req );
    int parse_heart_req( char *data, int len, HeartBeatReq &heartbeat_req );
    int pack_xml_resp( XmlReq xml_req );
    int parse_xml_req( char *data, int len, XmlReq &xml_req);
    int parse_data( char *data, int len );
    int check_alive();
private:
    bool mb_init;

    int mi_port;
    int mi_conn_fd;

    pthread_t mo_thread_id;
    pthread_attr_t mo_thread_attr;
};

#endif // CONTROLPROTOCOL_H
