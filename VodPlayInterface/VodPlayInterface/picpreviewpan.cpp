#include "picpreviewpan.h"
#include "wndconfig.h"

#include <QBuffer>
#include <QPainter>
#include <QColor>
#include <QRgb>
#include <QDebug>

#define SPACE 20
#define IMAGE_WIDTH 100
//边框与图片间隔
#define FRAME_SPACE 5

PicPreviewPan::PicPreviewPan(QWidget *parent) :
    QFrame(parent)
    ,mn_scroll_pos( 0 )
    ,mp_erase( NULL )
    ,mp_scrollBar( NULL )
    ,ms_image_name("")
    ,mp_wnd_config( NULL )
    ,mn_class(2)
{
    mp_scrollBar = new QScrollBar( Qt::Horizontal, this );
    mp_scrollBar->setMinimum( 0 );
    mp_scrollBar->hide();

    mp_erase = new QPushButton( this );
    mp_add_button = new QPushButton( this );

    mp_erase->hide();

    connect( mp_scrollBar, SIGNAL( valueChanged( int ) ), this, SLOT( set_line_edit_value( int ) ) );
    connect( mp_erase, SIGNAL( clicked() ), this, SLOT( delete_image() ) );
    connect( mp_add_button, SIGNAL( clicked() ), this, SLOT( slot_add_image() ) );
}

PicPreviewPan::~PicPreviewPan()
{
    clear_ptz();

    delete mp_erase;
    delete mp_add_button;
    delete mp_scrollBar;
}

void PicPreviewPan::clear_ptz()
{
    QMap< QString, PicPreviewInfo >::iterator it = m_map_pic_pan.begin();
    for( ; it != m_map_pic_pan.end(); ++it ) {
       if( it.value().p_image!=NULL ) {
           delete it.value().p_image;
       }
    }

    m_map_pic_pan.clear();
}

void PicPreviewPan::slot_add_image()
{
    add_ptz( "云台 1 号");
}

void PicPreviewPan::slot_add_ptz( const PicPreviewInfo &pic_info )
{
    QMap< QString, PicPreviewInfo >::iterator it_find = m_map_pic_pan.find( pic_info.str_sn );
    if ( it_find != m_map_pic_pan.end() ) { return; }

    m_map_pic_pan.insert( pic_info.str_sn, pic_info );

    resize_scorll();
}

bool PicPreviewPan::add_ptz( QString str_sn )
{
    if ( str_sn.isEmpty() ) return false;

    QMap< QString, PicPreviewInfo >::iterator it_find = m_map_pic_pan.find( str_sn );
    if ( it_find != m_map_pic_pan.end() ) { return false; }

    PicPreviewInfo ptz_info;
    ptz_info.p_image = 0;
    ptz_info.str_path = "";
    ptz_info.str_filename = "";
    ptz_info.str_sn = str_sn;

    m_map_pic_pan.insert( str_sn, ptz_info );

    resize_scorll();
}

void PicPreviewPan::resize_scorll()
{
    int n_size = m_map_pic_pan.size();
    int n_len_ptz = n_size*( ( height()-SPACE*3)*4/3+SPACE ) + SPACE;
    if( n_len_ptz <=width() ) {
        mp_scrollBar->hide();
        mn_scroll_pos =0;
    } else {
        mp_scrollBar->setMaximum( n_len_ptz - width() );
        mp_scrollBar->setPageStep( n_len_ptz - width() );
        mp_scrollBar->show();
    }
}

void PicPreviewPan::paintEvent( QPaintEvent *event )
{
    QPainter draw;
    draw.begin( this );

    QFont font;
    font.setPixelSize(25);
    draw.setFont( font );

    QMap< QString, PicPreviewInfo >::iterator it = m_map_pic_pan.begin();

    for( int j=0; it != m_map_pic_pan.end(); ++it, ++j ) {
        QRect rct_ptz = QRect( ( ( height()-SPACE*3)*4/3+SPACE )*j+SPACE-FRAME_SPACE-mn_scroll_pos, SPACE-FRAME_SPACE, ( height()-SPACE*3)*4/3+FRAME_SPACE*2, height()-SPACE*2 );
        QString str_sn = it.key();

        if( ms_image_name == str_sn ) {
            draw.setPen( QColor( 125, 162, 206 ) );
            draw.fillRect(  rct_ptz, QBrush( QColor( 193, 220, 252 ) ));
            draw.setPen( Qt::black );
        }

        draw.setPen( QColor( 125, 162, 206 ) );
        draw.drawRect( rct_ptz );
        draw.setPen( Qt::black );
        draw.drawText( rct_ptz, Qt::AlignCenter, str_sn );
    }

    draw.setPen( QColor( 112, 112, 112 ) );
    draw.drawRect( QRect( 0, 0, width()-1, height()-1 ) );

    draw.end();
}

void PicPreviewPan::resizeEvent( QResizeEvent *event )
{
    mp_scrollBar->move( 1, height()-21 );
    mp_scrollBar->resize( width()-2, 20 );

    mp_erase->move( width()-60, 0 );
    mp_erase->resize( 20, 20 );

    mp_add_button->move( width()-20, 0 );
    mp_add_button->resize( 20, 20 );

    resize_scorll();
}

void PicPreviewPan::set_line_edit_value( int value )
{
    mn_scroll_pos = mp_scrollBar->value();

    update();
}

bool PicPreviewPan::pt_in_image_rect( const QPointF *p_point, PicPreviewInfo &pic_info )
{
    QMap< QString, PicPreviewInfo >::iterator it = m_map_pic_pan.begin();

    for( int j=0; it != m_map_pic_pan.end(); ++it, ++j ) {

        if( ( p_point->x()>( ( height()-SPACE*3)*4/3+SPACE )*j+SPACE-mn_scroll_pos
              && p_point->x()<=( ( height()-SPACE*3)*4/3+SPACE )*j+SPACE-mn_scroll_pos +(height()-SPACE*3)*4/3 ) \
                &&  ( p_point->y()>SPACE && p_point->y()<= SPACE+height()-SPACE*3 ) )
        {
            pic_info = it.value();
            ms_image_name = it.key();
            update();
            return true;
        }
    }

    return false;
}

void PicPreviewPan::mousePressEvent( QMouseEvent *event )
{
    if ( event->button() == Qt::LeftButton ) {
        QPointF pt_press_local = event->localPos();
        PicPreviewInfo pic_info;

        if ( pt_in_image_rect( &pt_press_local, pic_info ) && mp_wnd_config ) {
            QPoint pt = mapTo( mp_wnd_config, pt_press_local.toPoint() );
            mp_wnd_config->mouse_press( pt, pic_info, mn_class );
        }
    }
}

void PicPreviewPan::mouseReleaseEvent(  QMouseEvent *event  )
{
    if ( mp_wnd_config ) {
        QPoint pt = mapTo( mp_wnd_config, event->localPos().toPoint() );
        mp_wnd_config->mouse_release( pt, mn_class );
    }
}

bool PicPreviewPan::delete_ptz( QString str_sn )
{
    QMap< QString, PicPreviewInfo >::iterator it = m_map_pic_pan.find( str_sn );
    if ( it == m_map_pic_pan.end() ) { return false; }
    m_map_pic_pan.erase( it );

    resize_scorll();
    return true;
}
