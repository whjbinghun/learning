#include "netcapture.h"
#include <QDebug>

NetCapture::NetCapture( const char *s_server_ip, int i_server_port )
{
    mp_tcp_socket = new QTcpSocket( this );
    ms_server_ip = s_server_ip;
    mi_server_port = i_server_port;
    mb_connected = false;
    mb_own_disconnected = false;

    connect( mp_tcp_socket, SIGNAL( connected() ), this, SLOT( on_connected() ) );
    connect( mp_tcp_socket, SIGNAL( readyRead() ), this, SLOT( on_receive() ) );
    connect( mp_tcp_socket, SIGNAL( error( QAbstractSocket::SocketError ) )
             , this, SLOT( on_connection_error( QAbstractSocket::SocketError ) ) );
}

NetCapture::~NetCapture()
{
    if( mp_tcp_socket ) {
        delete mp_tcp_socket;
        mp_tcp_socket = NULL;
    }
}

void NetCapture::on_receive()
{
    qint64 len = mp_tcp_socket->bytesAvailable();
    if( len > 0 ) {
        NetworkData nd;
        nd.buf = new char[len];

        nd.len = mp_tcp_socket->read( nd.buf, len );
        if( nd.len <= 0 ) {
            delete []nd.buf;
            qDebug() << nd.len;
        } else {
            if( nd.len != len ) {
                qDebug() << nd.len << len;
            }

            m_data_list.push_back( nd );
        }
    } else {
        qDebug() << len;
    }
}

void NetCapture::on_connected()
{
    mb_connected = true;
}

void NetCapture::on_connection_error( QAbstractSocket::SocketError socket_err )
{
    switch (socket_err) {
    case QAbstractSocket::RemoteHostClosedError:
        break;
    case QAbstractSocket::HostNotFoundError:
        qDebug() << mp_tcp_socket->errorString();
        break;
    case QAbstractSocket::ConnectionRefusedError:
        qDebug() << mp_tcp_socket->errorString();
        break;
    default:
        qDebug() << mp_tcp_socket->errorString();
        break;

    }
}

QAbstractSocket::SocketState NetCapture::get_connect_state()
{
    return mp_tcp_socket->state();
}

bool NetCapture::connect_server( const char *s_server_ip, int i_server_port )
{
    ms_server_ip = s_server_ip;
    mi_server_port = i_server_port;

    mp_tcp_socket->abort();
    mp_tcp_socket->connectToHost( ms_server_ip, (quint16)mi_server_port );
    mb_own_disconnected = false;
    mb_connected = false;
    if( mp_tcp_socket->waitForConnected() ) {
        return true;
    }

    return false;
}

bool NetCapture::connect_server()
{
    mp_tcp_socket->abort();
    mp_tcp_socket->connectToHost( ms_server_ip, (quint16)mi_server_port );
    mb_own_disconnected = false;
    mb_connected = false;
    if( mp_tcp_socket->waitForConnected() ) {
        qDebug() << "connected!";
        return true;
    } else {
        qDebug() << "failed to connect" << ms_server_ip << ":" << mi_server_port;
        return false;
    }
}

bool NetCapture::disconnect_server()
{
    mp_tcp_socket->disconnectFromHost();
    mp_tcp_socket->waitForDisconnected();
    mb_own_disconnected = true;
    mb_connected = false;

    return true;
}

int NetCapture::send_data( char *p_send_buf, int i_send_len )
{
    if( !p_send_buf || i_send_len <= 0 ) {
        qDebug() << p_send_buf << i_send_len;
        return -1;
    }

    int a = mp_tcp_socket->state();
    qDebug() << a;

    qint64 i_sent_len = 0;
    while( i_sent_len < i_send_len ) {
        qint64 w_len = mp_tcp_socket->write( p_send_buf + i_sent_len, i_send_len - i_sent_len );
        if( w_len <= 0 ) {
            qDebug() << w_len;
            return w_len;
        } else {
            i_sent_len += w_len;
        }
    }

    return i_sent_len;
}
