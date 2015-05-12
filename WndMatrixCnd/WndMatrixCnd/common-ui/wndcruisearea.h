#ifndef WNDCRUISEAREA_H
#define WNDCRUISEAREA_H

#include <QWidget>
#include "cptzrailview.h"
#include "../common-ui/wndpresetlist.h"

namespace Ui {
class WndCruiseArea;
}

class WndCruiseArea : public QWidget, CPtzRailView
{
    Q_OBJECT

public:
    explicit WndCruiseArea(QWidget *parent , int n_funtion = FuntionShowCruiseAllpt );
    ~WndCruiseArea();

public:
    void init_ctrl();
    void init_conn();

    void move_widget();
    void move_group_name( QRect rct );
    void move_combox( QRect rct );
    void move_btn( QRect rct );
    void move_image( QRect rct );
    bool get_cur_preset_no( int &n_rail, int &n_ptz );
    void set_cruise_state( bool b_cruise );
    void selected_preset(int n_rail , int n_ptz );
    void get_cruise_area_info();
    QString get_group_str_name( int n_group_num );
public:
    void resizeEvent( QResizeEvent *event );

    virtual int onResponse(CBaseMsg& reqMsg,CBaseMsg& rspMsg);
    virtual int onNotify(CBaseMsg& msg);

public:
    void set_group_num( int n_num );
    void set_widget_hidden( bool b_hidden );

    void update_preset_map();
    void update_combo_cruise( QMap<int, CruiseSchemeInfo> &map_cruise );

    void enable_all_ctrl( bool b_enable );
    void clear_all_ctrl_info();

    void update_cruise_ctrl( bool b_cruise );
    void update_map_preset( int n_rail, int n_ptz );
    void set_cruise_selected_preset( int n_rail, int n_ptz ); //选中正在巡检的预置点

signals:
    void sig_enable_ctrl( bool );
    void sig_update_ir_ana( int, int );
 ////   void sig_update_start_cruise();
    void sig_cruise_state( bool );


    // 巡检离开， 删除预置点信息
    void sig_del_preset_all_ana( int );
    void sig_cruise_selected_preset( int, int );
    void sig_get_alarm_info();

public slots:
    void slot_btn_clicked();
    void slot_combo_cruise_change( int );
    void slot_call_preset( int , int );

private:
    bool is_cruiseing();

private:
    Ui::WndCruiseArea *ui;

    int mn_group_number;
    int mn_funtion_map;
    bool mb_preset_display;
    int mn_get_pic_count;

};

#endif // WNDCRUISEAREA_H
