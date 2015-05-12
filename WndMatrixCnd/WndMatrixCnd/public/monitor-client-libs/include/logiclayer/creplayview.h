#ifndef CREPLAYVIEW_H
#define CREPLAYVIEW_H

#include "cbasemsg.h"
#include "cbaseview.h"
#include "netcommon.h"
#include "datamode.h"

class CRePlayView : public CBaseView
{
public:
    CRePlayView();
    virtual ~CRePlayView();
    int replay_num( int n_replay );

    int replay_stop( int n_group_num,int n_session );
    int replay_start( int n_group_num, float f_speed, PLAYBACK_REQ_S &s_playback, QString str_name = "", bool b_clear_buf = true );  //播放请求
    int replay_pause( int n_group_num, int n_session, int n_ctrl );//暂停,恢复,心跳,回放获取调色板类型请求

    int replay_frame( int n_group_num, unsigned int n_session, unsigned int n_frame_num );        //帧播放
    int replay_preset_info( int n_group_num, PLAY_PRESET_REQ_S& m_play_preset );                //预置点播放信息请求

    int replay_set_pelete_clr( int n_group_num, int n_index_color, bool b_pause );                          //回放设置调色板类型请求
    int replay_set_pelete_range( int n_group_num, REPLAY_SET_ADJUSTRANGE_S &s_adjust_range, bool b_pause );    //回放中手动设置色标温度范围请求
    int replay_set_pelete_adjust( int n_group_num, int n_adjust, bool b_pause );                            //回放中设置色标类型手动还是自动请求

    //. 回放分析增删改
    int replay_add_ana( int n_group_num, QString &str_sn, AnaInfo &ana_info, bool b_pause, bool b_change_ana = false );
    int replay_del_ana( int n_group_num, QString &str_sn, AnaInfo &ana_info, bool b_pause, bool b_change_ana = false );
    int replay_change_ana( int n_group_num, QString& str_sn, AnaInfo &old_ana, AnaInfo &new_ana, bool b_pause );

    int replay_play_init( int n_group_num, PLAYBACK_REQ_S &s_playback );//播放初始化请求

    int get_alarm_pic_list( int n_group_num, AlarmPicListSearch &alarm_pic_search );
    int get_alarm_pic_info( int n_group_num, AlarmPicInfoSearch &alarm_pic_search );

    virtual int onResponse( CBaseMsg& reqMsg, CBaseMsg& rspMsg );

};

#endif // CREPLAYVIEW_H
