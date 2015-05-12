#ifndef PDUPRESETSETTING_H
#define PDUPRESETSETTING_H

#include "iprotocol.h"
#include "netcommon.h"
#include "cpduheader.h"

#include "../logiclayer/datacommon.h"


//云台设置速度请求
class CPtzSetSpeedReq : public CPduHeader
{
public:
    CPtzSetSpeedReq();
    virtual ~CPtzSetSpeedReq();

    virtual int type();
    virtual int length();
    virtual int encode(unsigned char* pbuff, int nlen);
    virtual int decode(unsigned char* pbuff, int nlen);
    virtual void dump();

public:
    int          mn_group_num;
    int          mn_speed;        // speed or preset index when del or gotp preset
};

class CPresetGotoReq : public CPduHeader
{
public:
    CPresetGotoReq();
    virtual ~CPresetGotoReq();

    virtual int type();
    virtual int length();
    virtual int encode(unsigned char* pbuff, int nlen);
    virtual int decode(unsigned char* pbuff, int nlen);
    virtual void dump();

public:
    unsigned int mn_group_num;              //云台所在组号
    unsigned short mn_ptz_preset_no;     //云台预置点号，填0表示由服务器分配
    unsigned short mn_rail_preset_no;     //导轨预置点号，填0表示由服务器分配
};

//设置预置位请求
class CPresetSetReq : public CPresetGotoReq
{
public:
    CPresetSetReq();
    virtual ~CPresetSetReq();

    virtual int type();
    virtual int length();
    virtual int encode(unsigned char* pbuff, int nlen);
    virtual int decode(unsigned char* pbuff, int nlen);
    virtual void dump();
    void cal_anas_len();
    int get_ana_len( AnaItem &it_ana );

public:
    char  m_sz_preset_name[128]; //别名，不超过127个字节
    int mn_ana_len;  //别名长度
    QVector< AnaItem > m_vct_anas;
};

class CPresetSetAck : public CBaseAck
{
public:
    CPresetSetAck();
    virtual ~CPresetSetAck();

    virtual int type();
    virtual int length();
    virtual int encode(unsigned char* pbuff, int nlen);
    virtual int decode(unsigned char* pbuff, int nlen);
    virtual void dump();

public:
    unsigned short mn_ptz_preset_no;
    unsigned short mn_rail_preset_no;
};

class CPresetAnaAddReq : public CPresetGotoReq
{
public:
    CPresetAnaAddReq();
    virtual ~CPresetAnaAddReq();

    virtual int type();
    virtual int length();
    virtual int encode(unsigned char* pbuff, int nlen);
    virtual int decode(unsigned char* pbuff, int nlen);
    virtual void dump();

    void set_anas_buf( char *p_buf, int n_len_buf );
    void clear_anas_buf();

public:
    char m_sz_name[128];
    char *mp_anas_buf;
    int mn_len_buf;
};

class CPresetAnaDelReq : public CPresetGotoReq
{
public:
    CPresetAnaDelReq();
    virtual ~CPresetAnaDelReq();

    virtual int type();
    virtual int length();
    virtual int encode(unsigned char* pbuff, int nlen);
    virtual int decode(unsigned char* pbuff, int nlen);
    virtual void dump();

public:
    unsigned int mn_ana_id;
};

class CPresetAnaChangeReq : public CPresetAnaDelReq
{
public:
    CPresetAnaChangeReq();
    virtual ~CPresetAnaChangeReq();

    virtual int type();
    virtual int length();
    virtual int encode(unsigned char* pbuff, int nlen);
    virtual int decode(unsigned char* pbuff, int nlen);
    virtual void dump();

    void set_anas_buf( char *p_buf, int n_len_buf );
    void clear_anas_buf();
public:
    char m_sz_name[128];
    char *mp_anas_buf;
    int mn_len_buf;
};

//. 报警条件获取
class CPresetAlarmGetReq : public CPresetGotoReq
{
public:
    CPresetAlarmGetReq();
    virtual ~CPresetAlarmGetReq();

    virtual int type();
    virtual int length();
    virtual int encode(unsigned char* pbuff, int nlen);
    virtual int decode(unsigned char* pbuff, int nlen);
    virtual void dump();

public:
    unsigned int mn_alarm_id;
};

//. 报警条件设置和新增
class CPresetAlarmSetReq : public CPresetAlarmGetReq
{
public:
    CPresetAlarmSetReq();
    virtual ~CPresetAlarmSetReq();

    virtual int type();
    virtual int length();
    virtual int encode(unsigned char* pbuff, int nlen);
    virtual int decode(unsigned char* pbuff, int nlen);
    virtual void dump();

    void set_alarm_buf( char *p_buf, int n_len_buf );
    void clear_alarm_buf();
public:
    char m_sz_name[128];
    char *mp_alarm_buf;
    int mn_len_buf;
};

//. 报警条件设置和新增返回
class CPresetAlarmSetAck : public CBaseAck
{
public:
    CPresetAlarmSetAck();
    virtual ~CPresetAlarmSetAck();

