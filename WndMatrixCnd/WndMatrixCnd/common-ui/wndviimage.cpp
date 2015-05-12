#include "wndviimage.h"
#include <QPainter>

WndViImage::WndViImage(QWidget *parent) :
    QWidget(parent)
    ,ms_image_path( "" )
{
    //m_image = QImage( 1920, 1080, QImage::Format_RGB32 );
}

WndViImage::~WndViImage()
{

}

void WndViImage::paintEvent( QPaintEvent *p_event )
{
    QPainter draw( this );
    QRect rct( 0, 0, width(), height() );

    draw.setPen( QColor( 0,0,0) );//66, 123, 122 ) );

    if( m_image.isNull() ) {
        draw.setPen( Qt::white );
        draw.drawText( rct, Qt::AlignCenter, "没有可见光图片" );
    } else {
        draw.drawImage( rct, m_image );
    }

    draw.setPen( Qt::white );
    draw.drawRect( QRect( 0, 0, width()-1, height()-1 ) );

    draw.end();
}

bool WndViImage::set_image_path( QString str_image_path )
{
    if ( str_image_path.isEmpty() ) {
        m_image = QImage();
    } else {
        m_image = QImage( str_image_path );
    }

    ms_image_path = str_image_path;
    update();
}

