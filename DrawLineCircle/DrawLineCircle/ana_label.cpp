#include "ana_label.h"
#include <QPainter>
#include <QPoint>
#include <QDebug>

Ana_Label::Ana_Label(QWidget *parent) :
    QLabel(parent)
{
    init_ir_widget();
}

Ana_Label::~Ana_Label()
{

}
void Ana_Label::init_ir_widget() {
    m_list.append( "点" );
    m_list.append( "线" );
    m_list.append( "框" );
    m_list.append( "圆" );
}
void Ana_Label::mousePressEvent( QMouseEvent *event )
{
    if ( event->button() == Qt::LeftButton ) {
        press_status_shape( QPoint( event->x(), event->y() ) );
    }
}


void Ana_Label::press_status_shape( QPoint pt ) {
    //如果鼠标点击 点 线位置   ，mb_ana_status =true
    int n_width = width()/4;
    int i = 0;
    for( ; i<4; i++ ) {
        if( pt.x()>n_width*i && pt.x()<=n_width*(i+1) \
                && pt.y()>0&& pt.y()<=height() ) {
            switch ( i ) {
            case 0:
                me_press_status = point_status;
                update();
                break;
            case 1:
                me_press_status = line_status;
                update();
                break;
            case 2:
                me_press_status = rect_status;
                update();
                break;
            case 3:
                me_press_status = circle_status;
                update();
                break;
            default:
                me_press_status = none_press_status;
                update();
                break;
            }
        }
    }
}

void Ana_Label::paintEvent( QPaintEvent *event )
{
    QPainter draw;
    draw.begin( this );

    int n_size = m_list.size();
    QList<QString>::iterator i;
    int j = 0;
    int n_width = width()/4;
    int n_height = height();
    for( i=m_list.begin(),j=0; i!=m_list.end(); i++,j++ ) {
        if( j == 0 && me_press_status == point_status ) {
            draw.fillRect( n_width*j, 0, n_width, n_height, QBrush( QColor(177,1,1) ) );
        } else if( j== 1 && me_press_status == line_status ) {
            draw.fillRect( n_width*j, 0, n_width, n_height, QBrush( QColor(177,1,1) ) );
        } else if( j== 2 && me_press_status == rect_status ) {
            draw.fillRect( n_width*j, 0, n_width, n_height, QBrush( QColor(177,1,1) ) );
        } else if( j== 3 && me_press_status == circle_status ) {
            draw.fillRect( +n_width*j, 0, n_width, n_height, QBrush( QColor(177,1,1) ) );
        } else {
            draw.fillRect( n_width*j, 0, n_width, n_height, QBrush( QColor(125,125,125) ) );
        }

        draw.setPen( Qt::white );
        draw.drawText( n_width*j, 0, n_width, n_height, Qt::AlignCenter, *i );
        draw.setPen( Qt::black );
        draw.drawRect( n_width*j, 0, n_width, n_height );

    }

    draw.end();
}

Ana_Label::enum_press_status Ana_Label::get_mouse_press_status()
{
    return me_press_status;
}
