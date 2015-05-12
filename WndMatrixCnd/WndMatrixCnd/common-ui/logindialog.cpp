#include "logindialog.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QRegExpValidator>
#include <QCoreApplication>
#include <QDebug>
#include <QMessageBox>
#include <QtEndian>
#include <QTimer>
#include <QKeyEvent>
#include <QPainter>
#include "datamode.h"
#include "../common-funtion/dataconfig.h"
#include "../common-funtion/mscclienttype.h"
#include "../common-funtion/msclientlog.h"

#define VERSION             1
#define VERSION_SUB     0
#define DIALOG_WIDTH 400
#define DIALOG_HEIGHT 380
#define TITLE_BAR_HEIGHT 70
#define SPACE 20
#define LAB_WIDTH 80
#define EDIT_WIDTH 200
#define LAB_EDIT_HEIGHT 40

YHANDLE g_handle;
QMap< QString, SrvInfo_S> g_map_server_info;// QString:sn,服务器信息
bool gb_realtime_status = false;//false表示回放客户端

void ipv4_ntoa( const unsigned int ui_ipv4, QString &str_ipv4 )
{
    int n_ip1 = ui_ipv4>>24;
    int n_ip2 = ui_ipv4>>16&255;
    int n_ip3 = ui_ipv4>>8 & 255;
    int n_ip4 = ui_ipv4 &255;
    str_ipv4 = QString::number( n_ip4 ) + "." + QString::number( n_ip3 ) + "." + QString::number( n_ip2 ) + "."  + QString::number( n_ip1 );
}

void mac_ntoa( unsigned char *p_mac, int n_number, QString &str_mac )
{
    for( int i=0; i<n_number; i++ ) {
        if( i != n_number-1 ) {
            str_mac = str_mac + QString::number( p_mac[i], 16).toUpper()+":";
        } else {
            str_mac = str_mac + QString::number( p_mac[i], 16).toUpper();
        }
    }
}

bool on_discover( SrvDiscDevInfo_S *pst_dev_info, void *p_user_data )
{
    if ( 0 == pst_dev_info ) return true;

    LoginDialog* p_dlg = ( LoginDialog* )p_user_data;

    SrvInfo_S srv_info;
    srv_info.str_sn = QString( QLatin1String( pst_dev_info->ac_sn ) );
    srv_info.i_dev_type = pst_dev_info->i_dev_type;
    ipv4_ntoa( pst_dev_info->ui_ipv4_addr, srv_info.str_ipv4_addr );
    srv_info.str_ipv6_addr = QString( QLatin1String( (char*)pst_dev_info->auc_ipv6 ) );
    //srv_info.str_mac = QString::number(a, 16).toUpper();     // t == "3F";QString( QLatin1String( (char*)pst_dev_info->auc_mac ) );
    mac_ntoa( pst_dev_info->auc_mac, sizeof( pst_dev_info->auc_mac ),srv_info.str_mac );
    srv_info.us_port = pst_dev_info->us_port;

    if( srv_info.str_sn == "" ) return true;
    if( gb_realtime_status && pst_dev_info->i_dev_type == REALTIME_SERVER ) {
        //g_map_server_info.insert( srv_info.str_sn, srv_info );
        QMap<QString, SrvInfo_S>::iterator it_srv = g_map_server_info.find( srv_info.str_sn );
        if( it_srv == g_map_server_info.end() ) {
            emit p_dlg->sig_add_server( srv_info.str_sn );
        }
        g_map_server_info[srv_info.str_sn ] =  srv_info;

    } else if( !gb_realtime_status && pst_dev_info->i_dev_type== PLAYBACK_SERVER ) {
        //g_map_server_info.insert( srv_info.str_sn, srv_info );
        QMap<QString, SrvInfo_S>::iterator it_srv = g_map_server_info.find( srv_info.str_sn );
        if( it_srv == g_map_server_info.end() ) {
            emit p_dlg->sig_add_server( srv_info.str_sn );
        }
        g_map_server_info[srv_info.str_sn ] =  srv_info;
   }
    qDebug()<<"on_discover"<<srv_info.us_port<<srv_info.str_ipv4_addr<<srv_info.str_sn;

    return false;
}

