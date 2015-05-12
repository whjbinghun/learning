#ifndef DATAMODE_H
#define DATAMODE_H

#include <QtCore>
#include <QColor>
#include <QMap>
#include <QList>
#include <QDomNodeList>
#include <QImage>
#include <list>
#include <vector>
#include <map>
#include "datacommon.h"
#include "cbasemsg.h"
#include "charttrenddelegate.h"

class ChartPoints;
using namespace std;

enum ChartTempType{
    CHART_MAX_TEMP = 0,
    CHART_MIN_TEMP,
    CHART_AVG_TEMP
};

enum StreamType{
    StreamMain,
    StreamSub,
    StreamKey,
};

//界面分析
typedef struct _tagAnaInfo{
    int n_sign_id;
    QString type_name;
    QString str_name;
    QPoint point_start;
    QPoint point_end;
    bool b_show;
    bool b_preset_ana;
    bool b_ctrl;
    bool b_alarm;

    _tagAnaInfo()
        : b_show( true )
        , b_preset_ana( false )
        , b_alarm( false )
        , b_ctrl( true ){}

}AnaInfo;
Q_DECLARE_METATYPE(AnaInfo)
Q_DECLARE_METATYPE( QList< AnaInfo > )

typedef struct _tagTempBuf{
    QPoint pt;
    float f_temp;
} TempBuf;

typedef struct _tagTempAnaBuf{
    TempBuf max_temp;
    TempBuf min_temp;
    TempBuf avg_temp;
    unsigned int ui_sec;  //  帧的UTC时间的秒值
    unsigned int ui_usec;  // 帧的微秒值
    QString str_ana_name;
} TempAnaBuf;

//typedef map< QString, QList<AnaInfo> > mapSnToAnaInfo;  // devcie_sn+"real or record"
//图表分析信息
typedef map< QString, list< ChartTrendDelegate* > > mapSn2ToChart;  // devcie_sn+ana_id 2 chart data
//对应设备分析温度
typedef map< QString, QMap<QString, TempAnaBuf> >   mapTempture;
//报警截取
typedef struct _tagCaptureInfo
{
    QString ms_path;
    QTime   mt_last_capture;
    bool    mb_flag;
}CaptureInfo;

typedef map<QString,CaptureInfo>  mapCaptureInfo;

//导轨信息
typedef struct   _tagPtzRialInfo
{
  int n_min;
  int n_max;
  int n_pos;
} PtzRailInfo;
typedef map<QString,PtzRailInfo> mapSn2RailInfo;

typedef struct _tagUsrInfo
{
    char m_sIp[32];
    int  m_nPort;
    char m_sName[32];
    char m_sPsw[32];
    ClientType n_client_type;

    _tagUsrInfo()
        : m_nPort(0)
        , n_client_type( ClientNone )
    {
        memset( m_sIp, 0, 32 );
        memset( m_sName, 0, 32 );
        memset( m_sPsw, 0, 32 );
    }
}UsrInfo;

Q_DECLARE_METATYPE(UsrInfo)

typedef struct _tagDeviceInfo{
    int n_class;
    int n_port;
    QString str_ip;
    QString str_sn;
    QString str_type;
    QString str_mac;
    bool b_have_ptz;
    _tagDeviceInfo()
        : n_class(0), n_port(0)
        , str_ip(""), str_sn("")
        , str_type(""), str_mac("")
        , b_have_ptz(false){}
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
    DeviceInfo dev_ptz;
    DeviceInfo dev_rail;
    QString str_name;
} DeviceGroup;

typedef struct _tagRealTimeImage
{
    QMutex *p_mutex;
    QImage *p_image;

    _tagRealTimeImage()
        : p_mutex( 0 )
        , p_image( 0 ) {}
} RealTimeImage;

