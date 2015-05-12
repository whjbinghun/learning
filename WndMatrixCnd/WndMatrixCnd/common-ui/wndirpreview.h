#ifndef WNDIRPREVIEW_H
#define WNDIRPREVIEW_H

#include <QFrame>
#include <QLabel>
#include <QMouseEvent>
#include <QList>
#include <QToolBar>
#include <QMainWindow>
#include <QPushButton>
#include <QSlider>
#include "WndCommon.h"
#include "cirpreview.h"
#include "datamode.h"
#include "ir_ana_label.h"
#include "deletelabel.h"
#include "ir_matrix_analysis_button.h"
#include "../../public/alarm_judge/comm_typedef.h"
#include "../../public/alarm_judge/alarmjudgeApi.h"
#include "matrixwidget.h"
#include <QTimer>
#include <cmainview.h>

typedef struct {
    int n_ana_id;  //分析id号
    QString str_ana; //分析字符串
    int n_time_count; //计时数
    bool b_alarm;     //是否报警
} ANA_ALARM_CONTROL_E;

class WndIrThread;

class WndIrPreview : public QMainWindow, public CIrPreview
{
    Q_OBJECT
public:
    explicit WndIrPreview( QWidget *parent = 0, int n_ui_type=1, bool b_hide_warn = false, bool b_hide_calibration = true );
    ~WndIrPreview();

public:
    int onResponse( CBaseMsg& reqMsg, CBaseMsg& rspMsg );
    void paintEvent( QPaintEvent *event );
    void resizeEvent( QResizeEvent *event );
    void mousePressEvent( QMouseEvent *event );
    void mouseMoveEvent( QMouseEvent *event );
    void mouseReleaseEvent( QMouseEvent *event );
    void enterEvent( QEvent *e );
    void leaveEvent( QEvent *e );
    //void keyPressEvent( QKeyEvent *p_key );
    //void keyReleaseEvent( QKeyEvent *p_key );

    virtual void add_chart_temp(int n_id_ana, TempAnaBuf &temp_info){}

public:
    void init_connect();
    void init_ir_widget();
    void set_ir_ana_num( int n_num );

    void set_group_number( int n_num );
    void set_preset_param( int n_rail, int n_ptz );

    void set_realtime( bool b_realtime ){ mb_realtime = b_realtime; }
    void set_preset_setting( bool b_preset );

    void delete_all_ana();
    void set_ui_type(int n_type);
    void set_rail_type( int n_rail );

    bool get_ir_sn( QString &s_sn );

    // 获取预置点矩阵分析窗口
    MatrixWidget* get_matrix_widget();


    void get_matrix_color_name_by_ana_id( int n_ana_id, QString& out_color_name );
    int get_ana_id_by_matrix_color( const QString matrix_color );
    void clear_all_ana();  //清除所有分析
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

    bool get_ana_now_temp(QString str_ana, TempAnaBuf &temp_info);
    bool get_ana_info( QString str_ana, int &n_id, QString &str_name ,QString& s_type);
    bool get_ana_name( int n_id, QString &str_name );
    void clear_alarm_map();
    void add_no_preset_ana_info( QList<AnaInfo> &ana_list );  //增加非预置点分析
    void clear_matrix_alarm_list();  //清除矩阵报警列表,解决切换到单窗口时会有矩阵报警残留
    void clear_matrix_ana_info();    //预置点删除时清除矩阵分析

    //鼠标按下状态
    void clear_ana_press_status();
    bool get_mouse_press_status();
    void set_mouse_press_status( bool b_press_ctrl );
signals:
    void signal_ana_menu(int n_id_ana, QPoint pt, QString& s_name);
    void signal_ana_delete( int n_id_ana );
    void sig_preset_ana_add( int n_id, AnaItem &ana );//新增预置点分析
    void sig_preset_ana_delete( int n_id );//删除预置点分析
    void sig_preset_ana_change( int n_id, AnaItem &ana_item );//call预置点
    void sig_get_alarm_condtion();

    void sig_alarm_cnd_delete( int n_id_ana );//删除报警条件