LoginDialog::LoginDialog( QMainWindow *parent, bool b_realtime_status ) :
    QDialog(parent)
    ,mb_realtime_status( b_realtime_status )
    ,mp_btn_login( NULL )
    ,mp_btn_cancel( NULL )
    //,mp_btn_change( NULL )
    //,mp_lab_ip( NULL )
    //,mp_lab_port( NULL )
    ,mp_lab_name( NULL )
    ,mp_lab_pwd( NULL )
    ,mp_lab_server_sn( NULL )
    //,mp_edit_ip( NULL )
    //,mp_edit_port( NULL )
    ,mp_edit_name( NULL )
    ,mp_edit_pwd( NULL )
    ,mp_box_server_sn( NULL )
    ,mp_login_frame( 0 )
    ,m_pixmap_backgroup( ":image/image/caption_background.bmp" )
    ,mp_lab_warninfo( NULL )
    ,m_str_cfg_path( "" )
    ,mp_titlebar( NULL )
    ,mp_timer_find_server( NULL )
{
    showFullScreen();

    setWindowTitle( QString("登录窗口 v%1.%2").arg(VERSION).arg(VERSION_SUB) );
    setWindowIcon( QIcon( ":image/image/mission_logo.png" ) );

    create_ctrl();
    init_auto_find_server();
    init_ctrl();
    init_ctrl_font();
}
LoginDialog::~LoginDialog( )
{
    yf_net_discover_server_uninit( g_handle );

    if( mp_timer_find_server ) {
        mp_timer_find_server->stop();
        delete mp_timer_find_server;
        mp_timer_find_server = NULL;
    }
}

void LoginDialog::init_auto_find_server()
{
    gb_realtime_status = mb_realtime_status;
    yf_net_discover_server_init( g_handle, on_discover, this );
    connect( this, SIGNAL( sig_add_server( QString ) ), this, SLOT( slot_add_server(QString) ) );
    yf_net_discover_server_find_equipments( g_handle );

    mp_timer_find_server = new QTimer( this );
    connect( mp_timer_find_server, SIGNAL( timeout() ), this, SLOT( slot_timer_find_server( ) ) );
    mp_timer_find_server->start( 1000 );
}

void LoginDialog::init_ctrl_font()
{
    //设置颜色
    QPalette pal;
    pal.setColor(QPalette::WindowText,Qt::white );
    mp_lab_warninfo->setPalette( pal );
}

void LoginDialog::paintEvent(QPaintEvent *event)
{
    QPainter draw( this );
    draw.drawPixmap( 0, TITLE_BAR_HEIGHT, width(), height()-TITLE_BAR_HEIGHT, m_pixmap_backgroup );
    draw.end();
}

void LoginDialog::closeEvent(QCloseEvent *event)
{
    exit( 0 );
}