typedef struct _tagMediaImgInfo{
  QMutex *p_mutex_ir;
  QImage *p_ir_image;
  unsigned int ui_ir_sec, ui_ir_usec;
  unsigned int ui_ir_frame;

  QMutex *p_mutex_vi;
  QImage *p_vi_image;
  unsigned int ui_vi_sec, ui_vi_usec;

  _tagMediaImgInfo()
    : p_vi_image( 0 )
    , p_mutex_ir( 0 )
    , p_mutex_vi( 0 )
    , p_ir_image( 0 )
    , ui_ir_sec( 0 )
    , ui_ir_usec( 0 )
    , ui_vi_sec( 0 )
    , ui_vi_usec( 0 )
    , ui_ir_frame( 0 ){}
} MediaImgInfo;


//分析图表命名信息
typedef struct _tagAnaChartInfo{
  QString s_tittle;
  int     n_index;
} AnaChartInfo;
//红外调色板信息
typedef struct _tagPaletteInfo{
    bool b_auto;
    float f_max_range, f_min_range;
    float f_level, f_level_span;
    int n_clr_id;

    _tagPaletteInfo()
        : b_auto( true )
        , f_max_range( 50 )
        , f_min_range( 10 )
        , f_level( 24.3 )
        , f_level_span( 35.4 )
        , n_clr_id( 1 ) {}
}  PaletteInfo ;

class CDataMode
{
public:
    static CDataMode* Instance();
    static void Uninstance();

    void set_user_info( const UsrInfo &st_usr_info );
    void get_user_info( UsrInfo &st_usr_info );

    void set_connected( bool b_conn );
    bool is_connected();

    void set_client_id( int n_id_client );
    int get_client_id();

    bool get_group_info(  int n_num, DeviceGroup &dev_group );
    void get_group_device( QMap< int, DeviceGroup > &map_group  );
    void get_no_group_ir_device( QList< DeviceInfo > &list_ir );
    void get_no_group_vi_device( QList< DeviceInfo > &list_vi );
    void get_no_group_ptz_rail_device( QList< DeviceInfo > &list_ptz_rail );
    bool get_device_group_info( const QString &str_sn, int &n_group_num, int &n_class );

    void handle_device_has_ptz_rail();
    void update_group_device_status();
    void clear_device_group();
    bool analysis_xml( QString str_xml );
    void analysis_xml_device( QDomNodeList& doc_node_list, int n_group_num = -1, QString str_name = "" );

    void set_image( int n_group_num, int n_class,int n_type
                    , unsigned char *p_buf, unsigned int ui_data_len, int width, int height, bool b_sub_stream = false
                    , unsigned int ui_sec = 0, unsigned int ui_usec = 0, unsigned int ui_frame = 0 );
    void set_image_jpg( int n_group_num, int n_class,int n_type
                        , unsigned char *p_buf, unsigned int ui_data_len
                        , unsigned int ui_sec = 0, unsigned int ui_usec = 0, unsigned int ui_frame = 0 );
    void set_frame_image( QString str_sn, int n_class, unsigned char *p_buf, unsigned int ui_data_len );
    bool clear_image( int n_group_num, DeviceType device_type, bool b_replay, bool b_sub_stream );
    bool clear_image( QString str_sn, DeviceType device_type );

    int get_replay_ir_frame_now();

    QImage *get_main_image( int n_group_num, int n_class, int n_type, QImage &image );
    QImage *get_sub_image( int n_group_num, int n_class, QImage &image );
    bool get_frame_image( QString &str_sn, QImage &image );

    bool get_ana_info( QString &str_sn, QList< AnaInfo > &lst_anas );
    bool get_ana_info( QString &str_sn, QMap< int , AnaInfo > &map_anas );
    bool get_ana_info( QString &str_sn, int &n_id_ana, AnaInfo &ana_info );
    bool set_ana_name( QString &str_sn, int &n_id_ana, QString &str_name );
    bool set_ana_info( QString &str_sn, QList<AnaInfo> &list_anas );
    bool add_ana_info( QString &str_sn, AnaInfo &ana_info );
    bool del_ana_info( QString &str_sn, AnaInfo &ana_info );
    bool clear_ana_info( QString &str_sn );

    bool get_ana_info( const QString &str_sn, const QString &str_ana, int &n_id_ana, QString &str_name );
    bool get_ana_info( const QString &str_sn, const QString &str_ana, int &n_id_ana, QString &str_name, QString &str_type );
    bool get_ana_abbreviation_name( QString &str_sn, int &n_id_ana, QString &str_name );        //. 缩略名 例：R01