    // 矩阵分析信息
    void sig_ana_matrix(QVector<AnaInfo> matrix_ana_list);
    //更新报警条件
    void sig_update_alarm_info();

public slots:
    void slot_temp_ana();
    void timer_update();
    void slot_update_alarm_info();
    void slot_clear_btn_click();

protected:
    void pt_judge_area( const QPoint &pt, QPoint &pt_area );
    void normalized_pt( QPoint &pt1, QPoint &pt2 );
    void set_cursor_pos( );
    void start_area( QPoint &pt_start, int &n_r );
    void shape_append_list( QPoint pt );
    void shape_move_or_change_size( QPoint pt );
    void judge_area( QPoint &pt_area );

public:
    void set_org_sz( int width, int height ) ;
    void judge_org_area( QPoint &pt_area );

    void press_ir_template_ana( QPoint pt );
    void move_ir_template_ana( QPoint pt );
    void release_ir_template_ana( QPoint pt );
    void delete_ana_shape( QPoint pt );
    void delete_shape_mouse_release( QPoint pt );
    void add_list_pt( AnaInfo anainfo );
    void delete_list_pt( int signed_id, bool b_sig = false );

    void show_ptz_guidrail(bool bshow);

    QString get_ana_draw_text( const AnaInfo &ana_info, const float &f_temp, const bool &b_has_temp );

    void set_dynamic_toolbar( bool b_dyc ) { mb_dynamic_toolbar = b_dyc; }
    void set_toolbar_hidden( bool b_hidden );
    void set_draw_line_cfg( QColor clr, int n_line_width, int n_line_style );

    bool get_ana_info( QMap< int , AnaInfo > &map_ana_info );
    void set_ana_info_list( QMap<int, AnaInfo> ana_info_list );
    bool get_sn(QString &s_sn);
    void update_preset_alarm_map( QMap<int, PresetAlarmCondition > &preset_alarm_map );
    void update_no_preset_alarm_map(QMap<int, PresetAlarmCondition > &no_preset_alarm_map );
    void set_matrix_ana_list( QList<QString> ana_info_list );
    void draw_alarm_matrix_ana( QPainter &draw );
    void set_ir_connect( bool b_ir_connected ){ mb_ir_connected = b_ir_connected; }
    bool get_ir_connect(){ return mb_ir_connected; }
    void set_object_wnd_name( QString str_name ) { m_obj_name = str_name; }
    int get_group_num() { return mn_group_number; }

public:
    /**
    * @brief
    * load_preset_all_ana load所有预置点分析信息
    * @param[in] ana_list 分析信息列表
    */
    void load_preset_all_ana( const QList<AnaItem> &ana_item_list, QMap<int, int> &map_sign_id_to_ana_id );

    // 除了预置点客户端，其他客户端调用
    void load_preset_all_ana( int n_group, int n_rail, int n_ptz );
    /**
     * @brief load_all_ana  加载所有分析（包括预置点分析和临时分析）
     * @param n_group
     * @param n_rail
     * @param n_ptz
     * @param list_no_preset_ana_info
     */
    void load_all_ana( int n_group, int n_rail, int n_ptz, QList< AnaInfo > list_no_preset_ana_info );

    // 删除预置点分析
    void delete_preset_all_ana( int n_group );

    /**
     * @brief delete_no_preset_all_ana 删除非预置点分析
     * @param n_group
     */
    void delete_no_preset_all_ana( int n_group );

//    /**
//     * @brief add_preset_all_ana
//     * @param n_group
//     * @param n_rail
//     * @param n_ptz
//     */
//    void add_preset_all_ana( int n_group, int n_rail, int n_ptz );

    /**
    * @brief
    * remove_all_no_perset_chart 清除图表
    */
    void remove_all_no_perset_chart();

    /**
    * @brief
    * delete_single_ana 删除单个分析
    * @param[in] ana_info 分析信息
    */
    void delete_single_ana( AnaInfo &ana_info );

//    void delete_ana( AnaInfo &ana_info );

//    /**
//     * @brief delete_ana
//     * @param ana_info
//     */
//    void delete_ana( QList<AnaInfo> &list_ana_info );

    /**
     * @brief delete_local_ana
     * @param ana_info
     */
    void delete_local_ana( AnaInfo &ana_info );

    /**
     * @brief delete_ana_to_server
     * @param ana_info
     */
    void delete_ana_to_server( AnaInfo &ana_info );

    /**
     * @brief delete_ana_to_server
     * @param list_ana_info
     */
    void delete_ana_to_server( QList<AnaInfo> &list_ana_info );