    virtual int type();
    virtual int length();
    virtual int encode(unsigned char* pbuff, int nlen);
    virtual int decode(unsigned char* pbuff, int nlen);
    virtual void dump();

public:
    unsigned int mn_alarm_id;
};

//. 报警条件获取返回
class CPresetAlarmGetAck : public CBaseAck
{
public:
    CPresetAlarmGetAck();
    virtual ~CPresetAlarmGetAck();

    virtual int type();
    virtual int length();
    virtual int encode(unsigned char* pbuff, int nlen);
    virtual int decode(unsigned char* pbuff, int nlen);
    virtual void dump();

public:
    QMap<int, PresetAlarmCondition> m_map_alarm;
};

//. 报警图片列表获取
class CPresetAlarmPicListReq : public CPresetGotoReq
{
public:
    CPresetAlarmPicListReq();
    virtual ~CPresetAlarmPicListReq();

    virtual int type();
    virtual int length();
    virtual int encode(unsigned char* pbuff, int nlen);
    virtual int decode(unsigned char* pbuff, int nlen);
    virtual void dump();

public:
    char m_sz_start_time[14];
    char m_sz_end_time[14];
    QVector< unsigned int > m_vct_alarm_id;
};

//. 报警图片信息
class CPresetAlarmPicInfoReq : public CPresetAlarmGetReq
{
public:
    CPresetAlarmPicInfoReq();
    virtual ~CPresetAlarmPicInfoReq();

    virtual int type();
    virtual int length();
    virtual int encode(unsigned char* pbuff, int nlen);
    virtual int decode(unsigned char* pbuff, int nlen);
    virtual void dump();

public:
    unsigned int mn_time_sec;
    unsigned int mn_time_msec;
};

class CPresetAlarmPicListAck : public CBaseAck
{
public:
    CPresetAlarmPicListAck();
    virtual ~CPresetAlarmPicListAck();

    virtual int type();
    virtual int length();
    virtual int encode(unsigned char* pbuff, int nlen);
    virtual int decode(unsigned char* pbuff, int nlen);
    virtual void dump();

public:
    QVector< AlarmPicTime > m_vct_pic_time;
};

class CPresetAlarmPicInfoAck : public CBaseAck
{
public:
    CPresetAlarmPicInfoAck();
    virtual ~CPresetAlarmPicInfoAck();

    virtual int type();
    virtual int length();
    virtual int encode(unsigned char* pbuff, int nlen);
    virtual int decode(unsigned char* pbuff, int nlen);
    virtual void dump();

public:
    QVector< AlarmPicInfo > m_vct_pic_info;
};

//. 获取预置点存储条件
class CPresetRecordGetAck : public CBaseAck
{
public:
    CPresetRecordGetAck();
    virtual ~CPresetRecordGetAck();

    virtual int type();
    virtual int length();
    virtual int encode(unsigned char* pbuff, int nlen);
    virtual int decode(unsigned char* pbuff, int nlen);
    virtual void dump();

public:
    unsigned int mn_group_num;
    unsigned short ms_rail_num;
    unsigned short ms_ptz_num;
    unsigned int mn_interval_msec; //.间隔时间
    float mf_interva_diff_temp;         //.间隔温差
    float mf_absolute_diff_temp;      //.绝对温差
};

class CPresetRecordSetReq : public CPduHeader
{
public:
    CPresetRecordSetReq();
    virtual ~CPresetRecordSetReq();

    virtual int type();
    virtual int length();
    virtual int encode(unsigned char* pbuff, int nlen);
    virtual int decode(unsigned char* pbuff, int nlen);
    virtual void dump();

public:
    unsigned int mn_group_num;
    unsigned short ms_rail_num;
    unsigned short ms_ptz_num;
    unsigned int mn_interval_msec; //.间隔时间
    float mf_interva_diff_temp;         //.间隔温差
    float mf_absolute_diff_temp;      //.绝对温差
};

//. 通过时间获取图片请求
class CGetPicOnTimeReq : public CPduHeader
{
public:
    CGetPicOnTimeReq();
    virtual ~CGetPicOnTimeReq();

    virtual int type();
    virtual int length();
    virtual int encode( unsigned char* pbuff, int nlen );
    virtual int decode( unsigned char* pbuff, int nlen );
    virtual void dump();

public:
    unsigned int mn_group_num;
    unsigned int mn_utc_sec;
    unsigned int mn_utc_msec;
};

typedef CPresetGotoReq CPresetRecordGetReq;
typedef CPresetGotoReq CPresetDelReq;           //删除预置点
typedef CPresetGotoReq CPresetSetPosReq;      //.重新设置预置点位置
typedef CPtzSetSpeedReq CPtzPresetCtrlReq;  //

typedef CPresetAlarmGetReq CPresetAlarmDelReq;  //.删除报警条件

typedef CPresetAlarmSetAck CPresetAnaAddAck;
typedef CBaseAck CPtzDelPresetAck ;
typedef CBaseAck CPtzGoToPresetAck ;
typedef CBaseAck CPtzSetSpeedAck;

typedef CPresetAlarmPicInfoAck CGetPicOnTimeAck;

#endif // PDUPRESETSETTING_H
