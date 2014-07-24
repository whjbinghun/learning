#include "logindialog.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include "ParseConFile.h"
#include <QDebug>

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
{
    load_cfg();
    mp_lab_IP = new QLabel( tr("IP:") );
    mp_lab_port = new QLabel( tr("port:") );
    mp_lab_name = new QLabel( tr( "用户名" ) );
    mp_lab_pwd = new QLabel( tr("密码：") );
    mp_line_IP = new QLineEdit();
    mp_line_port = new QLineEdit();
    mp_line_name = new QLineEdit();
    mp_line_pwd = new QLineEdit();
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
    h1->addWidget(mp_lab_IP);
    h1->addWidget(mp_line_IP);
    h2->addWidget(mp_lab_port);
    h2->addWidget( mp_line_port );
    h3->addWidget( mp_lab_name );
    h3->addWidget( mp_line_name );
    h4->addWidget(mp_lab_pwd);
    h4->addWidget( mp_line_pwd );
    h5->addWidget(btn_Login);
    h5->addWidget(btn_Cancle);

    QVBoxLayout *v = new QVBoxLayout();
    v->addLayout( h1 );
    v->addLayout( h2 );
    v->addLayout( h3 );
    v->addLayout( h4 );
    v->addLayout( h5 );
    this->setLayout( v );
    this->resize( 200, 150 );
    this->setMaximumSize( 200, 150 );
    connect(btn_Cancle, SIGNAL(clicked()), this, SLOT(close()));
    connect(btn_Login, SIGNAL(clicked()), this, SLOT(login_clicked()));
    set_login_info();
    login_set_text();
}
void LoginDialog::login_clicked()
{
    ms_IP = mp_line_IP->text();
    ms_port = mp_line_port->text();
    ms_name = mp_line_name->text();
    ms_pwd = mp_line_pwd->text();
    qDebug()<<"LoginDialog::login_clicked():"<<ms_pwd;
    set_host_ip( ms_IP );
    set_server_port( ms_port );
    set_user_name( ms_name );
    set_user_pwd( ms_pwd );
    save_login_info();
    /*QSqlTableModel model;
    model.setTable("student");
    model.setFilter(tr("id = '%1' and pwd = '%2'").arg(name).arg(pwd));
    model.select();
    if(model.rowCount()==1)//查询到有一个结果
    {
        accept();//隐含窗口，并返回结果QDialg::Accepted
    }else
    {
        QMessageBox::warning(this, tr("warn"), tr("用户名或者密码不正确"));
        line_Name->clear();
        line_Pwd->clear();
        line_Name->setFocus();
    }*/
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

void LoginDialog::set_login_info()
{
    ms_IP = get_host_ip();
    ms_port = get_server_port();
    ms_name = get_user_name();
    ms_pwd = get_user_pwd();
    //qDebug()<<"LoginDialog::set_login_info"<<ms_IP<<ms_port<<ms_name<<ms_pwd;
}

void LoginDialog::login_set_text()
{
    mp_line_IP->setText( ms_IP );
    mp_line_port->setText( ms_port );
    mp_line_name->setText( ms_name );
    mp_line_pwd->setText( ms_pwd );
}
