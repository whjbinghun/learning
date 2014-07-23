#ifndef CONNECTCTRL_H
#define CONNECTCTRL_H

#include <QString>
#include "netcontrol.h"


class ConnectCtrl
{
public:
    ConnectCtrl( QObject *p_parent );
    ~ConnectCtrl();
public:
    bool init_connect_ctrl( QString str_ip, int n_port );
    bool disconnect();
    bool is_connect();

    bool transport_ir( bool b_transport );
    bool transport_vi( bool b_transport );

private:
    NetControl *mp_net_control;
    //QList<NetCaptureGroup *> m_lst_capture_group;

    bool mb_connected;
    QObject *mp_main_window;

};

#endif // CONNECTCTRL_H
