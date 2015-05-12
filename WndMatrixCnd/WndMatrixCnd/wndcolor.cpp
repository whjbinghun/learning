#include "wndcolor.h"
#include <QPainter>

WndColor::WndColor(QWidget *parent) :
    QWidget(parent)
  , mn_row( -1 )
  , m_clr( Qt::white )
{

}

WndColor::~WndColor()
{

}

void WndColor::set_color( QColor clr )
{
    m_clr = clr;
}

QColor WndColor::get_color()
{
    return m_clr;
}

void WndColor::set_row( int n_row )
{
    mn_row = n_row;
}

int WndColor::get_row()
{
    return mn_row;
}

void WndColor::paintEvent(QPaintEvent *event)
{
    QPainter draw;
    draw.begin( this );
    draw.fillRect( QRect( 0, 0, width(), height() ), QBrush( m_clr ) );
    draw.end();
}

void WndColor::mousePressEvent( QMouseEvent *event )
{
    if( event->button() == Qt::LeftButton ) {
        emit sig_color_dialog();
    }
}
