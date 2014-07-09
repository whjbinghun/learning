#include "irframe.h"
#include <QPainter>
#include <QGraphicsBlurEffect>
#include <QTimer>
#include <QtMath>
#include <QDebug>
#include <windef.h>

#define LABEL_WIDTH 161
#define LABEL_HEIGHT 40
#define SPACE_IR  20
#define REGIONAL_SCOPE 5

static int sign_id = 0;

IrFrame::IrFrame(QWidget *parent) :
    QFrame(parent)
    ,mp_transparent_ir_label( NULL )
    ,mp_ir_slider( NULL )
    ,mn_ir_slider_value( 0 )
    ,mn_sign_id( -1 )
    ,mb_move_type( false )
    ,mb_change_size( false )
    ,ms_type_name( "" )
    ,me_draw_status( none_draw_status )
{
    init_ir_widget();
    me_mouse_press_status = (Ana_Label::enum_press_status)mp_transparent_ir_label->get_mouse_press_status();
    setMouseTracking( true );
    set_cursor_pos();
    set_org_sz( 320, 240 );
}

IrFrame::~IrFrame()
{

}

void IrFrame::init_ir_widget() {
    mp_transparent_ir_label = new Ana_Label( this );
    QGraphicsOpacityEffect *effect = new QGraphicsOpacityEffect( this );
    effect->setOpacity( 0.8 );
    mp_transparent_ir_label->setGraphicsEffect( effect );
    //mp_transparent_ir_label->hide();
}

void IrFrame::set_ir_slider( IrSlider *p_ir_slider )
{
    mp_ir_slider = p_ir_slider;
    //设置QSlider的范围
    mp_ir_slider->setRange( 0, 100 );
    //在拖动条上按一下改变的值
    //mp_slider_play->setPageStep( 10 );
    //设置显示刻度的位置
    //mp_ir_slider->setTickPosition( QSlider::TicksRight );
    //设置刻度间隔
    mp_ir_slider->setTickInterval( 1 );
    //设置滑动条控件的值
    mp_ir_slider->setValue( 100 );
    connect( mp_ir_slider, SIGNAL( signal_value_change(int) ), this, SLOT( slot_value_change(int) ) );
    QTimer *timer = new QTimer( this );
    //开始运行定时器，定时时间间隔为1000ms
    connect( timer, SIGNAL( timeout() ), this, SLOT( slot_slider_value() ) );
    timer->start( 50 );
    //将定时器超时信号与槽(功能函数)联系起来

}

void IrFrame::slot_slider_value()
{
    static int n = 0;
    if( mp_ir_slider->get_value_status() ) {//按下
        n = mn_ir_slider_value;
        mp_ir_slider->setValue( n );
    } else {
        n = ++n>400?0:n;
        mp_ir_slider->setValue( n );
    }
}

void IrFrame::slot_value_change( int n_pos )
{
    mn_ir_slider_value = n_pos;
}

