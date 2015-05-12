#ifndef CLOGINVIEW_H
#define CLOGINVIEW_H

#include "cbasemsg.h"
#include "cbaseview.h"

class CLoginView : public CBaseView
{
public:
    CLoginView();
    virtual ~CLoginView();

    int login_check_version( char *p_user, char *p_password, char *p_ip, int n_port, ClientType n_client_type = ClientNone );
    int login_in();

    int get_group_xml();
    int send_heartbeat();

    int get_pic_on_time( int n_group, unsigned int n_sec, unsigned int n_msec );

    virtual int onResponse(CBaseMsg& reqMsg,CBaseMsg& rspMsg);
    virtual int onNotify(CBaseMsg& msg);

};

#endif // CLOGINVIEW_H
