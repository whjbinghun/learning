#include "wndconfig.h"
#include <QMouseEvent>
#include <QPainter>
#include <QMessageBox>
#include "ui_vodplayinterface.h"
#include "picpreview.h"
#include "picpreviewgroup.h"
#include "picpreviewpan.h"
#include "dlgconfigprs.h"

WndConfig::WndConfig(QWidget *parent) :
    QWidget(parent)
  , mb_mouse_press( false )
  , mp_label_image( 0 )
{
    //m_pixmap_image.load( "D:/1.jpg" );
}

void WndConfig::init_ctrl( Ui::VodPlayInterface *p_main_wnd, NetControl *p_net_ctrl )
{
    mp_label_image = p_main_wnd->id_config_label_move ;
    mp_wnd_ir_preview = p_main_wnd->id_config_frame_ir;
    mp_wnd_vi_preview = p_main_wnd->id_config_frame_vi;
    mp_wnd_ptz_preview = p_main_wnd->id_config_frame_pan;
    mp_wnd_group_preview = p_main_wnd->id_config_frame_group;
    mp_btn_save = p_main_wnd->id_config_save;

    mp_label_image->resize( 160, 120 );
    mp_net_control = p_net_ctrl;
    mp_wnd_ir_preview->set_config_wnd( this, 0 );
    mp_wnd_vi_preview->set_config_wnd( this, 1 );
    mp_wnd_ptz_preview->set_config_wnd( this );
    mp_wnd_group_preview->set_config_chld_wnd( mp_wnd_ir_preview, mp_wnd_vi_preview, mp_wnd_ptz_preview );

    mp_btn_save->hide();
    p_main_wnd->id_config_change->hide();
    connect( mp_btn_save, SIGNAL( clicked() ), this, SLOT( on_id_config_save_clicked() ) );
    connect( p_main_wnd->id_config_change, SIGNAL( clicked() ), this, SLOT( on_id_config_change_clicked() ) );
    connect( p_main_wnd->id_config_btn_save, SIGNAL( clicked() ), this, SLOT( on_id_config_btn_save_clicked() ) );
}

void WndConfig::update_group_info()
{
    if ( !mp_net_control ) return;

    m_map_device_group.clear();
    m_list_device_ir_no_group.clear();
    m_list_device_vi_no_group.clear();

    mp_net_control->get_group_device( m_map_device_group  );
    mp_net_control->get_no_group_ir_device( m_list_device_ir_no_group );
    mp_net_control->get_no_group_vi_device( m_list_device_vi_no_group );

    mp_wnd_ir_preview->init( m_list_device_ir_no_group );
    mp_wnd_vi_preview->init( m_list_device_vi_no_group );
    mp_wnd_group_preview->init( m_map_device_group );
}

void WndConfig::mousePressEvent( QMouseEvent *event )
{

}

void WndConfig::mouseMoveEvent( QMouseEvent *event )
{
    if ( !mb_mouse_press ) return;

    mp_label_image->move( mpt_now_pos );

    QPoint pt_now = event->pos();
    mpt_now_pos = pt_now;

    QPoint pt_pos = mp_wnd_group_preview->pos();
    QPoint pt( pt_now-pt_pos );

    int n_group_num(0);
    if ( mp_wnd_group_preview->pt_in_group_num( pt, n_group_num ) ) {
        setCursor( Qt::ArrowCursor );
    } else {
        setCursor( Qt::ForbiddenCursor );
    }
}

void WndConfig::mouseReleaseEvent( QMouseEvent *event )
{
}

void WndConfig::mouseDoubleClickEvent( QMouseEvent *event )
{
}

void WndConfig::resizeEvent( QResizeEvent *event )
{

}

void WndConfig::paintEvent( QPaintEvent *event )
{
}

void WndConfig::mouse_press( const QPointF &pt, const PicPreviewInfo &pic_info, int n_class )
{
    mb_mouse_press = true;
    mpt_now_pos = QPoint( pt.x(), pt.y() );
    m_pic_preview_info_down = pic_info;

    mp_label_image->move( mpt_now_pos );

    m_pixmap_image.load( pic_info.str_path );

    //m_pixmap_image.load( "D:/1.jpg" );
    mp_label_image->setPixmap( m_pixmap_image.scaled( QSize( 160, 120 ) ) );
    mp_label_image->show();
}

