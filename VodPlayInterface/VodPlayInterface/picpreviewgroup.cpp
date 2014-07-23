#include "picpreviewgroup.h"
#include <QBuffer>
#include <QPainter>
#include <QColor>
#include <QRgb>
#include <QDebug>
#include "picpreviewpan.h"
#include "picpreview.h"
#include <qcoreapplication.h>

#define SPACE 20
#define IMAGE_WIDTH 100
//边框与图片间隔
#define FRAME_SPACE 5

PicPreviewGroup::PicPreviewGroup(QWidget *parent) :
    QFrame(parent)
    ,mn_scroll_pos( 0 )
    ,mn_group_number(0)
    ,mp_v_scrollBar( NULL )
    ,mb_move_add_image( false )
    ,mb_press_add_image( false )
{
    QString str_app_path = QCoreApplication::applicationDirPath();
    mp_add_image = new QImage( str_app_path + "/add_group.png" );
    mp_delete_image = new QImage( str_app_path + "/delete_group.png" );

    mp_v_scrollBar = new QScrollBar( Qt::Vertical, this );
    mp_v_scrollBar->setMinimum( 0 );
    mp_v_scrollBar->hide();

    connect( mp_v_scrollBar, SIGNAL( valueChanged( int ) ), this, SLOT( set_v_edit_value( int ) ) );
}

PicPreviewGroup::~PicPreviewGroup()
{
    clear_group();

    delete mp_add_image;
    delete mp_delete_image;
    delete mp_v_scrollBar;
}

void PicPreviewGroup::set_config_chld_wnd(PicPreview *p_ir, PicPreview *p_vi, PicPreviewPan *p_ptz)
{
    mp_wnd_ir = p_ir;
    mp_wnd_vi = p_vi;
    mp_wnd_ptz = p_ptz;
}

void PicPreviewGroup::clear_group()
{
    QList< PicPreviewGroupInfo >::iterator i;
    for( i=m_list_pic_preview_group.begin(); i!=m_list_pic_preview_group.end(); i++ ) {
       if( i->pic_vi.p_image!=NULL ) {
           //删除image
           delete i->pic_vi.p_image;
       }
       if( i->pic_ir.p_image!=NULL ) {
           delete i->pic_ir.p_image;
       }
       if( i->pic_cloud_pan.p_image!=NULL ) {
           delete i->pic_cloud_pan.p_image;
       }
    }

    m_list_pic_preview_group.clear();
}

void PicPreviewGroup::slot_add_image()
{
    add_new_group();
}

void PicPreviewGroup::init( const QMap<int, DeviceGroup> &map_device )
{
    clear_group();

    QMap<int, DeviceGroup>::const_iterator it = map_device.begin();
    for ( ; it != map_device.end(); ++it ) {
        QString str_app_path = QCoreApplication::applicationDirPath();

        PicPreviewGroupInfo group_info;
        group_info.pic_ir.str_sn = it.value().dev_ir.str_sn;
        if ( !group_info.pic_ir.str_sn.isEmpty() ) {
        group_info.pic_ir.str_path = str_app_path+"/1.bmp";
        group_info.pic_ir.p_image = new QImage( group_info.pic_ir.str_path );
        }

        group_info.pic_vi.str_sn = it.value().dev_vi.str_sn;
        if ( !group_info.pic_vi.str_sn.isEmpty() ) {
            group_info.pic_vi.str_path = str_app_path+"/1.jpg";
            group_info.pic_vi.p_image = new QImage( group_info.pic_vi.str_path );
        }
        group_info.n_group_num = it.key();

        group_info.pic_cloud_pan.str_filename = "云台(空)";
        group_info.str_group_name = "组号：" + QString::number( group_info.n_group_num );

        m_list_pic_preview_group.append( group_info );
    }

    resize_scorll();
    update();
}

