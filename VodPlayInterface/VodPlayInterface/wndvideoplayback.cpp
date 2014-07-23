#include "wndvideoplayback.h"
#include <QMessageBox>
#include "ui_vodplayinterface.h"
#include "netcontrol.h"

WndVideoPlayback::WndVideoPlayback(QWidget *parent) :
    QWidget(parent)
  , mb_playback_connect_req( false )
  , mn_timer_connect_count( 0 )
  , mp_timer_connect_req( NULL )
  , mp_end_time( 0 )
  , mp_end_date( 0 )
  , mp_start_date( 0 )
  , mp_start_time( 0 )
  , mn_replay_cmd( 0 )
  , mb_first_connect( true )
{
    mp_timer_connect_req = new QTimer(this);
    connect( mp_timer_connect_req, SIGNAL( timeout() ), this, SLOT( slot_timer_connect_req() ) );

    mp_timer_playback_cmd = new QTimer(this);
    connect( mp_timer_playback_cmd, SIGNAL( timeout() ), this, SLOT( slot_timer_playback_cmd_req() ) );

    mp_timer_update_slider = new QTimer(this);
    connect( mp_timer_update_slider, SIGNAL( timeout() ), this, SLOT( slot_timer_update_slider() ) );
}

WndVideoPlayback::~WndVideoPlayback()
{
    if ( mp_timer_connect_req ) {
        mp_timer_connect_req->stop();
        delete mp_timer_connect_req;
        mp_timer_connect_req = NULL;
    }

    if ( mp_timer_playback_cmd ) {
        mp_timer_playback_cmd->stop();
        delete mp_timer_playback_cmd;
        mp_timer_playback_cmd = NULL;
    }

    if ( mp_timer_update_slider ) {
        mp_timer_update_slider->stop();
        delete mp_timer_update_slider;
        mp_timer_update_slider = NULL;
    }
}

void WndVideoPlayback::init_ctrl( Ui::VodPlayInterface *p_main_wnd, NetControl *p_net_ctrl )
{
    mp_wnd_ir_display = p_main_wnd->id_back_frame_ir;
    mp_wnd_vi_display = p_main_wnd->id_back_frame_vi;

    mp_wnd_ir_display->set_net_ctrl( p_net_ctrl );

    mp_slider_play = p_main_wnd->id_back_slider_play;
    mp_btn_start_or_pause = p_main_wnd->id_back_btn_start;
    mp_btn_stop = p_main_wnd->id_back_btn_stop;
    mp_btn_pre_frame = p_main_wnd->id_back_btn_pre_frame;
    mp_slider_play = p_main_wnd->id_back_slider_play;
    mp_combo_group = p_main_wnd->id_back_combobox_device;
    mp_start_date = p_main_wnd->id_back_date_start;
    mp_start_time = p_main_wnd->id_back_time_start;
    mp_end_date = p_main_wnd->id_back_date_end;
    mp_end_time = p_main_wnd->id_back_time_end;

    mp_start_date->setDate( QDate(2013, 9, 23) );
    mp_start_time->setTime( QTime(16, 37, 54) );

    mp_end_date->setDate( QDate(2014, 1, 1) );
    mp_end_time->setTime( QTime(0, 0, 0) );

    mp_net_ctrl = p_net_ctrl;
    mp_main_wnd = p_main_wnd;

    connect( mp_btn_start_or_pause, SIGNAL( clicked() ), this, SLOT( slot_btn_start_or_pause() ) );
    connect( mp_btn_stop, SIGNAL( clicked() ), this, SLOT( slot_btn_strop() ) );
    connect( mp_slider_play, SIGNAL( valueChanged(int) ), this, SLOT( slot_slider_play_change( int ) ) );
    connect( mp_combo_group, SIGNAL( currentIndexChanged(int) ), this, SLOT( slot_combo_change_group(int) ) );
}

void WndVideoPlayback::update_ctrl_info()
{
    QMap< int, DeviceGroup > map_device_group;
    mp_net_ctrl->get_group_device( map_device_group  );

    disconnect( mp_combo_group, SIGNAL( currentIndexChanged(int) ), this, SLOT( slot_combo_change_group(int) ) );
    mp_combo_group->clear();

    QMap< int, DeviceGroup >::iterator it = map_device_group.begin();
    for ( ; it != map_device_group.end(); ++it ) {
        mp_combo_group->addItem( QString( "组号 %1").arg(it.key()), it.key() );
    }

    connect( mp_combo_group, SIGNAL( currentIndexChanged(int) ), this, SLOT( slot_combo_change_group(int) ) );
}

void WndVideoPlayback::slot_btn_start_or_pause()
{
    bool b_pause = mp_net_ctrl->get_playback_pause();
    int n_group_connect = mp_net_ctrl->get_playback_group_num();
    if ( b_pause ) {
        if ( true || mb_first_connect ) {
            QString str_min = mp_start_date->date().toString("yyyyMMdd")
                    + mp_start_time->time().toString("hhmmss");
            QString str_max = mp_end_date->date().toString("yyyyMMdd")
                    + mp_end_time->time().toString("hhmmss");;
            mn_replay_cmd = mp_net_ctrl->send_playback_play( n_group_connect, 1, str_min, str_max );
        } else {
            mn_replay_cmd = mp_net_ctrl->send_playback_play( n_group_connect, 1 );
        }
    } else {
        mn_replay_cmd = mp_net_ctrl->send_playback_pause( n_group_connect );
    }
    mn_timer_connect_count = 0;
    mp_timer_playback_cmd->start( 100 );
    setEnabled( false );
}

