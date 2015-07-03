#include "wndimage.h"
#include <QPainter>

WndImage::WndImage(QWidget *parent) :
    QWidget(parent)
    ,mp_btn_normal( NULL )
{
    mp_btn_normal = new QPushButton( this );
    connect( mp_btn_normal, SIGNAL(clicked()), this, SIGNAL( sig_show_normal() ) );
}

WndImage::~WndImage()
{

}

void WndImage::paintEvent( QPaintEvent *event )
{
    QPainter draw( this );
    draw.drawImage( QRect( 0, 0, width(), height() ), QImage( "E:\\WP_20130725_008.jpg"));
    draw.end();
}
