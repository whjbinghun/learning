#include "wndpresetlist.h"
#include <QPainter>
#include <QPoint>
#include <QMouseEvent>
#include <QMessageBox>
#include <QLineEdit>
#include <QDebug>
#include <QLabel>
#include "../common-funtion/dataconfig.h"

WndPresetList::WndPresetList(QWidget *parent) :
    QWidget(parent)
  , mb_hor_stutas( false )
  , m_str_pic_path( "" )
  , mf_image_scale_x( 1.0 )
  , mf_image_scale_y( 1.0 )
  , mb_mouse_down( false )
  , mp_select_preset_info( 0 )
  , mn_width(0)
  , mn_height(0)
  , mn_draw_offset_x(0)
  , mn_draw_offset_y(0)
  , mb_get_info( false )
  , mn_funtion( (int) FuntionSetCruise )
  , mp_double_preset_info( 0 )
  , mb_preset_change( false )
  , mn_rail( -1 )
  , mn_ptz( -1 )
{
    //mb_hor_stutas = true;

    mp_v_scrollbar = new QScrollBar( Qt::Vertical, this );//垂直
    mp_v_scrollbar->setMinimum( 0 );
    mp_v_scrollbar->hide();

    mp_h_scrollbar = new QScrollBar( Qt::Horizontal, this );//水平
    mp_h_scrollbar->setMinimum( 0 );
    mp_h_scrollbar->hide();

    connect( mp_v_scrollbar, SIGNAL( valueChanged( int ) ), this, SLOT( slot_scroll_value_changed( int ) ) );
    connect( mp_h_scrollbar, SIGNAL( valueChanged( int ) ), this, SLOT( slot_scroll_value_changed( int ) ) );

    //右击菜单
    mp_menu = new QMenu(this);
    QAction  *p_action = mp_menu->addAction(tr("删除"));
    connect( p_action, SIGNAL(triggered()), this, SLOT(slot_delete_preset()) );
    QAction *p_action_edit = mp_menu->addAction( tr( "编辑" ) );
    connect( p_action_edit, SIGNAL( triggered() ), this, SLOT( slot_edit_preset() ) );
//    mp_menu_save_btn = mp_menu->addAction( tr( "重新保存预置点分析" ) );
//    connect( mp_menu_save_btn, SIGNAL( triggered() ), this, SLOT( slot_save_ana() ) );
}

bool WndPresetList::set_status_get( bool b_get )
{
    mb_get_info = b_get;
    update();
}

//加载图片
bool WndPresetList::add_pic(QString str_pic_path)
{
    bool b_reult = m_image_pic.load( str_pic_path );
    if ( b_reult ) {
        m_str_pic_path = str_pic_path;
        cal_image_draw_rect( rect() );
        update();
    }

    return b_reult;
}

void WndPresetList::paintEvent( QPaintEvent *event )
{
    QPainter draw;
    draw.begin( this );

    draw_image( draw, rect() );
    if ( !m_image_pic.isNull() ) {
        draw_preset( draw, rect() );
    }
    //draw.setPen( QColor( 66, 123, 122 ) );
    draw.setPen( QColor( 255, 255, 255 ) );
    draw.drawRect( QRect( 0, 0, width()-1, height()-1 ) );

    draw.end();
}

void WndPresetList::resizeEvent( QResizeEvent *event )
{
    QRect rct = rect();

    cal_image_draw_rect( rect() );

    //垂直
    mp_v_scrollbar->move( rct.x()+rct.width()-20, rct.y() );
    mp_v_scrollbar->resize( 20, rct.height() );
    //水平
    mp_h_scrollbar->move( rct.x(), rct.y()+rct.height()-20 );
    mp_h_scrollbar->resize( rct.width(), 20 );

}

