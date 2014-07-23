#ifndef NETCAPTUREVI_H
#define NETCAPTUREVI_H

#include <QObject>
#include <QImage>
#include "video_decode_api.h"
#include <QDebug>
#include <QTcpSocket>
#include "qmutex.h"


class VodPlayInterface;

class NetCapturevi : public QObject
{
    Q_OBJECT
public:
    explicit NetCapturevi( QObject *parent = 0 );
public:
    void set_message( QString url, int n_class, int group_num );
    void init_vi_show();
    void callback_vi_decode( unsigned char *puc_out_buf, unsigned int ui_data_len, int width, int height );
    void connect_play_vi();
    QImage *get_image() { return &m_vi_image; }
signals:
    void get_vi_image( QImage &image, int &group );
public slots:
    void receiver_vi_play_message();
    void displayError( QAbstractSocket::SocketError );  //显示错误
    void set_image( QImage& image, int& group);
private:
    QImage m_vi_image;
    char *m_vi_buf;
    quint64 m_vi_buf_len;
    int mn_vi_connect_status;       //.可见光连接状态
    YHANDLE  mh_vi_video_decode;

    QString m_str_url;
    QString m_str_ip;
    int mn_port;
    int mn_class;
    int mn_group_num;

    QTcpSocket *mp_vi_tcp_client;
    VodPlayInterface *mp_main_window;

    QMutex m_mutex_vi;

};

#endif // NETCAPTUREVI_H