int LoginDialog::onResponse( CBaseMsg& reqMsg,CBaseMsg& rspMsg )
{
    int n_ret( 0 );
    int n_result = rspMsg.map_msgdata[MSGDATA_NETRESULT].toInt();

    switch( rspMsg.m_nCmd )
    {
    case SessionCmdLogin:
        if( rspMsg.m_nResult == ResultOk ) {
            MS_LOGER_INFO( "登录成功,正在获取配置数据！" ) ;
            QString str_info = tr("登录成功,正在获取配置数据！");
            show_warn_info( str_info );
            get_group_xml();
            if( mp_timer_find_server ) {
                mp_timer_find_server->stop();
            }
            yf_net_discover_server_uninit( g_handle );
        } else {
            QString str_info = tr("登录失败，");
            if( n_result == ERR_MSG_LOGIN_NO_USER ) {
                str_info += tr("用户名不正确！");
            } else if( n_result == ERR_MSG_LOGIN_PASSWORD ){
                str_info += tr("密码错误！");
            }
            show_warn_info( str_info );

            MS_LOGER_INFO( "%s", str_info.toStdString().data() ) ;
            set_ctrl_enable(true);
        }
        break;

    case SessionCmdVersion:
        if( rspMsg.m_nResult==ResultOk ) {
            QString str_info = tr("版本校验通过,正在登入！");
            show_warn_info( str_info );
            MS_LOGER_INFO( "版本校验通过,正在登入！" ) ;
            login_in();
        } else {
            QString str_info = tr("版本确认失败！");
            if( n_result == ERR_MSG_VER_NEGO_HIGH ) {
                str_info += tr("，客户端与服务器版本不匹配！");
            } else if( n_result == ERR_MSG_VER_NEGO_LOW ){
                str_info += tr("，客户端版本过低，请升级客户端！");
            }

            show_warn_info( str_info );
            MS_LOGER_INFO( "%s", str_info.toStdString().data() ) ;
            set_ctrl_enable( true );
        }
        break;
    case ConfigGetXml:
        if( rspMsg.m_nResult==ResultOk ) {
              //关闭登录对话框
            accept();
        } else {
            show_warn_info( tr("获取配置数据失败") );
            MS_LOGER_INFO( "获取配置数据失败！" ) ;
            set_ctrl_enable(true);
        }
        break;
    default:
        n_ret = -1;
        break;
    }

    return n_ret;
}

int LoginDialog::onNotify(CBaseMsg& msg)
{
    if( ServerOffLine == msg.m_nCmd ){ //connect failed
        set_ctrl_enable(true);
        show_login();
        QString sInfo=tr("网络连接失败 请检查服务器ip地址和端口");
        MS_LOGER_INFO( "网络连接失败 请检查服务器ip地址和端口" ) ;
        show_warn_info(sInfo);
    } else if( ServerOnLine==msg.m_nCmd) {
        QString sInfo=tr("网络连接成功 正在登录认证");
        MS_LOGER_INFO( "网络连接成功 正在登录认证" ) ;
        show_warn_info(sInfo);
    }
    return 0;
}

void LoginDialog::login_clicked()
{
    m_str_user = mp_edit_name->text();
    m_str_password = mp_edit_pwd->text();

    if ( !detection_login_is_correct(  m_str_user, m_str_password ) ) {
        return;
    }

    QMap<QString, SrvInfo_S >::iterator it_srv = g_map_server_info.begin();
    for( ; it_srv != g_map_server_info.end(); it_srv++ ) {
        if( mp_box_server_sn->currentText() == it_srv.key() ) {
            m_str_ip = it_srv.value().str_ipv4_addr;
            m_str_port = QString::number( it_srv.value().us_port );
            break;
        }
    }

    //m_str_ip = mp_edit_ip->text();
    //m_str_port = mp_edit_port->text();

    if( !detection_conn_is_correct(m_str_ip, m_str_port) ) {
        return ;
    }


    set_ctrl_enable( false );

    int nPort = m_str_port.toInt();
    if( login_check_version( m_str_user.toLatin1().data(),
                             m_str_password.toLatin1().data(),
                             m_str_ip.toLatin1().data(),
                             nPort,
                             MSCClientType::Instance()->get_client_type() ) < 0 ) {

        return ;
    }

    //write_to_cfg();

    //accept();//隐含窗口，并返回结果QDialg::Accepted
    //accept();
    return;
}

void LoginDialog::slot_change()
{
    show_ip_and_port();
    move_login();
}

bool LoginDialog::detection_conn_is_correct( QString str_ip, QString str_port )
{
    if ( str_ip.isEmpty() ) {
        QMessageBox::information( this, "提示", "“IP地址”不能为空" );
        return false;
    }

    QString str_temp = str_ip.right(1);
    if ( str_temp == "." ) {
        QMessageBox::information( this, "提示", "“IP地址”格式不正确" );
        return false;
    }

    int n_count = 0;
    int n_pos=0;
    str_temp = str_ip;
    do {
        n_pos = str_temp.indexOf( ".", n_pos ) + 1;
        if ( n_pos > 0 ) {
            n_count++;
        }
    } while ( n_pos > 0 );

    if ( n_count != 3 ) {
        QMessageBox::information( this, "提示", "“IP地址”格式不正确" );
        return false;
    }

    if ( str_port.isEmpty() ) {
        QMessageBox::information( this, "提示", "“端口”不能为空" );
        return false;
    }

    return true;
}