    /**
     * @brief delete_ana_in_data
     * @param ana_info
     */
    void delete_ana_in_data( AnaInfo &ana_info );

    /**
     * @brief delete_ana_to_chart
     * @param ana_info
     */
    void delete_ana_to_chart( AnaInfo &ana_info );

    /**
     * @brief get_preset_ana_info 获取预置点分析
     * @return
     */
    QList< AnaInfo > get_preset_ana_info( );

    /**
     * @brief get_no_preset_ana_info 获取非预置点分析
     * @param list_ana_info
     */
    QList< AnaInfo > get_no_preset_ana_info( );

    /**
    * @brief
    * load_ana 将当前的分析上传到服务器; 获取温度数据; 增加到图表中
    * @param[in] ana_list 分析列表
    */
    void load_ana( QList<AnaInfo> & ana_list );

    /**
    * @brief
    * load_ana 将当前的分析上传到服务器; 获取温度数据; 增加到图表中
    * @param[in] ana_list 分析列表
    */
    void load_no_preset_ana( QList<AnaInfo> & no_preset_ana_list );

    /**
    * @brief
    * add_no_preset_ana_list_to_server 将非预置点分析上传到服务器; 获取温度数据
    * @param[in] ana_list 分析列表
    */
    void add_no_preset_ana_list_to_server( QList<AnaInfo> & no_preset_ana_list );
    /**
    * @brief
    * add_ana_list_in_data 增加到DataMode中
    * @param[in] ana_info 分析列表
    */
    void add_ana_list_in_data( QList<AnaInfo> & ana_list );
    /**
    * @brief
    * add_ana_list_to_chart 将分析加入图表中
    * @param[in] ana_list 分析列表
    */
    void add_ana_list_to_chart( QList<AnaInfo> & ana_list );

    /**
    * @brief
    * add_ana 将分析上传服务器、增加到DataMode中、增加图表中
    * @param[in] ana_info 单个分析
    */
    void add_ana( AnaInfo &ana_info );
    /**
    * @brief
    * add_ana 将分析上传服务器获取温度数据
    * @param[in] ana_info 单个分析
    */
    void add_ana_to_server( AnaInfo &ana_info );
    /**
    * @brief
    * add_ana 增加到DataMode中
    * @param[in] ana_info 单个分析
    */
    void add_ana_in_data( AnaInfo &ana_info );
    /**
    * @brief
    * add_ana 增加图表中
    * @param[in] ana_info 单个分析
    */
    void add_ana_to_chart( AnaInfo &ana_info );

    /**
     * @brief get_preset_sign_id
     * @param n_sign_id
     * @return
     */
    int get_preset_sign_id( int n_sign_id );

    /**
     * @brief get_rail_no
     * @return 获取导轨号
     */
    int get_rail_no();

    /**
     * @brief get_ptz_no
     * @return 获取云台号
     */
    int get_ptz_no();

    /**
     * @brief set_rail_no
     * @param n_rail_no
     */
    void set_rail_no( const int n_rail_no );

    /**
     * @brief set_ptz_no
     * @param n_ptz_no
     */
    void set_ptz_no( const int n_ptz_no );

    //获取普通分析还是矩阵分析的状态
    bool get_matrix_show();
    //设置普通分析还是矩阵分析的状态
    void set_matrix_show( bool b_matrix_show );

signals:
    void sig_add_ana_to_server( int &n_group, AnaInfo &ana_info );
    void sig_delete_ana_to_server( int &n_group, AnaInfo &ana_info );
    void sig_change_ana_to_server( int &n_group, AnaInfo &old_ana, AnaInfo &new_ana );

    void sig_preset_info( int n_group_number, int n_rail_no, int n_ptz_no );
    void sig_selected_preset( bool &b_selected_preset );

public Q_SLOTS:
     void on_add_point();
     void on_add_line();
     void on_add_rect();
     void on_add_circle();
     void on_alarm_cfg();
     void on_delete();
     void on_focus_out();
     void on_focus_in();
     void on_correct_ir();

     void slot_ptz_pressed();
     void slot_ptz_release();

     //void slot_pan_stop();

