#ifndef MS_PRESET_TEMP_ANALYSIS_H
#define MS_PRESET_TEMP_ANALYSIS_H

#include <QtCore/qglobal.h>

#if defined(MS_PRESET_TEMP_ANALYSIS_LIBRARY)
#  define MS_PRESET_TEMP_ANALYSISSHARED_EXPORT Q_DECL_EXPORT
#else
#  define MS_PRESET_TEMP_ANALYSISSHARED_EXPORT Q_DECL_IMPORT
#endif
/*
 * CMSTemperatureDataSqlite 类对温度数据进行操作sqlite本地数据库
 *
 *
 *
 * */


//typedef struct
//{

//}MSDateTime;

//typedef struct
//{
//    unsigned int sec;   // 秒
//    unsigned int msec;  // 毫秒
//}MSUTCTime;

typedef struct
{
    int group_number;    // 组号
    int rail;            // 导轨
    int ptz;             // 去台
    int ana_id;          // 分析ID
    long long sec;       // 秒
    // int msec;  // 毫秒
    float max_temp;      // 最大温度
    float avg_temp;      // 最大温度
    float min_temp;      // 最大温度
    char   reverse[255]; // 保留
}TemperatureData_S;

class MS_PRESET_TEMP_ANALYSISSHARED_EXPORT MSPresetTempAnalysisSqlite
{

public:
    MSPresetTempAnalysisSqlite(QString db_name);
   ~MSPresetTempAnalysisSqlite();

    bool add_data(const TemperatureData_S & data);
    bool query_data(int group_number, int rail, int ptz, long long start_time, long long end_time, QVector<TemperatureData_S> & data_list);
    bool delete_data(int group_number, int rail, int ptz);
private:
    bool mb_is_open;
};

#endif // MS_PRESET_TEMP_ANALYSIS_H
