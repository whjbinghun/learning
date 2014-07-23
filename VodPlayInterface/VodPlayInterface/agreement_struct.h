#ifndef AGREEMENT_STRUCT_H
#define AGREEMENT_STRUCT_H

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
    IR_MONITOR_GROUP_CFG_REQ     = 0x0020, /* 配置信息请求 */

    IR_MONITOR_VER_NEGO_RESP = IR_MONITOR_VER_NEGO_REQ + 0x8000,
    IR_MONITOR_LINK_AUTH_RESP = IR_MONITOR_LINK_AUTH_REQ + 0x8000,
    IR_MONITOR_LINK_HEART_RESP = IR_MONITOR_LINK_HEART_REQ + 0x8000,
    IR_MONITOR_LINK_REL_RESP = IR_MONITOR_LINK_REL_REQ + 0x8000,
    IR_MONITOR_XML_DATA_RESP = IR_MONITOR_XML_DATA_REQ + 0x8000,
    IR_MONITOR_VID_DATA_PLAY_RESP = IR_MONITOR_VID_DATA_PLAY_REQ + 0x8000,
    IR_MONITOR_VID_DATA_STOP_RESP = IR_MONITOR_VID_DATA_STOP_REQ + 0x8000,
    IR_MONITOR_PLATFORM_CTRL_RESP = IR_MONITOR_PLATFORM_CTRL_REQ + 0x8000,
    IR_MONITOR_GROUP_CFG_RESP     = IR_MONITOR_GROUP_CFG_REQ + 0x8000,
    IR_MONITOR_MSG_TYPE_BUTT = 0xFFFF
} IR_MONITOR_MSG_TYPE_E;

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

typedef struct _tagNetPackageHead{
    unsigned int n_msg_type;
    unsigned int n_total_len;       //总长度
    unsigned int n_seq_cmd;         //序列号
    _tagNetPackageHead() : n_msg_type( 0 ), n_total_len( 0 ), n_seq_cmd( 0 ){ }
}NetPackageHead, *PNetPackageHead;

typedef struct _tagNetPackageRev{
    NetPackageHead package_head;
    unsigned char by_data[1024];
    _tagNetPackageRev() {
        memset( by_data, 0, 1024 );
    }
} NetPackageRev, *PNetPackageRev;

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

typedef struct _tagDeviceGroup{
    DeviceInfo dev_ir;
    DeviceInfo dev_vi;
} DeviceGroup;

//视频播放
typedef struct {
    unsigned int seq_id;//序列号
    int n_class;
    int n_group_num;
}DevicePlayInfo;

#endif // AGREEMENT_STRUCT_H
