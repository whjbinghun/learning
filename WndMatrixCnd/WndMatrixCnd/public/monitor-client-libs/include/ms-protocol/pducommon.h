#ifndef PDUCOMMON_H
#define PDUCOMMON_H
#include "iprotocol.h"
#include "netcommon.h"
#include "cpduheader.h"

#include "../logiclayer/datacommon.h"

//版本协商请求
class CPduVerNegoReq : public CPduHeader
{
public:
    CPduVerNegoReq();
    virtual ~CPduVerNegoReq();

    virtual int type();
    virtual int length();
    virtual int encode(unsigned char* pbuff, int nlen);
    virtual int decode(unsigned char* pbuff, int nlen);
    virtual void dump();

public:
    VerNegoReq m_ver;
};
typedef CBaseAck CPduVerNegoAck;

//用户鉴权请求
class CPduLinkAuthReq : public CPduHeader
{
public:
    CPduLinkAuthReq();
    virtual ~CPduLinkAuthReq();

    virtual int type();
    virtual int length();
    virtual int encode(unsigned char* pbuff, int nlen);
    virtual int decode(unsigned char* pbuff, int nlen);
    virtual void dump();

public:
    LinkAuthReq m_auth;
    QString     mstr_pwd;
    int mn_client_type;
};

class CAuthAck : public CBaseAck
{
public:
    CAuthAck();
    virtual ~CAuthAck();

    virtual int type();
    virtual int length();
    virtual int encode(unsigned char* pbuff, int nlen);
    virtual int decode(unsigned char* pbuff, int nlen);
    virtual void dump();

public:
    int    m_clientid;
    int    m_nLevel;
};
//XML文件请求
class CPduXmlReq : public CPduHeader
{
public:
    CPduXmlReq();
    virtual ~CPduXmlReq();

    virtual int type();
    virtual int length();
    virtual int encode(unsigned char* pbuff, int nlen);
    virtual int decode(unsigned char* pbuff, int nlen);
    virtual void dump();

public:
    XmlReq m_xmlreq;
};

typedef CPduXmlReq   CPduHeartbeatReq;
//配置文件获取应答
class CGroupXmlAck : public CBaseAck
{
public:
    CGroupXmlAck();
    virtual ~CGroupXmlAck();

    virtual int type();
    virtual int length();
    virtual int encode(unsigned char* pbuff, int nlen);
    virtual int decode(unsigned char* pbuff, int nlen);
    virtual void dump();

public:
   char*        m_pxml;
};

//配置信息新增请求
class CAddGroupReq : public CPduHeader
{
public:
    CAddGroupReq();
    virtual ~CAddGroupReq();

    virtual int type();
    virtual int length();
    virtual int encode(unsigned char* pbuff, int nlen);
    virtual int decode(unsigned char* pbuff, int nlen);
    virtual void dump();

public:
    int mn_count;
    char *mp_sns;  //split device sn by ;
    char *mp_str_name;
};

//配置信息新增应答
class CAddGroupAck : public CBaseAck
{
public:
    CAddGroupAck();
    virtual ~CAddGroupAck();

    virtual int type();
    virtual int length();
    virtual int encode(unsigned char* pbuff, int nlen);
    virtual int decode(unsigned char* pbuff, int nlen);
    virtual void dump();

public:
    int    m_nGroupNum;
};
//配置信息删除请求
class CDelGroupReq : public CPduHeader
{
public:
    CDelGroupReq();
    virtual ~CDelGroupReq();

    virtual int type();
    virtual int length();
    virtual int encode(unsigned char* pbuff, int nlen);
    virtual int decode(unsigned char* pbuff, int nlen);
    virtual void dump();

public:
    int          m_nGroupNum;
};

typedef CBaseAck   CDelGroupAck;

//配置信息变更请求
class CChangeGroupReq : public CPduHeader
{
public:
    CChangeGroupReq();
    virtual ~CChangeGroupReq();

    virtual int type();
    virtual int length();
    virtual int encode(unsigned char* pbuff, int nlen);
    virtual int decode(unsigned char* pbuff, int nlen);
    virtual void dump();

public:
    int  mn_group_num;
    int  mn_count;
    char *mp_sns;  //split device sn by ;
    char *mp_str_name;
};