bool LoginDialog::detection_login_is_correct( QString str_user, QString str_pwd )
{
    if ( str_user.isEmpty() ) {
        QMessageBox::information( this, "提示", "“用户名”不能为空" );
        return false;
    }

    if ( str_pwd.isEmpty() ) {
        QMessageBox::information( this, "提示", "“密码”不能为空" );
        return false;
    }

    return true;
}

void LoginDialog::init_ctrl()
{
    QString str_ip, str_port, str_user, str_password;

    DataConfig::get_instance()->get_login_info( str_ip, str_port, str_user, str_password, mb_realtime_status );

    mp_box_server_sn->clear();
    QMap< QString, SrvInfo_S >::iterator it_srv = g_map_server_info.begin();
    for( ; it_srv != g_map_server_info.end(); it_srv++ ) {
        mp_box_server_sn->addItem( it_srv.key() );
    }

    //mp_edit_ip->setText( str_ip );
    //mp_edit_port->setText( str_port );
    mp_edit_name->setText( str_user );
    mp_edit_pwd->setText( str_password );

    /*if( !str_ip.isEmpty() && !str_port.isEmpty() ) {
        mp_lab_ip->hide();
        mp_lab_port->hide();
        mp_edit_ip->hide();
        mp_edit_port->hide();
    } else {
        mp_lab_ip->show();
        mp_lab_port->show();
        mp_edit_ip->show();
        mp_edit_port->show();
    }*/
}