void WndPresetList::mousePressEvent( QMouseEvent *event )
{
    if( event->button() == Qt::LeftButton ) {
        mb_mouse_down = true;

        PresetInfo *p_preset_info = pt_in_preset( event->pos() ) ;//获取预置点的位
        if (p_preset_info){
            mp_select_preset_info = p_preset_info;
        }
        if ( mn_funtion == FuntionSetPreset ) {
            if ( p_preset_info ) {
                m_pt_start_down = event->pos() - QPoint(p_preset_info->n_x / mf_image_scale_x, p_preset_info->n_y / mf_image_scale_x );
            } 
        }
    }

    if ( FuntionShowCruise <= mn_funtion ) { return; }

    if( !mb_mouse_down && event->button() == Qt::RightButton ) {
        PresetInfo *p_preset_info = pt_in_preset( event->pos() ) ;//获取预置点的位置
        mp_select_preset_info = p_preset_info;

        if ( p_preset_info ) {
            if ( mn_funtion == FuntionSetCruise ) {
                if ( p_preset_info == 0 ) return;
                emit sig_scheme_pt_ctrl( p_preset_info->n_rail_preset_no, p_preset_info->n_ptz_preset_no );
            } else {
              /*  if ( mp_double_preset_info == p_preset_info ) {
                    mp_menu->addAction( mp_menu_save_btn );
                } else {
                    mp_menu->removeAction( mp_menu_save_btn );
                }*/
                QCursor cur=this->cursor();
                mp_menu->exec( cur.pos() );
            }
        }
    }
}

void WndPresetList::mouseMoveEvent( QMouseEvent *event )
{
    if ( mb_mouse_down && FuntionSetPreset == mn_funtion ) {
        QPoint pt_end = event->pos();

        PresetInfo *p_pre_info = mp_select_preset_info ;
        if ( p_pre_info == 0 ) return;

        p_pre_info->n_x = ( pt_end.rx() - m_pt_start_down.rx() ) * mf_image_scale_x;
        p_pre_info->n_y = ( pt_end.ry() - m_pt_start_down.ry() ) * mf_image_scale_x;

        QRect rct = m_image_pic.rect();
        if ( p_pre_info->n_x < 0 ) { p_pre_info->n_x = 0; }
        else if ( p_pre_info->n_x > rct.width() ) { p_pre_info->n_x = rct.width(); }

        if ( p_pre_info->n_y < 0 ) { p_pre_info->n_y = 0; }
        else if ( p_pre_info->n_y > rct.height() ) { p_pre_info->n_y = rct.height(); }

        update();
    } else {
        //set_cursor( event->pos() );
        //qDebug() << "mouseMoveEvent";
    }
}

void WndPresetList::mouseReleaseEvent( QMouseEvent *event )
{
    if ( mb_mouse_down ) {
        mb_mouse_down = false;
        if (mp_select_preset_info){
            emit sig_move_preset_pos();
            mp_select_preset_info = NULL;
        }
    } else {
        if ( event->button() == Qt::RightButton && mn_funtion == FuntionSetPreset ) {
            PresetInfo *p_pre_info = mp_select_preset_info ;
            if ( p_pre_info == 0 ) return;

            QPoint pt( p_pre_info->n_x*mf_image_scale_x, p_pre_info->n_y*mf_image_scale_x);
            pt = this->mapToGlobal(pt);

            mp_menu->popup(pt);
        }
    }
}

void WndPresetList::mouseDoubleClickEvent( QMouseEvent *event )
{
    if ( FuntionShowCruise == mn_funtion ) { return; }

    if( event->button() == Qt::LeftButton  ) {
        mp_select_preset_info = NULL;
        mb_mouse_down = false;
        PresetInfo *p_preset_info = pt_in_preset( event->pos() ) ;//获取预置点的位置

        if ( p_preset_info ) {
            mp_double_preset_info = p_preset_info;
            emit sig_call_preset( mp_double_preset_info->n_rail_preset_no, mp_double_preset_info->n_ptz_preset_no );
            update();
        } else {
            pt_in_image( event->pos() );
        }
    }
}