int PicPreviewGroup::add_new_group()
{
    int n_size = m_list_pic_preview_group.size();
    PicPreviewGroupInfo pic_preview_group_info;

    pic_preview_group_info.pic_cloud_pan.str_filename = "云台(空)";

    pic_preview_group_info.str_group_name = "别名" + QString::number( n_size + 1 );

    if ( m_list_pic_preview_group.isEmpty() || m_list_pic_preview_group.back().n_group_num >= 0 ) {
        pic_preview_group_info.n_group_num = -1;
    } else {
        pic_preview_group_info.n_group_num = m_list_pic_preview_group.back().n_group_num - 1;
    }
    m_list_pic_preview_group.append( pic_preview_group_info );

    resize_scorll();
    update();
}

void PicPreviewGroup::paintEvent( QPaintEvent *event )
{
    QPainter draw;
    draw.begin( this );

    //修改字体大小
    QFont font;
    font.setPixelSize( 16 );
    draw.setFont( font );
    QFontMetrics fm( font );
    QString str_add = "新增";
    int pixels_width = fm.width( str_add );
    int pixels_height = fm.height();

    int n_height = (width()-SPACE*3)*3/8+SPACE*2+SPACE;

    int n_size = m_list_pic_preview_group.size();
    QList<PicPreviewGroupInfo>::iterator i = m_list_pic_preview_group.begin();
    for( int j=0; i!=m_list_pic_preview_group.end(), j<n_size; i++,j++ ) {

        QRect rct_ir( SPACE, SPACE+n_height*j-mn_scroll_pos, (width()-SPACE*3)/2, (width()-SPACE*3)*3/8 );
        QRect rct_vi( SPACE+(width()-SPACE*3)/2, SPACE+n_height*j-mn_scroll_pos, (width()-SPACE*3)/2, (width()-SPACE*3)*3/8 );

        QRect rct_close_btn( width()-SPACE*3, SPACE+n_height*j-mn_scroll_pos, SPACE, SPACE );
        QRect rct_ptz( SPACE, SPACE+(width()-SPACE*3)*3/8+n_height*j-mn_scroll_pos, (width()-SPACE*3)/2, SPACE*2 );
        QRect rct_group_name( SPACE+(width()-SPACE*3)/2, SPACE+(width()-SPACE*3)*3/8+n_height*j-mn_scroll_pos, (width()-SPACE*3)/2, SPACE*2 );

        //.画红外
        if( i->pic_ir.p_image &&  !i->pic_ir.p_image->isNull() ) {
            draw.drawImage( rct_ir, *(  i->pic_ir.p_image ) );
        } else {
            draw.drawText( rct_ir, Qt::AlignCenter, i->pic_ir.str_sn );
        }
        draw.drawRect( rct_ir );

        //.画可见光
        if( i->pic_vi.p_image && !i->pic_vi.p_image->isNull() ) {
            draw.drawImage( rct_vi, *(  i->pic_vi.p_image ) );
        } else {
            draw.drawText( rct_vi, Qt::AlignCenter, i->pic_vi.str_sn );
        }
        draw.drawImage( rct_close_btn, *mp_delete_image );
        draw.drawRect( rct_vi );

        //.画云台
        QString str_ptz_sn = i->pic_cloud_pan.str_sn;
        QString str_ptz_show = str_ptz_sn.isEmpty()?i->pic_cloud_pan.str_filename:str_ptz_sn;
        if( i->n_group_num == mn_group_number ) {
            draw.fillRect( rct_ptz , QBrush( QColor( 51, 153, 255 ) ));
            draw.setPen( QColor( 187, 255, 255 ) );
            draw.drawText( rct_ptz, Qt::AlignCenter, str_ptz_show );
        } else {
            draw.drawText( rct_ptz, Qt::AlignCenter, str_ptz_show );
        }
        draw.setPen( Qt::black );
        draw.drawRect( rct_ptz );

        //.画组名称
        if( i->n_group_num == mn_group_number ) {
            draw.fillRect( rct_group_name, QBrush( QColor( 51, 153, 255 ) ));
            draw.setPen( QColor( 187, 255, 255 ) );
        }

        draw.drawText( rct_group_name, Qt::AlignCenter, i->str_group_name );
        draw.setPen( Qt::black );
        draw.drawRect( rct_group_name );
    }

    QRect rct_new_group( SPACE, SPACE+n_height*n_size-mn_scroll_pos, width()-SPACE*3, SPACE*4  );
    QRect rct_new_img( SPACE+(width()-SPACE*3)/2-pixels_width-SPACE*2, SPACE*2+n_height*n_size-mn_scroll_pos, SPACE*2, SPACE*2 );

    if( mb_move_add_image == true ) {  //移动时颜色
        draw.fillRect( rct_new_group, QBrush( QColor( 185, 227, 251 ) ) );
    }

    if( mb_press_add_image == true ) {  //点击时颜色
        draw.fillRect( rct_new_group, QBrush( QColor( 140, 202, 235 ) ) );
    }

    draw.drawImage( rct_new_img, *mp_add_image );
    draw.drawText( rct_new_group, Qt::AlignCenter, str_add );
    draw.drawRect( rct_new_group );

    draw.setPen( QColor( 112, 112, 112 ) );
    draw.drawRect( QRect( 0, 0, width()-1, height()-1 ) );

    draw.end();
}

