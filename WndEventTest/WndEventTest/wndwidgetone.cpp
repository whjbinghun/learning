#include "wndwidgetone.h"
#include <QPainter>
#include <QDebug>

WndWidgetOne::WndWidgetOne(QWidget *parent) :
    QWidget(parent)
{
}

void WndWidgetOne::paintEvent( QPaintEvent *p_event )
{
    QPainter draw;
    draw.begin( this );
    draw.fillRect(QRect( 0, 0, width(), height() ), QBrush( Qt::red ) );
    draw.end();
}

void WndWidgetOne::resizeEvent( QResizeEvent *p_event )
{

}

void WndWidgetOne::mousePressEvent( QMouseEvent *p_event )
{
    if( p_event->button() == Qt::LeftButton ) {
        qDebug()<<"WndWidgetOne::mousePressEvent"<<endl;
    }
}

