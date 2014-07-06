#include "irframe.h"
#include <QPainter>
#include <QGraphicsBlurEffect>
#include <QTimer>
#include <QtMath>
#include <QDebug>
#include <windef.h>

#define LABEL_WIDTH 160
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
    ,me_press_status( none_press_status )
    ,me_draw_status( none_draw_status )
{
    init_ir_widget();
    setMouseTracking( true );
    set_cursor_pos();
}

IrFrame::~IrFrame()
{

}

void IrFrame::init_ir_widget() {
    mp_transparent_ir_label = new QLabel( this );
    QGraphicsOpacityEffect *effect = new QGraphicsOpacityEffect( this );
    effect->setOpacity( 0.8 );
    mp_transparent_ir_label->setGraphicsEffect( effect );
    mp_transparent_ir_label->hide();

    m_list.append( "点" );
    m_list.append( "线" );
    m_list.append( "框" );
    m_list.append( "圆" );
    qDebug()<<"RealTime:ir_list"<<m_list;
}

void IrFrame::set_ir_slider( IrSlider *p_ir_slider )
{
    mp_ir_slider = p_ir_slider;
    //设置QSlider的范围
    mp_ir_slider->setRange( 0, 100 );
    //在拖动条上按一下改变的值
    //mp_slider_play->setPageStep( 10 );
    //设置显示刻度的位置
    mp_ir_slider->setTickPosition( QSlider::TicksRight );
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
        //n = mp_ir_slider->get_value_pos();
        n = mn_ir_slider_value;
    } else {
        n = ++n>100?0:n;
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
    draw.fillRect( 0, 0, width(), height(), QBrush( QColor( 125,125,125 ) ) );

    int n_size = m_list.size();
    QList<QString>::iterator i;
    int j = 0;
    int n_width = LABEL_WIDTH/4;
    int n_height = LABEL_HEIGHT;
    for( i=m_list.begin(),j=0; i!=m_list.end(); i++,j++ ) {
        if( j == 0 && me_press_status == point_status ) {
            draw.fillRect( mp_transparent_ir_label->x()+n_width*j, mp_transparent_ir_label->y(), n_width, n_height, QBrush( QColor(177,1,1) ) );
        } else if( j== 1 && me_press_status == line_status ) {
            draw.fillRect( mp_transparent_ir_label->x()+n_width*j, mp_transparent_ir_label->y(), n_width, n_height, QBrush( QColor(177,1,1) ) );
        } else if( j== 2 && me_press_status == rect_status ) {
            draw.fillRect( mp_transparent_ir_label->x()+n_width*j, mp_transparent_ir_label->y(), n_width, n_height, QBrush( QColor(177,1,1) ) );
        } else if( j== 3 && me_press_status == circle_status ) {
            draw.fillRect( mp_transparent_ir_label->x()+n_width*j, mp_transparent_ir_label->y(), n_width, n_height, QBrush( QColor(177,1,1) ) );
        } else {
            draw.fillRect( mp_transparent_ir_label->x()+n_width*j, mp_transparent_ir_label->y(), n_width, n_height, QBrush( QColor(125,125,125) ) );
        }

        draw.setPen( Qt::white );
        draw.drawText( mp_transparent_ir_label->x()+n_width*j, mp_transparent_ir_label->y(), n_width, n_height, Qt::AlignCenter, *i );
        draw.setPen( Qt::black );
        draw.drawRect( mp_transparent_ir_label->x()+n_width*j, mp_transparent_ir_label->y(), n_width, n_height );
    }

    //1.鼠标按下，没有移动
    //2.鼠标按下并移动，但没有释放
    switch ( me_draw_status ) {
    case draw_point_status:
        draw.drawPoint( m_pt_middle.x(), m_pt_middle.y() );
        draw.drawLine( QPoint(m_pt_middle.x()-10,m_pt_middle.y()), QPoint(m_pt_middle.x()+10,m_pt_middle.y()) );
        draw.drawLine( QPoint(m_pt_middle.x(), m_pt_middle.y()-10), QPoint(m_pt_middle.x(),m_pt_middle.y()+10) );
        update();
        break;
    case draw_line_status:
        draw.drawLine( m_pt_start, m_pt_middle );
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
        if( it->type_name == "point" ) {
            draw.drawPoint( (*it).point_end.x(), (*it).point_end.y() );
            draw.drawLine( QPoint((*it).point_end.x()-10,(*it).point_end.y()), QPoint((*it).point_end.x()+10,(*it).point_end.y()) );
            draw.drawLine( QPoint((*it).point_end.x(), (*it).point_end.y()-10), QPoint((*it).point_end.x(),(*it).point_end.y()+10) );
        } else if( it->type_name == "line" ) {
            draw.drawLine( (*it).point_start, (*it).point_end );
            if( me_ana_move!=move_null && (*it).n_sign_id == mn_sign_id ) {
                draw.drawRect( (*it).point_start.x()-2,(*it).point_start.y()-2, 4, 4 );
                draw.drawRect( (*it).point_end.x()-2,(*it).point_end.y()-2, 4, 4 );
            }
        } else if( it->type_name == "rect" ) {
            draw.drawRect( QRect( (*it).point_start.x(), (*it).point_start.y(), (*it).point_end.x()-(*it).point_start.x(), (*it).point_end.y()-(*it).point_start.y() ));
            if( me_ana_move!=move_null && (*it).n_sign_id == mn_sign_id ) {
                draw.drawRect( (*it).point_start.x()-2, (*it).point_start.y()-2, 4, 4 );
                draw.drawRect( (*it).point_end.x()-2, (*it).point_end.y()-2, 4, 4 );
                draw.drawRect( (*it).point_start.x()-2, (*it).point_end.y()-2, 4, 4 );
                draw.drawRect( (*it).point_end.x()-2, (*it).point_start.y()-2, 4, 4 );
            }
        } else if( it->type_name == "circle" ) {
            int n_x = (*it).point_end.x()-(*it).point_start.x();
            int n_y = (*it).point_end.y()-(*it).point_start.y();
            int n_r = (int)qSqrt( n_x*n_x + n_y*n_y );
            draw.drawEllipse( (*it).point_start.x()-n_r, (*it).point_start.y()-n_r, n_r*2, n_r*2 );
            if( me_ana_move!=move_null && (*it).n_sign_id == mn_sign_id ) {
                draw.drawRect( (*it).point_start.x()-2-n_r, (*it).point_start.y()-2, 4, 4 );
                draw.drawRect( (*it).point_start.x()-2, (*it).point_start.y()-2-n_r, 4, 4 );
                draw.drawRect( (*it).point_start.x()-2+n_r, (*it).point_start.y()-2, 4, 4 );
                draw.drawRect( (*it).point_start.x()-2, (*it).point_start.y()-2+n_r, 4, 4 );
            }
        }
    }

    draw.setPen( Qt::red );
    for( it=m_list_anainfo.begin(); it!=m_list_anainfo.end(); it++ ) {
        if( it->n_sign_id == mn_sign_id ) {
            if( ms_type_name == "point" ) {
                draw.drawPoint( m_release_pt.x(), m_release_pt.y() );
                draw.drawLine( QPoint(m_release_pt.x()-10,m_release_pt.y()), QPoint(m_release_pt.x()+10,m_release_pt.y()) );
                draw.drawLine( QPoint(m_release_pt.x(), m_release_pt.y()-10), QPoint(m_release_pt.x(), m_release_pt.y()+10) );
                if( me_ana_move!=move_null && (*it).n_sign_id == mn_sign_id ) {
                    draw.drawRect( m_release_pt.x()-2, m_release_pt.y()-2, 4, 4 );
                }
            } else if( ms_type_name == "line" ) {
                if( me_ana_move == move_self ) {
                    draw.drawLine( m_pt_move_start, m_pt_move_end );
                    draw.drawRect( m_pt_move_start.x()-2, m_pt_move_start.y()-2, 4, 4 );
                    draw.drawRect( m_pt_move_end.x()-2, m_pt_move_end.y()-2, 4, 4 );
                } else if( me_ana_move == move_left ) {
                    draw.drawLine( m_release_pt, it->point_end );

                    draw.drawRect( m_release_pt.x()-2, m_release_pt.y()-2, 4, 4 );
                    draw.drawRect( it->point_end.x()-2, it->point_end.y()-2, 4, 4 );
                } else if( me_ana_move == move_right ) {
                    draw.drawLine( it->point_start, m_release_pt );

                    draw.drawRect( it->point_start.x()-2, it->point_start.y()-2, 4, 4 );
                    draw.drawRect( m_release_pt.x()-2, m_release_pt.y()-2, 4, 4 );
                }
            } else if( ms_type_name == "rect" ) {
                if( me_ana_move == move_left ) {
                    draw.drawRect( QRect( m_release_pt.x(), (*it).point_start.y(), (*it).point_end.x()-m_release_pt.x(), (*it).point_end.y()-(*it).point_start.y() ));
                } else if( me_ana_move == move_lefttop ) {
                    draw.drawRect( QRect( m_release_pt.x(), m_release_pt.y(), (*it).point_end.x()-m_release_pt.x(), (*it).point_end.y()-m_release_pt.y() ));
                } else if( me_ana_move == move_top ) {
                    draw.drawRect( QRect( it->point_start.x(), m_release_pt.y(), (*it).point_end.x()-it->point_start.x(), (*it).point_end.y()-m_release_pt.y() ));
                } else if( me_ana_move == move_righttop ) {
                    draw.drawRect( QRect( it->point_start.x(), m_release_pt.y(), m_release_pt.x()-it->point_start.x(), (*it).point_end.y()-m_release_pt.y() ));
                } else if( me_ana_move == move_right ) {
                    draw.drawRect( QRect( it->point_start.x(), it->point_start.y(), m_release_pt.x()-it->point_start.x(), (*it).point_end.y()-it->point_start.y() ));
                } else if( me_ana_move == move_rightbottom ) {
                    draw.drawRect( QRect( it->point_start.x(), it->point_start.y(), m_release_pt.x()-it->point_start.x(), m_release_pt.y()-it->point_start.y() ));
                } else if( me_ana_move == move_bottom ) {
                    draw.drawRect( QRect( it->point_start.x(), it->point_start.y(), it->point_end.x()-it->point_start.x(), m_release_pt.y()-it->point_start.y() ));
                } else if( me_ana_move == move_leftbottom ) {
                    draw.drawRect( QRect( m_release_pt.x(), it->point_start.y(), it->point_end.x()-m_release_pt.x(), m_release_pt.y()-it->point_start.y() ));
                } else if( me_ana_move == move_self ) {
                    draw.drawRect( QRect( m_pt_move_start.x(), m_pt_move_start.y(), m_pt_move_end.x()-m_pt_move_start.x(), m_pt_move_end.y()-m_pt_move_start.y() ) );
                    draw.drawRect( m_pt_move_start.x()-2, m_pt_move_start.y()-2, 4, 4 );
                    draw.drawRect( m_pt_move_end.x()-2, m_pt_move_end.y()-2, 4, 4 );
                    draw.drawRect( m_pt_move_start.x()-2, m_pt_move_end.y()-2, 4, 4 );
                    draw.drawRect( m_pt_move_end.x()-2, m_pt_move_start.y()-2, 4, 4 );
                }
            } else if( ms_type_name == "circle" ) {
                int n_x = m_release_pt.x()-(*it).point_start.x();
                int n_y = m_release_pt.y()-(*it).point_start.y();
                int n_r = (int)qSqrt( n_x*n_x + n_y*n_y );
                if( me_ana_move == move_lefttop || me_ana_move == move_righttop ||
                        me_ana_move == move_rightbottom || me_ana_move == move_leftbottom ) {
                    /*draw.drawEllipse( (*it).point_start.x()-n_r, (*it).point_start.y()-n_r, n_r*2, n_r*2 );
                    draw.drawLine( m_press_pt, it->point_start );
                    draw.drawLine( m_release_pt, it->point_start );
                    if( me_ana_move!=move_null && (*it).n_sign_id == mn_sign_id ) {
                        draw.drawRect( (*it).point_start.x()-2-n_r, (*it).point_start.y()-2, 4, 4 );
                        draw.drawRect( (*it).point_start.x()-2, (*it).point_start.y()-2-n_r, 4, 4 );
                        draw.drawRect( (*it).point_start.x()-2+n_r, (*it).point_start.y()-2, 4, 4 );
                        draw.drawRect( (*it).point_start.x()-2, (*it).point_start.y()-2+n_r, 4, 4 );
                    }*/
                    int n_point_x = m_pt_move_end.x()-it->point_start.x();
                    int n_point_y = m_pt_move_end.y()-it->point_start.y();
                    n_r = (int)qSqrt( n_point_x*n_point_x + n_point_y*n_point_y );
                    draw.drawEllipse( it->point_start.x()-n_r, it->point_start.y()-n_r, n_r*2, n_r*2 );
                    draw.drawLine( m_press_pt, it->point_start );
                    draw.drawLine( m_pt_move_end, it->point_start );
                } else if( me_ana_move == move_self ) {
                    int n_point_x = m_pt_move_end.x()-m_pt_move_start.x();
                    int n_point_y = m_pt_move_end.y()-m_pt_move_start.y();
                    n_r = (int)qSqrt( n_point_x*n_point_x + n_point_y*n_point_y );
                    draw.drawEllipse( m_pt_move_start.x()-n_r, m_pt_move_start.y()-n_r, n_r*2, n_r*2 );
                }
            }
            break;
       }
    }


    draw.end();
}

