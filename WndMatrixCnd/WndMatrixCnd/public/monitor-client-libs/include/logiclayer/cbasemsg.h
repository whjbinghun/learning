#ifndef CBASEMSG_H
#define CBASEMSG_H
#include <map>
#include <QVariant>
#include <QTime>

using namespace std;

#define MAX_DFAULT_TIMEOUT  8000

enum MsgType
{
    MsgUnkonow,
    MsgRequest,
    MsgResponse,
    MsgNotify,
};

enum  MsgResult
{
    ResultUnkonow,
    ResultTimeout,
    ResultParamErr,
    ResultSendFailed,
    ResultFailed=-1,
    ResultOk=0,
};

enum MsgErr
{
    ERR_MSG_LOGIN_NO_USER = -4,         //. 登入没有用户
    ERR_MSG_LOGIN_PASSWORD = -5,        //. 登入密码错误
    ERR_MSG_VER_NEGO_HIGH = -10,        //. 版本过高
    ERR_MSG_VER_NEGO_LOW = -11,         //. 版本过低
    ERR_VOD_CRUISE_HAVE_STARTED = -2000 //. 代表此时巡航已存在，不再响应巡航开始消息
};

enum  MsgCmd
{
    NotifyBegin,
    ServerOnLine,
    ServerOffLine,
    PeleteRang,
    NotifyEnd,

    MsgCmdKonow,
    SessionCmdStart,
    SessionCmdLogin,
    SessionCmdVersion,
    SessionCmdHeartbeat,
    SessionCmdEnd,

    ConfigCmdStart,
    ConfigAnaAdd,
    ConfigAnaDel,
    ConfigAnaLoad,
    ConfigGetPeleteIndex,
    ConfigSetPeleteIndex,
    ConfigSetPeleteRange,
    ConfigSetPeleteAuto,
    ConfigAddGroup,
    ConfigDelGroup,
    ConfigChangeGroup,
    ConfigGetXml,
    ConfigCmdEnd,

    PtzRailCmdStart,
    PtzCmdControl,
    PtzCmdPreset,
    PtzCmdSetSpeed,
    PresetSetPreset,
    PresetSetPos,
    PresetCallPreset,
    PresetDelPreset,
    PresetGetInfo,
    PresetAnaAdd,
    PresetAnaDel,
    PresetAnaChange,
    PresetAlarmGet,
    PresetAlarmAdd,
    PresetAlarmDel,
    PresetAlarmChange,
    PresetRecordConditionGet,
    PresetRecordConditionSet,
    CruiseSetInfo,
    CruiseGetInfo,
    CruiseDel,
    CruiseStart,
    CruiseStop,
    CruiseCurPreset,
    PicInfoSet,
    PicSet,
    PicGet,
    PicDel,
    RailCmdControl,
    RailCmdDelPreset,
    RailCmdSetPreset,
    RailCmdGotoPreset,
    RailCmdGotoPos,
    RailCmdSetSpeed,
    RailCmdGetSpeedPos,
    PtzRailCmdEnd,

    StartRealPlayFrame,         //. 开始单帧视频请求
    StopRealPlayFrame,          //. 停止单帧视频
    StartRealPlay,              //. 开始实时视频传输请求
    StopRealPlay,               //. 停止实时视频传输

    IRFocusOut,
    IRFocusIn,
    IRCorrect,

    VIFocusOut,
    VIFocusIn,
    VIFocusInStop,
    VIFocusOutStop,

    NotifyCruiseStart,
    NotifyCruiseStop,
    NotifyLeavePreset,
    NotifyGetToPreset,
    NotifyIRAnaTempUpdate,

    NotifyNetConneted, //. 视频连接通知
    NotifyReplayVideoEnd,    //. 视频数据异常结束通知
    NotifyReplayConnError,
    NotifyReplayFrameTime,

    //2014.11.16 blm add
    RePlayNum,   //回放个数
    RePlayBuild, //建立请求
    RePlayBuildNotify, //
    RePlayBuildSuss,
    RePlayStart,      //回放请求
    RePlayPause, //回放暂停
    RePlayResume, //回放恢复
    RePlayStop,   //停止
    RePlayHeartBeat, //心跳
    RePlayGetPeleteType, //调色板类型获取
    RePlayFrame,  //回放帧
    RePlayPreset,  //回放预置点信息
    RePlaySetPeleteType, //调色板类型获取
    RePlaySetPeleteRange, //回放中手动设置色标温度范围请求
    RePlayTypeManualOrAuto,   //回放中设置色标类型手动还是自动请求
    RePlayInit,   //播放初始化

    PresetAlarmPicList,
    PresetAlarmPicInfo,

    PicGetOnTime        //. 通过时间获取图片
};


enum PtzDirect
{
    PtzLeft=0x0060,
    PtzRight,
    PtzUp,
    PtzDown,
    PtzLeftUp,
    PtzRightUp,
    PtzLeftDwon,
    PtzRightDown,
    PtzStop,
};