void LoginDialog::create_ctrl()
{
    QRegExpValidator *p_ip_validator = new QRegExpValidator( QRegExp("((2[0-4]\\d|25[0-5]|[01]?\\d\\d?)\\.){3}(2[0-4]\\d|25[0-5]|[01]?\\d\\d?)"), this );
    QRegExpValidator *p_port_validator = new QRegExpValidator( QRegExp("[0-9]{10}"), this );
    QRegExpValidator *p_user_validator = new QRegExpValidator( QRegExp("[a-zA-Z0-9\-\\\_]{31}"), this );
    QRegExpValidator *p_pwd_validator = new QRegExpValidator( QRegExp("[a-zA-Z0-9\-\\\_]{15}"), this );

    mp_lab_server_sn = new QLabel( tr( "服务器："), this );
    //mp_lab_ip = new QLabel( tr("IP地址："), this );
    //mp_lab_port = new QLabel( tr("端口号："), this );
    mp_lab_name = new QLabel( tr( "用户名：" ), this );
    mp_lab_pwd = new QLabel( tr("密码："), this );
    mp_lab_warninfo = new QLabel( this );

    mp_box_server_sn = new QComboBox( this );
    //mp_edit_ip = new QLineEdit( this );
    //mp_edit_port = new QLineEdit( this );
    mp_edit_name = new QLineEdit( this );
    mp_edit_pwd = new QLineEdit( this );

    //mp_edit_ip->setValidator( p_ip_validator );
    //mp_edit_port->setValidator( p_port_validator );
    mp_edit_name->setValidator( p_user_validator );
    mp_edit_pwd->setValidator( p_pwd_validator );
    mp_edit_pwd->setEchoMode( QLineEdit::Password );

    mp_btn_login = new QPushButton( tr("登录"), this );
    mp_btn_cancel = new QPushButton( tr("取消"), this );
    //mp_btn_change = new QPushButton( tr("端口设置"), this );

    connect( mp_btn_login, SIGNAL( clicked() ), this, SLOT( login_clicked() ) );
    connect( mp_btn_cancel, SIGNAL( clicked() ), this, SLOT( slot_close_login() ) );
    //connect( mp_btn_change, SIGNAL( clicked() ), this, SLOT( slot_change() ) );

    mp_lab_server_sn->setMaximumHeight( 40 );
    //mp_lab_ip->setMaximumHeight( 40 );
    //mp_lab_port->setMaximumHeight( 40 );
    mp_lab_name->setMaximumHeight( 40 );
    mp_lab_pwd->setMaximumHeight( 40 );
    mp_box_server_sn->setMaximumHeight( 40 );
    //mp_edit_ip->setMaximumHeight( 40 );
    //mp_edit_port->setMaximumHeight( 40 );
    mp_edit_name->setMaximumHeight( 40 );
    mp_edit_pwd->setMaximumHeight( 40 );
    mp_btn_login->setMaximumHeight( 40 );
    mp_btn_cancel->setMaximumHeight( 40 );
    //mp_btn_change->setMaximumHeight( 40 );
    mp_lab_warninfo->setMaximumHeight( 40 );

    mp_lab_server_sn->setMaximumWidth( 80 );
    //mp_lab_ip->setMaximumWidth( 80 );
    //mp_lab_port->setMaximumWidth( 80 );
    mp_lab_name->setMaximumWidth( 80 );
    mp_lab_pwd->setMaximumWidth( 80 );
    mp_box_server_sn->setMaximumWidth( 200 );
    //mp_edit_ip->setMaximumWidth( 200 );
    //mp_edit_port->setMaximumWidth( 200 );
    mp_edit_name->setMaximumWidth( 200 );
    mp_edit_pwd->setMaximumWidth( 200 );
    mp_btn_login->setMaximumWidth( 140 );
    mp_btn_cancel->setMaximumWidth( 140 );
    mp_lab_warninfo->setMaximumWidth( 300 );

    //mp_lab_ip->setStyleSheet( "font-size : 16px" );
    //mp_lab_port->setStyleSheet( "font-size : 16px" );
    mp_lab_name->setStyleSheet( "font-size : 16px" );
    mp_lab_pwd->setStyleSheet( "font-size : 16px" );
    //mp_edit_ip->setStyleSheet( "font-size : 16px" );
    //mp_edit_port->setStyleSheet( "font-size : 16px" );
    mp_edit_name->setStyleSheet( "font-size : 16px" );
    mp_edit_pwd->setStyleSheet( "font-size : 16px" );
    mp_btn_login->setStyleSheet( "font-size : 16px" );
    mp_btn_cancel->setStyleSheet( "font-size : 16px" );
    //mp_btn_change->setStyleSheet( "font-size : 16px" );
    mp_lab_warninfo->setStyleSheet( "font-size : 16px" );
    mp_lab_server_sn->setStyleSheet( "font-size : 16px" );
    mp_box_server_sn->setStyleSheet( "font-size : 16px" );

    show_login();
    move_login();

    //mp_btn_change->move( 200, height()-200 );
    //mp_btn_change->resize( 120, 39 );
    //mp_btn_change->show();

    mp_titlebar = new TitleBar(this);
    mp_titlebar->move( 0, 0 );
    mp_titlebar->resize( width(), TITLE_BAR_HEIGHT );
    mp_titlebar->show();

    QPalette pe;
    pe.setColor( QPalette::WindowText, Qt::white );
    mp_lab_server_sn->setPalette( pe );
    //mp_lab_ip->setPalette( pe );
   // mp_lab_port->setPalette( pe );
    mp_lab_name->setPalette( pe );
    mp_lab_pwd->setPalette( pe );
}


//void LoginDialog::timer_xml()
//{
//    //if( mp_parent->get_xml_rsp() == 1 || ++mn_wait_xml_count > 10)
//    {
//        mp_timer_xml->stop();

//        //if( mp_parent->get_xml_rsp() == 1 )
//       //     mp_parent->change_show_widget();

//        accept();
//        return ;
//    }
//}

//void LoginDialog::timer_ver_nego()
//{
//    if ( mn_ver_nego == 0 ) {
//        mp_timer_ver_nego->stop();

