#include "wndwidgettwo.h"
#include <QPainter>
#include <QDebug>
#include "wndeventtest.h"

WndWidgetTwo::WndWidgetTwo(QWidget *parent) :
    QWidget(parent)
    ,mp_wnd_event( NULL )
{
    //setAttribute( Qt::WA_TransparentForMouseEvents );
}

void WndWidgetTwo::paintEvent( QPaintEvent *p_event )
{
    QPainter draw;
    draw.begin( this );
    draw.fillRect(QRect( 0, 0, width(), height() ), QBrush( Qt::red ) );
    draw.end();
}

void WndWidgetTwo::mousePressEvent( QMouseEvent *p_event )
{
    if( p_event->button() == Qt::LeftButton ) {
        qDebug()<<"WndWidgetTwo::mousePressEvent"<<endl;
    }
    QWidget::mousePressEvent( p_event );
}

/*bool WndWidgetTwo::event( QEvent *p_event )
{
    if ( p_event->type()==QEvent::MouseButtonPress ) {
        QMouseEvent *p_mouse_event = (QMouseEvent *)p_event;
        if ( p_mouse_event && p_mouse_event->button() == Qt::LeftButton ) {
            if( mp_wnd_event!=NULL ) {
                //setAttribute( Qt::WA_TransparentForMouseEvents );

            }
        }
    }
    qDebug()<<"WndWidgetTwo::event"<<b_true<<endl;

    return QWidget::event( p_event );

}*/

void WndWidgetTwo::init_ctrl( WndEventTest *p_wnd_event )
{
    mp_wnd_event = p_wnd_event;
}