     void slot_rail_move_left_or_top();
     void slot_rail_move_right_or_buttom();
     void slot_rail_release();
private slots:
     void slot_on_matrix_btn_clicked();
     void slot_matrix_widget_show();
     void slot_matrix_widget_hide();
private:
    int mn_group_number;

    int mn_rail_no;
    int mn_ptz_no;

    bool mb_realtime;

    int mn_rail_type;             //0 未知类型 3 水平导轨  4 垂直导轨
    int mn_ui_type;               //0 主界面 1 单画面 2 回放界面
    int mb_hide_warn;
    bool mb_hide_calibration;
    bool mb_dynamic_toolbar;

    QColor m_clr_ana;
    int mn_line_width, mn_line_style;

    int mn_alarm_flag; //报警颜色切换标志值

    WndIrThread *mp_thread;
    QMap< int, AnaInfo > m_list_ana_info;
    bool mb_ir_connected;   //判断红外是否断开

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

    //
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

    QPushButton  *mp_left;
    QPushButton  *mp_right;
    QPushButton  *mp_top;
    QPushButton  *mp_bottom;
    QPushButton  *mp_lefttop;
    QPushButton  *mp_leftbottom;
    QPushButton  *mp_righttop;
    QPushButton  *mp_rightbottom;
    //QPushButton  *mp_stop;
    QPushButton *mp_btn_hor_rail_left;
    QPushButton *mp_btn_hor_rail_right;
    QPushButton *mp_btn_ver_rail_top;
    QPushButton *mp_btn_ver_rail_buttom;

    DeleteLabel *mp_delete_ana_label;

    QList<QString> m_matrix_ana_list;
    QMutex m_matrix_ana_mutex;

    bool mb_press_ctrl;//按下Ctrl键的状态
protected:
    //QMap<QString, TempAnaBuf > m_map_temp_now;
    QTimer *mp_timer_update;

    bool mb_preset_setting;

public:
    MatrixWidget* mw_matrix_widget;
    Ir_Ana_Label *mp_ir_label;
    static QMutex m_mutex_sign;
    static int mn_sign_id;
    QMap<int, PresetAlarmCondition > m_preset_alarm_map;
    QMap<int, PresetAlarmCondition > m_no_preset_alarm_map;

    QMutex m_preset_alarm_mutex;
    QMutex m_ana_info_mutex;
    QMutex m_no_preset_alarm_mutex;

    bool mb_matrix_show;//矩阵分析窗口的显示
    QString m_obj_name;

    static int get_sign_id();
    static int get_matrix_sign_id();
    static void shape_to_string( QString &str_shape, const AnaInfo &anainfo );
    static bool string_to_shape( const QString &str_shape, AnaInfo &anainfo );

    void ir_wnd_update();
private:
    // 非预置点 引用id
    static QMap<int, bool> m_map_no_preset_id;
    static void init_no_preset_sign_id_map();
    static void uninit_no_preset_sign_id_map();
    static int  get_no_preset_sigin_id();
    static void update_no_preset_sign_id_map( int n_no_preset_id, bool b_use );
};


class WndIrThread : public QThread
{
     Q_OBJECT
public:
 //   WndIrThread();
    WndIrThread( WndIrPreview *p_ir_wnd );
    ~WndIrThread();
    void change_general_alarm_cond( QString &old_alarm_cond, QString &new_alarm_cond );
    void handle_preset_alarm( std::map<UINT32, std::list<THREE_MONITOR_TEMPERATURE_S> > &preset_ana_temp_map );
    void handle_no_preset_alarm( std::map<UINT32, std::list<THREE_MONITOR_TEMPERATURE_S> > &no_preset_ana_temp_map );
    void construct_ana_list( int n_id );
    void split_matrix_alarm_condition( PresetAlarmCondition &preset_alarm_cond );
    void split_general_alarm_condition( PresetAlarmCondition &preset_alarm_cond );
    int split_num( QString str_ana_id );
    void init_ana_list();
    void run();
    void set_exit( bool b_exit ){ mb_exit = b_exit; }
private:
     bool mb_exit;
     WndIrPreview *mp_ir_wnd;
     int mn_group_id;
     QMap<int, QString> m_matrix_ana_map;
     QMap< int, AnaInfo > m_ana_info_list;
     QList<QString> m_matrix_ana_list;
     FILE *p_file;
};

#endif // WNDIRPREVIEW_H