    void get_max_temp_string( int &n_num, float &f_max_temp, QString str_type_name, QString &str_shape_num );
    void shape_to_string( QString &str_shape, const AnaInfo &anainfo );

    void set_beep(bool b_beep);
    bool get_beep();

    void set_capture(QString& s_sn,QString& s_path);
    bool get_capture(QString& s_sn,QString& s_path);

    //rail info;
    bool set_rail_range(QString& s_rail_sn,int  n_min,int  n_max);
    bool get_rail_range(QString& s_rail_sn,int & n_min,int & n_max);
    bool get_rail_pos(QString& s_rail_sn,int& n_pos);
    bool set_rail_pos(QString& s_rail_sn,int n_pos);

    //.预置点信息
    bool add_preset( int n_group_num, PresetInfo &preset_info );
    bool delete_preset( int n_group_num, int n_rail, int n_ptz );
    bool change_preset_name( int n_group_num, PresetInfo &preset_info );
    bool clear_preset( int n_group_num );

    bool add_preset_pic( int n_group_num, QString str_pic_path );
    bool get_preset_pic( int n_group_num, int &n_buf_len, unsigned char *p_pic_buf );
    bool remove_preset_pic( int n_group_num );

    bool change_preset_pos_info( int n_group_num, QList< PresetItem > &list_preset_item );

    bool get_preset_info( int n_group_num, int n_rail, int n_ptz, PresetInfo &preset_info );
    bool get_preset_info( int n_group_num, QMap< int, PresetInfo > &map_preset_info );
    bool update_preset_pic_info( int n_group_num, PresetPicInfo &pic_info, QMap< int,  PresetInfo> &map_preset_info );

    bool update_cruise_scheme( int n_group_num, QVector< CruiseSchemeInfo > &vct_scheme );
    bool add_cruise_scheme( int n_group_num, CruiseSchemeInfo &cru_scheme );
    bool change_cruise_scheme( int n_group_num, CruiseSchemeInfo &cru_scheme );
    bool delete_cruise_scheme( int n_group_num, int n_cruise );
    bool change_cruise_status( int n_group_num, int n_cruise, bool b_cruise );

    bool get_cruise_scheme( int n_group_num, int n_cruise, CruiseSchemeInfo &cru_scheme );
    bool get_cruise_scheme( int n_group_num, QMap< int, CruiseSchemeInfo > &vct_scheme );
    bool set_send_cruise_scheme( int n_group_num, CruiseSchemeInfo &cruise_scheme );
    bool get_send_cruise_scheme( int n_group_num, CruiseSchemeInfo &cruise_scheme );

    bool get_preset_alarm( int n_group_num, int n_rail, int n_ptz, QMap< int, PresetAlarmCondition > &map_alarm );
    bool set_preset_alarm( int n_group_num, int n_rail, int n_ptz, QMap< int, PresetAlarmCondition > &map_alarm );

    bool get_alarm_pic_list( int n_group_num, int n_rail, int n_ptz, QVector< AlarmPicTime > &vct_pic );
    bool set_alarm_pic_list( int n_group_num, int n_rail, int n_ptz, QVector< AlarmPicTime > &vct_pic );

    //bool get_alarm_pic_info( int n_group_num, int n_rail, int n_ptz, QVector< AlarmPicInfo > &pic_info );
    bool get_cache_alarm_pic( const AlarmPicInfoSearch &alarm_pic_search, QString &str_ir_path, QString &str_vi_path );       //. 获取缓存的报警图片
    bool save_alarm_pic_info( const QVector< AlarmPicInfo > &vct_info, const AlarmPicInfoSearch &alarm_pic_search
                              , QString &str_ir_path, QString &str_vi_path );  //. 保存报警图片

    bool save_pic_on_time( const QVector< AlarmPicInfo > &vct_info, QString &str_ir_path, QString &str_vi_path );