void WndConfig::mouse_release( const QPointF &pt, int n_class )
{
    if ( !mb_mouse_press ) return;

    mb_mouse_press = false;
    mp_label_image->hide();
    setCursor( Qt::ArrowCursor );
    int n_group_num(0);
    if ( !mp_wnd_group_preview->pt_in_group_num( pt.toPoint(), n_group_num ) ) { return; }

    if ( n_class == 0 ) {
        if ( mp_wnd_group_preview->is_ir_in_group( n_group_num ) ) {
            if ( QMessageBox::No == QMessageBox::information( this, "提示", "此组红外已经存在，是否要替换该红外", QMessageBox::Yes | QMessageBox::No ) ) {
                return;
            }
        }
        mp_wnd_ir_preview->delete_image( m_pic_preview_info_down.str_sn );
        mp_wnd_group_preview->add_ir_in_group( n_group_num, m_pic_preview_info_down );
    } else if ( n_class == 1 ) {
        if ( mp_wnd_group_preview->is_vi_in_group( n_group_num ) ) {
            if ( QMessageBox::No == QMessageBox::information( this, "提示", "此组可见光已经存在，是否要替换该可见光", QMessageBox::Yes | QMessageBox::No ) ) {
                return;
            }
        }

        mp_wnd_vi_preview->delete_image( m_pic_preview_info_down.str_sn );
        mp_wnd_group_preview->add_vi_in_group( n_group_num, m_pic_preview_info_down );
    } else {
        if ( mp_wnd_group_preview->is_ptz_in_group( n_group_num ) ) {
            if ( QMessageBox::No == QMessageBox::information( this, "提示", "此组云台已经存在，是否要替换该云台", QMessageBox::Yes | QMessageBox::No ) ) {
                return;
            }
        }

        mp_wnd_ptz_preview->delete_ptz( m_pic_preview_info_down.str_sn );
        mp_wnd_group_preview->add_ptz_in_group( n_group_num, m_pic_preview_info_down );
    }
}

void WndConfig::on_id_config_change_clicked()
{
#if 0
    if ( m_map_device_group.isEmpty() ) return;
    mp_net_control->send_del_group( m_map_device_group.begin().key() );
    return;
#endif

    if ( m_map_device_group.isEmpty() ) return;

    int n_num(1);
    QString str_sns;

    str_sns = m_map_device_group.begin().value().dev_ir.str_sn;

    if ( !m_list_device_vi_no_group.isEmpty() ) {
        n_num++;
        str_sns += ";" + m_list_device_vi_no_group.back().str_sn;
    }

    mp_net_control->send_change_group( m_map_device_group.begin().key(), n_num, str_sns );
    return;
}

void WndConfig::on_id_config_save_clicked()
{

    QString str_ir_sn, str_vi_sn;
    int n_num(0);
    QString str_sns;
    QList< QString > list_sns;

    if ( !m_list_device_ir_no_group.isEmpty() ) {
        str_ir_sn = m_list_device_ir_no_group.back().str_sn;
        n_num++;
        str_sns = str_ir_sn;
        list_sns.push_back( str_ir_sn );
    }

    if ( !m_list_device_vi_no_group.isEmpty() ) {
        str_vi_sn = m_list_device_vi_no_group.back().str_sn;
        n_num++;
        str_sns += ";" + str_vi_sn;
        list_sns.push_back( str_vi_sn );
    }

    if ( str_ir_sn.isEmpty() && str_vi_sn.isEmpty() ) return;

    //mp_net_control->send_group_add_req( list_sns );
    mp_net_control->send_add_group( n_num, str_sns );
}

