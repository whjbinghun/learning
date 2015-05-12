#include "notifydialog.h"
#include "ui_notifydialog.h"
#include <QTextCodec>

NotifyDialog::NotifyDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NotifyDialog)
{
    ui->setupUi(this);
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));
    setWindowTitle("提示:");
    setMaximumSize( QSize( 245, 90 ) );
    setMinimumSize( QSize( 245, 90 ) );
    ui->label->setFont(QFont("Times", 12, QFont::Bold));
    ui->label->setText( "服务器断开,正在重连......" );
    ui->pushButton->setFont(QFont("Times", 12, QFont::Bold));
    setWindowFlags(Qt::WindowTitleHint);
    mn_count = 0;
 //   mp_timer = new QTimer(this);
 //   connect( mp_timer, SIGNAL( timeout() ), this, SLOT( slot_connect_timer() ) );
}

NotifyDialog::~NotifyDialog()
{
 /*   stop_timer();
    if( mp_timer ){
        delete mp_timer;
        mp_timer = NULL;
    }*/
    delete ui;
}

void NotifyDialog::on_pushButton_clicked()
{
    emit sig_application_exit();
}

void NotifyDialog::paintEvent( QPaintEvent *event )
{
 //   ui->label->setText( m_notify );
}


void NotifyDialog::slot_connect_timer()
{
    if( mn_count % 6 == 0 ){
        m_notify = "服务器断开,正在重连";
    }
  /*  if( mn_count == 30 ){
      //  emit sig_application_exit();
        close();
    }*/
    mn_count++;
    m_notify += ".";
    update();
}

void NotifyDialog::start_timer()
{
 //   mp_timer->start( 500 );
}

void NotifyDialog::stop_timer()
{
//    mp_timer->stop();
//    mn_count = 0;
}


void NotifyDialog::exit_notify_dlg()
{
    QDialog::close();
}