void IrFrame::paintEvent( QPaintEvent *event )
{
    QPainter draw;
    draw.begin( this );
    draw.fillRect( 1, 1, width()-2, height()-2, QBrush( QColor( 125,125,125 ) ) );
    int n_point_area = 10;
    int n_other_area = 2;
    int n_frame_size = 4;//小框的大小

    //新增
    switch ( me_draw_status ) {
    case draw_point_status:
        draw.drawPoint( m_pt_middle.x(), m_pt_middle.y() );
        draw.drawLine( QPoint(m_pt_middle.x()-10,m_pt_middle.y()), QPoint(m_pt_middle.x()+10,m_pt_middle.y()) );
        draw.drawLine( QPoint(m_pt_middle.x(), m_pt_middle.y()-10), QPoint(m_pt_middle.x(),m_pt_middle.y()+10) );
        update();
        break;
    case draw_line_status:
        draw.drawLine( m_pt_start, m_pt_middle );

        draw.drawRect( m_pt_start.x()-2, m_pt_start.y()-2, 4, 4 );
        draw.drawRect( m_pt_middle.x()-2, m_pt_middle.y()-2, 4, 4 );
        break;
    case draw_rect_status:
        draw.drawRect( m_pt_start.x(), m_pt_start.y(), m_pt_middle.x()-m_pt_start.x(), m_pt_middle.y()-m_pt_start.y() );
        break;
    case draw_circle_status:    
        int n_x = m_pt_middle.x()-m_pt_start.x();
        int n_y = m_pt_middle.y()-m_pt_start.y();
        int n_r = (int)qSqrt( n_x*n_x + n_y*n_y );
        draw.drawEllipse( m_pt_start.x()-n_r, m_pt_start.y()-n_r, n_r*2, n_r*2 );
        break;
    }

    int n_type_size = m_list_anainfo.size();
    QList<AnaInfo>::iterator it;
    for( it=m_list_anainfo.begin(); it!=m_list_anainfo.end(); it++ ) {
        //按当前窗口的大小比例缩放
        int n_pt_start_x = (*it).point_start.x()*mf_sz_width;
        int n_pt_start_y = (*it).point_start.y()*mf_sz_height;
        int n_pt_end_x = (*it).point_end.x()*mf_sz_width;
        int n_pt_end_y = (*it).point_end.y()*mf_sz_height;
        QPoint pt_start = QPoint( n_pt_start_x, n_pt_start_y );
        QPoint pt_end = QPoint( n_pt_end_x, n_pt_end_y );
        //判断当前窗口的边界
        judge_area( pt_start );
        judge_area( pt_end );
        n_pt_start_x = pt_start.x();
        n_pt_start_y = pt_start.y();
        n_pt_end_x = pt_end.x();
        n_pt_end_y = pt_end.y();

        if( it->type_name == "point" ) {
            draw.drawPoint( n_pt_end_x, n_pt_end_y );
            draw.drawLine( QPoint(n_pt_end_x-n_point_area,n_pt_end_y), QPoint(n_pt_end_x+n_point_area,n_pt_end_y) );
            draw.drawLine( QPoint(n_pt_end_x, n_pt_end_y-n_point_area), QPoint(n_pt_end_x,n_pt_end_y+n_point_area) );
        } else if( it->type_name == "line" ) {
            draw.drawLine( pt_start, pt_end );
            if( me_ana_move!=move_null && (*it).n_sign_id == mn_sign_id ) {
                draw.drawRect( n_pt_start_x-n_other_area,n_pt_start_y-n_other_area, n_frame_size, n_frame_size );
                draw.drawRect( n_pt_end_x-n_other_area,n_pt_end_y-n_other_area, n_frame_size, n_frame_size );
            }
        } else if( it->type_name == "rect" ) {
            draw.drawRect( QRect( n_pt_start_x, n_pt_start_y, n_pt_end_x-n_pt_start_x, n_pt_end_y-n_pt_start_y ));
            if( me_ana_move!=move_null && (*it).n_sign_id == mn_sign_id ) {
                draw.drawRect( n_pt_start_x-n_other_area, n_pt_start_y-n_other_area, n_frame_size, n_frame_size );
                draw.drawRect( n_pt_end_x-n_other_area, n_pt_end_y-n_other_area, n_frame_size, n_frame_size );
                draw.drawRect( n_pt_start_x-n_other_area, n_pt_end_y-n_other_area, n_frame_size, n_frame_size );
                draw.drawRect( n_pt_end_x-n_other_area, n_pt_start_y-n_other_area, n_frame_size, n_frame_size );
            }
        } else if( it->type_name == "circle" ) {
            int n_x = n_pt_end_x-n_pt_start_x;
            int n_y = n_pt_end_y-n_pt_start_y;
            int n_r = (int)qSqrt( n_x*n_x + n_y*n_y );
            draw.drawEllipse( n_pt_start_x-n_r, n_pt_start_y-n_r, n_r*2, n_r*2 );
            if( me_ana_move!=move_null && (*it).n_sign_id == mn_sign_id ) {
                draw.drawRect( n_pt_start_x-n_other_area-n_r, n_pt_start_y-n_other_area, n_frame_size, n_frame_size );
                draw.drawRect( n_pt_start_x-n_other_area,n_pt_start_y-n_other_area-n_r, n_frame_size, n_frame_size );
                draw.drawRect( n_pt_start_x-n_other_area+n_r, n_pt_start_y-n_other_area, n_frame_size, n_frame_size );
                draw.drawRect( n_pt_start_x-n_other_area, n_pt_start_y-n_other_area+n_r, n_frame_size, n_frame_size );
            }
        }
    }

    draw.setPen( Qt::red );
    if( ms_type_name == "point" ) {
        draw.drawPoint( m_pt_move_end.x(), m_pt_move_end.y() );
        draw.drawLine( QPoint(m_pt_move_end.x()-n_point_area,m_pt_move_end.y()), QPoint(m_pt_move_end.x()+n_point_area,m_pt_move_end.y()) );
        draw.drawLine( QPoint(m_pt_move_end.x(), m_pt_move_end.y()-n_point_area), QPoint(m_pt_move_end.x(), m_pt_move_end.y()+n_point_area) );
        if( me_ana_move!=move_null ) {
            draw.drawRect( m_pt_move_end.x()-n_other_area, m_pt_move_end.y()-n_other_area, 4, 4 );
        }
    } else if( ms_type_name == "line" ) {
        if( me_ana_move == move_self ) {
            draw.drawLine( m_pt_move_start, m_pt_move_end );
            draw.drawRect( m_pt_move_start.x()-n_other_area, m_pt_move_start.y()-n_other_area, 4, 4 );
            draw.drawRect( m_pt_move_end.x()-n_other_area, m_pt_move_end.y()-n_other_area, 4, 4 );
        } else if( me_ana_move == move_left ) {
            draw.drawLine( m_pt_move_start, m_pt_middle );
            draw.drawRect( m_pt_move_start.x()-n_other_area, m_pt_move_start.y()-n_other_area, 4, 4 );
            draw.drawRect( m_pt_middle.x()-n_other_area, m_pt_middle.y()-n_other_area, 4, 4 );
        } else if( me_ana_move == move_right ) {
            draw.drawLine( m_pt_start, m_pt_move_end );
            draw.drawRect( m_pt_start.x()-n_other_area, m_pt_start.y()-n_other_area, 4, 4 );
            draw.drawRect( m_pt_move_end.x()-n_other_area, m_pt_move_end.y()-n_other_area, 4, 4 );
        }
    } else if( ms_type_name == "rect" ) {
        if( me_ana_move == move_left ) {
            draw.drawRect( QRect( m_pt_move_start.x(), m_pt_start.y(), m_pt_middle.x()-m_pt_move_start.x(), m_pt_middle.y()-m_pt_start.y() ));
        } else if( me_ana_move == move_lefttop ) {
            draw.drawRect( QRect( m_pt_move_start.x(), m_pt_move_start.y(), m_pt_middle.x()-m_pt_move_start.x(), m_pt_middle.y()-m_pt_move_start.y() ));
        } else if( me_ana_move == move_top ) {
            draw.drawRect( QRect( m_pt_start.x(), m_pt_move_start.y(), m_pt_middle.x()-m_pt_start.x(), m_pt_middle.y()-m_pt_move_start.y() ));
        } else if( me_ana_move == move_righttop ) {  //???
            draw.drawRect( QRect( m_pt_start.x(), m_pt_move_start.y(), m_pt_move_end.x()-m_pt_start.x(), m_pt_middle.y()-m_pt_move_start.y() ));
        } else if( me_ana_move == move_right ) {
            draw.drawRect( QRect( m_pt_start.x(), m_pt_start.y(), m_pt_move_end.x()-m_pt_start.x(), m_pt_middle.y()-m_pt_start.y() ));
        } else if( me_ana_move == move_rightbottom ) {
            draw.drawRect( QRect( m_pt_start.x(), m_pt_start.y(), m_pt_move_end.x()-m_pt_start.x(), m_pt_move_end.y()-m_pt_start.y() ));
        } else if( me_ana_move == move_bottom ) {
            draw.drawRect( QRect( m_pt_start.x(), m_pt_start.y(), m_pt_middle.x()-m_pt_start.x(), m_pt_move_end.y()-m_pt_start.y() ));
        } else if( me_ana_move == move_leftbottom ) {
            draw.drawRect( QRect( m_pt_move_start.x(), m_pt_start.y(), m_pt_middle.x()-m_pt_move_start.x(), m_pt_move_end.y()-m_pt_start.y() ));
        } else if( me_ana_move == move_self ) {
            draw.drawRect( QRect( m_pt_move_start.x(), m_pt_move_start.y(), m_pt_move_end.x()-m_pt_move_start.x(), m_pt_move_end.y()-m_pt_move_start.y() ) );
            draw.drawRect( m_pt_move_start.x()-n_other_area, m_pt_move_start.y()-n_other_area, 4, 4 );
            draw.drawRect( m_pt_move_end.x()-n_other_area, m_pt_move_end.y()-n_other_area, 4, 4 );
            draw.drawRect( m_pt_move_start.x()-n_other_area, m_pt_move_end.y()-n_other_area, 4, 4 );
            draw.drawRect( m_pt_move_end.x()-n_other_area, m_pt_move_start.y()-n_other_area, 4, 4 );
        }
    } else if( ms_type_name == "circle" ) {
        int n_point_x;
        int n_point_y;
        int n_r;
        if( me_ana_move == move_lefttop || me_ana_move == move_righttop ||
                me_ana_move == move_rightbottom || me_ana_move == move_leftbottom ) {
            n_point_x = m_pt_move_end.x()-m_pt_start.x();
            n_point_y = m_pt_move_end.y()-m_pt_start.y();
            n_r = (int)qSqrt( n_point_x*n_point_x + n_point_y*n_point_y );
            draw.drawEllipse( m_pt_start.x()-n_r, m_pt_start.y()-n_r, n_r*2, n_r*2 );
            draw.drawLine( m_press_pt, m_pt_start );
            draw.drawLine( m_pt_move_end, m_pt_start );
            if( me_ana_move!=move_null ) {
                draw.drawRect( m_pt_start.x()-2-n_r, m_pt_start.y()-2, 4, 4 );
                draw.drawRect( m_pt_start.x()-2, m_pt_start.y()-2-n_r, 4, 4 );
                draw.drawRect( m_pt_start.x()-2+n_r, m_pt_start.y()-2, 4, 4 );
                draw.drawRect( m_pt_start.x()-2, m_pt_start.y()-2+n_r, 4, 4 );
            }
        } else if( me_ana_move == move_self ) {
            n_point_x = m_pt_move_end.x()-m_pt_move_start.x();
            n_point_y = m_pt_move_end.y()-m_pt_move_start.y();
            n_r = (int)qSqrt( n_point_x*n_point_x + n_point_y*n_point_y );
            draw.drawEllipse( m_pt_move_start.x()-n_r, m_pt_move_start.y()-n_r, n_r*2, n_r*2 );
            if( me_ana_move!=move_null ) {
                draw.drawRect( m_pt_move_start.x()-2-n_r, m_pt_move_start.y()-2, 4, 4 );
                draw.drawRect( m_pt_move_start.x()-2, m_pt_move_start.y()-2-n_r, 4, 4 );
                draw.drawRect( m_pt_move_start.x()-2+n_r, m_pt_move_start.y()-2, 4, 4 );
                draw.drawRect( m_pt_move_start.x()-2, m_pt_move_start.y()-2+n_r, 4, 4 );
            }
        }
    }


    draw.end();
}

