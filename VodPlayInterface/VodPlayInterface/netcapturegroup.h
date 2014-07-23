#ifndef NETCAPTUREGROUP_H
#define NETCAPTUREGROUP_H
#include <qstring.h>
#include <qdatetime.h>

class NetCaptureVod;

class NetCaptureGroup
{
public:
    NetCaptureGroup();
    ~NetCaptureGroup();

    NetCaptureVod *get_net_ir() { return mp_net_ir; }
    NetCaptureVod *get_net_vi() { return mp_net_vi; }

    void disconnect_ir();
    void disconnect_vi();

    bool connect_vod( int n_class, const char *s_ip, int n_port, QString str_sn );
    bool connect_vod_ir( const char *s_ip, int n_port, QString str_sn  );
    bool connect_vod_vi( const char *s_ip, int n_port, QString str_sn  );

    QString get_net_ir_sn();
    QString get_net_vi_sn();

    void get_ir_replay_secs( unsigned int &n_sec, unsigned int &n_usec );
    void init_replay();

private:
    NetCaptureVod *mp_net_ir;
    NetCaptureVod *mp_net_vi;
    int mn_group_num;
};

#endif // NETCAPTUREGROUP_H