void WndPresetList::hideEvent(QHideEvent *)
{

}

//图片区域
bool WndPresetList::cal_image_draw_rect( QRect rct )
{
    if ( m_image_pic.isNull() ) return false;

    float f_ratio = ( (float)rct.width() ) / rct.height();//窗口宽高比例
    float f_ratio_pic = ( (float)m_image_pic.width() ) / m_image_pic.height();//图片宽高比例

    int n_width(0), n_height(0);

    if ( mb_hor_stutas ) {//水平,图片高度==区域高度
        if ( f_ratio > f_ratio_pic ) {
            mp_h_scrollbar->hide();
            n_height = rct.height();
        } else {
            mp_h_scrollbar->show();
            n_height = rct.height() - 20;
        }
        mp_v_scrollbar->hide();

        n_width =  n_height * m_image_pic.width() / m_image_pic.height();
        mp_h_scrollbar->setRange( 0, n_width-rct.width()  );

        if ( f_ratio > f_ratio_pic ) {
            int n_image_width = m_image_pic.width() * n_height / m_image_pic.height();
            mn_draw_offset_x = ( rct.width() - n_image_width ) / 2;
        } else {
            mn_draw_offset_x = 0;
        }
        mn_draw_offset_y = 0;
    } else {//垂直，图片宽度==区域宽度
        if ( f_ratio > f_ratio_pic ) {
            mp_v_scrollbar->show();
            n_width = rct.width() - 20;
        } else {
            mp_v_scrollbar->hide();
            n_width = rct.width();
        }
        mp_h_scrollbar->hide();
        n_height = n_width * m_image_pic.height() / m_image_pic.width();
        mp_v_scrollbar->setRange( 0, n_height-rct.height() );

        if ( f_ratio > f_ratio_pic ) {
            mn_draw_offset_y = 0;
        } else {
            int n_image_hieght = m_image_pic.height() * n_width / m_image_pic.width() ;
            mn_draw_offset_y = ( rct.height() - n_image_hieght ) / 2;
        }
        mn_draw_offset_x = 0;
    }
    //图片占用空间的宽度和高度
    mn_width = n_width;
    mn_height = n_height;

    mf_image_scale_x = ( (float) m_image_pic.width() ) / n_width;//图片与窗口之间的宽度比例
    mf_image_scale_y = ( (float) m_image_pic.height() ) / n_height;//图片与窗口之间的高度比例
}

bool WndPresetList::update_preset_pos_info( QMap<int, PresetInfo> &map_preset_info )
{
    QList< PresetInfo * >::iterator it = m_list_preset_info.begin();
    for( ; it != m_list_preset_info.end(); ++it ) {
        PresetInfo *p_info = (*it);
        if ( 0 == p_info ) continue;

        int n_preset = ( p_info->n_rail_preset_no << 16 ) + p_info->n_rail_preset_no;

        p_info->n_x = map_preset_info[ n_preset ].n_x;
        p_info->n_y = map_preset_info[ n_preset ].n_y;
    }

    update();
}

bool WndPresetList::update_preset_info( QMap<int, PresetInfo> &map_preset_info
                                       , int n_buf_len, unsigned char *p_pic_buf )
{
    QMap< int, PresetInfo >::iterator it_map = map_preset_info.begin();
    for ( ; it_map != map_preset_info.end(); ++it_map ) {
        add_preset( it_map.value() );
    }

    if ( p_pic_buf && n_buf_len ) {
        m_image_pic = QImage::fromData( p_pic_buf, n_buf_len, "JPG" );
        qDebug() << "有图片数据,转化成m_image_pic变量成功";
        /*QFile file( "D:\\10.jpg" );
        file.open(QIODevice::ReadWrite );
        file.write( (const char *)p_pic_buf, n_buf_len );
        file.close();*/
    }
	
	if( mn_funtion == FuntionSetPreset ) {
    	if( m_image_pic.isNull() ) {
            qDebug() << "m_image_pic为空..........";
        	clear_preset_pos();
    	} else {
            qDebug() << "m_image_picbu不为空..........";
        	calibration_preset();
    	}
    } else if ( FuntionShowCruise == mn_funtion || mn_funtion == FuntionShowCruiseAllpt ) {
        mb_hor_stutas = m_image_pic.width() > m_image_pic.height();
    }

    emit sig_preset_list_update();
    //mp_preset_setting->update_preset_tab( m_list_preset_info );
    cal_image_draw_rect( rect() );

    update();
    return true;

}