typedef CBaseAck   CChangeGroupAck;




//红外数据分析新增请求
class CAnaCtrlReq : public CPduHeader
{
public:
    CAnaCtrlReq();
    virtual ~CAnaCtrlReq();

    virtual int type();
    virtual int length();
    virtual int encode(unsigned char* pbuff, int nlen);
    virtual int decode(unsigned char* pbuff, int nlen);
    virtual void dump();

public:
    AnaCtrlReq m_ana_ctrl;
};
typedef CBaseAck  CAnaCtrlAck ;

//云台向左请求
class CPtzCtrlReq : public CPduHeader
{
public:
    CPtzCtrlReq();
    virtual ~CPtzCtrlReq();

    virtual int type();
    virtual int length();
    virtual int encode(unsigned char* pbuff, int nlen);
    virtual int decode(unsigned char* pbuff, int nlen);
    virtual void dump();

public:
    int  m_nGroupNum;
};
typedef CBaseAck   CPtzCtrlAck ;


//设置图片请求
class CSetPicReq : public CPduHeader
{
public:
    CSetPicReq();
    ~CSetPicReq();

    virtual int type();
    virtual int length();
    virtual int encode(unsigned char* pbuff, int nlen);
    virtual int decode(unsigned char* pbuff, int nlen);
    virtual void dump();

public:
    int mn_group_num;
    int mn_len_buf;
    unsigned char *mp_buf_image;
};

class CSetPicInfoReq : public CPduHeader
{
public:
    CSetPicInfoReq();
    ~CSetPicInfoReq();

    virtual int type();
    virtual int length();
    virtual int encode(unsigned char* pbuff, int nlen);
    virtual int decode(unsigned char* pbuff, int nlen);
    virtual void dump();

public:
    int mn_group_num;
    QVector< PresetItem > m_vct_preset;
};

typedef CDelGroupReq CGetPicReq;
typedef CDelGroupReq CDelPicReq;

class CGetPicAck : public CBaseAck
{
public:
    CGetPicAck();
    ~CGetPicAck();

    virtual int type();
    virtual int length();
    virtual int encode(unsigned char* pbuff, int nlen);
    virtual int decode(unsigned char* pbuff, int nlen);
    virtual void dump();

public:
    int mn_len_buf;
    unsigned char *mp_buf_image;
    QVector< PresetItem > m_vct_preset;
};

//新增或修改巡航方案请求
class CCruiseAddReq : public CPduHeader
{
public:
    CCruiseAddReq();
    virtual ~CCruiseAddReq();

    virtual int type();
    virtual int length();
    virtual int encode(unsigned char* pbuff, int nlen);
    virtual int decode(unsigned char* pbuff, int nlen);
    virtual void dump();

public:
    unsigned int mn_group_num;
    unsigned int mn_crusie_num;
    char m_sz_crusie_name[128];
    QVector< CruisePoint > m_list_cruise_pts;
};

//删除巡航方案请求
class CCruiseDelReq : public CPduHeader
{
public:
    CCruiseDelReq();
    virtual ~CCruiseDelReq();

    virtual int type();
    virtual int length();
    virtual int encode(unsigned char* pbuff, int nlen);
    virtual int decode(unsigned char* pbuff, int nlen);
    virtual void dump();

public:
    unsigned int mn_group_num;
    unsigned int mn_crusie_num;
};

//新增或修改巡航方案应答
class CCruiseAddAck : public CBaseAck
{
public:
    CCruiseAddAck();
    ~CCruiseAddAck();

    virtual int type();
    virtual int length();
    virtual int encode(unsigned char* pbuff, int nlen);
    virtual int decode(unsigned char* pbuff, int nlen);
    virtual void dump();

public:
    unsigned int mn_crusie_num;
};

typedef CDelGroupReq CCruiseGetReq;
typedef CDelGroupReq CPresetGetReq;
typedef CDelGroupReq CCruiseGetCurReq;

//获取巡航信息应答
class CCruiseGetAck : public CBaseAck
{
public:
    CCruiseGetAck();
    ~CCruiseGetAck();

    virtual int type();
    virtual int length();
    virtual int encode(unsigned char* pbuff, int nlen);
    virtual int decode(unsigned char* pbuff, int nlen);
    virtual void dump();

public:
    QVector< CruiseSchemeInfo > m_map_scheme;
};

