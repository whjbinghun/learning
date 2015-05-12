#ifndef AUTORECONNECT_H
#define AUTORECONNECT_H

#include "cloginview.h"
#include "cmainview.h"
#include "datamode.h"
#include <QTimer>
#include <QWidget>

typedef enum{
    NONE,
    VERSION_CONNECTING,
    LOGIN_CONNECTIING,
    XML_CONNECTING,
}LOGIN_STATUS_E;

class AutoReconnect : public QObject, public CLoginView
{
    Q_OBJECT
public:
    AutoReconnect();
    ~AutoReconnect();
    void send_login_msg();   //登陆
    void save_client_login_info( QString str_user, QString str_password, QString str_ip, QString str_port ); //保存客户端登陆信息

    int onResponse( CBaseMsg& reqMsg, CBaseMsg& rspMsg );
private:
    int mn_group_num;  //组号
    QString m_str_user;  //用户名
    QString m_str_password; //密码
    QString m_str_ip;   //ip
    QString m_str_port; //端口号
    LOGIN_STATUS_E mn_login_status;
    QTimer *mp_connect_timer;
private slots:
    void slot_connect_timer();
signals:
    void sig_login_success();
};

#endif // AUTORECONNECT_H
