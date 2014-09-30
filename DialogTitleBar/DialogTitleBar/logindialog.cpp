#include "logindialog.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QRegExpValidator>
#include <QCoreApplication>
#include <QPainter>
#include "dialogtitlebar.h"
#include <QDebug>

#define TITLE_BAR_HEIGHT 50
#define DIALOG_WIDTH 400
#define DIALOG_HEIGHT 300

LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent)
    ,mp_lab_IP( NULL )
    ,mp_lab_port( NULL )
    ,mp_lab_name( NULL )
    ,mp_lab_pwd( NULL )
    ,mp_line_IP( NULL )
    ,mp_line_port( NULL )
    ,mp_line_name( NULL )
    ,mp_line_pwd( NULL )
    ,mp_cfg_file_info( NULL )
    ,mp_dialog_title_bar( (DialogTitleBar*) parent )
{
    setWindowFlags (Qt::FramelessWindowHint);

    ms_dir_path = QCoreApplication::applicationDirPath();
    ms_file_name = ms_dir_path+"/client.cfg";
    qDebug()<<"load_cfg:"<<ms_file_name;

    mp_cfg_file_info = new CfgFileInfo();
    mp_cfg_file_info->load_cfg( ms_file_name );
    mp_cfg_file_info->get_config_info( "SERVER_HOST_IP", ms_IP );
    mp_cfg_file_info->get_config_info( "SERVER_PORT", ms_port );
    mp_cfg_file_info->get_config_info( "MONITOR_USER_NAME", ms_name );
    mp_cfg_file_info->get_config_info( "MONITOR_USER_PASSWD", ms_pwd );
    qDebug()<<"LoginDialog::LoginDialog"<<ms_IP<<ms_port<<ms_name<<ms_pwd;

    init_login();
    login_set_text();


    connect( btn_Cancle, SIGNAL( clicked() ), this, SLOT( login_close() ) );
    connect( btn_Login, SIGNAL( clicked() ), this, SLOT( login_clicked() ) );
    qDebug()<<"LoginDialog::LoginDialog"<<width()<<height();

}

void LoginDialog::login_close()
{
    close();
    mp_dialog_title_bar->close();
}

void LoginDialog::login_clicked()
{
    ms_IP = mp_line_IP->text();
    ms_port = mp_line_port->text();
    ms_name = mp_line_name->text();
    ms_pwd = mp_line_pwd->text();
    mp_cfg_file_info->set_config_info( "SERVER_HOST_IP", ms_IP );
    mp_cfg_file_info->set_config_info( "SERVER_PORT", ms_port );
    mp_cfg_file_info->set_config_info( "MONITOR_USER_NAME", ms_name );
    mp_cfg_file_info->set_config_info( "MONITOR_USER_PASSWD", ms_pwd );

    mp_cfg_file_info->save_cofig_file( ms_file_name );

    accept();//隐含窗口，并返回结果QDialg::Accepted
    mp_dialog_title_bar->close();

}
//返回登陆名
QString LoginDialog::get_IP()
{
    return ms_IP;
}

QString LoginDialog::get_port()
{
    return ms_port;
}

QString LoginDialog::get_name()
{
    return ms_name;
}

//返回密码
QString LoginDialog::get_pwd()
{
    return ms_pwd;
}

void LoginDialog::login_set_text()
{
    mp_line_IP->setText( ms_IP );
    mp_line_port->setText( ms_port );
    mp_line_name->setText( ms_name );
    mp_line_pwd->setText( ms_pwd );
}

