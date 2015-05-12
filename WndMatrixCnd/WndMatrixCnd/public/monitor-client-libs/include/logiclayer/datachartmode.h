#ifndef DATA_CHART_MODE_H
#define DATA_CHART_MODE_H

#include <QtCore>
#include <QColor>
#include <QMap>
#include <QList>
#include <QImage>
#include <list>
#include <vector>
#include <map>
#include "charttrenddelegate.h"
#include "datacommon.h"
#include "datamode.h"
using namespace std;
class ChartPoints;


/**
* @brief
* CDataChartMode
* slf
*/
class CDataChartMode
{
public:
    static CDataChartMode* Instance();
    static void Uninstance();

    /**
    * @brief
    * get_ana_now_temp获取当前分析温度数据
    * @param[in] s_sn	设备SN
    * @param[in] str_ana 分析串
    * @param[in] temp_info 分析温度
    */
    bool get_ana_now_temp(QString& s_sn, QString str_ana, TempAnaBuf &temp_info );
    bool get_ana_now_temp( QString &str_sn, QMap< QString, TempAnaBuf > &map_temp );
    /**
    * @brief
    * get_ana_char_info获取sn图表数据
    * @param[in] s_sn	设备SN
    * @param[in] v_char_info 图表信息
    */
    void get_ana_char_info(QString& s_sn,vector<AnaChartInfo> & v_char_info);
    /**
    * @brief
    * add_ana_chart 增加分析图表
    * @param[in] s_sn	设备SN
    * @param[in] p_delegate 图表
    */
    void add_ana_chart(QString& s_sn,ChartTrendDelegate* p_delegate);
    /**
    * @brief
    * del_ana_chart 删除分析图表
    * @param[in] s_sn	设备SN
    * @param[in] p_delegate 图表
    */
    void del_ana_chart( QString& s_sn,ChartTrendDelegate* p_delegate );
    /**
    * @brief
    * delete_chart 删除图表
    * @param[in] s_sn	设备SN
    * @param[in] p_delegate 图表
    */
    void delete_chart( QString& s_sn,int n_index );
    /**
    * @brief
    * has_ana_chart是否存在图像
    * @param[in] s_sn	设备SN
    * @param[in] n_id_ana 分析id或窗口id
    * @param[in] n_type 最高温度或平均温度或平均温度
    */
    bool has_ana_chart( QString& s_sn,int  n_id_ana, int n_type );

    bool has_ana_chart( QString& s_sn, int n_index, int  n_id_ana, int n_type );

    ChartTrendDelegate *get_chart( QString& s_sn, int n_index );
    /**
    * @brief
    * remove_ana是否存在图像
    * @param[in] s_sn	设备SN
    * @param[in] n_id_ana 分析id或窗口id
    * @param[in] n_type 最高温度或平均温度或平均温度
    */
    void remove_ana( QString& s_sn,int n_index, int  n_id_ana, int n_type );
    /**
    * @brief
    * remove_ana是否存在图像
    * @param[in] s_sn	设备SN
    * @param[in] n_id_ana 分析id或窗口id
    * @param[in] n_type 最高温度或平均温度或平均温度
    */
    void remove_ana( QString& s_sn,int n_index, int  n_id_ana );

    /**
     * @brief remove_all_ana
     */
    void remove_all_ana( QString& s_sn,int n_index );

    /**
    * @brief
    * has_ana_chart是否存在图像
    * @param[in] s_sn	设备SN
    * @param[in] n_id_ana 分析id或窗口id
    * @param[in] n_type 最高温度或平均温度或平均温度
    */
    ChartPoints * add_ana( QString& s_sn,int n_index, int  n_id_ana, int n_type );
    /**
    * @brief
    * set_collect_temp_interval_time采集温度数据间隔时间
    * @param[in] sec	秒
    */
    void set_collect_temp_interval_time(  QString& s_sn, int n_index,  int n_sec );

    /**
    * @brief
    * get_collect_temp_interval_time获取采集温度数据间隔时间
    * @param[in]
    */
    int get_collect_temp_interval_time( QString& s_sn, int n_index );

    /**
    * @brief
    * set_first_add 设置是否第一次采集
    * @param[in] s_sn sn
    * @param[in] n_index 索引
    * @param[in] b_first 是否第一次
    */
    void set_first_add( QString& s_sn,int n_index, bool b_first );

    /**
    * @brief
    * set_first_add 设置是否第一次采集
    * @param[in] s_sn sn
    * @param[in] n_index 索引
    * @param[in] b_first 是否第一次
    */
    bool get_first_add( QString& s_sn,int n_index );

    /**
     * @brief load_backup_temp_data
     * @param s_sn
     * @param n_index
     */
    void load_backup_temp_data( QString& s_sn,int n_index );

    void set_chart_clear_period( QString& s_sn,int n_index, unsigned int nu_sec );


    void get_max_min_valule_info(QString& s_sn,int n_index, int& n_pos_max,float& f_temp_min, float& f_temp_max,bool& b_min_change, bool& b_max_change);


    void get_max_min_value( QString& s_sn,int n_index, int& n_pos_max,float& f_temp_min, float& f_temp_max );


    QString get_tooltip_string(QString& s_sn,int n_index, int n_pos );

    QString get_pos_time( QString &str_sn, int n_index, int n_pos );

    void  get_data(QString& s_sn,int n_index,QList< ChartPoints * > & list_data);

    void clear_chart( QString& s_sn );

    int  add_temp_data( const FuntionType &n_type, const QString &str_device_sn , const QMap<QString, TempAnaBuf> &map_temp_ana
                        , const unsigned int &n_utc_sec, const unsigned int &n_utc_msec );


    void recyle_deleted_data( QString& s_sn);

    /**
     * @brief get_pos_time 获取位置时间
     * @param s_sn  sn
     * @param n_index 索引
     * @param n_pos 位置
     * @param n_sec 秒
     * @param n_msec 毫秒
     * @return
     */
    bool get_pos_time( QString& s_sn, int n_index, const int n_pos, int & n_sec, int & n_msec );

protected:
    QReadWriteLock      m_mutex_temp;
    mapTempture         m_mapSn2Tempture;   //对应设备分析温度

    QReadWriteLock      m_mutex_chart;
    mapSn2ToChart       m_mapAnaChart;

    map<QString, list<ChartTrendDelegate*> >  map_deletes; // delay delete

private:
    int get_last_collect_temp_time() const;
    void set_last_collect_temp_time(const int time);

private:
   CDataChartMode();
   virtual ~CDataChartMode();

   static CDataChartMode* m_pInstance;
};


#endif // CALARMCFG_H
