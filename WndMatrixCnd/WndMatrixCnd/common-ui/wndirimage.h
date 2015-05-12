#ifndef WNDIRIMAGE_H
#define WNDIRIMAGE_H

#include <QWidget>
#include <QLabel>
#include <QMouseEvent>
#include <QList>
#include <QToolBar>
#include <QMainWindow>
#include <QPushButton>
#include <QSlider>
#include "../common-ui/wndcommon.h"
#include "cirpreview.h"
#include "datamode.h"
#include "../common-ui/ir_ana_label.h"
#include "../common-ui/deletelabel.h"
#include "../../public/ir-libs/YFIRImageEx1.h"

class WndIrImage : public QWidget, public CIrPreview
{
    Q_OBJECT
public:
    explicit WndIrImage(QWidget *parent = 0, int n_ui_type=1, bool b_hide_warn = false, bool b_hide_calibration = false );
    ~WndIrImage();

    void paintEvent( QPaintEvent *event );
    void resizeEvent( QResizeEvent *event );
    void mousePressEvent( QMouseEvent *event );
    void mouseMoveEvent( QMouseEvent *event );
    void mouseReleaseEvent( QMouseEvent *event );

    void leaveEvent( QEvent * event );

    virtual void add_chart_temp(int n_id_ana, TempAnaBuf &temp_info){}

public:
    void init_connect();
    void init_ir_widget();

    void set_ir_ana_num( int n_num );//设置红外工具栏上的个数
    void set_realtime( bool b_realtime ){ mb_realtime = b_realtime; }
    void set_preset_setting( bool b_preset ){ mb_preset_setting = b_preset; }

    void delete_all_ana();
    void set_ui_type(int n_type);

public: //红外温度分析
    enum enum_press_status{
        none_press_status = 0,
        point_status = 1,
        line_status = 2,
        rect_status = 3,
        far_status = 4,
        near_status = 5,
        calibration_status = 6, //校准
        warn_status = 7
    } me_press_status;
    enum enum_draw_status{
        none_draw_status = 0,
        draw_point_status = 1,
        draw_line_status = 2,
        draw_rect_status = 3,
        draw_circle_status = 4
    } me_draw_status;

    void draw_anas( QPainter &draw );
    void draw_preset_anas( QPainter &draw );
    void draw_matrix_anas( QPainter &draw );

    void draw_add_shape( QPoint pt );

    bool pt_in_ana( const QPoint &pt, AnaMove &ana_move, AnaInfo &ana_info );
    AnaMove pt_in_ana_point(const QPoint &pt, const AnaInfo &ana_info);
    AnaMove pt_in_ana_line( const QPoint &pt, const AnaInfo &ana_info );
    AnaMove pt_in_ana_rect( const QPoint &pt, const AnaInfo &ana_info );
    AnaMove pt_in_ana_circle( const QPoint &pt, const AnaInfo &ana_info );
    void get_pt_ana_shape( AnaInfo &ana_info, const QPoint &pt );
    void pt_move_ana_point( const QPoint &pt );
    void pt_move_ana_line( const QPoint &pt );
    void pt_move_ana_rect( const QPoint &pt );
    void pt_move_ana_circle( const QPoint &pt );
    void pt_move_self_boundary( QPoint &pt_start, QPoint &pt_end );
    void pt_circle_move_self_boundary( QPoint &pt_circle_center, QPoint &pt_end );
signals:
    void signal_ana_menu(int n_id_ana, QPoint pt, QString& s_name);
    void signal_ana_delete( int n_id_ana );

protected:
    void pt_judge_area( const QPoint &pt, QPoint &pt_area );
    void normalized_pt( QPoint &pt1, QPoint &pt2 );
    void start_area( QPoint &pt_start, int &n_r );
    void shape_append_list( QPoint pt );
    void shape_move_or_change_size( QPoint pt );
    void judge_area( QPoint &pt_area );

public:
    void set_org_sz( int width, int height ) ;//设置红外视频原来的size   640*480
    void judge_org_area( QPoint &pt_area );

