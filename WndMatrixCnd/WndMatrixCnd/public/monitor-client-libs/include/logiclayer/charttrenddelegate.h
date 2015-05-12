#ifndef CHARTTRENDDELEGATE_H
#define CHARTTRENDDELEGATE_H
#include <QMutex>
#include <QMap>
#include <QList>
#include <QPainter>
#include "datacommon.h"
#include "video_decode_api.h"
class ChartPoints;

#include <vector>
#include <map>

class ChartTrendThread;

using namespace std;

class   ChartTrendDelegate
{
public:
    explicit ChartTrendDelegate();
    virtual ~ChartTrendDelegate();
    int    get_index() { return mn_index; }
    void   set_index( const int index ) { mn_index = index; }

    void   set_tittle(QString& s_tittle);
    void   get_tittle(QString& s_tittle);
    void   get_data( QList< ChartPoints * > & list_data);

    /**
     * @brief add_ana
     * @param h_ana
     * @param n_type
     * @return
     */
    ChartPoints *add_ana( YHANDLE h_ana,  int n_type );
    /**
     * @brief remove_ana
     * @param h_ana
     * @param n_type
     */
    void remove_ana( YHANDLE h_ana, int n_type );
    /**
     * @brief remove_ana
     * @param h_ana
     */
    void remove_ana( YHANDLE h_ana );
    /**
     * @brief remove_all_ana
     */
    void remove_all_ana();
    /**
     * @brief record_trend_temperature
     * @param h_ana
     * @param yf_ana_pt
     */
    void record_trend_temperature( YHANDLE h_ana,  YF_ANA_CHART_TREND_POINT &yf_ana_pt );
    void draw_chart_line( QPainter &draw, const QRect &rct, float mf_axisy_mark_span_pos,float mf_axisy_min_scroll, float mf_axisy_max_scroll,int mn_mark_line_y,bool mb_axisy_scroll,float mf_axisy_mark_span_pixel, float mf_axisx_mark_span_temp,float mf_axisx_min,float mn_axisx_mark_span_pixel);
    QString get_tooltip_string(int n_pos);
    QString get_pos_time( int n_pos );
    bool has_ana_chart( YHANDLE h_ana, int n_type );
    bool is_empty();
    void set_first_add( bool b_first );
    bool get_first_add();

    void load_backup_temp_data();
    /**
     * @brief get_max_min_valule_info
     * @param n_pos_max
     * @param f_temp_min
     * @param f_temp_max
     * @param b_min_change
     * @param b_max_change
     */
    void get_max_min_valule_info( int& n_pos_max,float& f_temp_min, float& f_temp_max, bool& b_min_change, bool& b_max_change );
    /**
     * @brief get_max_min_value
     * @param n_pos_max
     * @param f_temp_min
     * @param f_temp_max
     */
    void get_max_min_value( int& n_pos_max, float& f_temp_min, float& f_temp_max );
    /**
     * @brief get_types
     * @param map_types
     */
    void get_types( map<int, vector< int > >& map_types);
    /**
     * @brief set_collect_temp_interval_time
     * @param nu_sec
     */
    void set_collect_temp_interval_time( unsigned int nu_sec );
    /**
     * @brief get_collect_temp_interval_time
     * @return
     */
    unsigned int get_collect_temp_interval_time();
    /**
     * @brief set_chart_clear_period 设置图表备份周期
     * @param nu_sec
     */
    void set_chart_clear_period( unsigned int nu_sec );

    /**
     * @brief get_chart_clear_period
     * @return 返回图表备份周期
     */
    unsigned int get_chart_clear_period();

    /**
     * @brief get_pos_time 获取曲线位置的时间
     * @param n_pos 位置
     * @param n_sec 秒
     * @param n_msec 毫秒
     */
    bool get_pos_time( const int n_pos, int & n_sec, int & n_msec );

protected:
    void draw_pts(QPainter &draw, const QRect &rct, ChartPoints *p_pts,float mf_axisy_mark_span_pos,float mf_axisy_min_scroll,float mf_axisy_max_scroll,int mn_mark_line_y,bool mb_axisy_scroll,float mf_axisy_mark_span_pixel, float mf_axisx_mark_span_temp,float mf_axisx_min,float mn_axisx_mark_span_pixel);
    void draw_mark_line( QPainter &draw , const QRect &rct  );
public:
    QMap< YHANDLE, QMap< int, ChartPoints *> > m_map_ana_pts;   // 图表温度数据的列表
    QMap< YHANDLE, QList< YF_ANA_CHART_TREND_POINT > > m_map_will_add_pts;  // 分析温度数据的列表


   // QMap< YHANDLE, QMap< int, ChartPoints *> > mn_map_ana_backup_pts; // 图表温度数据的备份列表

    /**
     * @brief mn_map_ana_backup_pts
     * key 分析号
     * value 温度列表
     */
    QMap< YHANDLE, QList< YF_ANA_CHART_TREND_POINT > > mn_map_ana_backup_pts;  // 分析温度数据的列表

    QMutex m_mutex_add_pt;

    int mn_index;
    int mn_mark_line_pix;
    bool mb_first_add_pt;
    static int  mn_count;
    QString  ms_tittle;

    QMap< YHANDLE, uint > mn_map_ana_pts_backup_time;
private:
    unsigned int mn_collect_temp_interval_time;
    ChartTrendThread* m_chart_trend_thread;

    int     mn_pos_max;
    float   mf_max;
    float   mf_min;

    unsigned int mnu_clear_period;
};

class ChartTrendThread: public QThread
{
public:
    ChartTrendThread( ChartTrendDelegate* delegate );

    bool mb_thread_exit;
protected:
    virtual void run();
private:
    ChartTrendDelegate* mp_delegate;
};




#endif // CHARTTRENDDELEGATE_H