//        m_cfg_file.set_config_info( "SERVER_HOST_IP", mp_edit_ip->text() );
//        m_cfg_file.set_config_info( "SERVER_PORT", mp_edit_port->text() );
//        m_cfg_file.set_config_info( "MONITOR_USER_NAME", mp_edit_name->text() );
//        m_cfg_file.set_config_info( "MONITOR_USER_PASSWD", mp_edit_pwd->text() );
//        m_cfg_file.save_cofig_file( m_str_cfg_path );

//        //mp_parent->init_net_timer();

//        mn_wait_xml_count=0;
//        mp_timer_xml->start(100);
//        //((LoginFullScreen*)parent())->accept();
//        //accept();
//        return;
//    }

//    if ( ++mn_ver_nego_count > 30 ) {
//        QMessageBox::information( this, "提示", "版本确认超时！" );
//    } else {
//        if ( mn_ver_nego == -1 ) {
//            return;
//        } else if ( mn_ver_nego == 1 ){
//            QMessageBox::information( this, "提示", "客户端与服务器版本不匹配！" );
//        } else if ( mn_ver_nego == 2 ){
//            QMessageBox::information( this, "提示", "客户端版本过低，请升级客户端！" );
//        } else {
//            QMessageBox::information( this, "提示", "版本确认失败！" );
//        }
//    }

//    mn_ver_nego = -1;
//    mn_ver_nego_count = 0;
//    mp_timer_ver_nego->stop();
//    set_ctrl_enable( true );
//}

void LoginDialog::set_ctrl_enable( bool b_enable )
{
    //mp_edit_ip->setEnabled( b_enable );
    //mp_edit_port->setEnabled( b_enable );
    mp_edit_name->setEnabled( b_enable );
    mp_edit_pwd->setEnabled( b_enable );

    //mp_btn_change->setEnabled( b_enable );
    mp_btn_login->setEnabled( b_enable );
    mp_btn_login->setText( b_enable?"登录":"登入中" );
}

void LoginDialog::slot_close_login()
{
    close();
    emit sig_close_login_full();
}

void LoginDialog::keyPressEvent( QKeyEvent *event )
{
    if( event->key() == Qt::Key_Escape )
    {
        return;
    } else if( event->key() == Qt::Key_Space ) {
        return;
    }
}

void LoginDialog::write_to_cfg()
{
    //DataConfig::get_instance()->set_login_info( mp_edit_ip->text(), mp_edit_port->text(), mp_edit_name->text(), mp_edit_pwd->text(), mb_realtime_status );
    //DataConfig::get_instance()->save();
}