bool WndPresetList::update_scheme_info(CruiseSchemeInfo &cruise)
{
    QMap< int, int >::iterator it_status = m_map_preset.begin();
    for ( ; it_status != m_map_preset.end(); ++it_status ) {
        it_status.value() &= 0xFFFE;
    }

    QVector< CruisePoint >::iterator it = cruise.vct_cruise_pts.begin();
    for ( ; it != cruise.vct_cruise_pts.end(); ++it ) {
        int n_key = ( it->n_rail_preset << 16 ) + it->n_ptz_preset;

        it_status = m_map_preset.find( n_key );
        if ( it_status != m_map_preset.end() ) {
            it_status.value() |= 1;
        }
    }

    m_vct_cruise_pts = cruise.vct_cruise_pts;

    update();
    emit sig_preset_scheme_update();
}

//draw 图片
bool WndPresetList::draw_image( QPainter &draw, QRect rct )
{
    QRect rc_draw;
 	get_image_pos( rc_draw );//确定函数内容？？？
    //int n_x_offset = get_offset_pos( true );
    //int n_y_offset = get_offset_pos( false );

    //rc_draw = QRect( QPoint( rct.left() - n_x_offset, rct.top() - n_y_offset ), QSize( mn_width, mn_height ) );

    //draw.setPen( QColor( 0,0,0));//66, 123, 122 ) );
    draw.setPen( QColor( 255, 255, 255 ) );
    if ( mb_get_info ) {
        if( m_image_pic.isNull() ){
            draw.drawText( rect(), Qt::AlignCenter, "正在获取图片" );
        }
    } else {
        if ( m_image_pic.isNull() ) {
            draw.drawText( rect(), Qt::AlignCenter, "没有设置图片" );
        } else {
            draw.drawImage(  rc_draw, m_image_pic,  m_image_pic.rect() );
        }
    }

    return true;
}

//画预置点
bool WndPresetList::draw_preset( QPainter &draw, QRect rct )
{
    int n_area; QColor clr_cru, clr_no_cru, clr_preset;
    DataConfig::get_instance()->get_map_cruise_int( n_area, clr_cru, clr_no_cru, clr_preset );

    int n_x_offset = get_offset_pos( true );
    int n_y_offset = get_offset_pos( false );

    QList< PresetInfo * >::iterator it = m_list_preset_info.begin();
    for ( int n=1; it != m_list_preset_info.end(); ++it, ++n ) {
        PresetInfo *p_pre_info = *it;
        QRect rct_draw;

        int n_pt_x = p_pre_info->n_x/mf_image_scale_x ;
        int n_pt_y = p_pre_info->n_y/mf_image_scale_x ;

        if ( n_pt_x > mn_width ) { n_pt_x = mn_width; }
        if ( n_pt_y > mn_height ) { n_pt_y = mn_height; }

        n_pt_x -= ( n_x_offset + n_area/2 );
        n_pt_y -= ( n_y_offset + n_area/2 );

        rct_draw = QRect( QPoint( n_pt_x, n_pt_y ), QSize( n_area, n_area ) );

        bool b_draw_cur(false);

        if ( mp_double_preset_info == NULL )
            mp_double_preset_info = get_double_preset_info( mn_rail, mn_ptz );
        if( mp_double_preset_info
                && mp_double_preset_info->n_ptz_preset_no == p_pre_info->n_ptz_preset_no
                && mp_double_preset_info->n_rail_preset_no == p_pre_info->n_rail_preset_no ) {
            draw.setBrush( clr_preset/*QColor( 74, 206, 18 ) */);
            draw.drawEllipse( rct_draw );
            b_draw_cur = true;
        }
        if ( mn_funtion != FuntionSetPreset ) {
            if ( has_cruise_pt( p_pre_info->n_rail_preset_no, p_pre_info->n_ptz_preset_no ) ) {
                draw.setPen( clr_cru/*QColor( 240, 240, 240 ) */);
            } else {
                draw.setPen( clr_no_cru /*QColor( 255, 222, 69 )*/ );
            }
        } else {
            draw.setPen( clr_no_cru /*QColor( 255, 222, 69 )*/);
        }

        if ( mn_funtion != FuntionShowCruise || b_draw_cur ) {
            draw.drawEllipse( rct_draw );
            draw.drawText( rct_draw, Qt::AlignCenter, QString("%1").arg(n) );
            draw.drawText( QRect( rct_draw.right() + 5, rct_draw.top(),  300, rct_draw.height() ), Qt::AlignVCenter | Qt::AlignLeft, p_pre_info->str_name );
        }

        if ( b_draw_cur ) {
            draw.setBrush( Qt::NoBrush );
        }
    }

    return true;
}

