#include "netcaptureir.h"
#include <QColor>
#include "vodplayinterface.h"
#include "video_decode_api.h"
#include "threadplayback.h"

#define FRAME_SIZE 640*480*4
//extern VodPlayInterface * vod_ir_play_window; //extern

NetCaptureVod::NetCaptureVod( QString str_sn, int n_class, const char *s_server_ip, int i_server_port )
    : NetCapture( s_server_ip, i_server_port )
    , m_str_sn ( str_sn )
    , mn_class( n_class )
    , mb_replay( false )
    , mp_rev_buff( 0 )
    , mn_rev_buff_len( 2048 )
    , mn_rev_read_len( 0 )
    , mn_replay_speed( 0 )
{
    if( mn_class == DEVICE_INFRARED ) {
        m_vod_image = QImage( 640, 480, QImage::Format_RGBA8888);
    } else if( mn_class == DEVICE_VISIBLELIGHT) {
        m_vod_image = QImage( 1920, 1080, QImage::Format_RGBA8888 );
    }
    video_decode_init( &mh_video_decode, 0, callback_ir_decode_out_data, this );

    mp_rev_buff = new unsigned char[mn_rev_buff_len];

    mp_thread_play_back = new ThreadPlayback( this );
    mp_thread_play_back->start();
}

NetCaptureVod::~NetCaptureVod()
{
    video_decode_deinit( &mh_video_decode );
    if ( mp_rev_buff ) {
        delete mp_rev_buff;
        mp_rev_buff = NULL;
    }

    if ( mp_thread_play_back ) {
        mp_thread_play_back->exit_thread();
        delete mp_thread_play_back;
        mp_thread_play_back = NULL;
    }
}

void NetCaptureVod::on_receive()
{
    qCritical( "no:%d,class:%d", 0, mn_class );
    NetCapture::on_receive();

    if( mn_class == DEVICE_VISIBLELIGHT ) {
        qDebug() << mn_class;
    }
    while( m_data_list.size() > 0 ) {
        NetworkData nd = m_data_list.front();
        m_data_list.pop_front();

        struct timeval begin,end;
        gettimeofday( &begin, NULL );
        if ( !mb_replay ) {
            on_receive_realtime( (unsigned char *)nd.buf, nd.len );
        } else {
            on_receive_playback( (unsigned char *)nd.buf, nd.len );
        }
        gettimeofday( &end, NULL );
        qWarning() << "delay" << end.tv_sec - begin.tv_sec << ":" << end.tv_usec - begin.tv_usec;
        delete []nd.buf;
    }
}

void NetCaptureVod::on_receive_realtime( unsigned char *p_buf, int n_buf_len )
{
    video_put_data( mh_video_decode, p_buf, n_buf_len  );
}

void NetCaptureVod::on_receive_playback( unsigned char *p_buf, int n_buf_len )
{
    int n_buf_total_len = n_buf_len + mn_rev_read_len;
    if ( n_buf_total_len > mn_rev_buff_len ) {
        unsigned char *p_new_buf = new unsigned char[n_buf_total_len];
        memset( p_new_buf, 0, n_buf_total_len );
        memcpy( p_new_buf, ( void * )( mp_rev_buff ), mn_rev_read_len );

        delete[] mp_rev_buff;
        mp_rev_buff = p_new_buf;
    }

    memcpy( ( void * )( mp_rev_buff+mn_rev_read_len ), p_buf, n_buf_len );
    mn_rev_read_len += n_buf_len;

    if ( n_buf_total_len < sizeof( MONITOR_TIME_HEAD_S ) ) return;

    while ( true ) {
        MONITOR_TIME_HEAD_S package_rev;
        memcpy( &package_rev, mp_rev_buff, sizeof( MONITOR_TIME_HEAD_S ) );

        int n_data_len = package_rev.ui_Length;
        if ( n_data_len > mn_rev_read_len ) return;

        static int n_count = 0;
        if ( package_rev.ui_Magic != 0xA1A11A1A ) {
            int a = n_count;
            a = 1;
        }
        n_count++;

        mn_rev_read_len -= n_data_len;

        int n_buf_date_len = n_data_len - sizeof( MONITOR_TIME_HEAD_S );
        unsigned char *p_buf_date = new unsigned char[n_buf_date_len];
        memcpy( p_buf_date, mp_rev_buff + sizeof( MONITOR_TIME_HEAD_S ), n_buf_date_len );

        PlaybackBuf play_buff;
        play_buff.p_buff = p_buf_date;
        play_buff.n_len_buff = n_buf_date_len;

        m_mutex_playback_buff.lock();
        m_list_replay_buff.push_back( play_buff );
        m_mutex_playback_buff.unlock();

        m_mutex_playback_head.lock();
        m_list_replay_time_head.push_back( package_rev );
        m_mutex_playback_head.unlock();

        memcpy( mp_rev_buff, mp_rev_buff+n_data_len, mn_rev_read_len );
    }
}

int NetCaptureVod::callback_ir_decode_out_data( unsigned char *puc_out_buf, unsigned int ui_data_len, int width, int height, void *p_user_data )
{
   NetCaptureVod *p_this = ( NetCaptureVod * )p_user_data;
   if ( p_this != NULL ) {
        p_this->callback_vod_decode( puc_out_buf, ui_data_len, width, height );
    }
    return 0;
}

void NetCaptureVod::callback_vod_decode( unsigned char *puc_out_buf, unsigned int ui_data_len, int width, int height )
{
    /*if ( mn_ir_connect_status == YF_Disconnected ) {
        m_ir_image.fill( QColor( 200, 200, 200 ) );
    } else {
        memcpy( m_ir_image.bits(), puc_out_buf, ui_data_len );
    }*/
    memcpy( m_vod_image.bits(), puc_out_buf, ui_data_len );

    if ( mb_replay ) {
        QMutexLocker lck( &m_mutex_playback_head );

        QList<MONITOR_TIME_HEAD_S>::iterator it = m_list_replay_time_head.begin();
        if ( it != m_list_replay_time_head.end() ) {
            QMutexLocker lc( &m_mutex_now_time_head );
            m_now_replay_time_head = *it;
        }
        m_list_replay_time_head.pop_front();
    }

    //update();
    //emit get_ir_image( m_ir_image, mn_group_num );
}

void NetCaptureVod::get_replay_secs(unsigned int &n_sec, unsigned int &n_usec)
{
    QMutexLocker lc( &m_mutex_now_time_head );
    n_sec = m_now_replay_time_head.ui_sec;
    n_usec = m_now_replay_time_head.ui_usec;
}

void NetCaptureVod::thread_play_one_frame()
{
    unsigned char *p_buff = 0;
    int n_buf_len = 0;
    m_mutex_playback_buff.lock();
    QList<PlaybackBuf>::iterator it = m_list_replay_buff.begin();
    if ( it != m_list_replay_buff.end() ) {
        p_buff = it->p_buff;
        n_buf_len = it->n_len_buff;
        m_list_replay_buff.pop_front();
    }
    m_mutex_playback_buff.unlock();

    if ( p_buff ) {
       // video_put_data( mh_video_decode, p_buff, n_buf_len  );
        delete[] p_buff;
    }
}