void WndConfig::on_id_config_btn_save_clicked()
{
    QMap< int , DeviceGroup> map_delete_group = m_map_device_group;
    QMap< int , DeviceGroup> map_change_group;
    QList< DeviceGroup > list_add_group;

    QList< PicPreviewGroupInfo > &list_groups = mp_wnd_group_preview->get_groups();
    QList< PicPreviewGroupInfo >::iterator it = list_groups.begin();
    for ( ; it != list_groups.end(); ++it ) {
        DeviceGroup group_info;
        int n_group_num = it->n_group_num;
        if ( n_group_num < 0 ) {
            group_info.dev_ir.str_sn = it->pic_ir.str_sn;
            group_info.dev_vi.str_sn = it->pic_vi.str_sn;
            list_add_group.push_back( group_info );
        } else {
            QMap< int , DeviceGroup>::iterator it_change = map_delete_group.find( n_group_num );
            if ( it_change != map_delete_group.end() ) {
                if ( it_change.value().dev_ir.str_sn != it->pic_ir.str_sn
                     || it_change.value().dev_vi.str_sn != it->pic_vi.str_sn )
                {
                    group_info.dev_ir.str_sn = it->pic_ir.str_sn;
                    group_info.dev_vi.str_sn = it->pic_vi.str_sn;
                    map_change_group.insert( n_group_num, group_info );
                }
            }
        }
    }

    QMap< int , DeviceGroup>::iterator it_del = map_delete_group.begin();
    for ( ; it_del != map_delete_group.end(); ) {

        bool b_del(true);
        QList< PicPreviewGroupInfo >::iterator it = list_groups.begin();
        for ( ; it != list_groups.end(); ++it ) {
            if ( it->n_group_num == it_del.key() ) {
                b_del = false;
                continue;
            }
        }

        if ( !b_del ) {
            it_del = map_delete_group.erase( it_del );
        } else {
            ++it_del;
        }
    }

    if ( map_delete_group.isEmpty() && map_change_group.isEmpty() && list_add_group.isEmpty() ) {
         QMessageBox::information( this, "提示", "未改变配置的设备" );
         return;
    }

    send_group_delete( map_delete_group );
    send_group_change( map_change_group );
    send_group_add( list_add_group );

    DlgConfigPrs dlg(this);
    dlg.set_net_ctrl( mp_net_control, map_delete_group.size() + map_change_group.size() + list_add_group.size() );
    dlg.exec();

    mp_net_control->clear_group_ctrl_req();
    update_group_info();
}

void WndConfig::send_group_delete( const QMap< int, DeviceGroup > &map_delete_group )
{
    QMap< int, DeviceGroup >::const_iterator it = map_delete_group.begin();
    for ( ; it != map_delete_group.end(); ++it ) {
        int n_group_num = it.key();
        mp_net_control->send_del_group( n_group_num );
    }
}

void WndConfig::send_group_change( const QMap< int, DeviceGroup > &map_change_group )
{
    QMap< int, DeviceGroup >::const_iterator it = map_change_group.begin();
    for ( ; it != map_change_group.end(); ++it ) {
        int n_group_num = it.key();
        int n_sn_num(0);
        QString str_sns;
        if ( !it.value().dev_ir.str_sn.isEmpty() ) {
            ++n_sn_num;
            str_sns += it.value().dev_ir.str_sn;
        }


        if ( !it.value().dev_vi.str_sn.isEmpty() ) {
            if ( n_sn_num>0 ) { str_sns += ";"; }
            ++n_sn_num;
            str_sns += it.value().dev_vi.str_sn;
        }

        mp_net_control->send_change_group( n_group_num, n_sn_num, str_sns );
    }
}

void WndConfig::send_group_add( const  QList< DeviceGroup > &list_add_group )
{
    QList< DeviceGroup >::const_iterator it = list_add_group.begin();
    for ( ; it != list_add_group.end(); ++it ) {

        int n_sn_num(0);
        QString str_sns;
        if ( !it->dev_ir.str_sn.isEmpty() ) {
            ++n_sn_num;
            str_sns += it->dev_ir.str_sn;
        }

        if ( !it->dev_vi.str_sn.isEmpty() ) {
            if ( n_sn_num>0 ) { str_sns += ";"; }

            ++n_sn_num;
            str_sns += it->dev_vi.str_sn;
        }

        mp_net_control->send_add_group( n_sn_num, str_sns );
    }
}

