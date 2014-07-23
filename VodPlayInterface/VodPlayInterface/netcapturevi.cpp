#include "netcapturevi.h"
#include <QColor>

#include "vodplayinterface.h"
#include <QApplication>

#define FRAME_VI_SIZE 1920*1080*4

extern VodPlayInterface * vod_ir_play_window;

int callback_vi_decode_out_data( unsigned char *puc_out_buf, unsigned int ui_data_len, int width, int height, void *p_user_data  )
{
    NetCapturevi *p_this = ( NetCapturevi * )p_user_data;
    if ( p_this != NULL ) {
    //if ( p_this == NULL ) {
        p_this->callback_vi_decode( puc_out_buf, ui_data_len, width, height );
    }
    return 0;
}

void NetCapturevi::callback_vi_decode( unsigned char *puc_out_buf, unsigned int ui_data_len, int width, int height )
{
    /*if ( mn_vi_connect_status == YF_Disconnected ) {
        m_vi_image.fill( QColor(200,200,200) );
    } else {
        memcpy( m_vi_image.bits(), puc_out_buf,  ui_data_len );
    }
    update();*/
    if( m_mutex_vi.tryLock() ) {
        memcpy( m_vi_image.bits(), puc_out_buf,  ui_data_len );
        m_mutex_vi.unlock();
    }
    //update();
    //connect( this, SIGNAL( get_vi_image( QImage&, int& ) ), mp_main_window, SLOT( slot_set_vi_image( QImage&, int& ) ) );
}


NetCapturevi::NetCapturevi( QObject *parent ) :
    QObject( parent )
  , mp_main_window( (VodPlayInterface *)parent )
  , mn_group_num( -1 )
{
    //init_vi_show();
    m_vi_buf_len = FRAME_VI_SIZE;
    m_vi_image = QImage( 1920, 1080, QImage::Format_RGBA8888 );
    //m_vi_image.fill( QColor( 200, 200, 200 ) );  //不在主窗口

    m_vi_buf = new char[ m_vi_buf_len ];
    memset( m_vi_buf, 0, m_vi_buf_len );

    video_decode_init( &mh_vi_video_decode, 0,  callback_vi_decode_out_data, this );

    mp_vi_tcp_client = new QTcpSocket( this );
    mp_main_window->set_vi_image( &m_vi_image, mn_group_num, m_mutex_vi );

    //mp_main_window = ( VodPlayInterface *)QApplication::focusWidget();
    //mp_main_window = vod_ir_play_window;

    connect( mp_vi_tcp_client, SIGNAL( readyRead() ), this, SLOT( receiver_vi_play_message() ) );
    connect( mp_vi_tcp_client, SIGNAL( error( QAbstractSocket::SocketError ) ), this, SLOT( displayError( QAbstractSocket::SocketError ) ) );
    //connect( this, SIGNAL( get_vi_image( QImage&, int& ) ), mp_main_window, SLOT( slot_set_vi_image( QImage&, int& ) ) );
}

void NetCapturevi::init_vi_show()
{
    m_vi_buf_len = FRAME_VI_SIZE;
    m_vi_image = QImage( 1920, 1080, QImage::Format_RGBA8888 );
    //m_vi_image.fill( QColor( 200, 200, 200 ) );  //不在主窗口

    m_vi_buf = new char[ m_vi_buf_len ];
    memset( m_vi_buf, 0, m_vi_buf_len );

    video_decode_init( &mh_vi_video_decode, 0,  callback_vi_decode_out_data );
}


void NetCapturevi::set_message( QString url, int n_class, int group_num )
{
    m_str_url = url;
    m_str_ip = m_str_url.split(":")[0];
    mn_port = m_str_url.split(":")[1].toInt();
    QString str_port = m_str_url.split(":")[1];
    mn_group_num = group_num;
    qDebug() << "视频IP，port" << url << m_str_ip << str_port << mn_port<<mn_group_num;
    connect_play_vi();

}

void NetCapturevi::connect_play_vi( )
{
    mp_vi_tcp_client->connectToHost( m_str_ip, mn_port );
    //mn_vi_connect_status = YF_Connected;
}

void NetCapturevi::receiver_vi_play_message()
{
    if( mp_vi_tcp_client->bytesAvailable()>0 )
    {
        quint64 n_read_len = 0;
        quint64 n_data_len = mp_vi_tcp_client->bytesAvailable();
        //qDebug()<<length;
        //读取红外视频数据 m_ir_buf_len表示位移
        if( n_data_len > m_vi_buf_len ) {
            delete []m_vi_buf;
            m_vi_buf = new char[n_data_len];
            m_vi_buf_len = n_data_len;
        }

        n_read_len = mp_vi_tcp_client->read( m_vi_buf, n_data_len );
        int n_video_data = video_put_data( mh_vi_video_decode, ( unsigned char * )m_vi_buf, n_read_len );
        if( n_video_data != 0 ) {
            qDebug()<<"数据读取出错";
        }
    }
}

void NetCapturevi::displayError( QAbstractSocket::SocketError )
{
    if( mp_vi_tcp_client->errorString() == "Connection refused" ) {
        //connect_server();
        qDebug() << "服务器链接失败";
    } else if( mp_vi_tcp_client->errorString() == "The remote host closed the connection") {
        qDebug() << "远程服务器被关闭";
    }
}

void NetCapturevi::set_image(QImage &image, int &group)
{
    qDebug()<<"image 成功";
}