//获取组中预置点信息应答
class CPresetGetAck : public CBaseAck
{
public:
    CPresetGetAck();
    ~CPresetGetAck();

    virtual int type();
    virtual int length();
    virtual int encode(unsigned char* pbuff, int nlen);
    virtual int decode(unsigned char* pbuff, int nlen);
    virtual void dump();

public:
    QVector< PresetPt > m_vct_preset;
};

class CCruiseGetCurAck : public CBaseAck
{
public:
    CCruiseGetCurAck();
    ~CCruiseGetCurAck();

    virtual int type();
    virtual int length();
    virtual int encode(unsigned char* pbuff, int nlen);
    virtual int decode(unsigned char* pbuff, int nlen);
    virtual void dump();

public:
    unsigned int mn_cruise_num;
    unsigned short mn_rail_num;
    unsigned short mn_ptz_num;
};

class CStopCruiseReq : public CPduHeader
{
public:
    CStopCruiseReq();
    virtual ~CStopCruiseReq();

    virtual int type();
    virtual int length();
    virtual int encode(unsigned char* pbuff, int nlen);
    virtual int decode(unsigned char* pbuff, int nlen);
    virtual void dump();

public:
    int          m_nGroupNum;
    int          m_nCrusieIndex;
};


//实时中设置调色板类型应答
class CGetLivePeleteAck : public CBaseAck
{
public:
    CGetLivePeleteAck();
    virtual ~CGetLivePeleteAck();

    virtual int type();
    virtual int length();
    virtual int encode(unsigned char* pbuff, int nlen);
    virtual int decode(unsigned char* pbuff, int nlen);
    virtual void dump();

public:
    int    m_nColorIndex;
};
//实时中设置调色板类型
class CSetLivePeleteReq : public CPduHeader
{
public:
    CSetLivePeleteReq();
    virtual ~CSetLivePeleteReq();

    virtual int type();
    virtual int length();
    virtual int encode(unsigned char* pbuff, int nlen);
    virtual int decode(unsigned char* pbuff, int nlen);
    virtual void dump();

public:
    unsigned  int  m_nColor;
    char           device_sn[128];
};
typedef CBaseAck   CSetLivePeleteAck ;
//实时中手动设置色标温度范围
class CSetLivePeleteRangeReq : public CPduHeader
{
public:
    CSetLivePeleteRangeReq();
    virtual ~CSetLivePeleteRangeReq();

    virtual int type();
    virtual int length();
    virtual int encode(unsigned char* pbuff, int nlen);
    virtual int decode(unsigned char* pbuff, int nlen);
    virtual void dump();

public:
    float f_max;
    float f_min;
    char sz_sn[1024];
};
typedef CBaseAck   CSetLivePeleteRangeAck ;

//实时中设置色标类型手动还是自动
class CSetLivePleleteAutoReq : public CPduHeader
{
public:
    CSetLivePleleteAutoReq();
    virtual ~CSetLivePleleteAutoReq();

    virtual int type();
    virtual int length();
    virtual int encode(unsigned char* pbuff, int nlen);
    virtual int decode(unsigned char* pbuff, int nlen);
    virtual void dump();

public:
    int n_adjust;
    char sz_sn[1024];
};
typedef CBaseAck   CSetLivePleleteAutoAck ;

//导轨单步向左请求
class CRailCtrlReq : public CPduHeader
{
public:
    CRailCtrlReq();
    virtual ~CRailCtrlReq();

    virtual int type();
    virtual int length();
    virtual int encode(unsigned char* pbuff, int nlen);
    virtual int decode(unsigned char* pbuff, int nlen);
    virtual void dump();

public:
    char m_sz_rail_sn[1024];
};
typedef CBaseAck   CRailCtrlAck ;

//导轨设置预置点请求
class CSetRailPresetReq : public CPduHeader
{
public:
    CSetRailPresetReq();
    virtual ~CSetRailPresetReq();

    virtual int type();
    virtual int length();
    virtual int encode(unsigned char* pbuff, int nlen);
    virtual int decode(unsigned char* pbuff, int nlen);
    virtual void dump();

public:
      int   m_nGroupNum;
      int   m_nPresetIndex;
      int   m_nNameLen;
      char  m_szName[1024];
};

