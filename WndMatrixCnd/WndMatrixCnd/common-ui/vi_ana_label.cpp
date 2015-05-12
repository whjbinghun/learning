#include "vi_ana_label.h"
#include <QPainter>
#include <QPoint>
#include <QDebug>
#include "wndvipreview.h"

#define BTN_WIDTH 32

Vi_Ana_Label::Vi_Ana_Label(QWidget *parent) :
    QLabel(parent)
   ,mp_vi_preview( (WndViPreview*) parent )
   ,mn_press_status( -1 )
   ,mb_fullscreen_show( false )
{
    init_ir_widget();
}

Vi_Ana_Label::~Vi_Ana_Label()
{

}

void Vi_Ana_Label::init_ir_widget()
{
    add_btn( "拉近", 1 );
    add_btn( "拉远", 1 );
    add_btn( "抓拍", 1 );
    add_btn( "全屏", 1 );
#ifdef QT_DEBUG
    add_btn( "连主", 1 );
    add_btn( "连次", 1 );
    add_btn( "断主", 1 );
    add_btn( "断次", 1 );

//    add_btn( "连红", 1 );
//    add_btn( "断红", 1 );
#endif
}

int Vi_Ana_Label::get_wnd_width()
{
    return m_list.size() * BTN_WIDTH + 1;
}

void Vi_Ana_Label::add_btn( QString str_txt, int n_type, QString str_image )
{
    ViBtnInfo vi_btn_info;
    vi_btn_info.str_text = str_txt;
    vi_btn_info.str_image = str_image;
    vi_btn_info.n_type = n_type;

    m_list.push_back( vi_btn_info );
}

//设置全屏按钮的文本
void Vi_Ana_Label::set_fullscreen_text()
{
    if( mb_fullscreen_show ) {
        m_list[3].str_text = "恢复";
    } else {
        m_list[3].str_text = "全屏";
    }

}

void Vi_Ana_Label::mousePressEvent( QMouseEvent *event )
{
    if ( event->button() == Qt::LeftButton ) {
        press_status_shape( QPoint( event->x(), event->y() ) );
        mp_vi_preview->tool_bar_function( mn_press_status, mb_fullscreen_show, true );
    }
}

void Vi_Ana_Label::mouseMoveEvent(QMouseEvent *event)
{

}

void Vi_Ana_Label::mouseReleaseEvent( QMouseEvent *event )
{
    if ( event->button() == Qt::LeftButton ) {
        if( mn_press_status == 3 ) {
            if( mb_fullscreen_show ) {
                mb_fullscreen_show = false;
            } else {
                mb_fullscreen_show = true;
            }
            set_fullscreen_text();
        }
#ifdef QT_DEBUG
        else if ( mn_press_status == 4 ){
              mp_vi_preview->test_connect_main_stream();
        }
        else if ( mn_press_status == 5 ){
            mp_vi_preview->test_connect_sub_stream();
        }
        else if ( mn_press_status == 6 ){
            mp_vi_preview->test_disconnect_main_stream();
        }
        else if ( mn_press_status == 7 ){
            mp_vi_preview->test_disconnect_sub_stream();
        }
//        else if ( mn_press_status == 8 ){
//            mp_vi_preview->test_connect_ir();
//        }
//        else if ( mn_press_status == 9 ){
//            mp_vi_preview->test_disconnect_ir();
//        }
#endif
        mp_vi_preview->tool_bar_function( mn_press_status, mb_fullscreen_show, false );

        mn_press_status = -1;
        update();
    }
}


void Vi_Ana_Label::press_status_shape( QPoint pt )
{
    //如果鼠标点击 点 线位置
    QList< ViBtnInfo >::iterator it = m_list.begin();

    for( int n=0; it!=m_list.end(); ++n, ++it ) {
        if( pt.x() >= n*BTN_WIDTH && pt.x() <= (n+1)*BTN_WIDTH ) {
            mn_press_status = n;
            return ;
        }
    }
}

void Vi_Ana_Label::paintEvent( QPaintEvent *event )
{
    QPainter draw;
    draw.begin( this );

    draw.fillRect( rect(), QBrush( QColor(50,50,50) ) );//工具栏底色

    QList<ViBtnInfo>::iterator it;
    int j = 0;
    int n_height = height();
    for( it=m_list.begin(),j=0; it!=m_list.end(); it++,j++ ) {
        if( j == 0 && mn_press_status == 0 ) {
            draw.fillRect( BTN_WIDTH*j, 0, BTN_WIDTH, n_height, QBrush( QColor(177,1,1) ) );
        } else if( j== 1 && mn_press_status == 1 ) {
            draw.fillRect( BTN_WIDTH*j, 0, BTN_WIDTH, n_height, QBrush( QColor(177,1,1) ) );
        } else if( j== 2 && mn_press_status == 2 ) {
            draw.fillRect( BTN_WIDTH*j, 0, BTN_WIDTH, n_height, QBrush( QColor(177,1,1) ) );
        } else if( j == 3 && mn_press_status == 3 ) {
            draw.fillRect( BTN_WIDTH*j, 0, BTN_WIDTH, n_height, QBrush( QColor(177,1,1) ) );
        }
#ifdef QT_DEBUG
        else if( j == 4 && mn_press_status == 4 ) {
            draw.fillRect( BTN_WIDTH*j, 0, BTN_WIDTH, n_height, QBrush( QColor(177,1,1) ) );
        }else if( j == 5 && mn_press_status == 5 ) {
            draw.fillRect( BTN_WIDTH*j, 0, BTN_WIDTH, n_height, QBrush( QColor(177,1,1) ) );
        }else if( j == 6 && mn_press_status == 6 ) {
            draw.fillRect( BTN_WIDTH*j, 0, BTN_WIDTH, n_height, QBrush( QColor(177,1,1) ) );
        }else if( j == 7 && mn_press_status == 7 ) {
            draw.fillRect( BTN_WIDTH*j, 0, BTN_WIDTH, n_height, QBrush( QColor(177,1,1) ) );
        }/*else if( j == 8 && mn_press_status == 8 ) {
            draw.fillRect( BTN_WIDTH*j, 0, BTN_WIDTH, n_height, QBrush( QColor(177,1,1) ) );
        }else if( j == 9 && mn_press_status == 9 ) {
            draw.fillRect( BTN_WIDTH*j, 0, BTN_WIDTH, n_height, QBrush( QColor(177,1,1) ) );
        }*/
#endif
        else {
            draw.fillRect( BTN_WIDTH*j, 0, BTN_WIDTH, n_height, QBrush( QColor(0,0,0) ) );
        }

        draw.setPen( Qt::white );
        draw.drawText( BTN_WIDTH*j, 0, BTN_WIDTH, n_height, Qt::AlignCenter, (*it).str_text );
        draw.drawRect( BTN_WIDTH*j, 0, BTN_WIDTH, n_height-1 );
    }
    draw.end();
}

int Vi_Ana_Label::get_mouse_press_status()
{
    return mn_press_status;
}

void Vi_Ana_Label::set_mouse_press_status( int n_press_status )
{
    mn_press_status = n_press_status;
}

void Vi_Ana_Label::set_fullscreen_status( bool b_fullscreen )
{
    mb_fullscreen_show = b_fullscreen;
}

bool Vi_Ana_Label::get_fullscreen_status()
{
    return mb_fullscreen_show;
}