void LoginDialog::init_login()
{
    QRegExp rx("[a-zA-Z0-9\-\\\_]{25}");
    QRegExpValidator *pRevalidotor = new QRegExpValidator( rx, this);

    mp_lab_IP = new QLabel( tr("IP:") );
    mp_lab_port = new QLabel( tr("port:") );
    mp_lab_name = new QLabel( tr( "用户名" ) );
    mp_lab_pwd = new QLabel( tr("密码：") );
    mp_line_IP = new QLineEdit();
    mp_line_port = new QLineEdit();
    mp_line_name = new QLineEdit();
    mp_line_pwd = new QLineEdit();
    mp_line_pwd->setValidator( pRevalidotor );

    btn_Login = new QPushButton( tr("登录") );
    btn_Cancle = new QPushButton( tr("取消") );

    mp_line_pwd->setEchoMode( QLineEdit::Password );

    mp_lab_IP->setMaximumHeight( 40 );
    mp_lab_port->setMaximumHeight( 40 );
    mp_lab_name->setMaximumHeight( 40 );
    mp_lab_pwd->setMaximumHeight( 40 );
    mp_line_IP->setMaximumHeight( 40 );
    mp_line_port->setMaximumHeight( 40 );
    mp_line_name->setMaximumHeight( 40 );
    mp_line_pwd->setMaximumHeight( 40 );
    btn_Login->setMaximumHeight( 40 );
    btn_Cancle->setMaximumHeight( 40 );

    mp_lab_IP->setMaximumWidth( 80 );
    mp_lab_port->setMaximumWidth( 80 );
    mp_lab_name->setMaximumWidth( 80 );
    mp_lab_pwd->setMaximumWidth( 80 );
    mp_line_IP->setMaximumWidth( 200 );
    mp_line_port->setMaximumWidth( 200 );
    mp_line_name->setMaximumWidth( 200 );
    mp_line_pwd->setMaximumWidth( 200 );

    mp_lab_IP->setStyleSheet( "font-size : 16px" );
    mp_lab_port->setStyleSheet( "font-size : 16px" );
    mp_lab_name->setStyleSheet( "font-size : 16px" );
    mp_lab_pwd->setStyleSheet( "font-size : 16px" );
    mp_line_IP->setStyleSheet( "font-size : 16px" );
    mp_line_port->setStyleSheet( "font-size : 16px" );
    mp_line_name->setStyleSheet( "font-size : 16px" );
    mp_line_pwd->setStyleSheet( "font-size : 16px" );
    btn_Login->setStyleSheet( "font-size : 16px" );
    btn_Cancle->setStyleSheet( "font-size : 16px" );

    /*mp_lab_IP->setMaximumWidth(40);
    mp_lab_port->setMaximumWidth(40);
    mp_lab_name->setMaximumWidth( 40 );
    mp_lab_pwd->setMaximumWidth( 40 );
    mp_line_IP->setMaximumWidth(100);
    mp_line_port->setMaximumWidth(100);
    mp_line_name->setMaximumWidth( 100 );
    mp_line_pwd->setMaximumWidth( 100 );
    */
    //把窗口等分成4行
    QHBoxLayout *h1 = new QHBoxLayout();
    QHBoxLayout *h2 = new QHBoxLayout();
    QHBoxLayout *h3 = new QHBoxLayout();
    QHBoxLayout *h4 = new QHBoxLayout();
    QHBoxLayout *h5 = new QHBoxLayout();
    h1->addWidget( mp_lab_IP );
    h1->addWidget( mp_line_IP );
    h2->addWidget( mp_lab_port );
    h2->addWidget( mp_line_port );
    h3->addWidget( mp_lab_name );
    h3->addWidget( mp_line_name );
    h4->addWidget( mp_lab_pwd );
    h4->addWidget( mp_line_pwd );
    h5->addWidget( btn_Login );
    h5->addWidget( btn_Cancle );

    QVBoxLayout *v = new QVBoxLayout();
    v->addLayout( h1 );
    v->addLayout( h2 );
    v->addLayout( h3 );
    v->addLayout( h4 );
    v->addLayout( h5 );
    setLayout( v );

    resize( DIALOG_WIDTH, DIALOG_HEIGHT );
    //resize( 200, 150 );
    //setMaximumSize( 200, 150 );
}

void LoginDialog::resizeEvent( QResizeEvent *event )
{

}

void LoginDialog::paintEvent( QPaintEvent *event )
{


}
