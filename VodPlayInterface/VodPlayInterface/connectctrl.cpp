#include "connectctrl.h"

ConnectCtrl::ConnectCtrl( QObject *p_parent )
    :mb_connected( false )
    , mp_main_window(p_parent)
{
    mp_net_control = new NetControl( p_parent );
}

ConnectCtrl::~ConnectCtrl()
{
    delete mp_net_control;
    disconnect();

    //if ( mp_net_control ) { delete mp_net_control; mp_net_control = 0; }
}

bool ConnectCtrl::init_connect_ctrl( QString str_ip, int n_port )
{
    if ( mb_connected ) {
        return false;
    }

    bool b_connected = mp_net_control->connect_server( str_ip, n_port );
    mb_connected = b_connected;
    return b_connected;
}

bool ConnectCtrl::disconnect()
{
    //if ( !mb_connected ) return true;
    //return mp_net_control->disconnect_server();
}

bool ConnectCtrl::is_connect()
{
    return mb_connected;
}

bool ConnectCtrl::transport_ir( bool b_transport )
{
    if ( !mb_connected ) {
        return false;
    }
}

bool ConnectCtrl::transport_vi( bool b_transport )
{
    if ( !mb_connected ) return false;

}