void LoginDialog::move_login()
{
    QRect login_rct( (width()-DIALOG_WIDTH)/2, (height() - DIALOG_HEIGHT-TITLE_BAR_HEIGHT)/2, DIALOG_WIDTH, DIALOG_HEIGHT );
    int n_width = login_rct.x();
    int n_height = login_rct.y();
    //if( !mp_lab_ip->isHidden() ) {
        mp_lab_server_sn->move( n_width+SPACE, n_height+SPACE*2 );
        mp_lab_server_sn->resize( LAB_WIDTH, LAB_EDIT_HEIGHT );
        mp_box_server_sn->move( n_width+SPACE*2+mp_lab_server_sn->width(), n_height+SPACE*2 );
        mp_box_server_sn->resize( EDIT_WIDTH, LAB_EDIT_HEIGHT );

        //mp_lab_ip->move( n_width+SPACE, n_height+SPACE );
        //mp_lab_ip->resize( LAB_WIDTH, LAB_EDIT_HEIGHT );
        //mp_lab_port->move( n_width+SPACE, n_height+SPACE*2+mp_lab_ip->height() );
        //mp_lab_port->resize( LAB_WIDTH, LAB_EDIT_HEIGHT );
        //mp_edit_ip->move( n_width+SPACE*2+mp_lab_ip->width(),n_height+SPACE );
        //mp_edit_ip->resize( EDIT_WIDTH, LAB_EDIT_HEIGHT );
        //mp_edit_port->move( n_width+SPACE*2+mp_lab_port->width(), n_height+SPACE*2+mp_lab_ip->height() );
        //mp_edit_port->resize( EDIT_WIDTH, LAB_EDIT_HEIGHT );

   // }

    mp_lab_name->move( n_width+SPACE, n_height+SPACE*3+mp_lab_server_sn->height() );
    mp_lab_name->resize( LAB_WIDTH, LAB_EDIT_HEIGHT );
    mp_lab_pwd->move( n_width+SPACE, n_height+SPACE*4+mp_lab_server_sn->height()+mp_lab_name->height() );
    mp_lab_pwd->resize( LAB_WIDTH, LAB_EDIT_HEIGHT );
    mp_edit_name->move( n_width+SPACE*2+mp_lab_name->width(), n_height+SPACE*3+mp_lab_server_sn->height());
    mp_edit_name->resize( EDIT_WIDTH, LAB_EDIT_HEIGHT );
    mp_edit_pwd->move( n_width+SPACE*2+mp_lab_name->width(), n_height+SPACE*4+mp_lab_server_sn->height() \
                        +mp_lab_name->height() );
    mp_edit_pwd->resize( EDIT_WIDTH, LAB_EDIT_HEIGHT );

    mp_lab_warninfo->move( n_width+SPACE, n_height+SPACE*4+mp_lab_server_sn->height()\
                           +mp_lab_name->height()+mp_lab_pwd->height() );
    mp_lab_warninfo->resize( 300, SPACE );

    mp_btn_login->move( n_width, n_height+SPACE*5+mp_lab_server_sn->height() \
                        +mp_lab_name->height()+mp_lab_pwd->height()+mp_lab_warninfo->height() );
    mp_btn_login->resize( 140, LAB_EDIT_HEIGHT );
    mp_btn_cancel->move( n_width+DIALOG_WIDTH-mp_btn_login->width()-SPACE*2, n_height+SPACE*5+mp_lab_server_sn->height() \
                         +mp_lab_name->height()+mp_lab_pwd->height()+mp_lab_warninfo->height() );
    mp_btn_cancel->resize( 140, LAB_EDIT_HEIGHT );
}

void LoginDialog::show_login()
{
    //mp_lab_ip->show();
    //mp_lab_port->show();
    mp_lab_name->show();
    mp_lab_pwd->show();
    //mp_edit_ip->show();
    //mp_edit_port->show();
    mp_edit_name->show();
    mp_edit_pwd->show();
    mp_lab_warninfo->show();
    mp_btn_login->show();
    mp_btn_cancel->show();
    mp_lab_server_sn->show();
    mp_box_server_sn->show();
}

void LoginDialog::show_warn_info( QString str_warning )
{
    mp_lab_warninfo->setText( str_warning );
}

void LoginDialog::show_ip_and_port()
{
    /*if ( mp_edit_ip->isHidden() ) {
        mp_lab_ip->show();
        mp_lab_port->show();
        mp_edit_ip->show();
        mp_edit_port->show();
    } else {
        mp_lab_ip->hide();
        mp_lab_port->hide();
        mp_edit_ip->hide();
        mp_edit_port->hide();
    }*/
}


void LoginDialog::get_client_login_info( QString &str_user, QString &str_password, QString &str_ip, QString &str_port )
{
    str_user = m_str_user;
    str_password = m_str_password;
    str_ip = m_str_ip;
    str_port = m_str_port;
}

QString LoginDialog::get_user()
{
    return m_str_user;
}

QString LoginDialog::get_password()
{
    return m_str_password;
}

QString LoginDialog::get_ip()
{
    return m_str_ip;
}

QString LoginDialog::get_port()
{
    return m_str_port;
}

void LoginDialog::slot_timer_find_server()
{
    yf_net_discover_server_find_equipments( g_handle );
}

void LoginDialog::slot_add_server( QString str_sn )
{
    /*mp_box_server_sn->clear();
    QMap< QString, SrvInfo_S >::iterator it_srv = g_map_server_info.begin();
    for( ; it_srv != g_map_server_info.end(); it_srv++ ) {
        mp_box_server_sn->addItem( it_srv.key() );
    }*/
    mp_box_server_sn->addItem( str_sn );
}
