#include "threadplayback.h"
#include "netcaptureir.h"

ThreadPlayback::ThreadPlayback(NetCaptureVod *p_net_vod, QObject *parent) :
    QThread(parent)
{
    mb_exit_run = false;
    mp_net_vod = p_net_vod;
}

void ThreadPlayback::run()
{
    while ( !mb_exit_run ) {
        mp_net_vod->thread_play_one_frame();
        int n = mp_net_vod->get_replay_speed();
        if ( n == 0 ) {
            msleep( 40 );
        } else {
            msleep( 1/n * 1000 );
        }
    }
}