void IrFrame::resizeEvent( QResizeEvent *event )
{
    //setStyleSheet( "border:10px solid black" );

    mp_transparent_ir_label->move( SPACE_IR, SPACE_IR );
    mp_transparent_ir_label->resize( LABEL_WIDTH, LABEL_HEIGHT );
}

void IrFrame::mousePressEvent( QMouseEvent *event )
{
    if ( event->button() == Qt::LeftButton ) {    
        if ( me_press_status != none_press_status ) {
            draw_add_shape( QPoint( event->x(), event->y() ) );
        } else {
            AnaInfo p_ana;
            bool b_in_ana = pt_in_ana( QPoint( event->x(), event->y()), me_ana_move, p_ana );
            if ( b_in_ana ) {
                //mp_changed_ana = &p_ana;
                //set_draw_info( p_ana, ana_move );
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

            }

            press_status_shape( QPoint( event->x(), event->y() ) );
        }
    }
}

void IrFrame::mouseMoveEvent( QMouseEvent *event )
{
    QPoint pt_cursor = QPoint( event->x(), event->y() );

    switch ( me_draw_status ) {
    case draw_point_status:
        pt_mouse_move_boundary( QPoint( event->x(), event->y() ), m_pt_middle );
        update();
        break;
    case draw_line_status:
        pt_mouse_move_boundary( QPoint( event->x(), event->y() ), m_pt_middle );
        update();
        break;
    case draw_rect_status:
        pt_mouse_move_boundary( QPoint( event->x(), event->y() ), m_pt_middle );
        update();
        break;
    case draw_circle_status:
        //m_pt_middle = QPoint( event->x(), event->y() );
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

            pt_add_release_boundary( QPoint( event->x(), event->y() ), anainfo.point_end );

            if ( me_draw_status == draw_rect_status ) {
                normalized_pt( anainfo.point_start, anainfo.point_end );
            } else if( me_draw_status == draw_circle_status ) {
                QPoint pt = QPoint( event->x(), event->y() );
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

            m_list_anainfo.append( anainfo );

            me_draw_status = none_draw_status;
            me_press_status = none_press_status;
            update();
        } else {
            QList<AnaInfo>::iterator it;
            for( it=m_list_anainfo.begin(); it!=m_list_anainfo.end(); it++ ) {
                int n_x = event->x()-m_press_pt.x();
                int n_y = event->y()-m_press_pt.y();
                if( mn_sign_id == it->n_sign_id ) {
                    if( ms_type_name == "point" ) {
                        pt_add_release_boundary( QPoint( event->x(), event->y() ), it->point_start );
                        it->point_end = it->point_start;
                    } else if( ms_type_name == "line" ) {
                        if( me_ana_move == move_left ) {
                            pt_add_release_boundary( QPoint( m_pt_start.x()+n_x, m_pt_start.y()+n_y ), it->point_start );
                        } else if( me_ana_move == move_right ) {
                            pt_add_release_boundary( QPoint( m_pt_middle.x()+n_x, m_pt_middle.y()+n_y ), it->point_end );
                        } else if( me_ana_move == move_self ) {
                            it->point_start = QPoint( m_pt_start.x()+n_x, m_pt_start.y()+n_y );
                            it->point_end = QPoint( m_pt_middle.x()+n_x, m_pt_middle.y()+n_y );
                            pt_move_self_boundary( it->point_start, it->point_end );
                        }
                    } else if( ms_type_name == "rect" ) {
                        if( me_ana_move == move_left ) {
                            pt_add_release_boundary( QPoint( m_pt_start.x()+n_x, m_pt_start.y() ), it->point_start );
                        } else if( me_ana_move == move_lefttop ) {
                            pt_add_release_boundary( QPoint( m_pt_start.x()+n_x, m_pt_start.y()+n_y ), it->point_start );
                        } else if( me_ana_move == move_top ) {
                            pt_add_release_boundary( QPoint( m_pt_start.x(), m_pt_start.y()+n_y ), it->point_start );
                        } else if( me_ana_move == move_righttop ) {
                            pt_add_release_boundary( QPoint( m_pt_start.x(), m_pt_start.y()+n_y ), it->point_start );
                            pt_add_release_boundary( QPoint( m_pt_middle.x()+n_x, m_pt_middle.y() ), it->point_end );
                        } else if( me_ana_move == move_right ) {
                            pt_add_release_boundary( QPoint( m_pt_middle.x()+n_x, m_pt_middle.y() ), it->point_end );
                        } else if( me_ana_move == move_rightbottom ) {
                            pt_add_release_boundary( QPoint( m_pt_middle.x()+n_x, m_pt_middle.y()+n_y ),  it->point_end );
                        } else if( me_ana_move == move_bottom ) {
                            pt_add_release_boundary( QPoint( m_pt_middle.x(), m_pt_middle.y()+n_y ), it->point_end );
                        } else if( me_ana_move == move_leftbottom ) {
                            pt_add_release_boundary( QPoint( m_pt_start.x()+n_x, m_pt_start.y() ), it->point_start );
                            pt_add_release_boundary( QPoint( m_pt_middle.x(), m_pt_middle.y()+n_y ), it->point_end );
                        } else if( me_ana_move == move_self ) {
                            it->point_start = QPoint( m_pt_start.x()+n_x, m_pt_start.y()+n_y );
                            it->point_end = QPoint( m_pt_middle.x()+n_x, m_pt_middle.y()+n_y );
                            pt_move_self_boundary( it->point_start, it->point_end );
                        }

                        normalized_pt( it->point_start, it->point_end  );
                    } else if( ms_type_name == "circle" ) {
                        if( me_ana_move == move_lefttop || me_ana_move == move_righttop ||
                                me_ana_move == move_rightbottom || me_ana_move == move_leftbottom ) {
                            //起始点也不能出界
                            QPoint pt = QPoint( event->x(), event->y() );
                            int n_x = pt.x()-m_pt_start.x();
                            int n_y = pt.y()-m_pt_start.y();
                            int n_r = qSqrt( n_x*n_x+n_y*n_y );
                            int n_r_temp=0;
                            start_area( m_pt_start, n_r_temp );
                            if( n_r<=n_r_temp ) {
                            it->point_end = QPoint( m_pt_start.x()+n_r, m_pt_start.y() );
                            } else {
                                n_r = n_r_temp;
                                it->point_end = QPoint( m_pt_start.x()+n_r, m_pt_start.y() );

                            }
                            update();
                        } else if( me_ana_move == move_self ) {
                            n_x = event->x()-m_press_pt.x();
                            n_y = event->y()-m_press_pt.y();
                            it->point_start = QPoint( m_pt_start.x()+n_x, m_pt_start.y()+n_y );
                            it->point_end = QPoint( m_pt_middle.x()+n_x, m_pt_middle.y()+n_y );
                            pt_circle_move_self_boundary( it->point_start, it->point_end );
                        }

                    }
                    update();
                    ms_type_name = "";
                    break;
                }
            }
        }
        //update();
    }

}