//导轨设置预置点应答
class CSetRailPresetAck : public CBaseAck
{
public:
    CSetRailPresetAck();
    virtual ~CSetRailPresetAck();

    virtual int type();
    virtual int length();
    virtual int encode(unsigned char* pbuff, int nlen);
    virtual int decode(unsigned char* pbuff, int nlen);
    virtual void dump();

public:
    int    m_nRailPresetIndex;
};

//导轨删除预置位请求
class CDelRailPresetReq : public CPduHeader
{
public:
    CDelRailPresetReq();
    virtual ~CDelRailPresetReq();

    virtual int type();
    virtual int length();
    virtual int encode(unsigned char* pbuff, int nlen);
    virtual int decode(unsigned char* pbuff, int nlen);
    virtual void dump();

public:
    char m_sz_rail_sn[1024];
    int mn_value;
};
typedef CBaseAck   CDelRailPresetAck ;

typedef CDelRailPresetReq  CRailGotoPresetReq;
typedef CBaseAck   CRailGotoPresetAck ;

typedef CDelRailPresetReq  CRailGotoDistanceReq;
typedef CBaseAck   CRailGotoDistanceAck ;

typedef CDelRailPresetReq  CRailSetSpeedReq;
typedef CBaseAck   CRailSetSpeedAck ;

typedef CSetRailPresetAck   CGetRailLengthAck;
typedef CSetRailPresetAck   CSetPtzPresetAck;

//获取导轨当前位置及速度
class CGetRailSpeedPosAck : public CBaseAck
{
public:
    CGetRailSpeedPosAck();
    virtual ~CGetRailSpeedPosAck();

    virtual int type();
    virtual int length();
    virtual int encode(unsigned char* pbuff, int nlen);
    virtual int decode(unsigned char* pbuff, int nlen);
    virtual void dump();

public:
    int mn_rail_pos;
    int mn_rail_speed;
    int mn_rail_len;
};

//导轨传输信息应答
class CRailTrandsInfoAck : public CBaseAck
{
public:
    CRailTrandsInfoAck();
    virtual ~CRailTrandsInfoAck();

    virtual int type();
    virtual int length();
    virtual int encode(unsigned char* pbuff, int nlen);
    virtual int decode(unsigned char* pbuff, int nlen);
    virtual void dump();

public:
    int    m_nLen;
    char   *m_pData;
};

//红外焦距调远请求
class CIRFousCtrlReq : public CPduHeader
{
public:
    CIRFousCtrlReq();
    virtual ~CIRFousCtrlReq();

    virtual int type();
    virtual int length();
    virtual int encode(unsigned char* pbuff, int nlen);
    virtual int decode(unsigned char* pbuff, int nlen);
    virtual void dump();

public:
    unsigned int mn_client_id;
    char ma_sn[1024];
};


//视频数据播放请求
class CPduVodPlayReq : public CPduHeader
{
public:
    CPduVodPlayReq();
    virtual ~CPduVodPlayReq();

    virtual int type();
    virtual int length();
    virtual int encode(unsigned char* pbuff, int nlen);
    virtual int decode(unsigned char* pbuff, int nlen);
    virtual void dump();

public:
   char device_sn[128];
};

//视频数据播放应答
class CVodPlayAck : public CBaseAck
{
public:
    CVodPlayAck();
    virtual ~CVodPlayAck();

    virtual int type();
    virtual int length();
    virtual int encode(unsigned char* pbuff, int nlen);
    virtual int decode(unsigned char* pbuff, int nlen);
    virtual void dump();

public:
   char   *p_strUrl;
};

typedef CPduVodPlayReq   CPduVodStopReq ;
typedef CBaseAck         CPduVodStopAck ;
typedef CPduVodPlayReq   CPduFrameVodPlayReq ;
typedef CPduVodPlayReq   CPduFrameVodStopReq ;

typedef CPduVodPlayReq   CGetLivePeleteReq;

//回放通信协议编解码
//blm add 2014.11.13

//回放个数请求
class CReplaySetNumReq : public CPduHeader
{
public:
       CReplaySetNumReq();
        virtual ~CReplaySetNumReq();