void WndPresetList::slot_scroll_value_changed( int n_value )
{
    update();
}

PresetInfo *WndPresetList::pt_in_preset( QPoint pt )
{
    //回去电子地图中预置点的区域
    int n_area; QColor clr_cru, clr_no_cru, clr_preset;
    DataConfig::get_instance()->get_map_cruise_int( n_area, clr_cru, clr_no_cru, clr_preset );

    int n_x_offset = get_offset_pos( true );
    int n_y_offset = get_offset_pos( false );

    QList< PresetInfo * >::iterator it = m_list_preset_info.begin();
    for ( ; it != m_list_preset_info.end(); ++it ) {
        PresetInfo *p_pre_info = *it;

        int n_pt_x = p_pre_info->n_x/mf_image_scale_x ;
        int n_pt_y = p_pre_info->n_y/mf_image_scale_x ;

        if ( n_pt_x > mn_width ) { n_pt_x = mn_width; }
        if ( n_pt_y > mn_height ) { n_pt_y = mn_height; }

        n_pt_x -= ( n_x_offset + n_area/2 );
        n_pt_y -= ( n_y_offset + n_area/2 );

        QRect rct = QRect( QPoint( n_pt_x, n_pt_y )
                                , QSize( n_area, n_area ) );

        if ( pt.x() > rct.left() && pt.x() < rct.right()
             && pt.y() > rct.top() && pt.y() < rct.bottom() )
        {
            return *it;
        }
    }

    return 0;
}

PresetInfo *WndPresetList::pt_in_image( QPoint pt )
{
    if ( FuntionSetPreset != mn_funtion ) { return 0; }

    QRect rc_image;

    int n_x_offset = get_offset_pos( true );
    int n_y_offset = get_offset_pos( false );
    rc_image = QRect( QPoint(  rect().left() - n_x_offset,  rect().top() - n_y_offset ), QSize( mn_width, mn_height ) );

    //看水平还是竖直
    if( rc_image.left() <= pt.x() && rc_image.right() > pt.x()
            && rc_image.top() <= pt.y() && rc_image.bottom() > pt.y() ) {

        AddPresetDialog *p_add_preset_dlg = new AddPresetDialog( this );
        if ( p_add_preset_dlg ){
            p_add_preset_dlg->setWindowTitle( QString("新增预置点") );
            int ret = p_add_preset_dlg->exec();
            if ( QDialog::Accepted == ret ){
           //     emit sig_preset_btn_enable( false );  //新建预置点的时候,发送到pressetting类中把报警条件三个按钮置灰
                if( p_add_preset_dlg->get_sure_press() ) {
                    QString str_preset_alias = p_add_preset_dlg->get_preset_text();
                    emit sig_add_preset( str_preset_alias
                                                   , (pt.x() + n_x_offset)*mf_image_scale_x, (pt.y() + n_y_offset)*mf_image_scale_x );
                    qDebug()<<"新增预置点WndPresetList::pt_in_image"<<(pt.x() - n_x_offset)*mf_image_scale_x<< (pt.y() - n_y_offset)*mf_image_scale_x \
                              <<pt.x()<<pt.y()<<mf_image_scale_x;
                }
            }
            delete p_add_preset_dlg;
        }

    }
    return 0;
}