void IrFrame::press_status_shape( QPoint pt ) {
    //如果鼠标点击 点 线位置   ，mb_ana_status =true
    int n_width = LABEL_WIDTH/4;
    int i = 0;
    for( ; i<4; i++ ) {
        if( pt.x()>SPACE_IR+n_width*i && pt.x()<= SPACE_IR+n_width*(i+1) \
                && pt.y()>SPACE_IR && pt.y()<=SPACE_IR+LABEL_HEIGHT ) {
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

void IrFrame::draw_add_shape( QPoint pt )
{
    //m_pt_start = pt;
    //m_pt_middle = pt;
    pt_add_release_boundary( pt, m_pt_start );
    m_pt_middle = pt;

    me_draw_status = (enum_draw_status)me_press_status;
}

void IrFrame::pt_add_boundary( const QPoint &pt )
{
    int n_offset_x=0;
    int n_offset_y=0;
    if( pt.x()<0 ) {
        n_offset_x = pt.x();
    } else if( pt.x()>width() ) {
        n_offset_x = pt.x()-width();
    }
    if( pt.y()<0 ) {
        n_offset_y = pt.y();
    } else if( pt.y()>height() ) {
        n_offset_y = pt.y()-height();
    }
    m_pt_start = QPoint( pt.x()-n_offset_x, pt.y()-n_offset_y );
    m_pt_middle = m_pt_middle;
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
    //ana_info = (*it);
    ana_info.n_sign_id = it->n_sign_id;
    ana_info.type_name = it->type_name;
    ana_info.point_end = it->point_end;
    ana_info.point_start = it->point_start;
    return true;
}

AnaMove IrFrame::pt_in_ana_point(const QPoint &pt, const AnaInfo &ana_info)
{
    if( pt.x()>ana_info.point_end.x()-4 && pt.x()<=ana_info.point_end.x()+4 &&
            pt.y()>ana_info.point_end.y()-4 && pt.y()<=ana_info.point_end.y()+4 ) {
        return move_self;
    }
    return move_null;
}

AnaMove IrFrame::pt_in_ana_line( const QPoint &pt, const AnaInfo &ana_info )
{
    if( pt.x()>ana_info.point_start.x()-2 && pt.x()<=ana_info.point_start.x()+2 &&
            pt.y()>ana_info.point_start.y()-2 && pt.y()<=ana_info.point_start.y()+2 ) {
        return move_left;
    } else if( ( pt.x()>ana_info.point_end.x()-2 && pt.x()<=ana_info.point_end.x()+2 &&
                 pt.y()>ana_info.point_end.y()-2 && pt.y()<=ana_info.point_end.y()+2 ) ) {
        return move_right;
    } else {
        int a = qSqrt((pt.x()-ana_info.point_start.x())*(pt.x()-ana_info.point_start.x())+(pt.y()-ana_info.point_start.y())*(pt.y()-ana_info.point_start.y()));
        int b = qSqrt((pt.x()-ana_info.point_end.x())*(pt.x()-ana_info.point_end.x())+(pt.y()-ana_info.point_end.y())*(pt.y()-ana_info.point_end.y()));
        int c = qSqrt((ana_info.point_start.x()-ana_info.point_end.x())*(ana_info.point_start.x()-ana_info.point_end.x())+(ana_info.point_start.y()-ana_info.point_end.y())*(ana_info.point_start.y()-ana_info.point_end.y()));
        if( a+b-c>-2 && a+b-c<=2 ) {
            return move_self;
        }
    }

    return move_null;
}

AnaMove IrFrame::pt_in_ana_rect( const QPoint &pt, const AnaInfo &ana_info )
{
    int n_start_x = ana_info.point_start.x();
    int n_start_y = ana_info.point_start.y();
    int n_end_x = ana_info.point_end.x();
    int n_end_y = ana_info.point_end.y();

    if( pt.x()>n_start_x-2 && pt.x()<=n_start_x+2 &&
            pt.y()>n_start_y+2 && pt.y()<=n_end_y-2 ) {
        return move_left;
    } else if( pt.x()>n_start_x-2 && pt.x()<=n_start_x+2 &&
               pt.y()>n_start_y-2 && pt.y()<=n_start_y+2 ) {
        return move_lefttop;
    } else if( pt.x()>n_start_x+2 && pt.x()<=n_end_x-2 &&
               pt.y()>n_start_y-2 && pt.y()<=n_start_y+2 ) {
        return move_top;
    } else if( pt.x()>n_end_x-2 && pt.x()<=n_end_x+2 &&
               pt.y()>n_start_y-2 && pt.y()<=n_start_y+2 ) {
        return move_righttop;
    } else if( pt.x()>n_end_x-2 && pt.x()<=n_end_x+2 &&
               pt.y()>n_start_y+2 && pt.y()<=n_end_y-2 ) {
        return move_right;
    } else if( pt.x()>n_end_x-2 && pt.x()<=n_end_x+2 &&
               pt.y()>n_end_y-2 && pt.y()<=n_end_y+2 ) {
        return move_rightbottom;
    } else if( pt.x()>n_start_x+2 && pt.x()<=n_end_x-2 &&
               pt.y()>n_end_y-2 && pt.y()<=n_end_y+2 ) {
        return move_bottom;
    } else if( pt.x()>n_start_x-2 && pt.x()<=n_start_x+2 &&
               pt.y()>n_end_y-2 && pt.y()<=n_end_y+2 ) {
        return move_leftbottom;
    } else if( pt.x()>n_start_x && pt.x()<=n_end_x &&
            pt.y()>n_start_y && pt.y()<=n_end_y ) {
       return move_self;
    }
    return move_null;
}

AnaMove IrFrame::pt_in_ana_circle( const QPoint &pt, const AnaInfo &ana_info )
{
    m_pt_start = ana_info.point_start;
    m_pt_middle = ana_info.point_end;
    int n_r = qSqrt( (m_pt_middle.x()-m_pt_start.x())*(m_pt_middle.x()-m_pt_start.x())+ (m_pt_middle.y()-m_pt_start.y())*(m_pt_middle.y()-m_pt_start.y()) );
    int n_distance = qSqrt((pt.x()-m_pt_start.x())*(pt.x()-m_pt_start.x())+(pt.y()-m_pt_start.y())*(pt.y()-m_pt_start.y()));
    if( n_distance>n_r-2 && n_distance<=n_r+2 ){
        int n_start_x = ana_info.point_start.x();
        int n_start_y = ana_info.point_start.y();
        if( pt.x()>n_start_x-2-n_r && pt.x()<=n_start_x &&
                pt.y()>n_start_y-2-n_r && pt.y()<=n_start_y ) {
            return move_lefttop;
        } else if( pt.x()>n_start_x && pt.x()<=n_start_x+2+n_r &&
                   pt.y()>n_start_y-2-n_r && pt.y()<=n_start_y ) {
            return move_righttop;
        } else if( pt.x()>n_start_x && pt.x()<=n_start_x+2+n_r &&
                   pt.y()>n_start_y && pt.y()<=n_start_y+2+n_r ) {
            return move_rightbottom;
        } else if( pt.x()>n_start_x-2-n_r && pt.x()<=n_start_x &&
                   pt.y()>n_start_y && pt.y()<=n_start_y+2+n_r ) {
            return move_leftbottom;
        }
    } else if( n_distance>0 && n_distance<=n_r ) {
        return move_self;
    }
    return move_null;
}

void IrFrame::get_pt_ana_shape( AnaInfo &ana_info, const QPoint &pt )
{
    mn_sign_id = ana_info.n_sign_id;
    m_press_pt = pt;
    m_pt_start = ana_info.point_start;
    m_pt_middle = ana_info.point_end;
}

void IrFrame::pt_move_ana_point( const QPoint &pt )
{
    if( me_ana_move == move_self ) {
        QList<AnaInfo>::iterator it;
        for( it=m_list_anainfo.begin(); it!=m_list_anainfo.end(); it++ ) {
            if( it->n_sign_id == mn_sign_id ) {
                m_release_pt = pt;
                pt_mouse_move_boundary( pt, m_release_pt );
                update();
                break;
            }
        }
    }
}

void IrFrame::pt_move_ana_line( const QPoint &pt )
{
    if( me_ana_move == move_left || me_ana_move == move_right || me_ana_move == move_self  ) {
        QList<AnaInfo>::iterator it;
        for( it=m_list_anainfo.begin(); it!=m_list_anainfo.end(); it++ ) {
            if( it->n_sign_id == mn_sign_id ) {
                m_release_pt = pt;
                int n_x = pt.x()-m_press_pt.x();
                int n_y = pt.y()-m_press_pt.y();
                m_pt_move_start = QPoint( m_pt_start.x()+n_x, m_pt_start.y()+n_y );
                m_pt_move_end = QPoint( m_pt_middle.x()+n_x, m_pt_middle.y()+n_y );
                pt_move_self_boundary( m_pt_move_start, m_pt_move_end );
                update();
                break;
            }
        }
    }
}

void IrFrame::pt_move_ana_rect( const QPoint &pt )
{
    QList<AnaInfo>::iterator it;
    for( it=m_list_anainfo.begin(); it!=m_list_anainfo.end(); it++ ) {
        if( it->n_sign_id == mn_sign_id ) {
            m_release_pt = pt;
            int n_x = pt.x()-m_press_pt.x();
            int n_y = pt.y()-m_press_pt.y();
            m_pt_move_start = QPoint( m_pt_start.x()+n_x, m_pt_start.y()+n_y );
            m_pt_move_end = QPoint( m_pt_middle.x()+n_x, m_pt_middle.y()+n_y );
            pt_move_self_boundary( m_pt_move_start, m_pt_move_end );
            update();
            break;
        }
    }
}

void IrFrame::pt_move_ana_circle( const QPoint &pt )
{
    if( me_ana_move == move_lefttop || me_ana_move == move_righttop ||
            me_ana_move == move_rightbottom || me_ana_move == move_leftbottom ) {
        QList<AnaInfo>::iterator it;
        for( it=m_list_anainfo.begin(); it!=m_list_anainfo.end(); it++ ) {
            if( it->n_sign_id == mn_sign_id ) {
                m_release_pt = pt;
                //int n_x = pt.x()-m_press_pt.x();
                //int n_y = pt.y()-m_press_pt.y();
                //m_pt_move_start = QPoint( m_pt_start.x()+n_x, m_pt_start.y()+n_y );
                //m_pt_move_end = QPoint( m_pt_middle.x()+n_x, m_pt_middle.y()+n_y );
                //pt_circle_move_self_boundary( m_pt_move_start, m_pt_move_end );

                int n_x = pt.x()-it->point_start.x();
                int n_y = pt.y()-it->point_start.y();
                int n_r = qSqrt( n_x*n_x+n_y*n_y );
                //QPoint pt_start, pt_end_temp;
               /* m_pt_move_start = QPoint( it->point_start.x()-n_r, it->point_start.y()-n_r );
                m_pt_move_end = QPoint( it->point_start.x()+n_r, it->point_start.y()+n_r );
                pt_draw_circle_boundary( m_pt_move_start );
                pt_draw_circle_boundary( m_pt_move_end );
                int n_r_x = (m_pt_move_end.x()-m_pt_move_start.x())/2;
                int n_r_y = (m_pt_move_end.y()-m_pt_move_start.y())/2;
                n_r = n_r_x<=n_r_y?n_r_x:n_r_y;*/
                int n_r_temp=0;
                start_area( it->point_start, n_r_temp );
                if( n_r <= n_r_temp ) {

                m_pt_move_end = QPoint( it->point_start.x()+n_r, it->point_start.y() );
                } else {
                    n_r = n_r_temp;
                    m_pt_move_end = QPoint( it->point_start.x()+n_r, it->point_start.y() );
                }

                update();
                break;
            }
        }
    } else if( me_ana_move == move_self ) {
        QList<AnaInfo>::iterator it;
        for( it=m_list_anainfo.begin(); it!=m_list_anainfo.end(); it++ ) {
            if( it->n_sign_id == mn_sign_id ) {
                //m_release_pt = pt;
                pt_mouse_move_boundary( pt, m_release_pt );
                int n_x = pt.x()-m_press_pt.x();
                int n_y = pt.y()-m_press_pt.y();
                m_pt_move_start = QPoint( m_pt_start.x()+n_x, m_pt_start.y()+n_y );
                m_pt_move_end = QPoint( m_pt_middle.x()+n_x, m_pt_middle.y()+n_y );
                pt_circle_move_self_boundary( m_pt_move_start, m_pt_move_end );
                update();
                break;
            }
        }
    }

}
void IrFrame::pt_mouse_move_boundary(const QPoint &pt , QPoint &pt_move)
{
    pt_move = pt;
    if( pt_move.x()<0 ) {
        pt_move.setX( 0 );
    } else if( pt.x()>width() ) {
        pt_move.setX( width()-1 );
    }
    if( pt_move.y()<0 ) {
        pt_move.setY( 0 );
    } else if( pt_move.y()>height() ) {
        pt_move.setY( height()-1 );
    }
}

void IrFrame::pt_add_release_boundary( const QPoint &pt, QPoint &pt_area )
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
    }

}

void IrFrame::pt_draw_circle_boundary( QPoint &pt_start )
{
    if( pt_start.x()<0 ) {
        pt_start.setX( 0 );
    } else if( pt_start.x()>width() ) {
        pt_start.setX( width()-1 );
    }

    if( pt_start.y()<0 ) {
        pt_start.setY( 0 );
    } else if( pt_start.y() > height() ) {
        pt_start.setY( height()-1 );
    }
}

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

void IrFrame::pt_move_self_start_boundary( const QPoint &pt , int x, int y, QPoint &pt_start, QPoint &pt_end )
{

}

void IrFrame::pt_move_self_end_boundary( const QPoint &pt , int x, int y, QPoint &pt_start, QPoint &pt_end )
{
    int n_x;
    int n_y;
    if( pt.x()<=0 && pt.y()>0 && pt.y()<height() ) {
        n_x = x+pt.x();
        pt_start = QPoint( 0, pt.y() );
        pt_end = QPoint( pt_end.x()+n_x, pt_end.y() );
    } else if( pt.x()<0 && pt.y()<0  ) {
        pt_start = QPoint( 0, 0 );
    } else if( pt.x()>0 && pt.x()<width() && pt.y()<=0 ) {
        pt_start = QPoint( pt.x(), 0 );
    } else if( pt.x()>width() && pt.y()<0 ) {
        pt_start = QPoint( width(), 0 );
    } else if( pt.x()>width() && pt.y()>0 && pt.y()<height() ) {
        pt_start = QPoint( width(), pt.y() );
    } else if( pt.x()>width() && pt.y()>height() ) {
        pt_start = QPoint( width(), height() );
    } else if( pt.x()>0 && pt.x()<width() && pt.y()>height() ) {
        pt_start = QPoint( pt.x(), height() );
    } else if( pt.x()<0 && pt.y()>height() ) {
        pt_start = QPoint( 0, height() );
    } else if( pt.x()>0 && pt.x()<width() && pt.y()>0 && pt.y()<height() ) {
        pt_start = pt;
    }
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
