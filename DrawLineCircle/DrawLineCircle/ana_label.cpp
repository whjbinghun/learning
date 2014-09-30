#include "ana_label.h"
#include <QPainter>
#include <QPoint>
#include <QDebug>

Ana_Label::Ana_Label(QWidget *parent) :
    QLabel(parent)
    ,mp_delete_label( NULL )
{
    init_ir_widget();
    me_press_status = none_press_status;
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

}

void Ana_Label::mouseMoveEvent(QMouseEvent *event)
{

}

void Ana_Label::mouseReleaseEvent( QMouseEvent *event )
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
                break;
            case 1:
                me_press_status = line_status;
                break;
            case 2:
                me_press_status = rect_status;
                break;
            case 3:
                me_press_status = circle_status;
                break;
            default:
                me_press_status = none_press_status;
                break;
            }
        }
    }
    //修改完mp_delete_label的状态后，还必须对mp_delete_label进行刷新
    mp_delete_label->set_delete_status( false );
    mp_delete_label->update();

    update();
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
            draw.fillRect( n_width*j, 0, n_width, n_height, QBrush( QColor(166,166,166) ) );
        }

        draw.setPen( Qt::white );
        draw.drawText( n_width*j, 0, n_width, n_height, Qt::AlignCenter, *i );
        draw.setPen( Qt::black );
        draw.drawRect( n_width*j, 0, n_width, n_height-1 );

    }

    draw.end();
}

void Ana_Label::resizeEvent( QResizeEvent *event )
{

}

Ana_Label::enum_press_status Ana_Label::get_mouse_press_status()
{
    return me_press_status;
}

void Ana_Label::set_mouse_press_status( enum_press_status e_press_status )
{
    me_press_status = e_press_status;
}