void WndPresetList::slot_delete_preset()
{
    if ( QMessageBox::Ok != QMessageBox::information( this, "提示", "确定要删除预置点吗？", QMessageBox::Ok | QMessageBox::Cancel ) ) {
           return ;
       }
    if ( mp_select_preset_info ) {
        emit sig_del_preset( mp_select_preset_info->n_rail_preset_no, mp_select_preset_info->n_ptz_preset_no );
    }
}

void WndPresetList::slot_edit_preset()
{
    if ( mp_select_preset_info ) {
        AddPresetDialog *p_add_preset_dlg = new AddPresetDialog( this );
        p_add_preset_dlg->setWindowTitle( QString("修改预置点") );
        p_add_preset_dlg->set_edit_text( mp_select_preset_info->str_name );

        p_add_preset_dlg->exec();
        if( p_add_preset_dlg->get_sure_press() ) {
            QString str_preset_alias = p_add_preset_dlg->get_preset_text();
            mp_select_preset_info->str_name = str_preset_alias;
            emit sig_change_preset( mp_select_preset_info->n_rail_preset_no, mp_select_preset_info->n_ptz_preset_no, mp_select_preset_info->str_name );
        }
        delete p_add_preset_dlg;

    }
}

void WndPresetList::slot_save_ana()
{
    if ( QMessageBox::Save != QMessageBox::information( this, "提示", "是否重新保存当前预置位的分析？", QMessageBox::Save | QMessageBox::Close ) ) {
        return ;
    }

    if ( mp_select_preset_info ) {
        emit sig_change_preset_ana( mp_select_preset_info->n_rail_preset_no, mp_select_preset_info->n_ptz_preset_no, mp_select_preset_info->str_name );
    }
}

PresetInfo *WndPresetList::get_double_preset_info( int n_rail, int n_ptz )
{
    QList< PresetInfo * >::iterator it = m_list_preset_info.begin();
    for ( ; it != m_list_preset_info.end(); ++it ) {
        if ( (*it)->n_rail_preset_no == n_rail && (*it)->n_ptz_preset_no== n_ptz ) {
            return (*it);
        }
    }
    return NULL;
}

bool WndPresetList::delete_preset_info( int n_rail, int n_ptz )
{
    int n_key = ( n_rail << 16 ) + n_ptz;
    QMap< int, int >::iterator it_p = m_map_preset.find( n_key );
    if ( it_p != m_map_preset.end() ) {
        m_map_preset.erase( it_p );
    }

    QList< PresetInfo * >::iterator it = m_list_preset_info.begin();
    for ( ; it != m_list_preset_info.end(); ++it ) {
        if ( (*it)->n_rail_preset_no == n_rail && (*it)->n_ptz_preset_no == n_ptz ) {
            delete *it;
            m_list_preset_info.erase( it );;
            emit sig_preset_list_update();
            //mp_preset_setting->update_preset_tab( m_list_preset_info );
            return true;
        }
    }

    return false;
}

//删除预置点信息
bool WndPresetList::delete_preset_info( PresetInfo *p_info )
{
    QList< PresetInfo * >::iterator it = m_list_preset_info.begin();
    for ( ; it != m_list_preset_info.end(); ++it ) {
        if ( *it == p_info ) {
            delete p_info;
            m_list_preset_info.erase( it );
            return true;
        }
    }

    return false;
}

