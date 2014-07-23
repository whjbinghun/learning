#include "wndpreview.h"
#include <QPainter>
#include "netcontrol.h"

WndPreview::WndPreview(QWidget *parent) :
    QFrame(parent)
  , mn_group_number( -1 )
  , mp_net_control( 0 )
{
}

void WndPreview::mousePressEvent( QMouseEvent *event )
{

}

void WndPreview::mouseMoveEvent( QMouseEvent *event )
{

}

void WndPreview::mouseReleaseEvent( QMouseEvent *event )
{

}

void WndPreview::mouseDoubleClickEvent( QMouseEvent *event )
{

}

void WndPreview::resizeEvent( QResizeEvent *event )
{

}

void WndPreview::paintEvent( QPaintEvent *event )
{
    NetCaptureGroup *p_capture_group = mp_net_control->get_group_connect( mn_group_number );

    DeviceGroup dev_group;
    bool b_group = mp_net_control->get_group_info( mn_group_number, dev_group );

    QPainter draw;
    draw.begin( this );

    NetCaptureVod *p_capture_ir(NULL);
    NetCaptureVod *p_capture_vi(NULL);

    if ( p_capture_group ) {
        p_capture_ir = p_capture_group->get_net_ir();
        p_capture_vi = p_capture_group->get_net_vi();
    }

    QRect rct_ir( 0, 0, width()/2, height() );
    QRect rct_vi( width()/2, 0, width()/2, height() );

    if( p_capture_ir ) {
        if( p_capture_ir->m_vod_image.isNull() ) {
            draw.fillRect( rct_ir, QBrush( QColor( 200, 200, 200 ) ) );
            draw.drawText( rct_ir, Qt::AlignCenter, "未正确解码" );
        } else {
            draw.drawImage( QRect( 0, 0, width()/2, height() ), p_capture_ir->m_vod_image );
        }
    } else {
        draw.fillRect( rct_ir, QBrush( QColor( 200, 200, 200 ) ) );
        if ( b_group && !dev_group.dev_ir.str_sn.isEmpty() ) {
            draw.drawText( rct_ir, Qt::AlignCenter, "正在连接红外" );
        } else {
            draw.drawText( rct_ir, Qt::AlignCenter, "未配置红外" );
        }
    }
    draw.drawRect( rct_ir );

    if( p_capture_vi ) {
        if( p_capture_vi->m_vod_image.isNull() ) {
            draw.fillRect( rct_vi, QBrush( QColor( 200, 200, 200 ) ) );
            draw.drawText( rct_vi, Qt::AlignCenter, "未正确解码" );
        } else {
            draw.drawImage( rct_vi, p_capture_vi->m_vod_image );
        }
    } else {
        draw.fillRect( rct_vi, QBrush( QColor( 200, 200, 200 ) ) );
        if ( b_group && !dev_group.dev_vi.str_sn.isEmpty() ) {
            draw.drawText( rct_vi, Qt::AlignCenter, "正在连接可见光" );
        } else {
            draw.drawText( rct_vi, Qt::AlignCenter, "未配置可见光" );
        }
    }
    draw.drawRect( rct_vi );

    draw.end();
}