    void press_ir_template_ana( QPoint pt );
    void move_ir_template_ana( QPoint pt );
    void release_ir_template_ana( QPoint pt );
    void delete_ana_shape( QPoint pt );
    void delete_shape_mouse_release( QPoint pt );

    QString get_ana_draw_text( const AnaInfo &ana_info, const float &f_temp, const bool &b_has_temp );

    void set_toolbar_hidden( bool b_hidden );
    void set_draw_line_cfg( QColor clr, int n_line_width, int n_line_style );

public:
    void delete_preset_all_ana( int n_group );

    void delete_ana( AnaInfo &ana_info );

    void add_ana( AnaInfo &ana_info );

public Q_SLOTS:
     void on_add_point();
     void on_add_line();
     void on_add_rect();
     void on_delete();
private:
    bool mb_realtime;

    int mn_ui_type;               //0 主界面 1 单画面 2 回放界面
    int mb_hide_warn;
    bool mb_hide_calibration;

    QColor m_clr_ana;
    int mn_line_width;
    int mn_line_style;

private:
    //起始点和中间点
    QPoint m_pt_start;
    QPoint m_pt_middle;
    //求鼠标便宜量
    QPoint m_press_pt;
    QPoint m_release_pt;
    //移动点或者change size的点
    QPoint m_pt_move_start;
    QPoint m_pt_move_end;

    QString ms_type_name;
    int mn_select_sign_id;//选中id
    AnaMove me_ana_move;

    int me_mouse_press_status;
    //
    QString ms_ir_sn;

    QPoint m_pt_release_start;
    QPoint m_pt_release_end;
    bool mb_modify_status;
private:
    //窗口的原始size
    int mn_org_width;
    int mn_org_height;
    //当前窗口的size与原始size的比例
    float mf_sz_width;
    float mf_sz_height;

    Ir_Ana_Label *mp_ir_label;
    DeleteLabel *mp_delete_ana_label;

protected:
    QTimer *mp_timer_update;

    bool mb_preset_setting;

public:
    static QMutex m_mutex_sign;
    static int mn_sign_id;

    static int get_sign_id();
    static void shape_to_string( QString &str_shape, const AnaInfo &anainfo );
    static bool string_to_shape( const QString &str_shape, AnaInfo &anainfo );

    void change_ana( AnaInfo &ana_info_new, AnaInfo &ana_info_old );

    //bool set_adjust_temp( float f_min, float f_max );
    bool open_ir_image( QString str_path );
    bool update_ir_image();

    bool get_adjust_temp( float &f_level, float &f_level_span );
    bool set_adjust_temp( float f_level, float f_level_span );
    bool get_adjust_type( YFAdjustType &adjust_type );
    bool set_adjust_type( YFAdjustType adjust_type );
    bool get_adjust_color( ColorIndex &color_index );
    bool set_adjust_color( ColorIndex color_index );
    bool get_adjust_max_temp( float &f_level_span );
    bool get_adjust_min_temp( float &f_level );

    bool init_image();

    void set_group_num( int n_group_num );

    void set_preset_ana_info( QVector<AnaItem> &vct_ana );
    bool string_to_matrix_list( int n_ana_id, const QString &str_ana, QString str_color_index );
    void add_preset_ana();
    void add_matrix_ana();
    void add_normal_ana();

    //按下Ctrl键，可以一直画分析
    void clear_ana_press_status();
    bool get_mouse_press_status();
    void set_mouse_press_status( bool b_press_ctrl );
private:
    YHANDLE get_ir_handle();

    YHANDLE mh_ir_image;
    QImage m_ir_image;
    QString m_str_ir_path;

    int mn_group_num;//组号
    bool mb_reset_preset_ana;

    QList<AnaInfo> m_list_anainfo;//在当前客户端画的分析
    QList<AnaInfo> m_list_pre_anainfo ;//预置点分析
    QList<AnaInfo> m_list_matrix_anainfo ;//阵列分析

    QMap< int, QColor > m_map_matrix_ana_clr;//阵列分析颜色

    bool mb_press_ctrl;
};

#endif // WNDIRIMAGE_H
