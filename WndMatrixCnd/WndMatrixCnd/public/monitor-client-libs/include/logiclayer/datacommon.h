#ifndef DATACOMMON_H
#define DATACOMMON_H

#include <QString>
#include <QVector>
#include <QtCore>
#include "netcommon.h"

#include<list>
using namespace std;

class QColor;

typedef struct _tagCruisePoint
{
    unsigned short n_rail_preset;
    unsigned short n_ptz_preset;
    unsigned int n_wait_time; ///.毫秒
} CruisePoint;

typedef struct _tagCruiseScheme
{
    unsigned short n_cruise_num;
    bool b_cruise;
    QString str_crusie_name;
    QVector< CruisePoint > vct_cruise_pts;
} CruiseSchemeInfo;

typedef struct _tagPresetAlarmCondition
{
    int n_id_alarm;
    QString str_name;
    QString str_condition;
} PresetAlarmCondition;

//预置点信息
typedef struct _tagPresetInfo {
    int n_ptz_preset_no;
    int n_rail_preset_no;
    int n_x;
    int n_y;
    QString str_name;
    QVector < AnaItem > vct_anas;

    _tagPresetInfo()
        :n_ptz_preset_no(0)
        ,n_rail_preset_no(0)
        ,n_x(0)
        ,n_y(0)
        ,str_name(""){}
} PresetInfo;
//预置点图片信息
typedef struct _tagPresetPicInfo{
    int n_len_buf;
    unsigned char *p_pic_buf;

    _tagPresetPicInfo()
        : n_len_buf(0)
        , p_pic_buf(0){}
} PresetPicInfo;
//开始时间和结束时间
typedef struct {
    int type;   //0表示可见光，1表示红外，2表示2个都有
    char start_time[14];
    char end_time[14];
}TIME_TYPR_S;
//typedef list<TIME_TYPR_S> time_list;

typedef struct{
    QList< TIME_TYPR_S > list_time;
}LIST_TIME_TYPE_S;

typedef struct {
    int n_type;
    unsigned int n_time_sec;
    unsigned int n_time_msec;
    QVector< int > vct_alarm;
} AlarmPicTime;

typedef struct _tagAlarmPicInfo{
    int n_type; ///.1表示红外 2.标识可见光
    int n_buf_len;
    unsigned char *p_buf;

    _tagAlarmPicInfo(): n_type(0)
      , n_buf_len(0), p_buf(0){}
} AlarmPicInfo;


typedef struct _tagYF_ANA_CHART_TREND_POINT{
    float d_temp_max;      ///.该帧温度
    float d_temp_min;
    float d_temp_avg;
    int n_frame;            ///.该帧帧号
    unsigned int  n_sys_time_sec;    ///.该帧时间
    unsigned int  n_sys_time_usec;    ///.该帧时间
//    unsigned int  n_time_start_sec;      ///.视频开始时间
//    unsigned int  n_time_start_usec;      ///.视频开始时间
//    unsigned int  n_time_end_sec;        ///.视频
//    unsigned int  n_time_end_usec;        ///.视频
//    QString str_device;  ///.

    _tagYF_ANA_CHART_TREND_POINT(){
        n_frame = 0;
        d_temp_max = 0.0;
        d_temp_min = 0.0;
        d_temp_avg = 0.0;
        n_sys_time_sec = 0;
        n_sys_time_usec = 0;
//        n_time_start_sec = 0;
//        n_time_start_usec = 0;
//        n_time_end_sec = 0;
//        n_time_end_usec = 0;
//        str_device = "";
    }
} YF_ANA_CHART_TREND_POINT;

#endif // DATACOMMON_H