void PicPreviewGroup::resizeEvent( QResizeEvent *event )
{
    mp_v_scrollBar->move( width()-20, 1 );
    mp_v_scrollBar->resize( 20-1, height()-2 );

    resize_scorll();
}

void PicPreviewGroup::set_frame( const QPointF *p_point )
{
    QPoint pt = p_point->toPoint();
    int n_group_num(0);

    if ( pt_in_new_btn_group_num( pt ) ) {
        mb_press_add_image = true;
        update();
        return;
    }

    if ( pt_in_delete_btn_group_num( pt, n_group_num ) ) {
        delete_group( n_group_num );//删除组
    } else if ( pt_in_group_num( pt, n_group_num ) ) {
        mn_group_number = n_group_num;
        update();
    }

    return ;
    int n_size = m_list_pic_preview_group.size();
    QList<PicPreviewGroupInfo>::iterator i;
    int j=0;
    for( i=m_list_pic_preview_group.begin(), j=0; i!=m_list_pic_preview_group.end(), j<n_size; i++, j++ ) {
        if( p_point->x()>width()-SPACE*3 && p_point->x()<=width()-SPACE*2 && \
                p_point->y()>SPACE+((width()-SPACE*3)*3/8+SPACE*2+SPACE)*j-mn_scroll_pos \
                && p_point->y()<= SPACE+((width()-SPACE*3)*3/8+SPACE*2+SPACE)*(j+1)-mn_scroll_pos )
        {
            delete_group( i->n_group_num );//删除组
        } else if( p_point->x()>SPACE && p_point->x()<=width()-SPACE*2 \
                   && p_point->y()>SPACE+((width()-SPACE*3)*3/8+SPACE*2+SPACE)*j-mn_scroll_pos \
                && p_point->y()<= SPACE+((width()-SPACE*3)*3/8+SPACE*2+SPACE)*(j+1)-mn_scroll_pos )
        {
            mn_group_number = i->n_group_num;//选中组状态
            update();
        }
        //ms_image_name = 1;
    }
    //当point在新增窗口内
    if( p_point->x()>SPACE && p_point->x()<=width()-SPACE*2
            && p_point->y()>SPACE+n_size*((width()-SPACE*3)*3/8+SPACE*2+SPACE)-mn_scroll_pos
            && p_point->y()<=SPACE+n_size*((width()-SPACE*3)*3/8+SPACE*2+SPACE)+SPACE*4-mn_scroll_pos )
    {
        mb_press_add_image = true;
    }
}

void PicPreviewGroup::mousePressEvent( QMouseEvent *event )
{
    if ( event->button() == Qt::LeftButton ) {
        QPointF pt_press_local = event->localPos();
        set_frame( &pt_press_local );
    }
}

void PicPreviewGroup::mouseMoveEvent ( QMouseEvent *event )//鼠标移动事件响应
{
    int n_size = m_list_pic_preview_group.size();
    if( event->x()>SPACE && event->x()<=width()-SPACE*2 && \
            event->y()>SPACE+n_size*((width()-SPACE*3)*3/8+SPACE*2+SPACE)-mn_scroll_pos
            && event->y()<=SPACE+n_size*((width()-SPACE*3)*3/8+SPACE*2+SPACE)+SPACE*4-mn_scroll_pos ) {
        mb_move_add_image = true;
    } else {
        mb_move_add_image = false;
    }
    update();
}

