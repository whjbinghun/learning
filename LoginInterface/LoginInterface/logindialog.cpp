#include "logindialog.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QRegExpValidator>
#include <QCoreApplication>
#include <QPainter>
#include <QDebug>

#define TITLE_BAR_HEIGHT 50

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
    ,mp_title_bar( NULL )
{
    init_title_bar();

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


    connect( btn_Cancle, SIGNAL( clicked() ), this, SLOT( close() ) );
    connect( btn_Login, SIGNAL( clicked() ), this, SLOT( login_clicked() ) );

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
    mp_lab_IP->setMaximumWidth(40);
    mp_lab_port->setMaximumWidth(40);
    mp_lab_name->setMaximumWidth( 40 );
    mp_lab_pwd->setMaximumWidth( 40 );
    mp_line_IP->setMaximumWidth(100);
    mp_line_port->setMaximumWidth(100);
    mp_line_name->setMaximumWidth( 100 );
    mp_line_pwd->setMaximumWidth( 100 );

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

    resize( width(), height() );
    //resize( 200, 150 );
    //setMaximumSize( 200, 150 );

}

void LoginDialog::init_title_bar()
{
    //隐藏标题栏
    showFullScreen();
    setWindowFlags (Qt::FramelessWindowHint);
    //创建标题栏  类
    mp_title_bar = new TitleBar( this );
    mp_title_bar->resize( width(), TITLE_BAR_HEIGHT );
    mp_title_bar->setMouseTracking( true );
    qDebug()<<"LoginDialog::init_title_bar"<<width()<<height();//1920*1080
}

void LoginDialog::resizeEvent( QResizeEvent *event )
{
    //mp_login_title_bar->move( 0, 0 );
    //mp_login_title_bar->resize( width(), TITLE_BAR_HEIGHT );
    /*mp_lab_IP->move( 0, 0 );
    mp_lab_port->move( 0, 30 );
    mp_lab_name->move( 0, 60 );
    mp_lab_pwd->move( 0, 90 );
    mp_line_IP->move( 0, 120 );
    mp_line_port->move( 0, 150 );
    mp_line_name->move( 0, 180 );
    mp_line_pwd->move( 0, 210 );
    */
}

void LoginDialog::paintEvent( QPaintEvent *event )
{
    QPainter painter(this);
    //标题栏背景图片
    painter.drawPixmap( 0, 0, width(), TITLE_BAR_HEIGHT, QPixmap(":image/res/background.bmp") );
    painter.end();

}
