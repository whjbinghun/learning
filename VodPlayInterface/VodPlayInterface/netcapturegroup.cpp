#include "netcapturegroup.h"
#include "netcaptureir.h"

NetCaptureGroup::NetCaptureGroup()
    : mp_net_ir( 0 )
    , mp_net_vi( 0 )
{
}

NetCaptureGroup::~NetCaptureGroup()
{
    if ( mp_net_ir ) {
        mp_net_ir->disconnect_server();
        delete mp_net_ir;
        mp_net_ir = 0;
    }

    if ( mp_net_vi ) {
        mp_net_vi->disconnect_server();
        delete mp_net_vi;
        mp_net_vi = 0;
    }
}

QString NetCaptureGroup::get_net_ir_sn()
{
    if ( !mp_net_ir ) return "";

    return mp_net_ir->get_sn();
}

QString NetCaptureGroup::get_net_vi_sn()
{
    if ( !mp_net_vi ) return "";

    return mp_net_vi->get_sn();
}

void NetCaptureGroup::disconnect_ir()
{
    if ( !mp_net_ir ) return;

    if( mp_net_ir->is_connected() ) {
        mp_net_ir->disconnect_server();
    }

    //.解压库销毁过程有问题
    //delete mp_net_ir;
    mp_net_ir = 0;
}

void NetCaptureGroup::disconnect_vi()
{
    if ( !mp_net_vi ) return;

    if( mp_net_vi->is_connected() ) {
        mp_net_vi->disconnect_server();
    }

    //.解压库销毁过程有问题
    //delete mp_net_vi;
    mp_net_vi = 0;
}

bool NetCaptureGroup::connect_vod( int n_class, const char *s_ip, int n_port, QString str_sn )
{
    if ( n_class == 1 ) {
        return connect_vod_ir( s_ip, n_port, str_sn );
    } else if ( n_class == 0 ) {
        return connect_vod_vi( s_ip, n_port, str_sn );
    } else {
        return false;
    }
}

bool NetCaptureGroup::connect_vod_ir( const char *s_ip, int n_port, QString str_sn  )
{
    if ( mp_net_ir ) {
         if ( mp_net_ir->get_sn() == str_sn ) {
             return true;
         }

         mp_net_ir->disconnect_server();
         mp_net_ir->set_sn( str_sn );
         mp_net_ir->connect_server( s_ip, n_port );
    } else {
        mp_net_ir = new NetCaptureVod( str_sn, 1, s_ip, n_port );
        mp_net_ir->connect_server();
    }

    return true;
}

bool NetCaptureGroup::connect_vod_vi( const char *s_ip, int n_port, QString str_sn  )
{
    if ( mp_net_vi ) {
         if ( mp_net_vi->get_sn() == str_sn ) {
             return true;
         }

         mp_net_vi->disconnect_server();
         mp_net_vi->set_sn( str_sn );
         mp_net_vi->connect_server( s_ip, n_port );
    } else {
        mp_net_vi = new NetCaptureVod( str_sn, 0, s_ip, n_port );
        mp_net_vi->connect_server();
    }

    return true;
}

void NetCaptureGroup::get_ir_replay_secs( unsigned int &n_sec, unsigned int &n_usec )
{
    if ( mp_net_ir ) {
        mp_net_ir->get_replay_secs( n_sec, n_usec );
    }
}

void NetCaptureGroup::init_replay()
{
    if ( mp_net_vi ) {
        mp_net_vi->set_replay( true );
    }

    if ( mp_net_ir ) {
        mp_net_ir->set_replay( true );
    }
}