void PicPreviewGroup::mouseReleaseEvent ( QMouseEvent * event )//鼠标松开事件响应
{
    if ( event->button() == Qt::LeftButton ) {
        if( mb_press_add_image == true ) {
            add_new_group();
            mb_press_add_image = false;
        }
    }

}

void PicPreviewGroup::delete_group()
{
}

void PicPreviewGroup::delete_group( QString str_group_name )
{
    QList<PicPreviewGroupInfo>::iterator it = m_list_pic_preview_group.begin();

    for( ; it !=m_list_pic_preview_group.end(); ++it ) {
        if( it->str_group_name == str_group_name ) {
            //删除image

            if ( !it->pic_cloud_pan.str_sn.isEmpty() ) {
                mp_wnd_ptz->slot_add_ptz( it->pic_cloud_pan );
            }
            if ( !it->pic_ir.str_sn.isEmpty() ){
                mp_wnd_ir->slot_add_image( it->pic_ir );
            }
            if ( !it->pic_vi.str_sn.isEmpty() ) {
                mp_wnd_vi->slot_add_image( it->pic_vi );
            }

            m_list_pic_preview_group.erase( it );
            break;
        }
    }

    resize_scorll();
    update();
}

void PicPreviewGroup::delete_group( int n_group_num )
{
    QList<PicPreviewGroupInfo>::iterator it = m_list_pic_preview_group.begin();

    for( ; it != m_list_pic_preview_group.end(); ++it ) {
        if( it->n_group_num == n_group_num ) {

            if ( !it->pic_cloud_pan.str_sn.isEmpty() ) {
                mp_wnd_ptz->slot_add_ptz( it->pic_cloud_pan );
            }
            if ( !it->pic_ir.str_sn.isEmpty() ){
                mp_wnd_ir->slot_add_image( it->pic_ir );
            }
            if ( !it->pic_vi.str_sn.isEmpty() ) {
                mp_wnd_vi->slot_add_image( it->pic_vi );
            }

            it = m_list_pic_preview_group.erase( it );
            break;
        }
    }

    resize_scorll();
    update();
}

void PicPreviewGroup::set_v_edit_value( int value )
{
    mn_scroll_pos = mp_v_scrollBar->value();
    update();
}

void PicPreviewGroup::resize_scorll()
{
    int n_size = m_list_pic_preview_group.size();
    if( n_size*( (width()-SPACE*3)*3/8+SPACE*2+SPACE ) +SPACE+SPACE*4+SPACE <=height() ) {
        mp_v_scrollBar->hide();
        mn_scroll_pos =0;
    } else {
        mp_v_scrollBar->setMaximum( n_size*( (width()-SPACE*3)*3/8+SPACE*2+SPACE )-height()+SPACE+SPACE*4+SPACE );
        mp_v_scrollBar->setPageStep( n_size*( (width()-SPACE*3)*3/8+SPACE*2+SPACE )-height()+SPACE+SPACE*4+SPACE );

        mp_v_scrollBar->show();
    }
}

bool PicPreviewGroup::pt_in_delete_btn_group_num( const QPoint &pt, int &n_group_num )
{
    QList<PicPreviewGroupInfo>::iterator it = m_list_pic_preview_group.begin();
    const QPoint *p_point = &pt;

    for( int j=0; it != m_list_pic_preview_group.end(); ++it, ++j ) {
        if( p_point->x()>width()-SPACE*3 && p_point->x()<=width()-SPACE*2 && \
                p_point->y()>SPACE+((width()-SPACE*3)*3/8+SPACE*2+SPACE)*j-mn_scroll_pos \
                && p_point->y()<= SPACE+((width()-SPACE*3)*3/8+SPACE*2+SPACE)*j+20 -mn_scroll_pos )
        {
            n_group_num = it->n_group_num;//删除组
            return true;
        }
    }

    return false;
}