void IrFrame::resizeEvent( QResizeEvent *event )
{
    //setStyleSheet( "border:1px solid black" );

    mp_transparent_ir_label->move( SPACE_IR, SPACE_IR );
    mp_transparent_ir_label->resize( LABEL_WIDTH, LABEL_HEIGHT );
    mf_sz_width = width()/(float)mn_org_width;
    mf_sz_height = height()/(float)mn_org_height;
}

void IrFrame::mousePressEvent( QMouseEvent *event )
{
    if ( event->button() == Qt::LeftButton ) {
        me_mouse_press_status = (Ana_Label::enum_press_status)mp_transparent_ir_label->get_mouse_press_status();

        if ( me_mouse_press_status != Ana_Label::none_press_status ) {
            draw_add_shape( QPoint( event->x(), event->y() ) );
        } else {
            AnaInfo p_ana;
            bool b_in_ana = pt_in_ana( QPoint( event->x(), event->y()), me_ana_move, p_ana );
            if ( b_in_ana ) {
                ms_type_name = p_ana.type_name;
                if( p_ana.type_name == "point" ) {
                    get_pt_ana_shape( p_ana, QPoint( event->x(), event->y() ) );
                } else if( p_ana.type_name == "line" ) {
                    get_pt_ana_shape( p_ana, QPoint( event->x(), event->y() ) );
                } else if( p_ana.type_name == "rect" ) {
                    get_pt_ana_shape( p_ana, QPoint( event->x(), event->y() ) );
                } else if( p_ana.type_name == "circle" ) {
                    get_pt_ana_shape( p_ana, QPoint( event->x(), event->y() ) );
                }

                update();
            }
            //me_mouse_press_status = (Ana_Label::enum_press_status)mp_transparent_ir_label->get_mouse_press_status();
        }
    }
}

void IrFrame::mouseMoveEvent( QMouseEvent *event )
{
    QPoint pt_cursor = QPoint( event->x(), event->y() );

    switch ( me_draw_status ) {
    case draw_point_status:
        pt_judge_area( QPoint( event->x(), event->y() ), m_pt_middle );
        update();
        break;
    case draw_line_status:
        pt_judge_area( QPoint( event->x(), event->y() ), m_pt_middle );
        update();
        break;
    case draw_rect_status:
        pt_judge_area( QPoint( event->x(), event->y() ), m_pt_middle );
        update();
        break;
    case draw_circle_status:
        QPoint pt = QPoint( event->x(), event->y() );
        int n_x = pt.x()-m_pt_start.x();
        int n_y = pt.y()-m_pt_start.y();
        int n_r = qSqrt( n_x*n_x+n_y*n_y );
        int n_r_temp=0;
        start_area( m_pt_start, n_r_temp );
        if( n_r <= n_r_temp ) {

        m_pt_middle = QPoint( m_pt_start.x()+n_r, m_pt_start.y() );
        } else {
            n_r = n_r_temp;
            m_pt_middle = QPoint( m_pt_start.x()+n_r, m_pt_start.y() );
        }
        update();
        break;
    }

    if( me_draw_status == none_draw_status ) {
        if( ms_type_name == "point" ) {
            pt_move_ana_point( QPoint( event->x(), event->y() ) );
        } else if( ms_type_name == "line" ) {
            pt_move_ana_line( QPoint( event->x(), event->y() ) );
        } else if( ms_type_name == "rect" ) {
            pt_move_ana_rect( QPoint( event->x(), event->y() ) );
        } else if( ms_type_name == "circle" ) {
            pt_move_ana_circle( QPoint( event->x(), event->y() ) );
        }
    }
}

