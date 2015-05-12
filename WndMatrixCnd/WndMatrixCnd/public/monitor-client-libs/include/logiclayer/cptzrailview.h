#ifndef CPTZRAILVIEW_H
#define CPTZRAILVIEW_H

#include "cbaseview.h"
#include "cpeleteview.h"

class CPtzRailView : public CPeleteView
{
public:
    CPtzRailView();
    virtual ~CPtzRailView();

    int ptz_ctrl( int n_group_num, PtzDirect n_dir );//参考  PtzDirect
    int ptz_ctrl( QString str_sn, PtzDirect n_dir );
    int ptz_set_speed( int n_group_num, int n_param );

    int rail_ctrl( QString str_sn, RailDirect n_dir );
    int rail_ctrl( int n_group_num, RailDirect n_dir );
    int rail_preset_ctrl( int n_group_num, int n_ctrl, int n_ctrl_param );// n_ctrl :RailCmdDelPreset  RailCmdGotoPreset RailCmdGotoPos RailCmdSetSpeed

    int set_preset( int n_group_num, int n_rail, int n_ptz, QString str_name );
    int set_preset_pos( int n_group_num, int n_rail, int n_ptz );  //.PresetSetPos

    int call_preset( int n_group_num, int n_rail, int n_ptz );
    int delete_preset( int n_group_num, int n_rail, int n_ptz );
    int get_preset_pts( int n_group_num );

    int add_preset_ana( int n_group_num, int n_rail, int n_ptz, QString str_ana, QString str_name );    //PresetAnaAdd
    int change_preset_ana( int n_group_num, int n_rail, int n_ptz, int n_id_ana, QString str_ana, QString str_name ); //PresetAnaChange
    int delete_preset_ana( int n_group_num, int n_rail, int n_ptz, int n_id_ana );  //PresetAnaDel

    int get_preset_alarm( int n_group_num, int n_rail, int n_ptz );         //PresetAlarmGet
    int add_preset_alarm( int n_group_num, int n_rail, int n_ptz, QString str_alarm, QString str_name );  //PresetAlarmAdd
    int change_preset_alarm( int n_group_num, int n_rail, int n_ptz, int n_id_alarm, QString str_alarm, QString str_name ); //PresetAlarmChange
    int delete_preset_alarm( int n_group_num, int n_rail, int n_ptz, int n_id_alarm );  //PresetAlarmDel

    int get_preset_record_condition( int n_group_num, int n_rail, int n_ptz );    //  PresetRecordConditionGet
    int set_preset_record_condition( int n_group_num, int n_rail, int n_ptz, int n_interval, float f_temp, float f_hreshold ); //PresetRecordConditionSet

    int set_cruise_info( int n_group_num );
    int get_cruise_info( int n_group_num );
    int get_cruise_cur_preset( int n_group_num );

    int delete_cruise( int n_group_num, int n_cruise );
    int start_cruise( int n_group_num, int n_cruise );
    int strop_cruise( int n_group_num, int n_cruise );

    int set_pic_info( int n_group_num, QList< PresetItem > &list_pst_pos );
    int set_pic( int n_group_num, QString str_pic );
    int get_pic( int n_group_num );
    int del_pic( int n_group_num );

public:
    virtual int onResponse(CBaseMsg& reqMsg,CBaseMsg& rspMsg);
    virtual int onNotify(CBaseMsg& msg);

};

#endif // CPTZRAILVIEW_H