bool PicPreviewGroup::pt_in_new_btn_group_num( const QPoint &pt )
{
    int n_size = m_list_pic_preview_group.size();
    const QPoint *p_point = &pt;

    if( p_point->x()>SPACE && p_point->x()<=width()-SPACE*2
            && p_point->y()>SPACE+n_size*((width()-SPACE*3)*3/8+SPACE*2+SPACE)-mn_scroll_pos
            && p_point->y()<=SPACE+n_size*((width()-SPACE*3)*3/8+SPACE*2+SPACE)+SPACE*4-mn_scroll_pos )
    {
        return true;
    }

    return false;
}

bool PicPreviewGroup::pt_in_group_num( const QPoint &pt, int &n_group_num )
{
    QList<PicPreviewGroupInfo>::iterator it = m_list_pic_preview_group.begin();
    const QPoint *p_point = &pt;

    for( int j=0; it != m_list_pic_preview_group.end(); ++it, ++j ) {

        if( p_point->x()>SPACE && p_point->x()<=width()-SPACE*2 \
                   && p_point->y()>SPACE+((width()-SPACE*3)*3/8+SPACE*2+SPACE)*j-mn_scroll_pos \
                && p_point->y()<= SPACE+((width()-SPACE*3)*3/8+SPACE*2+SPACE)*(j+1)-mn_scroll_pos )
        {
            n_group_num = it->n_group_num;
            return true;
        }
    }

    return false;
}

bool PicPreviewGroup::is_vi_in_group( const int &n_group_num )
{
    return is_device_in_group( VI, n_group_num );
}

bool PicPreviewGroup::is_ir_in_group( const int &n_group_num )
{
    return is_device_in_group( IR, n_group_num );
}

bool PicPreviewGroup::is_ptz_in_group( const int &n_group_num )
{
    return is_device_in_group( PTZ, n_group_num );
}

bool PicPreviewGroup::is_device_in_group( const int &n_class, const int &n_group_num )
{
    QList<PicPreviewGroupInfo>::iterator it = m_list_pic_preview_group.begin();

    for( ; it != m_list_pic_preview_group.end(); ++it ) {
        if ( it->n_group_num == n_group_num  ) {
            switch ( n_class ) {
            case IR:
                return !it->pic_ir.str_sn.isEmpty();
            case VI:
                return !it->pic_vi.str_sn.isEmpty();
            default:
                return !it->pic_cloud_pan.str_sn.isEmpty();
            }
        }
    }

    return false;
}

bool PicPreviewGroup::add_device_in_group( const int &n_class, const int &n_group_num, const PicPreviewInfo &pic_info )
{
    QList<PicPreviewGroupInfo>::iterator it = m_list_pic_preview_group.begin();

    for( ; it != m_list_pic_preview_group.end(); ++it ) {
        if ( it->n_group_num == n_group_num  ) {
            switch ( n_class ) {
            case IR:
            {
                if ( !it->pic_ir.str_sn.isEmpty() ){
                    mp_wnd_ir->slot_add_image( it->pic_ir );
                }
                it->pic_ir = pic_info;
                return true;
            }
            case VI:
            {
                if ( !it->pic_vi.str_sn.isEmpty() ) {
                    mp_wnd_vi->slot_add_image( it->pic_vi );
                }
                it->pic_vi = pic_info;
                return true;
            }
            default:
            {
                if ( !it->pic_cloud_pan.str_sn.isEmpty() ) {
                    mp_wnd_ptz->slot_add_ptz( it->pic_cloud_pan );
                }
                it->pic_cloud_pan = pic_info;
                return true;
            }
            }
        }
    }

    return false;
}

bool PicPreviewGroup::add_vi_in_group( const int &n_group_num, const PicPreviewInfo &pic_info )
{
    if ( !add_device_in_group( VI, n_group_num, pic_info ) ) { return false; }

    update();
    return true;
}

bool PicPreviewGroup::add_ir_in_group( const int &n_group_num, const PicPreviewInfo &pic_info )
{
    if ( !add_device_in_group( IR, n_group_num, pic_info ) ) { return false; }

    update();
    return true;
}

bool PicPreviewGroup::add_ptz_in_group( const int &n_group_num, const PicPreviewInfo &pic_info )
{
    if ( !add_device_in_group( PTZ, n_group_num, pic_info ) ) { return false; }

    update();
    return true;
}
