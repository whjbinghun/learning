#include "wndirdisplay.h"
#include <QPainter>
#include "netcontrol.h"
#include "netcapturegroup.h"

WndIRDisplay::WndIRDisplay(QWidget *parent) :
    QFrame(parent)
  , mp_net_control( 0 )
{
}

void WndIRDisplay::paintEvent( QPaintEvent *event )
{
    if ( !mp_net_control ) return;

    NetCaptureGroup *p_group = mp_net_control->get_playback_group();

    QPainter draw;
    draw.begin( this );
    if ( p_group ) {
        NetCaptureVod *p_ir = p_group->get_net_ir();

        if ( p_ir->m_vod_image.isNull() ) {
            draw.fillRect( QRect( 0, 0, width(), height() ), QBrush( QColor( 51, 153, 255 ) ) );
        } else {
            //draw.drawImage( QRect( 0, 0, width(), height() ), p_ir->m_vod_image );
        }
    } else {
        draw.fillRect( QRect( 0, 0, width(), height() ), QBrush( QColor( 51, 153, 255 ) ) );
    }

    draw.end();
}
