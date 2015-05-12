#include "ir_ana_label.h"
#include <QPainter>
#include <QPoint>
#include <QDebug>
#include "deletelabel.h"
#include "mscclienttype.h"

#define BTN_WIDTH 32

Ir_Ana_Label::Ir_Ana_Label(QWidget *parent, bool b_hide_warn, bool b_hide_calibration ) :
    QLabel(parent)
  , mb_hide_warn( b_hide_warn )
  , mb_hide_calibration( b_hide_calibration )
  , mp_timer_focus( NULL )
  , mb_focus_far( false )
  , mn_down_btn( -1 )
{
    init_ir_btn();

    //定时器，用来定时发送调焦的命令
    mp_timer_focus = new QTimer(this);
    connect( mp_timer_focus, SIGNAL( timeout() ), this, SLOT( slot_timer_focus_far_or_near() ) );
}

Ir_Ana_Label::~Ir_Ana_Label()
{

}

void Ir_Ana_Label::init_ir_btn()
{
    add_btn( "点", 1 );
    add_btn( "线", 1 );
    add_btn( "框", 1 );
    if( mb_hide_calibration ) {
        add_btn( "-", -1 );
        add_btn( "+", -1 );
        add_btn( "校准", -1 );
    }

    //if( MSCClientType::Instance()->get_client_type() == ClientPresetSet ) {
        add_btn( "阵列分析", -1 );
    //}


    if ( mb_hide_warn ) {
        add_btn( "警告", 0 );
    }
}

int Ir_Ana_Label::get_wnd_width()
{
    if( m_list_btn.size() > 6 ) {
        return (m_list_btn.size()+2) * BTN_WIDTH + 1;
    }
    return m_list_btn.size() * BTN_WIDTH + 1;
}

void Ir_Ana_Label::add_btn( QString str_txt, int n_type, QString str_image )
{
    BtnInfo btn_info;
    btn_info.str_text = str_txt;
    btn_info.str_image = str_image;
    btn_info.n_type = n_type;

    m_list_btn.push_back( btn_info );
}

void Ir_Ana_Label::set_ir_ana_num( int n_num )
{
    for( int i=0; i<n_num; i++ ) {
        m_list_btn.pop_back();
    }
}

int Ir_Ana_Label::pt_in_btn( QPoint pt, BtnInfo &btn_info )
{
    QList< BtnInfo >::iterator it = m_list_btn.begin();

    for ( int n=0; it != m_list_btn.end(); ++n, ++it ) {
        if( n==6 ) {
            if ( pt.x() >= n*BTN_WIDTH && pt.x() <= (n+3)*BTN_WIDTH ) {
                btn_info = *it;
                return n;
            }
        } else if( n<6 ) {
            if ( pt.x() >= n*BTN_WIDTH && pt.x() <= (n+1)*BTN_WIDTH ) {
                btn_info = *it;
                return n;
            }
        }

    }

    return -1;
}

void Ir_Ana_Label::clear_down_status()
{
    if ( mn_down_btn >= 0 ) {
        mn_down_btn = -1;
        update();
    }
}

void Ir_Ana_Label::mousePressEvent( QMouseEvent *event )
{
    if ( event->button() == Qt::LeftButton ) {
        BtnInfo btn_info;
        int n_btn = pt_in_btn( event->pos(), btn_info );
        switch( n_btn ) {
        case 3:
            emit sig_modify_far();
            mb_focus_far = true;
            mp_timer_focus->start( 200 );
            break;
        case 4:
            emit sig_modify_near();
            mb_focus_far = false;
            mp_timer_focus->start( 200 );
            break;
        case 5:
            break;
        default: break;
        }
        mn_down_btn = n_btn;
    }
}

void Ir_Ana_Label::mouseMoveEvent(QMouseEvent *event)
{

}

void Ir_Ana_Label::mouseReleaseEvent( QMouseEvent *event )
{
    if ( event->button() == Qt::LeftButton ) {
        BtnInfo btn_info;
        int n_btn = pt_in_btn( event->pos(), btn_info );
        switch( n_btn ) {
        case 0: emit sig_add_point(); break;
        case 1: emit sig_add_line(); break;
        case 2: emit sig_add_rect(); break;
        case 3: mp_timer_focus->stop(); break;
        case 4: mp_timer_focus->stop(); break;
        case 5: emit sig_modify_calibration(); break;
        case 6:
            emit sig_add_matrix_ana();
            break;
        case 7: emit sig_modify_warning(); break;
        default: break;
        }

        if ( btn_info.n_type > 0 ) {
            mn_down_btn = n_btn;
        } else {
            mn_down_btn = -1;
        }
        emit sig_clear_btn_click();
        update();
    }
}

void Ir_Ana_Label::paintEvent( QPaintEvent *event )
{
    QPainter draw;
    draw.begin( this );

    QList< BtnInfo >::iterator it = m_list_btn.begin();

    draw.fillRect( rect(), QBrush( QColor(50,50,50) ) );

    int n_btn_width = BTN_WIDTH;
    int n_btn_height = height();

    for ( int n=0; it != m_list_btn.end(); ++n, ++it ) {
        BtnInfo btn_info = *it;

        if ( mn_down_btn == n ) {
            if( btn_info.n_type > 0 || btn_info.n_type == -1 ) {
                if( n<6 ) {
                    draw.fillRect( n_btn_width*n, 0, n_btn_width, n_btn_height, QBrush( QColor(177,1,1) ) );//红色
                } else if( n==6 ) {
                    draw.fillRect( n_btn_width*n, 0, n_btn_width*3, n_btn_height, QBrush( QColor(177,1,1) ) );//红色
                }
            }
        }

        draw.setPen( Qt::white );
        if( n< 6 ) {
            draw.drawText( n_btn_width*n, 0, n_btn_width, n_btn_height, Qt::AlignCenter, btn_info.str_text );
            draw.drawRect( n_btn_width*n, 0, n_btn_width, n_btn_height-1 );
        } else if( n==6 ) {
            draw.drawText( n_btn_width*n, 0, n_btn_width*3, n_btn_height, Qt::AlignCenter, btn_info.str_text );
            draw.drawRect( n_btn_width*n, 0, n_btn_width*3, n_btn_height-1 );
        }
    }

    draw.end();
}

void Ir_Ana_Label::resizeEvent( QResizeEvent *event )
{

}

void Ir_Ana_Label::slot_timer_focus_far_or_near()
{
    if( mb_focus_far ) {
        emit sig_modify_far();
    } else {
        emit sig_modify_near();
    }
}

int Ir_Ana_Label::get_press_btn_pos()
{
    return mn_down_btn;
}

void Ir_Ana_Label::set_press_btn_pos( int n_down_btn )
{
    mn_down_btn = n_down_btn;
}

void Ir_Ana_Label::update_ir_ana()
{
    update();
}

