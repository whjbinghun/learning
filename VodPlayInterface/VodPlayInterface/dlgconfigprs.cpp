#include "dlgconfigprs.h"
#include "ui_DlgConfigProgress.h"
#include "netcontrol.h"
#include <QMutexLocker>
#include <QMessageBox>

DlgConfigPrs::DlgConfigPrs(QWidget *parent) :
    QDialog(parent)
  , ui( new Ui::DlgConfigProgress )
  , mp_net_ctrl( NULL )
  , mp_timer( NULL )
  , mn_status( cmd_confirm_group )
{
    ui->setupUi( this );

    unsigned int dw_flags = windowFlags();
    dw_flags ^= Qt::WindowCloseButtonHint;
    dw_flags ^= Qt::WindowContextHelpButtonHint;
    setWindowFlags( (Qt::WindowFlags)dw_flags );

    mp_timer = new QTimer( this );
    connect( mp_timer,SIGNAL( timeout() ), this, SLOT( timer_update_progress() ) );

    mp_timer_timeout = new QTimer( this );
    connect( mp_timer_timeout,SIGNAL( timeout() ), this, SLOT( timer_cmd_progress_timeout() ) );
    //setWindowFlags( Qt::FramelessWindowHint );
    //setWindowFlags(Qt::WindowTitleHint | Qt::CustomizeWindowHint );
}

DlgConfigPrs::~DlgConfigPrs()
{
    close_timer_cmd();
    close_timer_timeout();
}

void DlgConfigPrs::close_timer_cmd()
{
    if ( mp_timer ) {
        mp_timer->stop();
        delete mp_timer;
        mp_timer = NULL;
    }
}

void DlgConfigPrs::close_timer_timeout()
{
    if ( mp_timer_timeout ) {
        mp_timer_timeout->stop();
        delete mp_timer_timeout;
        mp_timer_timeout = NULL;
    }
}

void DlgConfigPrs::set_net_ctrl(NetControl *p_ctrl, int n_cmd_num )
{
    mp_net_ctrl = p_ctrl;
    mn_status = cmd_confirm_group;
    mp_timer->start( 300 );
    mp_timer_timeout->start( 30000 );
    ui->id_progress_bar->setRange( 0, n_cmd_num+1 );
    ui->id_progress_bar->setValue(0);
    mn_cmd_num = n_cmd_num;
}

void DlgConfigPrs::timer_cmd_progress_timeout()
{
    QMutexLocker lck( &m_mutex_status );
    if ( mn_status == cmd_confirm_group ) {
        lck.unlock();
        close_timer_cmd();
        close_timer_timeout();
        QMessageBox::information( this, "提示", "保存配置超时！" );
        close();
    }
}

void DlgConfigPrs::timer_update_progress()
{
    static int n_count = 0; n_count++;
    QString str_s;
    for ( int n=0; n<=n_count%3; ++n ) { str_s += "·"; }
    ui->id_label_title->setText( "正在保存" + str_s );

    switch ( mn_status ) {
    case cmd_confirm_group:
        {
            int n_cof, n_fail;
            mp_net_ctrl->get_complete_group_req( n_cof, n_fail );
            ui->id_progress_bar->setValue( n_cof+n_fail );
            if ( n_cof + n_fail == mn_cmd_num ) {
                QMutexLocker lck( &m_mutex_status );
                mn_status = cmd_send_xml;
                mb_has_fail_cmd = n_fail > 0;
            }
        }
        break;
    case cmd_send_xml:
        {
            mn_cmd_update_xml = mp_net_ctrl->send_update_xml();
            QMutexLocker lck( &m_mutex_status );
            mn_status = cmd_confirm_xml;
        }
        break;
    case cmd_confirm_xml:
        {
            if ( mp_net_ctrl->is_update_xml( mn_cmd_update_xml ) ) {
                close_timer_cmd();
                close_timer_timeout();

                ui->id_progress_bar->setValue( mn_cmd_num+1 );
                if ( mb_has_fail_cmd )  {
                    QMessageBox::information( this, "提示", "部分组未配置成功！" );
                } else {
                    QMessageBox::information( this, "提示", "完成配置！" );
                }
                close();
            }
        }
        break;
    }
}