void IrFrame::normalized_pt( QPoint &pt1, QPoint &pt2 )
{
    QRect rct( pt1, pt2 );
    rct = rct.normalized();//得出对称点，topleft点和bottomRight
    pt1 = rct.topLeft();
    pt2 = rct.bottomRight();
}

void IrFrame::mouseReleaseEvent( QMouseEvent *event )
{
    if ( event->button() == Qt::LeftButton ) {
        if( me_draw_status != none_draw_status ) {
            shape_append_list( QPoint( event->x(), event->y() ) );
            update();
        } else {
            shape_move_or_change_size( QPoint( event->x(), event->y() ) );
            update();

        }
    }
}

void IrFrame::draw_add_shape( QPoint pt )
{
    pt_judge_area( pt, m_pt_start );
    m_pt_middle = pt;
    me_draw_status = (enum_draw_status)me_mouse_press_status;
}

bool IrFrame::pt_in_ana( const QPoint &pt, AnaMove &ana_move, AnaInfo &ana_info )
{
    QList<AnaInfo>::iterator it = m_list_anainfo.begin();
    for( ; it != m_list_anainfo.end(); ++it ) {
        if( it->type_name == "point" ) {
            ana_move = pt_in_ana_point( pt, *it );
            if( ana_move != move_null ) {
                break;
            }
        } else if ( it->type_name == "line" ) {
           ana_move = pt_in_ana_line( pt, *it );
           if( ana_move != move_null ) {
             break;
           }

        } else if ( it->type_name == "rect" ) {
            ana_move = pt_in_ana_rect( pt, *it );
            if( ana_move != move_null ) {
                break;
            }
        } else if ( it->type_name == "circle" ) {
            ana_move = pt_in_ana_circle( pt, *it );
            if( ana_move != move_null ) {
                break;
            }
        }
    }

    if ( ana_move == move_null ) return false;
    if ( it == m_list_anainfo.end() ) return false;
    //如果it== m_list_anainfo.end()，那么就不能用下面的语句进行赋值，否则会报错
    ana_info.n_sign_id = it->n_sign_id;
    ana_info.type_name = it->type_name;
    ana_info.point_end = it->point_end;
    ana_info.point_start = it->point_start;
    return true;
}

//对点的分析
AnaMove IrFrame::pt_in_ana_point(const QPoint &pt, const AnaInfo &ana_info)
{
    QPoint pt_end = QPoint( (int)ana_info.point_end.x()*mf_sz_width, (int)ana_info.point_end.y()*mf_sz_height );
    judge_area( pt_end );
    int n_area = 4;
    if( pt.x()>pt_end.x()-n_area && pt.x()<=pt_end.x()+n_area &&
            pt.y()>pt_end.y()-n_area && pt.y()<=pt_end.y()+n_area ) {
        return move_self;
    }
    return move_null;
}

//对线的分析
AnaMove IrFrame::pt_in_ana_line( const QPoint &pt, const AnaInfo &ana_info )
{
    int n_area = 2;
    QPoint pt_start = QPoint( (int)ana_info.point_start.x()*mf_sz_width, (int)ana_info.point_start.y()*mf_sz_height );
    QPoint pt_end = QPoint( (int)ana_info.point_end.x()*mf_sz_width, (int)ana_info.point_end.y()*mf_sz_height );
    judge_area( pt_start );
    judge_area( pt_end );
    int n_start_x = pt_start.x();
    int n_start_y = pt_start.y();
    int n_end_x = pt_end.x();
    int n_end_y = pt_end.y();

    if( pt.x()>n_start_x-n_area && pt.x()<=n_start_x+n_area &&
            pt.y()>n_start_y-n_area && pt.y()<=n_start_y+n_area ) {
        return move_left;
    } else if( ( pt.x()>n_end_x-n_area && pt.x()<=n_end_x+n_area &&
                 pt.y()>n_end_y-n_area && pt.y()<=n_end_y+n_area ) ) {
        return move_right;
    } else {  //对于线的判断区域，用三角形的区域方式判断方式
        int a_min = qSqrt( (pt.x()-n_start_x)*(pt.x()-n_start_x) + (pt.y()-n_start_y)*(pt.y()-n_start_y) );
        int b_mid = qSqrt( (pt.x()-n_end_x)*(pt.x()-n_end_x) + (pt.y()-n_end_y)*(pt.y()-n_end_y) );
        int c_max = qSqrt( (n_start_x-n_end_x)*(n_start_x-n_end_x) + (n_start_y-n_end_y)*(n_start_y-n_end_y) );
        if( a_min+b_mid-c_max>-n_area && a_min+b_mid-c_max<=n_area ) {
            return move_self;
        }
    }

    return move_null;
}

