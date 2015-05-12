#ifndef WNDPRESETLIST_H
#define WNDPRESETLIST_H

#include <QWidget>
#include <QString>
#include <QList>
#include <QMap>
#include <QImage>
#include <QMutex>
#include <QScrollBar>
#include <QMenu>
#include "datacommon.h"
#include "addpresetdialog.h"

enum{
    FuntionSetPreset,
    FuntionSetCruise,
    FuntionShowCruise,
    FuntionShowCruiseAllpt
};

class WndPresetList : public QWidget
{
    Q_OBJECT
public:
    explicit WndPresetList(QWidget *parent = 0);

public:
    void paintEvent( QPaintEvent *event );
    void resizeEvent( QResizeEvent *event );
    void mousePressEvent( QMouseEvent *event );
    void mouseMoveEvent( QMouseEvent *event );
    void mouseReleaseEvent( QMouseEvent *event );
    void mouseDoubleClickEvent( QMouseEvent *event );
    void hideEvent(QHideEvent *);

    bool set_status_get( bool b_get );
    bool cal_image_draw_rect( QRect rct );

    bool update_preset_pos_info( QMap< int,  PresetInfo> &map_preset_info );        //. 更新预置点的位置信息
    bool update_preset_info( QMap< int,  PresetInfo> &map_preset_info, int n_buf_len, unsigned char *p_pic_buf );
    bool update_scheme_info( CruiseSchemeInfo  &cruise );

    bool draw_image( QPainter &draw, QRect rct );
    bool draw_preset( QPainter &draw, QRect rct );

    PresetInfo *pt_in_preset( QPoint pt );
    PresetInfo *pt_in_image( QPoint pt );

    void clear_preset_pos();
    bool delete_preset_info( int n_rail, int n_ptz );
    bool delete_preset_info( PresetInfo *p_info );
    bool clear_preset_info();

    bool add_pic( QString str_pic_path );
    bool add_preset( PresetInfo &preset_info );
    bool change_preset( PresetInfo &preset_info );
    bool set_hor_wnd( bool b_hor_stutas ) {
        mb_hor_stutas = b_hor_stutas;
        cal_image_draw_rect( rect() );
    }
    bool set_funtion( int n_funtion ) { mn_funtion = n_funtion; }

    bool go_to_preset( const int &n_rail_no , const int &n_ptz_no );


    QString get_image_path();
    void set_image_path( QString str_image_path );
    QList< PresetInfo * > & get_preset_info(){ return m_list_preset_info; }
    QVector< CruisePoint > &get_cruise_scheme(){ return m_vct_cruise_pts; }
    PresetInfo *&get_double_preset_info();
    PresetInfo * get_curr_select_preset_info(){ return mp_select_preset_info; }

    void set_cursor( QPoint pt_pos );

    int get_offset_pos( bool b_hor );
    void preset_area( QPoint &pt, QPoint &pt_area );
    void get_image_pos( QRect &rct );
    void calibration_preset();
	bool get_image_null() { return m_image_pic.isNull(); }

    bool has_cruise_pt( int n_rail, int n_ptz );
    void set_arrive_preset( int n_rail, int n_ptz );

    bool get_cur_preset( int &n_rail, int &n_ptz );
    void set_cruise_state( bool b_cruise );
    void set_selected_preset(PresetInfo &selected_preset_info);

signals:
    void sig_add_preset( QString str_name, int n_x, int n_y );
    void sig_del_preset( int n_rail_no, int n_ptz_no );
    void sig_change_preset( int n_rail_no, int n_ptz_no, QString str_name );
    void sig_call_preset( int n_rail_no, int n_ptz_no );
    void sig_preset_list_update();
    void sig_preset_scheme_update();
    void sig_change_preset_ana( int n_rail, int n_ptz, QString str_name );    
    void sig_scheme_pt_ctrl( int n_rail_no, int n_ptz_no );
    void sig_preset_btn_enable( bool b_btn_enable );

    void sig_move_preset_pos();

public slots:
    void slot_scroll_value_changed( int n_value );
    void slot_delete_preset();
    void slot_edit_preset();
    void slot_save_ana();

private:
    PresetInfo *get_double_preset_info( int n_rail, int n_ptz );

private:
    PresetInfo m_selected_preset_info; //选中的预置点信息
    int mn_funtion;
    bool mb_get_info;
    bool mb_hor_stutas; //.是否水平状态
    float mf_image_scale_x, mf_image_scale_y;
    int mn_width, mn_height;
    int mn_draw_offset_x, mn_draw_offset_y;//图片与当前窗口之间的偏移量

    QString m_str_pic_path;
    QImage m_image_pic;

    QList< PresetInfo * > m_list_preset_info;
    QMap< int, int > m_map_preset;//为了不让预置点号出现重复
    QVector< CruisePoint > m_vct_cruise_pts;

    QScrollBar *mp_v_scrollbar;
    QScrollBar *mp_h_scrollbar;
    QMenu *mp_menu;
    QAction *mp_menu_save_btn;
    PresetInfo *mp_select_preset_info;
    PresetInfo *mp_double_preset_info;

    bool mb_mouse_down;

    QPoint m_pt_start_down;
    bool mb_preset_change; //预置点是否改变过(比如移动)

    int mn_rail;
    int mn_ptz;
};

#endif // WNDPRESETLIST_H