        virtual int type();
        virtual int length();
        virtual int encode(unsigned char* pbuff, int nlen);
        virtual int decode(unsigned char* pbuff, int nlen);
        virtual void dump();
public:
         int mn_replay;
};

//回放个数应答
typedef CBaseAck CRePlaySetNumAck;

//建立请求
class CReplayBuildReq : public CPduHeader
{
public:
       CReplayBuildReq();
        virtual ~CReplayBuildReq();

        virtual int type();
        virtual int length();
        virtual int encode(unsigned char* pbuff, int nlen);
        virtual int decode(unsigned char* pbuff, int nlen);
        virtual void dump();
public:
         int  mn_group;  //组号
         int   mn_namelen;  //组别名长度
         char  mp_name[128];    //组别名

};


//建立请求应答
class CReplayBuildAck : public CBaseAck
{
public:
    CReplayBuildAck();
    virtual ~CReplayBuildAck();

    virtual int type();
    virtual int length();
    virtual int encode(unsigned char* pbuff, int nlen);
    virtual int decode(unsigned char* pbuff, int nlen);
    virtual void dump();
public:
    unsigned int mu_session;
    QVector<URLITEM_S> ms_vct_url; //URL

};



//视频数据回放请求  2014.11.13 blm add
class CPduVodReplayReq : public CPduHeader
{
public:
    CPduVodReplayReq();
    virtual ~CPduVodReplayReq();

    virtual int type();
    virtual int length();
    virtual int encode(unsigned char* pbuff, int nlen);
    virtual int decode(unsigned char* pbuff, int nlen);
    virtual void dump();

public:
     PLAYBACK_REQ_S ms_playback;

};

//播放初始化请求
typedef CPduVodReplayReq CPduRePlayInitReq;

//视频数据回放应答  2014.11.13 blm modify
class CReplayAck : public CBaseAck
{
public:
    CReplayAck();
    virtual ~CReplayAck();

    virtual int type();
    virtual int length();
    virtual int encode(unsigned char* pbuff, int nlen);
    virtual int decode(unsigned char* pbuff, int nlen);
    virtual void dump();

public:
   unsigned int mu_session;  //会话标识
};

//播放初始化应答
typedef CReplayAck CRePlayInitAck;

//播放暂停请求
//2014.11.13 add
class CReplayPauseReq : public CPduHeader
{
public:
    CReplayPauseReq();
    virtual ~CReplayPauseReq();

    virtual int type();
    virtual int length();
    virtual int encode(unsigned char* pbuff, int nlen);
    virtual int decode(unsigned char* pbuff, int nlen);
    virtual void dump();

public:
    unsigned int mu_session;
};

//播放暂停应答
//2014.11.13 add
/*class CPduVodStopAck : public CBaseAck
{
public:
    CPduVodStopAck();
    virtual ~CPduVodStopAck();

    virtual int type();
    virtual int length();
    virtual int encode(unsigned char* pbuff, int nlen);
    virtual int decode(unsigned char* pbuff, int nlen);
    virtual void dump();

public:
   unsigned int m_nsession;  //会话标识
};*/

//播放暂停应答
typedef CReplayAck  CReplayPauseAck;
//播放恢复请求
typedef CReplayPauseReq CReplayResumeReq;
//播放恢复应答
typedef CReplayAck CReplayResumeAck;
//播放停止请求
typedef CReplayPauseReq CReplayStopReq;
//播放停止应答
typedef CReplayAck CReplayStopAck;
//回放心跳包请求
typedef CReplayPauseReq CReplayHeartBeatReq;
//回放心跳包应答
typedef CBaseAck CReplayHeartBeatAck;
//播放帧请求
class CReplayFrameReq : public CPduHeader
{
public:
    CReplayFrameReq();
    virtual ~CReplayFrameReq();

    virtual int type();
    virtual int length();
    virtual int encode(unsigned char* pbuff, int nlen);
    virtual int decode(unsigned char* pbuff, int nlen);
    virtual void dump();
public:
    unsigned int mu_session;
    unsigned int mu_framenum;   //帧号
};

//播放帧应答
typedef CReplayAck CReplayFrameAck;


//预置点播放信息请求
class CPresetPlayInfoReq : public CPduHeader
{
public:
    CPresetPlayInfoReq();
    virtual ~CPresetPlayInfoReq();

