#ifndef CMAINVIEW_H
#define CMAINVIEW_H

#include "cbasemsg.h"
#include "cbaseview.h"

class CMainView : public CBaseView
{
public:
    CMainView();
    virtual ~CMainView();

    int connect_group_device( int n_group_num, int n_type, bool b_sub_stream = false );
    int disconnect_group_device( int n_group_num, int n_type, bool b_sub_stream = false );

    int connect_device_key_media( QString str_sn, int n_type );
    int disconnect_device_key_media( QString str_sn, int n_type );

    int ctrl_live_device( int n_ctrl, QString str_sn, int n_type, bool b_sub_stream = false, int n_group_num = 0 );

    virtual int onResponse(CBaseMsg& reqMsg,CBaseMsg& rspMsg);
    virtual int onNotify(CBaseMsg& msg);
};

#endif
