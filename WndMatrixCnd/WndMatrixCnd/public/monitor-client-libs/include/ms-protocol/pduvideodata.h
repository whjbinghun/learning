#ifndef PDUVIDEODATA_H
#define PDUVIDEODATA_H

#include "iprotocol.h"
#include "netcommon.h"
#include "cpduheader.h"

#include "../logiclayer/datacommon.h"

///.视频帧数据
class CVideoDataFrameNotify : public CPduHeader
{
public:
    CVideoDataFrameNotify();
    virtual ~CVideoDataFrameNotify();

    virtual int type();
    virtual int length();
    virtual int encode(unsigned char* pbuff, int nlen);
    virtual int decode(unsigned char* pbuff, int nlen);
    virtual void dump();

private:
    bool mb_replay;

public:
    MONITOR_TIME_HEAD_S m_tm;
    int mn_buf_len;
    unsigned char *mp_buff;
};

///.视频温度数据
class CVideoDataTempNotify : public CPduHeader
{
public:
    CVideoDataTempNotify();
    virtual ~CVideoDataTempNotify();

    virtual int type();
    virtual int length();
    virtual int encode(unsigned char* pbuff, int nlen);
    virtual int decode(unsigned char* pbuff, int nlen);
    virtual void dump();

    int decode_temp( unsigned char *p_buff, int n_len );
    void transform_temp_info( const MONITOR_TEMPERATURE_PKT_S &temp_ana, TEMP_ANA_S &temp_data );
    void int_to_float( int n_temp, float &f_temp );

public:
    TEMP_ANA_S m_max_temp;
    TEMP_ANA_S m_min_temp;
    TEMP_ANA_S m_avg_temp;
    char m_sz_ana_name[1024];
};


///.视频调色板数据
class CVideoDataPaletteNotify : public CPduHeader
{
public:
    CVideoDataPaletteNotify();
    virtual ~CVideoDataPaletteNotify();

    virtual int type();
    virtual int length();
    virtual int encode(unsigned char* pbuff, int nlen);
    virtual int decode(unsigned char* pbuff, int nlen);
    virtual void dump();

public:
    MONITOR_ADJUST_DATA_S m_adjust_data;
};

class CVideoDataTimeNotify : public CPduHeader
{
public:
    CVideoDataTimeNotify();
    virtual ~CVideoDataTimeNotify();

    virtual int type();
    virtual int length();
    virtual int encode(unsigned char* pbuff, int nlen);
    virtual int decode(unsigned char* pbuff, int nlen);
    virtual void dump();

public:
   unsigned int mu_sec;
   unsigned int mu_usec;
};

#endif // PDUVIDEODATA_H