AnaMove IrFrame::pt_in_ana_rect( const QPoint &pt, const AnaInfo &ana_info )
{
    QPoint pt_start = QPoint( (int)ana_info.point_start.x()*mf_sz_width, (int)ana_info.point_start.y()*mf_sz_height );
    QPoint pt_end = QPoint( (int)ana_info.point_end.x()*mf_sz_width, (int)ana_info.point_end.y()*mf_sz_height );
    judge_area( pt_start );
    judge_area( pt_end );
    int n_start_x = pt_start.x();
    int n_start_y = pt_start.y();
    int n_end_x = pt_end.x();
    int n_end_y = pt_end.y();
    int n_area = 2;
    if( pt.x()>n_start_x-n_area && pt.x()<=n_start_x+n_area &&
            pt.y()>n_start_y+n_area && pt.y()<=n_end_y-n_area ) {
        return move_left;
    } else if( pt.x()>n_start_x-n_area && pt.x()<=n_start_x+n_area &&
               pt.y()>n_start_y-n_area && pt.y()<=n_start_y+n_area ) {
        return move_lefttop;
    } else if( pt.x()>n_start_x+n_area && pt.x()<=n_end_x-n_area &&
               pt.y()>n_start_y-n_area && pt.y()<=n_start_y+n_area ) {
        return move_top;
    } else if( pt.x()>n_end_x-n_area && pt.x()<=n_end_x+n_area &&
               pt.y()>n_start_y-n_area && pt.y()<=n_start_y+n_area ) {
        return move_righttop;
    } else if( pt.x()>n_end_x-n_area && pt.x()<=n_end_x+n_area &&
               pt.y()>n_start_y+n_area && pt.y()<=n_end_y-n_area ) {
        return move_right;
    } else if( pt.x()>n_end_x-n_area && pt.x()<=n_end_x+n_area &&
               pt.y()>n_end_y-n_area && pt.y()<=n_end_y+n_area ) {
        return move_rightbottom;
    } else if( pt.x()>n_start_x+n_area && pt.x()<=n_end_x-n_area &&
               pt.y()>n_end_y-n_area && pt.y()<=n_end_y+n_area ) {
        return move_bottom;
    } else if( pt.x()>n_start_x-n_area && pt.x()<=n_start_x+n_area &&
               pt.y()>n_end_y-n_area && pt.y()<=n_end_y+n_area ) {
        return move_leftbottom;
    } else if( pt.x()>n_start_x && pt.x()<=n_end_x &&
            pt.y()>n_start_y && pt.y()<=n_end_y ) {
       return move_self;
    }
    return move_null;
}

AnaMove IrFrame::pt_in_ana_circle( const QPoint &pt, const AnaInfo &ana_info )
{
    QPoint pt_start = QPoint( (int)ana_info.point_start.x()*mf_sz_width, (int)ana_info.point_start.y()*mf_sz_height );
    QPoint pt_end = QPoint( (int)ana_info.point_end.x()*mf_sz_width, (int)ana_info.point_end.y()*mf_sz_height );
    judge_area( pt_start );
    judge_area( pt_end );
    int n_start_x = pt_start.x();
    int n_start_y = pt_start.y();
    int n_end_x = pt_end.x();
    int n_end_y = pt_end.y();
    int n_area = 2;
    int n_r = qSqrt( (n_end_x-n_start_x)*(n_end_x-n_start_x) + (n_end_y-n_start_y)*(n_end_y-n_start_y) );
    int n_distance = qSqrt( (pt.x()-n_start_x)*(pt.x()-n_start_x) + (pt.y()-n_start_y)*(pt.y()-n_start_y) );
    if( n_distance>n_r-n_area && n_distance<=n_r+n_area ){
        if( pt.x()>n_start_x-n_area-n_r && pt.x()<=n_start_x &&
                pt.y()>n_start_y-n_area-n_r && pt.y()<=n_start_y ) {
            return move_lefttop;
        } else if( pt.x()>n_start_x && pt.x()<=n_start_x+n_area+n_r &&
                   pt.y()>n_start_y-n_area-n_r && pt.y()<=n_start_y ) {
            return move_righttop;
        } else if( pt.x()>n_start_x && pt.x()<=n_start_x+n_area+n_r &&
                   pt.y()>n_start_y && pt.y()<=n_start_y+n_area+n_r ) {
            return move_rightbottom;
        } else if( pt.x()>n_start_x-n_area-n_r && pt.x()<=n_start_x &&
                   pt.y()>n_start_y && pt.y()<=n_start_y+n_area+n_r ) {
            return move_leftbottom;
        }
    } else if( n_distance>=0 && n_distance<n_r ) {
        return move_self;
    }
    return move_null;
}

void IrFrame::get_pt_ana_shape( AnaInfo &ana_info, const QPoint &pt )
{
    mn_sign_id = ana_info.n_sign_id;
    m_press_pt = pt;
    m_pt_start = QPoint( (int)ana_info.point_start.x()*mf_sz_width, (int)ana_info.point_start.y()*mf_sz_height );
    m_pt_middle = QPoint( (int)ana_info.point_end.x()*mf_sz_width, (int)ana_info.point_end.y()*mf_sz_height );
    judge_area( m_pt_start );
    judge_area( m_pt_middle );
    m_pt_move_start = m_pt_start;
    m_pt_move_end = m_pt_middle;
    qDebug()<<"IrFrame::get_pt_ana_shape"<<ana_info.point_start<<ana_info.point_end<<m_pt_start<<m_pt_middle<<mf_sz_width<<mf_sz_height;
}

void IrFrame::pt_move_ana_point( const QPoint &pt )
{
    if( me_ana_move == move_self ) {
        m_release_pt = pt;
        pt_judge_area( pt, m_release_pt );

        int n_x = pt.x()-m_press_pt.x();
        int n_y = pt.y()-m_press_pt.y();
        m_pt_move_start = QPoint( m_pt_start.x()+n_x, m_pt_start.y()+n_y );
        m_pt_move_end = QPoint( m_pt_middle.x()+n_x, m_pt_middle.y()+n_y );
        judge_area( m_pt_move_start );
        judge_area( m_pt_move_end );

        update();
    }
}

void IrFrame::pt_move_ana_line( const QPoint &pt )
{
    if( me_ana_move == move_left || me_ana_move == move_right ) {
        m_release_pt = pt;
        int n_x = pt.x()-m_press_pt.x();
        int n_y = pt.y()-m_press_pt.y();
        m_pt_move_start = QPoint( m_pt_start.x()+n_x, m_pt_start.y()+n_y );
        m_pt_move_end = QPoint( m_pt_middle.x()+n_x, m_pt_middle.y()+n_y );
        judge_area( m_pt_move_start );
        judge_area( m_pt_move_end );
        update();
    } else if( me_ana_move == move_self ) {
        m_release_pt = pt;
        int n_x = pt.x()-m_press_pt.x();
        int n_y = pt.y()-m_press_pt.y();
        m_pt_move_start = QPoint( m_pt_start.x()+n_x, m_pt_start.y()+n_y );
        m_pt_move_end = QPoint( m_pt_middle.x()+n_x, m_pt_middle.y()+n_y );
        pt_move_self_boundary( m_pt_move_start, m_pt_move_end );
        update();
    }
}

