#ifndef WNDVIPREVIEW_H
#define WNDVIPREVIEW_H

#include <QFrame>
#include <QTimer>
#include "cmainview.h"
#include "cvipreview.h"
#include "vi_ana_label.h"
#include <QPushButton>
#include "cptzrailview.h"
#include "WndCommon.h"
#include <QGraphicsOpacityEffect>

class WndViPreview : public QFrame,public CViPreview
{
    Q_OBJECT
public:
    explicit WndViPreview(QWidget *parent = 0);
    ~WndViPreview();

public:
    void paintEvent( QPaintEvent *event );
    void resizeEvent( QResizeEvent *event );
    void enterEvent( QEvent *e );
    void leaveEvent( QEvent *e );
    void keyPressEvent( QKeyEvent  *event );
    void mouseDoubleClickEvent(QMouseEvent *event);
    virtual bool event( QEvent *e );

    void init_widget();

    void set_group_number( int n_num );
    void set_dynamic_toolbar( bool b_dyc ) { mb_dynamic_toolbar = b_dyc; }
    void set_funtion( bool b_backplay );

    void tool_bar_function( int n_press_status, bool b_fullscreen_show, bool b_down_or_up );
    void capture_image();

    void init_connect();
    void set_toolbar_hidden( bool b_hidden );
#ifdef QT_DEBUG
    void test_connect_main_stream();
    void test_connect_sub_stream();
    void test_disconnect_main_stream();
    void test_disconnect_sub_stream();

//    void test_connect_ir();
//    void test_disconnect_ir();
#endif
    void vi_recovery_show();//可见光恢复显示
    void set_vi_connect( bool b_vi_connected ){ mb_vi_connected = b_vi_connected; }
    void set_streaming( bool b_stream ) { mb_sub_streaming = b_stream; }
    void set_vi_stream_status( int n_stream_type, bool b_connected );
    QString get_group_str_name();
public:
    //云台和导轨控制
    void init_pan_rail_btn();

    void add_pan_btn();
    void add_rail_btn();
    void init_btn_connect();
    void move_btn_pan();
    void move_btn_rail();
    void set_pan_hidden( bool b_hidden );
    void set_rail_hidden( bool b_hidden );
    //云台和导轨透明度
    void set_btn_pan_transparency( float f_level );
    void set_btn_rail_transparency( float f_level );
signals:
    void sig_focus_in();
    void sig_focus_out();
    void sig_focus_in_stop();
    void sig_focus_out_stop();

    void sig_show_fullscreen();
    void sig_show_normal();
    void sig_connect_device( int n_group_num, DeviceType device_type, bool b_sub_stream  );
    void sig_disconnect_device( int n_group_num, DeviceType device_type, bool b_sub_stream  );
public slots:
    void timer_update();

    void slot_ptz_pressed();
    void slot_ptz_release();
    void slot_rail_move_left_or_top();
    void slot_rail_move_right_or_buttom();
    void slot_rail_stop();
private:
    int mn_group_number;
    bool mb_backplay;

    QTimer *mp_timer_update;

    Vi_Ana_Label *mp_tool_label;
    bool mb_dynamic_toolbar;
    QPoint m_pt_wnd_start;
    QPoint m_pt_wnd_end;
    bool mb_sub_streaming;
    bool mb_connecting;
    bool mb_vi_connected;
    bool mb_full_screen;
    bool mb_sub_stream_connected;   //子码流是否连接
    bool mb_main_stream_connected;  //主码流是否连接
    QString m_str_group_name;
private:
    //云台和导轨控制
    QPushButton *mp_btn_left;
    QPushButton *mp_btn_right;
    QPushButton *mp_btn_top;
    QPushButton *mp_btn_buttom;
    QPushButton *mp_btn_lefttop;
    QPushButton *mp_btn_righttop;
    QPushButton *mp_btn_leftbuttom;
    QPushButton *mp_btn_rightbuttom;

    QPushButton *mp_btn_hor_rail_left;
    QPushButton *mp_btn_hor_rail_right;
    QPushButton *mp_btn_ver_rail_top;
    QPushButton *mp_btn_ver_rail_buttom;

    QString ms_pan_sn;
    QString ms_rail_sn;
    CPtzRailView *mp_ptz_rail_view;

    int mn_rail;
    //云台
    QList<QPushButton*> m_list_btn;
    QList<QGraphicsOpacityEffect*> m_list_effect;
    //导轨
    QList<QPushButton*> m_list_rail_btn;
    QList<QGraphicsOpacityEffect*> m_list_rail_effect;
};

#endif // WNDVIPREVIEW_H
