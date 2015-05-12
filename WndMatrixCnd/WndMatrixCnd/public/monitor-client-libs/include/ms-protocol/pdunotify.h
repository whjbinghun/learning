#ifndef PDUNOTIFY_H
#define PDUNOTIFY_H

#include "iprotocol.h"
#include "netcommon.h"
#include "cpduheader.h"

//巡航停止通知
class CCruiseStopNotify : public CPduHeader
{
public:
    CCruiseStopNotify();
    virtual ~CCruiseStopNotify();

    virtual int type();
    virtual int length();
    virtual int encode(unsigned char* pbuff, int nlen);
    virtual int decode(unsigned char* pbuff, int nlen);
    virtual void dump();

public:
    int mn_group_num;
};

class CCruiseStartNotify : public CPduHeader
{
public:
    CCruiseStartNotify();
    virtual ~CCruiseStartNotify();

    virtual int type();
    virtual int length();
    virtual int encode(unsigned char* pbuff, int nlen);
    virtual int decode(unsigned char* pbuff, int nlen);
    virtual void dump();

public:
    int mn_group_num;
    int mn_cruise;
};

//到达预置点通知
class CCruiseGetToPresetNotify : public CCruiseStopNotify
{
public:
    CCruiseGetToPresetNotify();
    virtual ~CCruiseGetToPresetNotify();

    virtual int type();
    virtual int length();
    virtual int encode(unsigned char* pbuff, int nlen);
    virtual int decode(unsigned char* pbuff, int nlen);
    virtual void dump();

public:
    unsigned short mn_rail_num;
    unsigned short mn_ptz_num;
};

//离开预置点通知
class CCruiseLeavePresetNotify : public CCruiseStopNotify
{
public:
    CCruiseLeavePresetNotify();
    virtual ~CCruiseLeavePresetNotify();

    virtual int type();
    virtual int length();
    virtual int encode(unsigned char* pbuff, int nlen);
    virtual int decode(unsigned char* pbuff, int nlen);
    virtual void dump();

public:
    unsigned short mn_leave_rail_num;
    unsigned short mn_leave_ptz_num;

    unsigned short mn_go_rail_num;
    unsigned short mn_go_ptz_num;
};

//视频异常时服务器通知客户端机制（此消息服务端发起，客户端无需回复）
class CReplayDataErrorNotify : public CPduHeader
{
public:
    CReplayDataErrorNotify();
    virtual ~CReplayDataErrorNotify();

    virtual int type();
    virtual int length();
    virtual int encode( unsigned char* pbuff, int nlen );
    virtual int decode( unsigned char* pbuff, int nlen );
    virtual void dump();

public:
    unsigned int mu_session;
    int mn_error;
};


#endif // PDUNOTIFY_H