enum RailDirect
{
    RailGoHome=0x0041, // call back
    RailStepLeft=0x0043,
    RailStepRight,
    RailLeft,
    RailRight,
    RailStop,
    RailCheckLength=0x004b,
    RailGetSpeedPos=0x004c,
};

enum PtzPresetCmd
{
     PtzSetSpeed= 0x006a,
     PtzGotoPreset,
     PtzDelPreset,
     PtzCurise,
};

enum DeviceType{
    DeviceVI = 0L,
    DeviceIR = 1L,
    DevicePTZ = 2L,
    DeviceRailH = 3L,
    DeviceRailV = 4L
};

enum FuntionType{
    FuntionRealtime = 0L,
    FuntionReplay = 1L
};

enum ClientType{
    ClientNone = 0L,
    ClientConfig = 1L,
    ClientPresetSet = 2L,
    ClientMonitor = 3L,
    ClientReplay = 4L
};

typedef map<int, QVariant> MsgData;

#define MSGDATA_USRINFO     0
#define MSGDATA_VIEWID      1
#define MSGDATA_NETRESULT   2

#define MSGDATA_SN          3
#define MSGDATA_COUNT       4
#define MSGDATA_GROUPNUM    5
#define MSGDATA_STR_ANA     6
#define MSGDATA_TYPE        7  //类型
#define MSGDATA_INDEX       8
#define MSGDATA_MAX         9
#define MSGDATA_MIN         10
#define MSGDATA_AJUST       11
#define MSGDATA_URL         12
#define MSGDATA_SPEED       13
#define MSGDATA_POS         14
#define MSGDATA_DIRECT      15
#define MSGDATA_PRESETCMD   16
#define MSGDATA_NAME        17
#define MSGDATA_RAIL_LENGTH  18
#define MSGDATA_ANA          20
#define MSGDATA_ANA_ID        21
#define MSGDATA_RANGE_MIN    22
#define MSGDATA_RANGE_MAX    23
#define MSGDATA_RAIL_PRESET  24
#define MSGDATA_PTZ_PRESET  25

//2014.11.16 blm add
#define MSGDATA_REPLAYNUM  26  //回放个数
//#define MSGDATA_GROUPNO    27  //组号
#define MSGDATA_GROUPNAME  28  //组名
#define MSGDATA_REPLAYDATA    29  //请求回放结构体
#define MSGDATA_SESSION   30  //会话标志
#define MSGDATA_FRAME         31   //帧号
#define MSGDATA_PRESET        32   //预置点播放信息
#define MSGDATA_COLOR_INDEX   33   //调色板类型
#define MSGDATA_RANGE         34   //回放中手动设置色标温度范围请求
#define MSGDATA_ADJUST        35   //回放中设置色标类型手动还是自动请求

#define MSGDATA_VCT_URL       36   //回放响应中URL存储
#define MSGDATA_FILR_NUM      37   //文件个数
#define MSGDATA_TIMELIST      38   //存放时间对
#define MSGDATA_ERROR         39   //异常结束
#define MSGDATA_REQ_ID            40

#define MSGDATA_ALARM_CONDITION 50      //.报警条件
#define MSGDATA_ALARM_ID        51                 //.报警条件id

#define MSGDATA_INTERVAL 61                  //.预置点存储间隔时间
#define MSGDATA_INTERVAL_TEMP 62      //.预置点存储间隔温度
#define MSGDATA_HRESHOLD   63             //.存储间隔阈值

#define MSGDATA_CHANGE_ANA 70          //.是否修改分析
#define MSGDATA_IR_PATH 71          //.报警红外图片路径
#define MSGDATA_VI_PATH 72          //.报警可见光图片路径

#define MSGDATA_CLEAR_REPLAY_BUF 80 //.清除回放缓存
#define MSGDATA_REPLAY_PAUSE     81 //.播放暂停
#define MSGDATA_STATUS        82    //.状态信息

#define MSGDATA_SECONDS         90   //.秒
#define MSGDATA_MILLSECOND      91   //.毫秒


//#define MSGDATA_


class CBaseMsg
{
public:
    CBaseMsg();
    virtual ~CBaseMsg();

    int  dump();

public:
    int   m_nType;         //参考  MsgType
    int   m_nSeq;          //消息序号  每个模块内部要唯一
    int   m_nFromMdl;      //消息源模块
    int   m_nTomdl;        //消息目标模块
    int   m_nExpire;       //消息超时时间 单位毫秒
    QTime m_timeEnqueue;   //消息入队时刻
    int   m_nCmd;           //具体消息命令 MsgCmd
    int   m_nResult;        //应答消息的出来结果 <0 失败
    MsgData   map_msgdata; //消息携带的数据 为了效率不要放大块内存数据
};

#endif // CBASEMSG_H