void IrFrame::pt_move_ana_rect( const QPoint &pt )
{
    if( me_ana_move == move_left || me_ana_move == move_lefttop || me_ana_move == move_top ||
            me_ana_move == move_righttop || me_ana_move == move_right || me_ana_move == move_rightbottom ||
            me_ana_move == move_bottom || me_ana_move == move_leftbottom ){
        m_release_pt = pt;
        int n_x = pt.x()-m_press_pt.x();
        int n_y = pt.y()-m_press_pt.y();
        m_pt_move_start = QPoint( m_pt_start.x()+n_x, m_pt_start.y()+n_y );
        m_pt_move_end = QPoint( m_pt_middle.x()+n_x, m_pt_middle.y()+n_y );
        judge_area( m_pt_move_start );
        judge_area( m_pt_move_end );
        update();
    } else if( me_ana_move == move_self ) {
        m_release_pt = pt;
        int n_x = pt.x()-m_press_pt.x();
        int n_y = pt.y()-m_press_pt.y();
        m_pt_move_start = QPoint( m_pt_start.x()+n_x, m_pt_start.y()+n_y );
        m_pt_move_end = QPoint( m_pt_middle.x()+n_x, m_pt_middle.y()+n_y );
        pt_move_self_boundary( m_pt_move_start, m_pt_move_end );
        update();
    }
}

void IrFrame::pt_move_ana_circle( const QPoint &pt )
{
    if( me_ana_move == move_lefttop || me_ana_move == move_righttop ||
            me_ana_move == move_rightbottom || me_ana_move == move_leftbottom ) {
        m_release_pt = pt;
        int n_x = pt.x()-m_pt_start.x();
        int n_y = pt.y()-m_pt_start.y();
        int n_r = qSqrt( n_x*n_x+n_y*n_y );
        int n_r_temp=0;
        start_area( m_pt_start, n_r_temp );
        if( n_r <= n_r_temp ) {
            m_pt_move_end = QPoint( m_pt_start.x()+n_r, m_pt_start.y() );
        } else {
            n_r = n_r_temp;
            m_pt_move_end = QPoint( m_pt_start.x()+n_r, m_pt_start.y() );
        }

        update();
    } else if( me_ana_move == move_self ) {
        pt_judge_area( pt, m_release_pt );
        int n_x = pt.x()-m_press_pt.x();
        int n_y = pt.y()-m_press_pt.y();
        m_pt_move_start = QPoint( m_pt_start.x()+n_x, m_pt_start.y()+n_y );
        m_pt_move_end = QPoint( m_pt_middle.x()+n_x, m_pt_middle.y()+n_y );
        pt_circle_move_self_boundary( m_pt_move_start, m_pt_move_end );
        update();
    }

}

//判断pt点是否在区域内
void IrFrame::pt_judge_area( const QPoint &pt, QPoint &pt_area )
{
    pt_area = pt;
    if( pt_area.x()<0 ) {
        pt_area.setX( 0 );
    } else if( pt_area.x()>width() ) {
        pt_area.setX( width()-1 );
    }

    if( pt_area.y()<0 ) {
        pt_area.setY( 0 );
    } else if( pt_area.y() > height() ) {
        pt_area.setY( height()-1 );
    }
}

//判断pt_area点是否在区域内
void IrFrame::judge_area( QPoint &pt_area )
{
    if( pt_area.x()<0 ) {
        pt_area.setX( 0 );
    } else if( pt_area.x()>width() ) {
        pt_area.setX( width()-1 );
    }

    if( pt_area.y()<0 ) {
        pt_area.setY( 0 );
    } else if( pt_area.y() > height() ) {
        pt_area.setY( height()-1 );
    }
}

//移动自己的时候，不能超出边界
void IrFrame::pt_move_self_boundary( QPoint &pt_start, QPoint &pt_end )
{
    int n_offset_x=0;
    int n_offset_y=0;

    if( pt_start.x()<pt_end.x() ) {
        if( pt_start.x()<0 ) {
            n_offset_x = pt_start.x();
        } else if( pt_end.x()>width() ){
            n_offset_x = pt_end.x()-width()+1;
        }
    } else {
        if( pt_end.x()<0 ) {
            n_offset_x = pt_end.x();
        } else if( pt_start.x()>width() ) {
            n_offset_x = pt_start.x()-width()+1;
        }
    }

    if( pt_start.y()<pt_end.y() ) {
        if( pt_start.y()<0 ) {
            n_offset_y = pt_start.y();
        } else if( pt_end.y()>height() ) {
            n_offset_y = pt_end.y()-height()+1;
        }
    } else {
        if( pt_end.y()<0 ) {
            n_offset_y = pt_end.y();
        } else if( pt_start.y()>height() ) {
            n_offset_y = pt_start.y()-height()+1;
        }
    }

    pt_start = QPoint( pt_start.x()-n_offset_x, pt_start.y()-n_offset_y );
    pt_end = QPoint( pt_end.x()-n_offset_x, pt_end.y()-n_offset_y );

    if( pt_start.x()<0 ) {
        pt_start.setX( 0 );
    } else if( pt_start.x()>width() ) {
        pt_start.setX( width()-1 );
    }

    if( pt_start.y()<0 ) {
        pt_start.setY( 0 );
    } else if( pt_start.y()>height() ) {
        pt_start.setY( height()-1 );
    }

    if( pt_end.x()<0 ) {
        pt_end.setX( 0 );
    } else if( pt_end.x()>width() ) {
        pt_end.setX( width()-1 );
    }

    if( pt_end.y()<0 ) {
        pt_end.setY( 0 );
    } else if( pt_end.y()>height() ) {
        pt_end.setY( height()-1 );
    }

}

