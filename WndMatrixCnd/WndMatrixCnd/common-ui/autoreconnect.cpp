#include "autoreconnect.h"
#include <windows.h>
#include <QDebug>
#include <QMessageBox>
#include "../common-funtion/mscclienttype.h"
#include "../common-funtion/msclientlog.h"

AutoReconnect::AutoReconnect() :
    m_str_user("")
  , m_str_password("")
  , m_str_ip("")
  , m_str_port("")
  , mn_group_num(0)
  , mn_login_status( NONE )
{
    mp_connect_timer = new QTimer(this);
    connect( mp_connect_timer, SIGNAL( timeout() ), this, SLOT( slot_connect_timer() ) );
}

AutoReconnect::~AutoReconnect()
{
}


void AutoReconnect::slot_connect_timer()
{
    if( mn_login_status == VERSION_CONNECTING ){
        MS_LOGER_INFO( "版本校验失败,再次发送版本校验命令......" );
        login_check_version( m_str_user.toLatin1().data(),
                             m_str_password.toLatin1().data(),
                             m_str_ip.toLatin1().data(),
                             m_str_port.toInt(),
                             MSCClientType::Instance()->get_client_type()  );
    }else if( mn_login_status == LOGIN_CONNECTIING ){
        MS_LOGER_INFO( "登陆失败,再次发送登陆命令......" );
        login_in();
    }else if( mn_login_status == XML_CONNECTING ){
        MS_LOGER_INFO( "获取xml失败,再次发送获取xml命令......" );
        get_group_xml();
    }
    mp_connect_timer->stop();
}

//保存客户端登陆信息
void AutoReconnect::save_client_login_info( QString str_user, QString str_password, QString str_ip, QString str_port )
{
    m_str_user = str_user;
    m_str_password = str_password;
    m_str_ip = str_ip;
    m_str_port = str_port;
}


//发送登陆请求
void AutoReconnect::send_login_msg()
{
    if( mn_login_status != VERSION_CONNECTING ){
        mn_login_status = VERSION_CONNECTING; //表示正在版本校验
        MS_LOGER_INFO( "发送登陆请求 用户名:%s, ip:%s, port:%s", m_str_user.toLatin1().data(),
                         m_str_ip.toLatin1().data(), m_str_port.toLatin1().data() );
        login_check_version( m_str_user.toLatin1().data(),
                             m_str_password.toLatin1().data(),
                             m_str_ip.toLatin1().data(),
                             m_str_port.toInt(),
                             MSCClientType::Instance()->get_client_type() );
    }
}


int AutoReconnect::onResponse( CBaseMsg& reqMsg, CBaseMsg& rspMsg )
{
    QString str;
    int n_ret( 0 );
    switch( rspMsg.m_nCmd )
    {
    case SessionCmdLogin:
        if( rspMsg.m_nResult == ResultOk ) {
            mn_login_status = XML_CONNECTING;
            MS_LOGER_INFO( "登陆成功,发送获取xml命令......" );
            get_group_xml();
        } else {
            if ( mn_login_status != VERSION_CONNECTING ){
                mn_login_status = LOGIN_CONNECTIING;
                mp_connect_timer->start(1000);
//                MS_LOGER_INFO( "登陆失败,再次发送登陆命令......" );
//                login_in();
            }
        }
        break;

    case SessionCmdVersion:
        if( rspMsg.m_nResult == ResultOk ) {
            mn_login_status = LOGIN_CONNECTIING;
            MS_LOGER_INFO( "版本校验成功,发送登陆命令......" );
            login_in();
        } else {
            mn_login_status = VERSION_CONNECTING;
            mp_connect_timer->start(1000);
//            MS_LOGER_INFO( "版本校验失败,再次发送版本校验命令......" );
//            login_check_version( m_str_user.toLatin1().data(),
//                                 m_str_password.toLatin1().data(),
//                                 m_str_ip.toLatin1().data(),
//                                 m_str_port.toInt(),
//                                 MSCClientType::Instance()->get_client_type()  );
        }
        break;
    case ConfigGetXml:
        if( rspMsg.m_nResult == ResultOk ) {
            mn_login_status = NONE;
            MS_LOGER_INFO( "登陆请求成功......" );
            emit sig_login_success();
        } else {
            if ( mn_login_status != VERSION_CONNECTING ){
                mn_login_status = XML_CONNECTING;
                mp_connect_timer->start(1000);
//                MS_LOGER_INFO( "获取xml失败,再次发送获取xml命令......" );
//                get_group_xml();
            }
        }
        break;
    default:
        n_ret = -1;
        break;
    }
    return n_ret;
}