bool WndPresetList::clear_preset_info()
{
    QList< PresetInfo * >::iterator it = m_list_preset_info.begin();
    for ( ; it != m_list_preset_info.end(); ++it ) {
        delete *it;
    }

    m_list_preset_info.clear();
    m_map_preset.clear();
    m_vct_cruise_pts.clear();

    m_image_pic = QImage();
    mp_select_preset_info = NULL;
    mp_double_preset_info = NULL;
    mp_v_scrollbar->hide();
    mp_h_scrollbar->hide();
    mb_mouse_down = false;

    emit sig_preset_list_update();
    return true;
}

//增加预置点
bool WndPresetList::add_preset(PresetInfo &preset_info)
{
    unsigned int n_key = ( preset_info.n_rail_preset_no << 16 ) + preset_info.n_ptz_preset_no;
    QMap< int, int >::iterator it = m_map_preset.find( n_key ) ;
    if ( it != m_map_preset.end() ) {
        return false;
    }

    PresetInfo *p_info = new PresetInfo;
    p_info->n_ptz_preset_no = preset_info.n_ptz_preset_no;
    p_info->n_rail_preset_no = preset_info.n_rail_preset_no;
    p_info->n_x = preset_info.n_x;
    p_info->n_y = preset_info.n_y;
    p_info->str_name = preset_info.str_name;
    p_info->vct_anas = preset_info.vct_anas;

    m_list_preset_info.push_back( p_info );
    m_map_preset.insert( n_key, 0 );


    return true;
}

bool WndPresetList::change_preset(PresetInfo &preset_info)
{
    QList< PresetInfo * >::iterator it = m_list_preset_info.begin();
    for ( ; it != m_list_preset_info.end(); ++it ) {
        if ( (*it)->n_rail_preset_no == preset_info.n_rail_preset_no
             && (*it)->n_ptz_preset_no == preset_info.n_ptz_preset_no ) {
            PresetInfo *p_info = (*it);
            p_info->n_ptz_preset_no = preset_info.n_ptz_preset_no;
            p_info->n_rail_preset_no = preset_info.n_rail_preset_no;
            p_info->str_name = preset_info.str_name;
            p_info->vct_anas = preset_info.vct_anas;
            return true;
        }
    }

    return false;
}

bool WndPresetList::go_to_preset( const int & n_rail_no, const int & n_ptz_no )
{
    bool b_ret = false;

    QList< PresetInfo * >::iterator it = m_list_preset_info.begin();
    for ( ; it != m_list_preset_info.end(); ++it ) {
        if ( (*it)->n_rail_preset_no == n_rail_no
             && (*it)->n_ptz_preset_no == n_ptz_no ) {
            PresetInfo *p_info = (*it);
            if ( p_info ){
                mp_double_preset_info = p_info;
                emit sig_call_preset( n_rail_no, n_ptz_no );


                b_ret =  true;

                break;
            }
        }
    }

    return b_ret;
}

//删除所有预置点的位置信息
void WndPresetList::clear_preset_pos()
{
    QList< PresetInfo * >::iterator it_pre=m_list_preset_info.begin();
    for( ;it_pre!=m_list_preset_info.end(); it_pre++ ) {
        (*it_pre)->n_x = 0;
        (*it_pre)->n_y = 0;
    }
}

void WndPresetList::set_cursor( QPoint pt_pos )
{
    PresetInfo *p_preset_info = pt_in_preset( pt_pos ) ;//获取预置点的位置
    if ( p_preset_info ) {
        setCursor( Qt::OpenHandCursor );
        //编辑
    } else {
        setCursor( Qt::ArrowCursor );
    }
}

QString WndPresetList::get_image_path()
{
    return m_str_pic_path;
}