//对圆自身移动的时候，判断边界位置
void IrFrame::pt_circle_move_self_boundary( QPoint &pt_circle_center, QPoint &pt_end )
{
    int n_offset_x=0;
    int n_offset_y=0;
    int n_x = pt_circle_center.x()-pt_end.x();
    int n_y = pt_circle_center.y()-pt_end.y();
    int n_r = qSqrt( n_x*n_x+n_y*n_y );
    QPoint pt_start, pt_end_temp;
    pt_start = QPoint( pt_circle_center.x()-n_r, pt_circle_center.y()-n_r );
    pt_end_temp = QPoint( pt_circle_center.x()+n_r, pt_circle_center.y()+n_r );
    if( pt_start.x()<pt_end_temp.x() ) {
        if( pt_start.x()<0 ) {
            n_offset_x = pt_start.x();
        } else if( pt_end_temp.x()>width() ){
            n_offset_x = pt_end_temp.x()-width();
        }
    } else {
        if( pt_end_temp.x()<0 ) {
            n_offset_x = pt_end_temp.x();
        } else if( pt_start.x()>width() ) {
            n_offset_x = pt_start.x()-width();
        }
    }


    if( pt_start.y()<pt_end_temp.y() ) {
        if( pt_start.y()<0 ) {
            n_offset_y = pt_start.y();
        } else if( pt_end_temp.y()>height() ) {
            n_offset_y = pt_end_temp.y()-height();
        }
    } else {
        if( pt_end_temp.y()<0 ) {
            n_offset_y = pt_end_temp.y();
        } else if( pt_start.y()>height() ) {
            n_offset_y = pt_start.y()-height();
        }
    }

    pt_circle_center = QPoint( pt_circle_center.x()-n_offset_x, pt_circle_center.y()-n_offset_y );
    pt_end = QPoint( pt_end.x()-n_offset_x, pt_end.y()-n_offset_y );

    /*if( pt_start.x()<0 ) {
        pt_start = QPoint( 0, pt_start.y() );
    }
    if( pt_start.y()>width() ) {
        pt_start = QPoint( pt_start.x(), width() );
    }
    if( pt_end.x()<0 ) {
        pt_end = QPoint( 0, pt_end.y() );
    }
    if( pt_end.y()>width() ) {
        pt_end = QPoint( pt_end.x(), width() );
    }*/
}


void IrFrame::set_cursor_pos( ) {
    RECT ir_frame_rect;
    ir_frame_rect.left = (LONG)this->geometry().left();
    ir_frame_rect.right = (LONG)this->geometry().right();
    ir_frame_rect.top = (LONG)this->geometry().top();
    ir_frame_rect.bottom = (LONG)this->geometry().bottom();
    /*if( pos.x()<0 ) {
         if( pos.y()<0 ) {
              QCursor::setPos ( 0, 0 );
         } else if( pos.y()> height() ) {
              QCursor::setPos ( 0, height() );
         }
    } else if( pos.x()>width() ) {
        if( pos.y()<0 ) {
             QCursor::setPos ( width(), 0 );
        } else if( pos.y()> height() ) {
             QCursor::setPos ( width(), height() );
        }
    }*/
}

void IrFrame::start_area( QPoint &pt_start, int &n_r )
{
    int n_r_left = pt_start.x();
    int n_r_top = pt_start.y();
    int n_r_right = width()-pt_start.x();
    int n_r_bottom = height()-pt_start.y();
    QList<int> n_r_list;
    n_r_list.append( n_r_left );
    n_r_list.append( n_r_top );
    n_r_list.append( n_r_right );
    n_r_list.append( n_r_bottom );
    qSort( n_r_list.begin(), n_r_list.end() );
    n_r = n_r_list.begin()[0];

}

void IrFrame::shape_append_list( QPoint pt )
{
    AnaInfo anainfo;
    switch ( me_draw_status ) {
    case draw_point_status: anainfo.type_name = "point"; break;
    case draw_line_status: anainfo.type_name = "line"; break;
    case draw_rect_status: anainfo.type_name = "rect"; break;
    case draw_circle_status: anainfo.type_name = "circle"; break;
    default: return;
    }

    anainfo.n_sign_id = ++sign_id;
    anainfo.point_start = m_pt_start;

    pt_judge_area( pt, anainfo.point_end );

    if( me_draw_status == draw_point_status ) {
        anainfo.point_start = anainfo.point_end;
    } else if ( me_draw_status == draw_rect_status ) {
        normalized_pt( anainfo.point_start, anainfo.point_end );
    } else if( me_draw_status == draw_circle_status ) {
        int n_x = pt.x()-m_pt_start.x();
        int n_y = pt.y()-m_pt_start.y();
        int n_r = qSqrt( n_x*n_x+n_y*n_y );
        int n_r_temp=0;
        start_area( m_pt_start, n_r_temp );
        if( n_r<=n_r_temp ) {
            anainfo.point_end = QPoint( m_pt_start.x()+n_r, m_pt_start.y() );
        } else {
            n_r = n_r_temp;
            anainfo.point_end = QPoint( m_pt_start.x()+n_r, m_pt_start.y() );

        }
    }

    anainfo.point_start = QPoint( (int)(anainfo.point_start.x()/mf_sz_width+0.5), (int)(anainfo.point_start.y()/mf_sz_height+0.5) );
    anainfo.point_end = QPoint( (int)(anainfo.point_end.x()/mf_sz_width+0.5), (int)(anainfo.point_end.y()/mf_sz_height+0.5) );
    judge_org_area( anainfo.point_start );
    judge_org_area( anainfo.point_end );

    m_list_anainfo.append( anainfo );

    me_draw_status = none_draw_status;
    me_mouse_press_status = Ana_Label::none_press_status;
    mp_transparent_ir_label->set_mouse_press_status( me_mouse_press_status );
}

