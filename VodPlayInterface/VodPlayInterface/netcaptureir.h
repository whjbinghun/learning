#ifndef NETCAPTUREIR_H
#define NETCAPTUREIR_H

#include <QTcpSocket>
#include <netcapture.h>
#include <QImage>
#include <QList>
#include <QMutex>
#include <QThread>
#include "video_decode_api.h"

class VodPlayInterface;
class ThreadPlayback;

typedef struct tagMONITOR_TIME_HEAD_S{
    unsigned int ui_Magic; //该值固定为0xA1A11A1A;
    unsigned int ui_type;  //  1表示校准包
    unsigned int ui_sec;  //  帧的UTC时间的秒值
    unsigned int ui_usec;  // 帧的微秒值
    unsigned int ui_Length;  // 数据长度，包括头长度和其携带的数据的长度。
} MONITOR_TIME_HEAD_S;

typedef struct _tagPlaybackBuf{
    unsigned char *p_buff;
    unsigned int n_len_buff;
    _tagPlaybackBuf():p_buff(0), n_len_buff(0){}
} PlaybackBuf;

class NetCaptureVod : public NetCapture
{
public:
    NetCaptureVod( QString str_sn, int n_class, const char *s_server_ip, int i_server_port );
    ~NetCaptureVod();

    QString get_sn() { return m_str_sn; }
    void set_sn( QString str_sn ) { m_str_sn = str_sn; }
    void set_replay( bool b_replay ) { mb_replay = b_replay; }
    void set_replay_speed( int n_spend ) { mn_replay_speed = n_spend; }
    int get_replay_speed() { return mn_replay_speed; }
    void get_replay_secs( unsigned int &n_sec, unsigned int &n_usec );

public:
    virtual void on_receive();
    void on_receive_realtime( unsigned char *p_buf, int n_buf_len );
    void on_receive_playback( unsigned char *p_buf, int n_buf_len );
    void thread_play_one_frame();
    static int callback_ir_decode_out_data( unsigned char *puc_out_buf, unsigned int ui_data_len, int width, int height, void *p_user_data );
    void callback_vod_decode( unsigned char *puc_out_buf, unsigned int ui_data_len, int width, int height );

public:
    QImage m_vod_image;

private:
    YHANDLE  mh_video_decode;
    QString m_str_sn;
    int mn_class;

    bool mb_replay;
    int mn_replay_speed;

    QList<MONITOR_TIME_HEAD_S> m_list_replay_time_head;
    QMutex m_mutex_playback_head;

    QList<PlaybackBuf> m_list_replay_buff;
    QMutex m_mutex_playback_buff;

    MONITOR_TIME_HEAD_S m_now_replay_time_head;
    QMutex m_mutex_now_time_head;

    unsigned char *mp_rev_buff;
    int mn_rev_buff_len;
    int mn_rev_read_len;

public:
    ThreadPlayback *mp_thread_play_back;
};

#endif // NETCAPTUREIR_H