void WndPresetList::set_image_path( QString str_image_path )
{
    m_str_pic_path = str_image_path;
    if ( mp_v_scrollbar && !mp_v_scrollbar->isHidden() ) { mp_v_scrollbar->hide(); }
    if ( mp_h_scrollbar && !mp_h_scrollbar->isHidden() ) { mp_h_scrollbar->hide(); }
}

int WndPresetList::get_offset_pos(bool b_hor)
{
    int n_h_scroll(0), n_v_scroll(0);
    if ( mb_hor_stutas ) {
        if ( !mp_h_scrollbar->isHidden() ) {
            n_h_scroll = mp_h_scrollbar->value();
        }
    } else {
        if ( !mp_v_scrollbar->isHidden() ) {
            n_v_scroll= mp_v_scrollbar->value();
        }
    }

    if ( b_hor ) {
        return n_h_scroll - mn_draw_offset_x;
    } else {
        return n_v_scroll - mn_draw_offset_y;
    }
}

void WndPresetList::preset_area( QPoint &pt, QPoint &pt_area )
{
    //图片宽高mn_width;mn_height;
    //mn_draw_offset_x;mn_draw_offset_y
    pt_area = pt;
    if( pt_area.x()<0 ) {
        pt_area.setX( 0 );
    } else if( pt_area.x()>m_image_pic.width() ) {
        pt_area.setX( m_image_pic.width() );
    }
    if( pt_area.y()<0 ) {
        pt_area.setY( 0 );
    } else if( pt_area.y() > m_image_pic.height() ) {
        pt_area.setY( m_image_pic.height() );
    }
}

//获取图片位置
void WndPresetList::get_image_pos( QRect &rct )
{
    int n_x_offset = get_offset_pos( true );
    int n_y_offset = get_offset_pos( false );

    rct = QRect( QPoint( rect().left() - n_x_offset, rect().top() - n_y_offset ), QSize( mn_width, mn_height ) );
}

//校准
void WndPresetList::calibration_preset()
{
    QList< PresetInfo * >::iterator it = m_list_preset_info.begin();
    for ( ; it != m_list_preset_info.end(); ++it ) {
        QPoint pt_pre_old( (*it)->n_x, (*it)->n_y );
        QPoint pt_pre_new;
        preset_area( pt_pre_old, pt_pre_new );
        (*it)->n_x = pt_pre_new.x();
        (*it)->n_y = pt_pre_new.y();
    }
}
bool WndPresetList::has_cruise_pt( int n_rail, int n_ptz )
{
    QVector< CruisePoint >::iterator it = m_vct_cruise_pts.begin();
    for ( ; it != m_vct_cruise_pts.end(); ++it ) {
        if ( it->n_ptz_preset == n_rail && it->n_rail_preset == n_ptz ) {
            return true;
        }
    }
    return false;
}

void WndPresetList::set_arrive_preset( int n_rail, int n_ptz )
{
    mn_rail = n_rail;
    mn_ptz = n_ptz;
    mp_double_preset_info = NULL;

    QList< PresetInfo * >::iterator it = m_list_preset_info.begin();
    for ( ; it != m_list_preset_info.end(); ++it ) {
        if ( (*it)->n_rail_preset_no == n_rail && (*it)->n_ptz_preset_no== n_ptz ) {
            mp_double_preset_info = (*it);
        }
    }
    update();
}

void WndPresetList::set_selected_preset(PresetInfo &selected_preset_info)
{
    m_selected_preset_info = selected_preset_info;
    mp_double_preset_info = &m_selected_preset_info;
//    emit sig_call_preset( m_selected_preset_info.n_rail_preset_no, m_selected_preset_info.n_ptz_preset_no );

}

bool WndPresetList::get_cur_preset(int &n_rail, int &n_ptz)
{
    if ( mp_double_preset_info == NULL ) return false;

    n_rail = mp_double_preset_info->n_rail_preset_no;
    n_ptz = mp_double_preset_info->n_ptz_preset_no;

    return true;
}


PresetInfo *&WndPresetList::get_double_preset_info()
{
    return mp_double_preset_info;
}