    virtual int type();
    virtual int length();
    virtual int encode(unsigned char* pbuff, int nlen);
    virtual int decode(unsigned char* pbuff, int nlen);
    virtual void dump();

public:
      PLAY_PRESET_REQ_S ms_playpreset;
};

//预置点播放信息回复
class CPresetPlayAck : public CBaseAck
{
public:
    CPresetPlayAck();
    virtual ~CPresetPlayAck();

    virtual int type();
    virtual int length();
    virtual int encode(unsigned char* pbuff, int nlen);
    virtual int decode(unsigned char* pbuff, int nlen);
    virtual void dump();

public:
   unsigned int mu_session;  //会话标识
   unsigned int mu_filenum;   //该时间段内所有的视频文件个数
//   unsigned int mu_type;      //0表示可见光，1表示红外，2表示2个都有
   LIST_TIME_TYPE_S ms_time_type; //若有多个文件就有多个时间
};

//回放获取调色板类型请求
typedef CReplayPauseReq CGetRePlayPeleteReq;
//回放获取调色板类型应答
class CGetRePlayPeleteAck : public CBaseAck
{
public:
    CGetRePlayPeleteAck();
    virtual ~CGetRePlayPeleteAck();

    virtual int type();
    virtual int length();
    virtual int encode(unsigned char* pbuff, int nlen);
    virtual int decode(unsigned char* pbuff, int nlen);
    virtual void dump();

public:
    int mn_indexcolor;
};


//回放设置调色板类型请求
class CSetRePlayPeleteReq : public CPduHeader
{
public:
    CSetRePlayPeleteReq();
    virtual ~CSetRePlayPeleteReq();

    virtual int type();
    virtual int length();
    virtual int encode(unsigned char* pbuff, int nlen);
    virtual int decode(unsigned char* pbuff, int nlen);
    virtual void dump();

public:
    int mn_indexcolor;
    unsigned int mu_session;
    unsigned int mn_frame_no;  //帧号
};

//回放设置调色板类型应答
typedef CBaseAck CSetRePlayPeleteAck;


//回放中手动设置色标温度范围请求
class CManualRePlayTemperatureReq : public CPduHeader
{
public:
    CManualRePlayTemperatureReq();
    virtual ~CManualRePlayTemperatureReq();

    virtual int type();
    virtual int length();
    virtual int encode(unsigned char* pbuff, int nlen);
    virtual int decode(unsigned char* pbuff, int nlen);
    virtual void dump();

public:
    REPLAY_SET_ADJUSTRANGE_S ms_setadjust;
//    float mf_max;  //最高温度
//    float mf_min;
//    unsigned int mu_session;
};
//回放中手动设置色标温度范围应答
typedef CBaseAck CManualRePlayTemperatureAck;

//回放中设置色标类型手动还是自动请求
class CRePlayManualOrAutoReq : public CPduHeader
{
public:
    CRePlayManualOrAutoReq();
    virtual ~CRePlayManualOrAutoReq();

    virtual int type();
    virtual int length();
    virtual int encode(unsigned char* pbuff, int nlen);
    virtual int decode(unsigned char* pbuff, int nlen);
    virtual void dump();

public:
    int mn_adjustype;
    unsigned int mu_session;
    unsigned int mn_frame_no;  //帧号
};

typedef CBaseAck CRePlayManualOrAutoAck;


//回放分析增加请求
class CReplayAnaAddReq : public CPduHeader
{
public:
    CReplayAnaAddReq();
    virtual ~CReplayAnaAddReq();

    virtual int type();
    virtual int length();
    virtual int encode( unsigned char* pbuff, int nlen );
    virtual int decode( unsigned char* pbuff, int nlen );
    virtual void dump();

public:
    unsigned int mu_session;
    unsigned int mn_frame_no;  //帧号
    char mc_ana[1024];   //分析字符串，标明要分析的数据
};

//回放分析增加应答
typedef CBaseAck CReplayAnaAddAck;
//回放分析删除请求
typedef CReplayAnaAddReq CReplayAnaDelReq;
//回放分析增加应答
typedef CBaseAck CReplayAnaDelAck;
// playback

#endif // PDUCOMMON_H
