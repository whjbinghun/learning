#ifndef WNDCHARTTREND_H
#define WNDCHARTTREND_H

#include "wndchart.h"
#include "chartpoints.h"
#include "QMap"
#include <qmutex.h>
#include <qtimer.h>
#include <QPushButton>
#include <QSettings>
#include "cloginview.h"
#include "common-funtion/msclientdefine.h"
#include "wndloadpointpicture.h"

#ifndef DEFINE_YHANDLE
typedef void *YHANDLE;
#define DEFINE_YHANDLE
#endif


enum {
    ANA_TYPE_MAX = 0,
    ANA_TYPE_MIN = 1,
    ANA_TYPE_AVG = 2
};

QT_BEGIN_NAMESPACE
class QThread;
QT_END_NAMESPACE

class WndChartTrend : public WndChart, public CLoginView
{
    Q_OBJECT
public:
    explicit WndChartTrend(QWidget *parent = 0);
    ~WndChartTrend();

    bool event(QEvent *);
    virtual void mouseMoveEvent( QMouseEvent *event );
    virtual void timerEvent( QTimerEvent *event );
    virtual void mouseDoubleClickEvent( QMouseEvent *event );


    virtual int onResponse( CBaseMsg& reqMsg,CBaseMsg& rspMsg );


    virtual void draw_chart_line( QPainter &draw, const QRect &rct  );
    virtual void show_tool_tip( const QPoint &pt, int n_pos_y );
    virtual void draw_chart_time( QPainter &draw, const QRect &rct );

    void draw_pts( QPainter &draw, const QRect &rct, ChartPoints *p_pts, int list_index );
    void draw_mark_line( QPainter &draw , const QRect &rct  );

    void set_group_num( int n_num, int n_index );
    void set_preset( const int n_preset_no ){ mn_preset_no = n_preset_no; }
    void set_preset_name( QString &str_preset_name );
    void set_type( bool b_realtime );
    void set_index( int n_index);
    int  get_index();
    void get_tooltip_string( int n_pos, QString &str_tool_tip );
    QString get_time_string( int n_pos );
    QString get_sn();

    void get_chart_color_list( );
    void set_chart_color_list( );

    bool get_color_by_index( int list_index, QColor& color );
    void add_color_to_color_list( QColor & color );

    void get_config_params();

signals:
    void sig_load_ir_vi_picture( bool, QString, QString );

public slots:
    void on_timer_update_ana();
    void on_chart_empty();


    void slot_btn_show_all_trend();
    void slot_btn_copy_trend();
    void slot_btn_copy_temp();
    void slot_btn_select_color();

private slots:

    void slot_load_ir_vi_picture( bool b_have, QString str_ir_path, QString str_vi_path );

public:
    QTimer *mp_timer_update;
    int mn_timer_update_axis;

    QString m_str_tooltip;

    bool mb_first_cal_axisy;
    bool mb_first_cal_axisx;
    bool mb_realtime;

    int mn_group_num;
    int mn_index;
    int mn_preset_no;
    int mn_mark_line_pix;


    int     mn_pos_max;
    float   mf_max;
    float   mf_min;
    QString m_str_preset_name;

    QMap< QString, QColor > m_map_chart_color;

    qint64 mn_double_click_mark_time;



    WndLoadPointPicture* mp_wnd_picture;
};


#endif // WNDCHARTTREND_H
