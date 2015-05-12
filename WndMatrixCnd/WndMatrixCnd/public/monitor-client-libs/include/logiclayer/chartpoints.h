#ifndef CHARTPOINTS_H
#define CHARTPOINTS_H

#include <QList>
#include <QColor>
#include <QPoint>
#include <QMutex>
#include "datamode.h"

typedef struct
{
    unsigned int n_sec;
    unsigned int n_msec;
    float temp;
}ChartPointInfo;

class ChartPoints
{
public:
    ChartPoints( int n_type );
    ~ChartPoints(){
        m_map_chart_point_info.clear();
    }

    int get_size(){ return m_map_chart_point_info.size(); }

    const QList< ChartPointInfo > & get_pts(){
        return m_map_chart_point_info;
    }

    QColor get_line_color() {  QMutexLocker lc(&m_mutex); return m_clr_line; }
    void set_line_color( const QColor &clr ) { QMutexLocker lc(&m_mutex); m_clr_line = clr;  }

    void insert_pts( const QList<YF_ANA_CHART_TREND_POINT> &list_pts );

    void get_y_range( float &f_min, float &f_max );
    QString get_info_string( int n_pos, float &f_out_temp );

    /**
     * @brief get_date_time_string 获取时间串
     * @param n_pos
     * @return
     */
    QString get_date_time_string( int n_pos );

    QString get_all_temp_string();
    QString get_name(){ return m_str_deivce; }
    void set_name( QString str_name ){ m_str_deivce = str_name; }

    QString get_type_string();

    void clear_data( int curr_time, int backup_time );

    /**
     * @brief get_pos_time
     * @param pos
     * @param n_sec
     * @param n_msec
     */
    bool get_pos_time( const int n_pos, int & n_sec, int & n_msec );
public:
    int mn_type;
    QColor m_clr_line;
    QList< ChartPointInfo > m_map_chart_point_info;
    float mf_min_temp, mf_max_temp;
    QString m_str_deivce;
    QMutex m_mutex;
};

#endif // CHARTPOINTS_H