void IrFrame::shape_move_or_change_size( QPoint pt )
{
    QList<AnaInfo>::iterator it;
    for( it=m_list_anainfo.begin(); it!=m_list_anainfo.end(); it++ ) {
        int n_x = pt.x()-m_press_pt.x();
        int n_y = pt.y()-m_press_pt.y();
        if( mn_sign_id == it->n_sign_id ) {
            if( ms_type_name == "point" ) {
                //pt_judge_area( pt, it->point_start );
                //m_pt_start = QPoint( m_pt_start.x()+n_x, m_pt_start.y()+n_y );
                pt_judge_area( QPoint( m_pt_start.x()+n_x, m_pt_start.y()+n_y ), m_pt_start );
                it->point_start = QPoint( (int)m_pt_start.x()/mf_sz_width+0.5, (int)m_pt_start.y()/mf_sz_height+0.5 );
                judge_org_area( it->point_start );
                it->point_end = it->point_start;
            } else if( ms_type_name == "line" ) {
                if( me_ana_move == move_left ) {
                    pt_judge_area( QPoint( m_pt_start.x()+n_x, m_pt_start.y()+n_y ), m_pt_start );
                } else if( me_ana_move == move_right ) {
                    //pt_judge_area( QPoint( m_pt_middle.x()+n_x, m_pt_middle.y()+n_y ), it->point_end );
                    pt_judge_area( QPoint( m_pt_middle.x()+n_x, m_pt_middle.y()+n_y ), m_pt_middle );
                } else if( me_ana_move == move_self ) {
                    m_pt_start = QPoint( m_pt_start.x()+n_x, m_pt_start.y()+n_y );
                    m_pt_middle = QPoint( m_pt_middle.x()+n_x, m_pt_middle.y()+n_y );
                    pt_move_self_boundary( m_pt_start, m_pt_middle );
                }
                it->point_start = QPoint( (int)(m_pt_start.x()/mf_sz_width+0.5), (int)(m_pt_start.y()/mf_sz_height+0.5) );
                it->point_end = QPoint( (int)(m_pt_middle.x()/mf_sz_width+0.5), (int)(m_pt_middle.y()/mf_sz_height+0.5) );
                judge_org_area( it->point_start );
                judge_org_area( it->point_end );
            } else if( ms_type_name == "rect" ) {
                if( me_ana_move == move_left ) {
                    pt_judge_area( QPoint( m_pt_start.x()+n_x, m_pt_start.y() ), m_pt_start );
                } else if( me_ana_move == move_lefttop ) {
                    pt_judge_area( QPoint( m_pt_start.x()+n_x, m_pt_start.y()+n_y ), m_pt_start );
                } else if( me_ana_move == move_top ) {
                    pt_judge_area( QPoint( m_pt_start.x(), m_pt_start.y()+n_y ), m_pt_start );
                } else if( me_ana_move == move_righttop ) {
                    pt_judge_area( QPoint( m_pt_start.x(), m_pt_start.y()+n_y ), m_pt_start );
                    pt_judge_area( QPoint( m_pt_middle.x()+n_x, m_pt_middle.y() ), m_pt_middle );
                } else if( me_ana_move == move_right ) {
                    pt_judge_area( QPoint( m_pt_middle.x()+n_x, m_pt_middle.y() ), m_pt_middle );
                } else if( me_ana_move == move_rightbottom ) {
                    pt_judge_area( QPoint( m_pt_middle.x()+n_x, m_pt_middle.y()+n_y ),  m_pt_middle );
                } else if( me_ana_move == move_bottom ) {
                    pt_judge_area( QPoint( m_pt_middle.x(), m_pt_middle.y()+n_y ), m_pt_middle );
                } else if( me_ana_move == move_leftbottom ) {
                    pt_judge_area( QPoint( m_pt_start.x()+n_x, m_pt_start.y() ), m_pt_start );
                    pt_judge_area( QPoint( m_pt_middle.x(), m_pt_middle.y()+n_y ), m_pt_middle );
                } else if( me_ana_move == move_self ) {
                    m_pt_start = QPoint( m_pt_start.x()+n_x, m_pt_start.y()+n_y );
                    m_pt_middle = QPoint( m_pt_middle.x()+n_x, m_pt_middle.y()+n_y );
                    pt_move_self_boundary( m_pt_start, m_pt_middle );
                }
                normalized_pt( m_pt_start, m_pt_middle  );
                it->point_start = QPoint( (int)m_pt_start.x()/mf_sz_width+0.5, (int)m_pt_start.y()/mf_sz_height+0.5 );
                it->point_end = QPoint( (int)m_pt_middle.x()/mf_sz_width+0.5, (int)m_pt_middle.y()/mf_sz_height+0.5 );
                judge_org_area( it->point_start );
                judge_org_area( it->point_end );
            } else if( ms_type_name == "circle" ) {
                if( me_ana_move == move_lefttop || me_ana_move == move_righttop ||
                        me_ana_move == move_rightbottom || me_ana_move == move_leftbottom ) {
                    //起始点也不能出界
                    int n_x = pt.x()-m_pt_start.x();
                    int n_y = pt.y()-m_pt_start.y();
                    int n_r = qSqrt( n_x*n_x+n_y*n_y );
                    int n_r_temp=0;
                    start_area( m_pt_start, n_r_temp );
                    if( n_r<=n_r_temp ) {
                    m_pt_middle = QPoint( m_pt_start.x()+n_r, m_pt_start.y() );
                    } else {
                        n_r = n_r_temp;
                        m_pt_middle = QPoint( m_pt_start.x()+n_r, m_pt_start.y() );

                    }
                    update();
                } else if( me_ana_move == move_self ) {
                    n_x = pt.x()-m_press_pt.x();
                    n_y = pt.y()-m_press_pt.y();
                    m_pt_start = QPoint( m_pt_start.x()+n_x, m_pt_start.y()+n_y );
                    m_pt_middle = QPoint( m_pt_middle.x()+n_x, m_pt_middle.y()+n_y );
                    pt_circle_move_self_boundary( m_pt_start, m_pt_middle );
                }
                it->point_start = QPoint( (int)m_pt_start.x()/mf_sz_width+0.5, (int)m_pt_start.y()/mf_sz_height+0.5 );
                it->point_end = QPoint( (int)m_pt_middle.x()/mf_sz_width+0.5, (int)m_pt_middle.y()/mf_sz_height+0.5 );
                judge_org_area( it->point_start );
                judge_org_area( it->point_end );

            }
            ms_type_name = "";
            qDebug()<<"IrFrame::shape_move_or_change_size"<<m_pt_start<<m_pt_middle\
                      <<it->point_start<<it->point_end<<n_x<<n_y;
            break;
        }

    }
}

void IrFrame::set_org_sz( int width, int height )
{
    mn_org_width = width;
    mn_org_height = height;
}

//判断pt_area点是否在原始区域内
void IrFrame::judge_org_area( QPoint &pt_area )
{
    if( pt_area.x()<0 ) {
        pt_area.setX( 0 );
    } else if( pt_area.x()>mn_org_width ) {
        pt_area.setX( mn_org_width );
    }

    if( pt_area.y()<0 ) {
        pt_area.setY( 0 );
    } else if( pt_area.y() > mn_org_height ) {
        pt_area.setY( height() );
    }
}
