#ifndef NETCAPTURE_H
#define NETCAPTURE_H

#include <QObject>
#include <QTcpSocket>
#include <QList>

typedef struct {
    char *buf;
    int len;
}NetworkData;

class NetCapture : public QObject
{
    Q_OBJECT
public:
    explicit NetCapture( const char *s_server_ip, int i_server_port );
    ~NetCapture();

public:
    bool connect_server( const char *s_server_ip, int i_server_port );
    bool connect_server();
    bool disconnect_server();
    bool is_connected(){ return mb_connected; }
    QAbstractSocket::SocketState get_connect_state();
    int send_data( char *p_send_buf, int i_send_len );

    QTcpSocket *get_tcp_socket() { return mp_tcp_socket; }
public slots:
    virtual void on_connected();
    virtual void on_receive();
    virtual void on_connection_error( QAbstractSocket::SocketError socket_err );
public:
    bool mb_connected;
    bool mb_own_disconnected;
    QString ms_server_ip;
    int mi_server_port;
    QList<NetworkData> m_data_list;
public:
    QTcpSocket *mp_tcp_socket;
};

#endif // NETCAPTURE_H