void WndVideoPlayback::slot_btn_strop()
{
    mn_replay_cmd = mp_net_ctrl->send_playback_disconnect( mp_net_ctrl->get_playback_group_num() );

    mn_timer_connect_count = 0;
    mp_timer_playback_cmd->start( 100 );
    setEnabled( false );
}

void WndVideoPlayback::slot_timer_update_slider()
{
    NetCaptureGroup *p_group = mp_net_ctrl->get_playback_group();
    int n_group_num = mp_net_ctrl->get_playback_group_num();

    unsigned int n_sec, n_usec;
    if ( p_group && n_group_num > 0 ) {
        p_group->get_ir_replay_secs( n_sec, n_usec );
        QDateTime tm;
        tm.setTime_t( n_sec );
        mp_main_wnd->id_back_btn_replay->setText( tm.toString("dd hh:mm:ss:") + QString("%1").arg(n_usec) );
    }
}

void WndVideoPlayback::slot_slider_play_change( int n_value )
{
    int n_group_connect = mp_net_ctrl->get_playback_group_num();
    mn_replay_cmd = mp_net_ctrl->send_playback_pause( n_group_connect );
    mn_timer_connect_count = 0;
    mp_timer_playback_cmd->start( 100 );
    setEnabled( false );
}

void WndVideoPlayback::slot_timer_connect_req()
{
    if ( ++mn_timer_connect_count > 80 ) {
        QMessageBox::information( this, "提示", "切换失败" );
        mp_timer_connect_req->stop();
        mb_playback_connect_req = false;
        mn_timer_connect_count = 0;
        mn_replay_cmd = 0;
        setEnabled( true );
        return;
    }

    int n_result_diconnect(0), n_result_connect(0);
    mp_net_ctrl->get_playback_complete_connect_req( IR_MONITOR_REPLAY_STOP, n_result_diconnect );
    mp_net_ctrl->get_playback_complete_connect_req( IR_MONITOR_REPLAY_SETUP, n_result_connect );
    if ( n_result_connect == 1 ) {
        mp_timer_connect_req->stop();
        mb_playback_connect_req = false;
        mn_timer_connect_count = 0;
        mn_replay_cmd = 0;
        setEnabled( true );

        set_enable_playback_ctrl( true );
    }
}

void WndVideoPlayback::slot_timer_playback_cmd_req()
{
    if ( ++mn_timer_connect_count > 100 ) {
        QMessageBox::information( this, "提示", "操作超时" );
        mp_timer_playback_cmd->stop();
        mn_timer_connect_count = 0;
        setEnabled( true );
        return;
    }

    int n_result( 0 );
    mp_net_ctrl->get_playback_complete_connect_req( mn_replay_cmd, n_result );
    if ( n_result >= 0 ) {
        if ( n_result != 1 ) {
            QMessageBox::information( this, "提示", "操作失败" );
        } else {
            if ( mn_replay_cmd == IR_MONITOR_REPLAY_PLAY ) {
                if ( mb_first_connect ) { mb_first_connect = false; }
                mp_btn_start_or_pause->setText("‖");
            } else if ( mn_replay_cmd == IR_MONITOR_REPLAY_PAUSE ) {
                mp_btn_start_or_pause->setText("▶");
            }
        }

        mp_timer_playback_cmd->stop();
        mn_timer_connect_count = 0;

        setEnabled( true );
    }
}

void WndVideoPlayback::slot_combo_change_group( int n_index )
{
    if ( mb_playback_connect_req ) {
        QMessageBox::information( this, "提示", "正在切换组！" );
        return;
    }

    int n_group_num = mp_combo_group->itemData( n_index ).toInt();
    int n_group_connet = mp_net_ctrl->get_playback_group_num();
    if (  n_group_connet == n_group_num ) return;

    setEnabled( false );
    set_enable_playback_ctrl( false );

    mp_net_ctrl->clear_playback_connect_req();
    mp_net_ctrl->send_playback_disconnect( n_group_connet );
    mp_net_ctrl->send_playback_connect( n_group_num );
    mp_btn_start_or_pause->setText("▶");
    mb_playback_connect_req = true;
    mn_timer_connect_count = 0;
    mp_timer_connect_req->start( 100 );
    mb_first_connect = true;
}

void WndVideoPlayback::set_enable_playback_ctrl(bool b_enable)
{
    mp_main_wnd->id_back_btn_first_frame->setEnabled( b_enable );
    mp_main_wnd->id_back_btn_pre_frame->setEnabled( b_enable );
    mp_main_wnd->id_back_btn_next_frame->setEnabled( b_enable );
    mp_main_wnd->id_back_btn_last_frame->setEnabled( b_enable );
    mp_main_wnd->id_back_btn_replay->setEnabled( b_enable );
    mp_main_wnd->id_back_btn_play_speed->setEnabled( b_enable );
    mp_main_wnd->id_back_btn_start->setEnabled( b_enable );
    mp_main_wnd->id_back_btn_stop->setEnabled( b_enable );
    mp_main_wnd->id_back_slider_play->setEnabled( b_enable );
    mp_main_wnd->id_back_widget_connect_status->setEnabled( b_enable );
}