    //2014.11.19 blm add
    //保存预置点播放信息回复中时间点
    bool set_time_list( int n_group_num, int n_rail, int n_ptz, LIST_TIME_TYPE_S &l_time );
    bool get_time_list( int n_group_num, int n_rail, int n_ptz, LIST_TIME_TYPE_S &list_video );
    void set_session(int n_group_num,int n_session);
    int get_session(int n_group_num);

protected:
    //.    QMap< 设备( sn + real or record), QMap< 分析id, 分析信息 > >
    typedef QMap< QString, QMap< int, AnaInfo > > MapAnaIdToInfo;
    //.    QMap< 设备( sn + real or record), QMap< 分析字符串（P;10:10;-）, 分析id的vector > >
    typedef QMap< QString, QMap< QString, QVector< int > > > MapAnaStringToId;

    //QReadWriteLock   m_rw_ana_lock;
    QReadWriteLock   m_rw_ana_lock;
    //. 所有图像分析
    MapAnaIdToInfo   m_map_ana_id_to_info;//m_mapSnToAnaInfo;
    //. 分析字符串对应的分析id
    MapAnaStringToId m_map_ana_string_to_id;

private:
   CDataMode();
   virtual ~CDataMode();

   static CDataMode* m_pInstance;

   QMutex   m_mutex_usr;            //. 锁用户信息
   UsrInfo  m_user_info;            //. 用户登入信息
   bool     mb_connected_server;    //. 主连接是否连接
   int      mn_id_client;           //. 服务器分配的id

   QString mstr_xml_change;
   QMap< int, bool > m_map_device_group_status;        //.已经分组设备连接状态
   QMap< int, DeviceGroup > m_map_device_group;    //. 已经分组设备
   QList< DeviceInfo > m_list_device_ir_no_group;            //. 未分组设备
   QList< DeviceInfo > m_list_device_vi_no_group;            //. 未分组设备
   QList< DeviceInfo > m_list_device_ptz_rail_no_group;            //. 未分组设备
   QMutex m_mutex_device_grp;

   QMutex m_mutex_media_image;
   QMap< int, MediaImgInfo > m_map_media_live_image;  //.实时视频图像
   QMap< int, MediaImgInfo > m_map_media_sub_live_image; //.副码流视频图像
   MediaImgInfo      mo_mediaReplayImg;     //.回放视频图像

   map<QString, list<ChartTrendDelegate*> >  map_deletes; // delay delete

   bool       mb_beep;

   QMutex           m_mutex_capture;
   mapCaptureInfo   m_mapSnToCapture;                     //alalrm capture info;

   mapSn2RailInfo   m_mapSn2RailInfo;                     //导轨信息

   QReadWriteLock m_mutex_preset;                                           //. 预置位信息锁
   QMap< int, QMap< int,  PresetInfo> > m_map_preset_info;       //. 预置位信息 第一层key 为组号， 第二层key为导轨和云台号的集合
   QMap< int, PresetPicInfo > m_map_preset_pic_info;                 //. 预置位信息 第一层key 为组号

   QReadWriteLock m_mutex_cruise;
   QMap< int , QMap< int, CruiseSchemeInfo > > m_map_cruise_scheme;
   QMap< int , CruiseSchemeInfo > m_map_send_cruise_scheme;

   //2014.11.19 blm add <组号,<导轨预置点<<16+云台预置点,TIME_S>>
   QReadWriteLock m_mutex_replay;
   QMap< int, QMap< int, LIST_TIME_TYPE_S > > m_map_time;
   QMap<int,int> m_map_session;//存放组号对应会话标志

   QReadWriteLock m_mutex_media_live_frame;
   QMap< QString, RealTimeImage > m_map_media_live_frame;   //. 单帧实时数据

   //. 预置点报警条件 第一层key 为组号， 第二层key为导轨和云台号的集合 第三层key为报警条件id
   QReadWriteLock m_mutex_alarm_condition;
   QMap< int , QMap< int , QMap< int, PresetAlarmCondition > > > m_map_preset_alarm_condition;

   //. 预置点报警条件 第一层key 为组号， 第二层key为导轨和云台号的集合
   QReadWriteLock m_mutex_alarm_pic;
   QMap< int , QMap< int , QVector< AlarmPicTime > > > m_map_preset_alarm_pic;
};
#endif // CALARMCFG_H
