#include "wndirimage.h"
#include <QPainter>
#include <QGraphicsOpacityEffect>
#include <QDialog>
#include <QtMath>
//#include <windef.h>
#include <QDockWidget>
#include <QWidgetAction>
#include <QListView>
#include <QDebug>
#include "datamode.h"
#include "../common-ui/dlgalarmconfig.h"
#include "../common-funtion/dataconfig.h"
#include <QFileDialog>
#include "string.h"

#define LABEL_HEIGHT 32
#define SPACE_IR  20
#define DRAW_COLOR_ALPHA 50  //颜色透明度

QMutex WndIrImage::m_mutex_sign;
int WndIrImage::mn_sign_id = 0;

WndIrImage::WndIrImage(QWidget *parent , int n_ui_type, bool b_hide_warn, bool b_hide_calibration) :
    QWidget(parent)
    ,mn_select_sign_id( -1 )
    ,ms_type_name( "" )
    ,me_draw_status( none_draw_status )
    ,me_mouse_press_status( none_press_status )
    ,ms_ir_sn( "" )
    ,mb_modify_status( false )
    ,mb_realtime( true )
    ,mp_timer_update(0)
    ,mp_ir_label( NULL )
    ,mp_delete_ana_label( NULL )
    ,mb_hide_warn( b_hide_warn )
    ,mb_hide_calibration( b_hide_calibration )
    ,mb_preset_setting( false )
    ,m_clr_ana( QColor( 0, 255, 255 ) )
    ,mn_line_width( 1 )
    ,mn_line_style( 1 )
    ,mn_ui_type( n_ui_type )
    ,mb_reset_preset_ana( true )
    ,mn_group_num( -1 )
    ,mb_press_ctrl( false )
{
   setWindowFlags(Qt::Widget);

   set_org_sz( 640, 480 );
   setMouseTracking( true );

   //ana_label
   init_ir_widget();

   init_connect();

   bool b_ret = false;
   b_ret = yf_ir_image_ex1_init( mh_ir_image );

}

WndIrImage::~WndIrImage()
{
    yf_ir_image_ex1_uninit( mh_ir_image );
}

void WndIrImage::init_ir_widget()
{
    mp_ir_label = new Ir_Ana_Label( this, mb_hide_warn, mb_hide_calibration );
    QGraphicsOpacityEffect *effect = new QGraphicsOpacityEffect( this );
    effect->setOpacity( 0.5 );
    mp_ir_label->setGraphicsEffect( effect );

    mp_delete_ana_label = new DeleteLabel( this );
    QGraphicsOpacityEffect *effect_delete = new QGraphicsOpacityEffect( this );
    effect_delete->setOpacity( 0.5 );
    mp_delete_ana_label->setGraphicsEffect( effect_delete );

    set_toolbar_hidden( true );
    mp_delete_ana_label->setHidden( true );
}

void WndIrImage::init_connect()
{
    //红外工具栏
    connect( mp_ir_label, SIGNAL( sig_add_point() ), this, SLOT( on_add_point() ) );
    connect( mp_ir_label, SIGNAL( sig_add_line() ), this, SLOT( on_add_line() ) );
    connect( mp_ir_label, SIGNAL( sig_add_rect() ), this, SLOT( on_add_rect() ) );
    connect( mp_delete_ana_label, SIGNAL( sig_delete_shape() ), this, SLOT( on_delete() ) );
}

void  WndIrImage::on_add_point()
{
    me_mouse_press_status=point_status;
    mp_delete_ana_label->set_delete_status( false );
}

void  WndIrImage::on_add_line()
{
    me_mouse_press_status=line_status;
    mp_delete_ana_label->set_delete_status( false );
}

void  WndIrImage::on_add_rect()
{
   me_mouse_press_status=rect_status;
   mp_delete_ana_label->set_delete_status( false );
 }

//删除
void  WndIrImage::on_delete()
{
   me_mouse_press_status=none_press_status;

   mp_ir_label->clear_down_status();

   QList<AnaInfo>::iterator it;
   int i;
   for( i=0, it=m_list_anainfo.begin(); it!=m_list_anainfo.end(); it++, i++ ) {
        if( (*it).n_sign_id == mn_select_sign_id ) {
            //删除分析
            QString str_shape;
            shape_to_string( str_shape, (*it) );
            bool b_delete_status = yf_ir_image_ex1_delete_ana( mh_ir_image, str_shape.toStdString() );

            //if( b_delete_status ) {
                m_list_anainfo.removeAt( i );
                mp_delete_ana_label->set_delete_status( false );
                mp_delete_ana_label->setHidden( true );
            //}
            update();
            break;
        }
   }

}

void WndIrImage::resizeEvent( QResizeEvent *event )
{
    mf_sz_width = width()/(float)mn_org_width;
    mf_sz_height = height()/(float)mn_org_height;

    mp_ir_label->move( SPACE_IR, SPACE_IR );
    mp_ir_label->resize( mp_ir_label->get_wnd_width(), LABEL_HEIGHT );
    mp_delete_ana_label->move( width()-SPACE_IR*2-1, SPACE_IR );
    mp_delete_ana_label->resize( LABEL_HEIGHT, LABEL_HEIGHT );
}

void WndIrImage::paintEvent( QPaintEvent *event )
{
    QPainter draw;
    draw.begin( this );

    QRect rct_ir( 0, 0, width(), height() );
    draw.setPen( QColor( 66, 123, 122 ) );//66, 123, 122 ) );

    if( m_ir_image.isNull() ) {
        //draw.setPen( Qt::white );
        if ( m_str_ir_path.isEmpty() ) {
            draw.drawText( rct_ir, Qt::AlignCenter, "没有红外图片" );
        } else {
            draw.drawText( rct_ir, Qt::AlignCenter, "红外图片错误" );
        }
    } else {
        draw.drawImage( rct_ir, m_ir_image );
        //画预置点分析
        draw_matrix_anas( draw );
        draw_preset_anas( draw );
        draw_anas(draw);
    }

    //draw.setPen( QColor( 0, 0, 0 ) );//66, 123, 122 ) );
    draw.setPen( Qt::white );
    draw.drawRect( QRect( 0, 0, width()-1, height()-1 ) );

    draw.end();
}

void WndIrImage::draw_anas(QPainter &draw)
{
    draw.setPen( QColor( 0, 255, 255 ) );

    //红外温度分析
    int n_point_area = 10;
    int n_other_area = 2;
    int n_frame_size = 4;//小框的大小

    //新增
    switch ( me_draw_status ) {
    case draw_point_status:
        draw.drawPoint( m_pt_middle.x(), m_pt_middle.y() );
        draw.drawLine( QPoint(m_pt_middle.x()-n_point_area,m_pt_middle.y()), QPoint(m_pt_middle.x()+n_point_area,m_pt_middle.y()) );
        draw.drawLine( QPoint(m_pt_middle.x(), m_pt_middle.y()-n_point_area), QPoint(m_pt_middle.x(),m_pt_middle.y()+n_point_area) );
        break;
    case draw_line_status:
        draw.drawLine( m_pt_start, m_pt_middle );

        draw.drawRect( m_pt_start.x()-n_other_area, m_pt_start.y()-n_other_area, n_frame_size, n_frame_size );
        draw.drawRect( m_pt_middle.x()-n_other_area, m_pt_middle.y()-n_other_area, n_frame_size, n_frame_size );
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

    int temp_width = 100;
    int temp_height = 15;
    QList<AnaInfo>::iterator it;
    for( it=m_list_anainfo.begin(); it!=m_list_anainfo.end(); it++ ) {

        AnaInfo &ana_info = *it;

        QString str_shape;
        shape_to_string( str_shape, ana_info );
        unsigned short us_x, us_y;
        float f_temp;
        yf_ir_image_ex1_get_ana_max_temp( mh_ir_image, str_shape.toStdString(), us_x, us_y, f_temp );
        QString str_text = get_ana_draw_text( ana_info, f_temp, true );//温度显示文本

        if ( !ana_info.b_show ) continue;
        QPen pen;
        //if ( ana_info.color[0]  != ana_info.color[1] ){
       //    pen.setColor( ana_info.color[ ana_info.colorindex%2] );
        //} else {
            if ( ana_info.b_preset_ana ) {
                pen.setColor( m_clr_ana );
                pen.setStyle( (Qt::PenStyle)mn_line_style );
                pen.setWidth( mn_line_width );
            } else {
                pen.setColor( QColor( 0, 255, 255 ) );
            }
       // }
        draw.setPen( pen );
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

        QPoint pt_draw_text;

        if( it->type_name == "point" ) {
            draw.drawPoint( n_pt_end_x, n_pt_end_y );
            draw.drawLine( QPoint(n_pt_end_x-n_point_area,n_pt_end_y), QPoint(n_pt_end_x+n_point_area,n_pt_end_y) );
            draw.drawLine( QPoint(n_pt_end_x, n_pt_end_y-n_point_area), QPoint(n_pt_end_x,n_pt_end_y+n_point_area) );
            if(  me_ana_move!=move_null && (*it).n_sign_id == mn_select_sign_id ) {
                draw.drawRect( n_pt_end_x-n_other_area,n_pt_end_y-n_other_area, n_frame_size, n_frame_size );
            }
            pt_draw_text = QPoint( n_pt_start_x, n_pt_start_y-temp_height );
        } else if( it->type_name == "line" ) {
            draw.drawLine( pt_start, pt_end );
            if( me_ana_move!=move_null && (*it).n_sign_id == mn_select_sign_id ) {
                draw.drawRect( n_pt_start_x-n_other_area,n_pt_start_y-n_other_area, n_frame_size, n_frame_size );
                draw.drawRect( n_pt_end_x-n_other_area,n_pt_end_y-n_other_area, n_frame_size, n_frame_size );
            }

            if( n_pt_start_y < n_pt_end_y ) {
                pt_draw_text = QPoint( n_pt_start_x, n_pt_start_y-temp_height );
            } else {
                pt_draw_text = QPoint( n_pt_end_x, n_pt_end_y-temp_height );
            }
        } else if( it->type_name == "rect" ) {
            draw.drawRect( QRect( n_pt_start_x, n_pt_start_y, n_pt_end_x-n_pt_start_x, n_pt_end_y-n_pt_start_y ));
            if( me_ana_move!=move_null && (*it).n_sign_id == mn_select_sign_id ) {
                draw.drawRect( n_pt_start_x-n_other_area, n_pt_start_y-n_other_area, n_frame_size, n_frame_size );
                draw.drawRect( n_pt_end_x-n_other_area, n_pt_end_y-n_other_area, n_frame_size, n_frame_size );
                draw.drawRect( n_pt_start_x-n_other_area, n_pt_end_y-n_other_area, n_frame_size, n_frame_size );
                draw.drawRect( n_pt_end_x-n_other_area, n_pt_start_y-n_other_area, n_frame_size, n_frame_size );
            }

            pt_draw_text = QPoint( n_pt_start_x, n_pt_start_y-temp_height );
        } else if( it->type_name == "circle" ) {
            int n_x = n_pt_end_x-n_pt_start_x;
            int n_y = n_pt_end_y-n_pt_start_y;
            int n_r = (int)qSqrt( n_x*n_x + n_y*n_y );
            draw.drawEllipse( n_pt_start_x-n_r, n_pt_start_y-n_r, n_r*2, n_r*2 );
            if( me_ana_move!=move_null && (*it).n_sign_id == mn_select_sign_id ) {
                draw.drawRect( n_pt_start_x-n_other_area-n_r, n_pt_start_y-n_other_area, n_frame_size, n_frame_size );
                draw.drawRect( n_pt_start_x-n_other_area,n_pt_start_y-n_other_area-n_r, n_frame_size, n_frame_size );
                draw.drawRect( n_pt_start_x-n_other_area+n_r, n_pt_start_y-n_other_area, n_frame_size, n_frame_size );
                draw.drawRect( n_pt_start_x-n_other_area, n_pt_start_y-n_other_area+n_r, n_frame_size, n_frame_size );
            }

           pt_draw_text = QPoint( n_pt_start_x-n_r, n_pt_start_y-n_r-temp_height );
        }

        draw.drawText( pt_draw_text.x(), pt_draw_text.y(), temp_width, temp_height, Qt::AlignLeft,  str_text );
    }

    draw.setPen( Qt::red );
    if( ms_type_name == "point" ) {
        draw.drawPoint( m_pt_move_end.x(), m_pt_move_end.y() );
        draw.drawLine( QPoint(m_pt_move_end.x()-n_point_area,m_pt_move_end.y()), QPoint(m_pt_move_end.x()+n_point_area,m_pt_move_end.y()) );
        draw.drawLine( QPoint(m_pt_move_end.x(), m_pt_move_end.y()-n_point_area), QPoint(m_pt_move_end.x(), m_pt_move_end.y()+n_point_area) );
        if( me_ana_move!=move_null ) {
            draw.drawRect( m_pt_move_end.x()-n_other_area, m_pt_move_end.y()-n_other_area, n_frame_size, n_frame_size );
        }
    } else if( ms_type_name == "line" ) {
        if( me_ana_move == move_self ) {
            draw.drawLine( m_pt_move_start, m_pt_move_end );
            draw.drawRect( m_pt_move_start.x()-n_other_area, m_pt_move_start.y()-n_other_area, n_frame_size, n_frame_size );
            draw.drawRect( m_pt_move_end.x()-n_other_area, m_pt_move_end.y()-n_other_area, n_frame_size, n_frame_size );
        } else if( me_ana_move == move_left ) {
            draw.drawLine( m_pt_move_start, m_pt_middle );
            draw.drawRect( m_pt_move_start.x()-n_other_area, m_pt_move_start.y()-n_other_area, n_frame_size, n_frame_size );
            draw.drawRect( m_pt_middle.x()-n_other_area, m_pt_middle.y()-n_other_area, n_frame_size, n_frame_size );
        } else if( me_ana_move == move_right ) {
            draw.drawLine( m_pt_start, m_pt_move_end );
            draw.drawRect( m_pt_start.x()-n_other_area, m_pt_start.y()-n_other_area, n_frame_size, n_frame_size );
            draw.drawRect( m_pt_move_end.x()-n_other_area, m_pt_move_end.y()-n_other_area, n_frame_size, n_frame_size );
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
            draw.drawRect( m_pt_move_start.x()-n_other_area, m_pt_move_start.y()-n_other_area, n_frame_size, n_frame_size );
            draw.drawRect( m_pt_move_end.x()-n_other_area, m_pt_move_end.y()-n_other_area, n_frame_size, n_frame_size );
            draw.drawRect( m_pt_move_start.x()-n_other_area, m_pt_move_end.y()-n_other_area, n_frame_size, n_frame_size );
            draw.drawRect( m_pt_move_end.x()-n_other_area, m_pt_move_start.y()-n_other_area, n_frame_size, n_frame_size );
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
                draw.drawRect( m_pt_start.x()-n_other_area-n_r, m_pt_start.y()-n_other_area, n_frame_size, n_frame_size );
                draw.drawRect( m_pt_start.x()-n_other_area, m_pt_start.y()-n_other_area-n_r, n_frame_size, n_frame_size );
                draw.drawRect( m_pt_start.x()-n_other_area+n_r, m_pt_start.y()-n_other_area, n_frame_size, n_frame_size );
                draw.drawRect( m_pt_start.x()-n_other_area, m_pt_start.y()-n_other_area+n_r, n_frame_size, n_frame_size );
            }
        } else if( me_ana_move == move_self ) {
            n_point_x = m_pt_move_end.x()-m_pt_move_start.x();
            n_point_y = m_pt_move_end.y()-m_pt_move_start.y();
            n_r = (int)qSqrt( n_point_x*n_point_x + n_point_y*n_point_y );
            draw.drawEllipse( m_pt_move_start.x()-n_r, m_pt_move_start.y()-n_r, n_r*2, n_r*2 );
            if( me_ana_move!=move_null ) {
                draw.drawRect( m_pt_move_start.x()-n_other_area-n_r, m_pt_move_start.y()-n_other_area, n_frame_size, n_frame_size );
                draw.drawRect( m_pt_move_start.x()-n_other_area, m_pt_move_start.y()-n_other_area-n_r, n_frame_size, n_frame_size );
                draw.drawRect( m_pt_move_start.x()-n_other_area+n_r, m_pt_move_start.y()-n_other_area, n_frame_size, n_frame_size );
                draw.drawRect( m_pt_move_start.x()-n_other_area, m_pt_move_start.y()-n_other_area+n_r, n_frame_size, n_frame_size );
            }
        }
    }
}

void WndIrImage::draw_preset_anas( QPainter &draw )
{
    //红外温度分析
    int n_point_area = 10;
    int n_other_area = 2;
    int n_frame_size = 4;//小框的大小
    int temp_width = 100;
    int temp_height = 15;

    QList<AnaInfo>::iterator it = m_list_pre_anainfo.begin();
    for( ; it!=m_list_pre_anainfo.end(); it++ ) {
        //it.str_ana//有好几个分析
        AnaInfo &ana_info = *it;

        QString str_shape;
        shape_to_string( str_shape, ana_info );
        unsigned short us_x, us_y;
        float f_temp;
        yf_ir_image_ex1_get_ana_max_temp( mh_ir_image, str_shape.toStdString(), us_x, us_y, f_temp );
        QString str_text = get_ana_draw_text( ana_info, f_temp, true );//温度显示文本

        if ( !ana_info.b_show ) continue;
        QPen pen;

        //if ( ana_info.color[0]  != ana_info.color[1] ){
        //    pen.setColor( ana_info.color[ ana_info.colorindex%2] );
        //} else {
            if ( ana_info.b_preset_ana ) {
                //m_clr_ana.setAlpha( DRAW_COLOR_ALPHA );
                pen.setColor( m_clr_ana );
                pen.setStyle( (Qt::PenStyle)mn_line_style );
                pen.setWidth( mn_line_width );
            } else {
                pen.setColor( QColor( 0, 255, 255 ) );
            }
        //}

        draw.setPen( pen );
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

        QPoint pt_draw_text;

        if( it->type_name == "point" ) {
            draw.drawPoint( n_pt_end_x, n_pt_end_y );
            draw.drawLine( QPoint(n_pt_end_x-n_point_area,n_pt_end_y), QPoint(n_pt_end_x+n_point_area,n_pt_end_y) );
            draw.drawLine( QPoint(n_pt_end_x, n_pt_end_y-n_point_area), QPoint(n_pt_end_x,n_pt_end_y+n_point_area) );
            if(  me_ana_move!=move_null && (*it).n_sign_id == mn_select_sign_id ) {
                draw.drawRect( n_pt_end_x-n_other_area,n_pt_end_y-n_other_area, n_frame_size, n_frame_size );
            }
            pt_draw_text = QPoint( n_pt_start_x, n_pt_start_y-temp_height );
        } else if( it->type_name == "line" ) {
            draw.drawLine( pt_start, pt_end );
            if( me_ana_move!=move_null && (*it).n_sign_id == mn_select_sign_id ) {
                draw.drawRect( n_pt_start_x-n_other_area,n_pt_start_y-n_other_area, n_frame_size, n_frame_size );
                draw.drawRect( n_pt_end_x-n_other_area,n_pt_end_y-n_other_area, n_frame_size, n_frame_size );
            }

            if( n_pt_start_y < n_pt_end_y ) {
                pt_draw_text = QPoint( n_pt_start_x, n_pt_start_y-temp_height );
            } else {
                pt_draw_text = QPoint( n_pt_end_x, n_pt_end_y-temp_height );
            }
        } else if( it->type_name == "rect" ) {
            draw.drawRect( QRect( n_pt_start_x, n_pt_start_y, n_pt_end_x-n_pt_start_x, n_pt_end_y-n_pt_start_y ));
            if( me_ana_move!=move_null && (*it).n_sign_id == mn_select_sign_id ) {
                draw.drawRect( n_pt_start_x-n_other_area, n_pt_start_y-n_other_area, n_frame_size, n_frame_size );
                draw.drawRect( n_pt_end_x-n_other_area, n_pt_end_y-n_other_area, n_frame_size, n_frame_size );
                draw.drawRect( n_pt_start_x-n_other_area, n_pt_end_y-n_other_area, n_frame_size, n_frame_size );
                draw.drawRect( n_pt_end_x-n_other_area, n_pt_start_y-n_other_area, n_frame_size, n_frame_size );
            }

            pt_draw_text = QPoint( n_pt_start_x, n_pt_start_y-temp_height );
        } else if( it->type_name == "circle" ) {
            int n_x = n_pt_end_x-n_pt_start_x;
            int n_y = n_pt_end_y-n_pt_start_y;
            int n_r = (int)qSqrt( n_x*n_x + n_y*n_y );
            draw.drawEllipse( n_pt_start_x-n_r, n_pt_start_y-n_r, n_r*2, n_r*2 );
            if( me_ana_move!=move_null && (*it).n_sign_id == mn_select_sign_id ) {
                draw.drawRect( n_pt_start_x-n_other_area-n_r, n_pt_start_y-n_other_area, n_frame_size, n_frame_size );
                draw.drawRect( n_pt_start_x-n_other_area,n_pt_start_y-n_other_area-n_r, n_frame_size, n_frame_size );
                draw.drawRect( n_pt_start_x-n_other_area+n_r, n_pt_start_y-n_other_area, n_frame_size, n_frame_size );
                draw.drawRect( n_pt_start_x-n_other_area, n_pt_start_y-n_other_area+n_r, n_frame_size, n_frame_size );
            }

           pt_draw_text = QPoint( n_pt_start_x-n_r, n_pt_start_y-n_r-temp_height );
        }

        draw.drawText( pt_draw_text.x(), pt_draw_text.y(), temp_width, temp_height, Qt::AlignLeft,  str_text );
    }
}

void WndIrImage::draw_matrix_anas( QPainter &draw )
{
    QList<AnaInfo>::iterator it = m_list_matrix_anainfo.begin();
    int i=0;
    for( ; it!=m_list_matrix_anainfo.end(); it++ ) {
        AnaInfo &ana_info = *it;
        QString str_shape;
        shape_to_string( str_shape, ana_info );
        unsigned short us_x, us_y;
        float f_temp;
        yf_ir_image_ex1_get_ana_max_temp( mh_ir_image, str_shape.toStdString(), us_x, us_y, f_temp );

        if ( !ana_info.b_show ) continue;

        //按当前窗口的大小比例缩放

        int n_pt_start_x = ana_info.point_start.x()*mf_sz_width;
        int n_pt_start_y = ana_info.point_start.y()*mf_sz_height;
        int n_pt_end_x = ana_info.point_end.x()*mf_sz_width;
        int n_pt_end_y = ana_info.point_end.y()*mf_sz_height;
        QPoint pt_start = QPoint( n_pt_start_x, n_pt_start_y );
        QPoint pt_end = QPoint( n_pt_end_x, n_pt_end_y );
        //判断当前窗口的边界
        judge_area( pt_start );
        judge_area( pt_end );
        n_pt_start_x = pt_start.x();
        n_pt_start_y = pt_start.y();
        n_pt_end_x = pt_end.x();
        n_pt_end_y = pt_end.y();

        QColor clr = m_map_matrix_ana_clr[ ana_info.n_sign_id ];

        draw.fillRect( n_pt_start_x, n_pt_start_y, n_pt_end_x-n_pt_start_x, n_pt_end_y-n_pt_start_y, QBrush( clr ) );
        //qDebug()<<"WndIrImage::draw_matrix_anas"<<i++<<pt_start<<pt_end;
\
        QColor cl( 0, 0, 0, DRAW_COLOR_ALPHA );
        draw.setPen( cl );
        draw.drawRect(  n_pt_start_x, n_pt_start_y, n_pt_end_x-n_pt_start_x, n_pt_end_y-n_pt_start_y );

        draw.drawText( n_pt_start_x, n_pt_start_y, n_pt_end_x-n_pt_start_x, n_pt_end_y-n_pt_start_y, Qt::AlignCenter,  QString::number( f_temp, '.', 1 ) );
    }
}

void WndIrImage::delete_all_ana()
{
    m_list_anainfo.clear();
}

void WndIrImage::mousePressEvent( QMouseEvent *event )
{
    if ( event->button() == Qt::LeftButton ) {
        QPointF pt_press_local = event->localPos();

        press_ir_template_ana( QPoint( event->x(), event->y() ) );
    } else if ( event->button() == Qt::RightButton ) {//修改分析别名

        AnaInfo ana_info;
        AnaMove ana_move;
        if ( pt_in_ana( event->pos(), ana_move, ana_info ) ) {
            QPoint pt_global = mapToGlobal( event->pos() );
            QString s_name;
        }
    }

}

void WndIrImage::mouseMoveEvent( QMouseEvent *event )
{
    delete_ana_shape( QPoint( event->x(), event->y() ) );
    //红外温度分析
    move_ir_template_ana( QPoint( event->x(), event->y() ) );
}

void WndIrImage::mouseReleaseEvent( QMouseEvent *event )
{
    if ( event->button() == Qt::LeftButton ) {
        qDebug()<<"WndIrPreview::mouseReleaseEvent";
        release_ir_template_ana( QPoint( event->x(), event->y() ) );
        if( !mb_press_ctrl ) {
            mp_ir_label->clear_down_status();
        }
        update();
    }
}

void WndIrImage::leaveEvent(QEvent *event)
{
    clear_ana_press_status();
}

//画框的时候，永远把点位置靠近左上角的放前面，靠近右下角的放后面
void WndIrImage::normalized_pt( QPoint &pt1, QPoint &pt2 )
{
    QRect rct( pt1, pt2 );
    rct = rct.normalized();//得出对称点，topleft点和bottomRight
    pt1 = rct.topLeft();
    pt2 = rct.bottomRight();
}

void WndIrImage::draw_add_shape( QPoint pt )
{
    pt_judge_area( pt, m_pt_start );
    m_pt_middle = pt;
    me_draw_status = (enum_draw_status)me_mouse_press_status;
}

bool WndIrImage::pt_in_ana( const QPoint &pt, AnaMove &ana_move, AnaInfo &ana_info )
{
    QList<AnaInfo>::iterator it = m_list_anainfo.begin();
    for( ; it != m_list_anainfo.end(); ++it ) {
        AnaInfo &ana_info = (*it);
        if ( !ana_info.b_ctrl ) { continue; }
        if ( !ana_info.b_show ) { continue; }
        if ( !mb_preset_setting && ana_info.b_preset_ana ) { continue; }

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
AnaMove WndIrImage::pt_in_ana_point(const QPoint &pt, const AnaInfo &ana_info)
{
    QPoint pt_end = QPoint( (int)ana_info.point_end.x()*mf_sz_width, (int)ana_info.point_end.y()*mf_sz_height );
    judge_area( pt_end );
    int n_area = 10;
    if( pt.x()>pt_end.x()-n_area && pt.x()<=pt_end.x()+n_area &&
            pt.y()>pt_end.y()-n_area && pt.y()<=pt_end.y()+n_area ) {
        return move_self;
    }
    return move_null;
}

//对线的分析
AnaMove WndIrImage::pt_in_ana_line( const QPoint &pt, const AnaInfo &ana_info )
{
    int n_area = 5;
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

AnaMove WndIrImage::pt_in_ana_rect( const QPoint &pt, const AnaInfo &ana_info )
{
    QPoint pt_start = QPoint( (int)ana_info.point_start.x()*mf_sz_width, (int)ana_info.point_start.y()*mf_sz_height );
    QPoint pt_end = QPoint( (int)ana_info.point_end.x()*mf_sz_width, (int)ana_info.point_end.y()*mf_sz_height );
    judge_area( pt_start );
    judge_area( pt_end );
    int n_start_x = pt_start.x();
    int n_start_y = pt_start.y();
    int n_end_x = pt_end.x();
    int n_end_y = pt_end.y();
    int n_area = 8;
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

AnaMove WndIrImage::pt_in_ana_circle( const QPoint &pt, const AnaInfo &ana_info )
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

void WndIrImage::get_pt_ana_shape( AnaInfo &ana_info, const QPoint &pt )
{
    mn_select_sign_id = ana_info.n_sign_id;//获得选中分析的id
    m_press_pt = pt;
    m_pt_start = QPoint( (int)ana_info.point_start.x()*mf_sz_width, (int)ana_info.point_start.y()*mf_sz_height );
    m_pt_middle = QPoint( (int)ana_info.point_end.x()*mf_sz_width, (int)ana_info.point_end.y()*mf_sz_height );
    judge_area( m_pt_start );
    judge_area( m_pt_middle );
    m_pt_move_start = m_pt_start;
    m_pt_move_end = m_pt_middle;
    //qDebug()<<"IrFrame::get_pt_ana_shape"<<ana_info.point_start<<ana_info.point_end<<m_pt_start<<m_pt_middle<<mf_sz_width<<mf_sz_height;
    mp_delete_ana_label->setHidden( false );
}

void WndIrImage::pt_move_ana_point( const QPoint &pt )
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

void WndIrImage::pt_move_ana_line( const QPoint &pt )
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

void WndIrImage::pt_move_ana_rect( const QPoint &pt )
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

void WndIrImage::pt_move_ana_circle( const QPoint &pt )
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
void WndIrImage::pt_judge_area( const QPoint &pt, QPoint &pt_area )
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
void WndIrImage::judge_area( QPoint &pt_area )
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
void WndIrImage::pt_move_self_boundary( QPoint &pt_start, QPoint &pt_end )
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
void WndIrImage::pt_circle_move_self_boundary( QPoint &pt_circle_center, QPoint &pt_end )
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
}

void WndIrImage::start_area( QPoint &pt_start, int &n_r )
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

void WndIrImage::shape_append_list( QPoint pt )
{
    QString str_type_name;
    QPoint pt_start, pt_end;

    switch ( me_draw_status ) {
    case draw_point_status: str_type_name = "point"; break;
    case draw_line_status: str_type_name = "line"; break;
    case draw_rect_status: str_type_name = "rect"; break;
    case draw_circle_status: str_type_name = "circle"; break;
    default: return;
    }

    pt_start = m_pt_start;
    pt_judge_area( pt, pt_end );

    if( me_draw_status == draw_point_status ) {
        pt_start = pt_end;
    } else if ( me_draw_status == draw_rect_status ) {
        normalized_pt( pt_start, pt_end );
    } else if( me_draw_status == draw_circle_status ) {
        int n_x = pt.x()-m_pt_start.x();
        int n_y = pt.y()-m_pt_start.y();
        int n_r = qSqrt( n_x*n_x+n_y*n_y );
        int n_r_temp=0;
        start_area( m_pt_start, n_r_temp );
        if( n_r<=n_r_temp ) {
            pt_end = QPoint( m_pt_start.x()+n_r, m_pt_start.y() );
        } else {
            n_r = n_r_temp;
            pt_end = QPoint( m_pt_start.x()+n_r, m_pt_start.y() );
        }
    }

    pt_start = QPoint( (int)( pt_start.x()/mf_sz_width+0.5 ), (int)( pt_start.y()/mf_sz_height+0.5 ) );
    pt_end = QPoint( (int)( pt_end.x()/mf_sz_width+0.5), (int)( pt_end.y()/mf_sz_height+0.5 ) );
    judge_org_area( pt_start );
    judge_org_area( pt_end );

    me_draw_status = none_draw_status;
    if( !mb_press_ctrl ) {
        me_mouse_press_status=none_press_status ;
    }

    AnaInfo ana_info;
    ana_info.type_name = str_type_name;
    ana_info.n_sign_id = WndIrImage::get_sign_id();
    ana_info.b_preset_ana = mb_preset_setting;
    ana_info.point_start = pt_start;
    ana_info.point_end = pt_end;

    add_ana( ana_info );
}

//图形移动或者改变图形size
void WndIrImage::shape_move_or_change_size( QPoint pt )
{
    QList<AnaInfo>::iterator it;
    for( it=m_list_anainfo.begin(); it!=m_list_anainfo.end(); it++ ) {
        int n_x = pt.x()-m_press_pt.x();
        int n_y = pt.y()-m_press_pt.y();
        if( mn_select_sign_id == it->n_sign_id ) {
            if( ms_type_name == "point" ) {
                pt_judge_area( QPoint( m_pt_start.x()+n_x, m_pt_start.y()+n_y ), m_pt_start );
                m_pt_release_start = QPoint( (int)m_pt_start.x()/mf_sz_width+0.5, (int)m_pt_start.y()/mf_sz_height+0.5 );
                judge_org_area( m_pt_release_start );
                m_pt_release_end = m_pt_release_start;
            } else if( ms_type_name == "line" ) {
                if( me_ana_move == move_left ) {
                    pt_judge_area( QPoint( m_pt_start.x()+n_x, m_pt_start.y()+n_y ), m_pt_start );
                } else if( me_ana_move == move_right ) {
                    pt_judge_area( QPoint( m_pt_middle.x()+n_x, m_pt_middle.y()+n_y ), m_pt_middle );
                } else if( me_ana_move == move_self ) {
                    m_pt_start = QPoint( m_pt_start.x()+n_x, m_pt_start.y()+n_y );
                    m_pt_middle = QPoint( m_pt_middle.x()+n_x, m_pt_middle.y()+n_y );
                    pt_move_self_boundary( m_pt_start, m_pt_middle );
                }
                m_pt_release_start = QPoint( (int)(m_pt_start.x()/mf_sz_width+0.5), (int)(m_pt_start.y()/mf_sz_height+0.5) );
                m_pt_release_end = QPoint( (int)(m_pt_middle.x()/mf_sz_width+0.5), (int)(m_pt_middle.y()/mf_sz_height+0.5) );
                judge_org_area( m_pt_release_start );
                judge_org_area( m_pt_release_end );
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
                m_pt_release_start = QPoint( (int)m_pt_start.x()/mf_sz_width+0.5, (int)m_pt_start.y()/mf_sz_height+0.5 );
                m_pt_release_end = QPoint( (int)m_pt_middle.x()/mf_sz_width+0.5, (int)m_pt_middle.y()/mf_sz_height+0.5 );
                judge_org_area( m_pt_release_start );
                judge_org_area( m_pt_release_end );
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
                } else if( me_ana_move == move_self ) {
                    n_x = pt.x()-m_press_pt.x();
                    n_y = pt.y()-m_press_pt.y();
                    m_pt_start = QPoint( m_pt_start.x()+n_x, m_pt_start.y()+n_y );
                    m_pt_middle = QPoint( m_pt_middle.x()+n_x, m_pt_middle.y()+n_y );
                    pt_circle_move_self_boundary( m_pt_start, m_pt_middle );
                }
                m_pt_release_start = QPoint( (int)m_pt_start.x()/mf_sz_width+0.5, (int)m_pt_start.y()/mf_sz_height+0.5 );
                m_pt_release_end = QPoint( (int)m_pt_middle.x()/mf_sz_width+0.5, (int)m_pt_middle.y()/mf_sz_height+0.5 );
                judge_org_area( m_pt_release_start );
                judge_org_area( m_pt_release_end );

            } else {
                continue;
            }

            mb_modify_status = true;
            AnaInfo temp_ana_info = (*it);
            /*temp_ana_info.b_ctrl = it->b_ctrl;
            temp_ana_info.b_preset_ana = it->b_preset_ana;
            temp_ana_info.b_show = it->b_show;
            temp_ana_info.str_name = it->str_name;
            temp_ana_info.n_sign_id = it->n_sign_id;
            temp_ana_info.point_start = m_pt_release_start;
            temp_ana_info.point_end = m_pt_release_end;
            temp_ana_info.b_preset_ana = mb_preset_setting;
            temp_ana_info.type_name = it->type_name;
            */
            (*it).point_start = m_pt_release_start;
            (*it).point_end = m_pt_release_end;
            (*it).b_preset_ana = mb_preset_setting;
            //change 分析
            change_ana( *it, temp_ana_info );

            break;
        }
    }
}

void WndIrImage::set_org_sz( int width, int height )
{
    mn_org_width = width;
    mn_org_height = height;
}

//判断pt_area点是否在原始区域内
void WndIrImage::judge_org_area( QPoint &pt_area )
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

//鼠标按下时，获得的红外分析
void WndIrImage::press_ir_template_ana( QPoint pt )
{
    enum_press_status en_status = (enum_press_status)me_mouse_press_status;

    if ( en_status != none_press_status ) {
        draw_add_shape( pt );
        setCursor( Qt::ArrowCursor );
        update();
    } else {
        AnaInfo p_ana;
        bool b_in_ana = pt_in_ana( pt, me_ana_move, p_ana );
        if ( b_in_ana ) {
            if( p_ana.type_name == "point" ) {
                get_pt_ana_shape( p_ana, pt );
            } else if( p_ana.type_name == "line" ) {
                get_pt_ana_shape( p_ana, pt );
            } else if( p_ana.type_name == "rect" ) {
                get_pt_ana_shape( p_ana, pt );
            } else if( p_ana.type_name == "circle" ) {
                get_pt_ana_shape( p_ana, pt );
            }
            ms_type_name = p_ana.type_name;
        } else { //如果没有选中分析，则隐藏垃圾桶
            mp_delete_ana_label->setHidden( true );
            mn_select_sign_id = -1;
        }
        update();
    }
}

void WndIrImage::move_ir_template_ana( QPoint pt )
{
    switch ( me_draw_status ) {
    case draw_point_status:
        pt_judge_area( pt, m_pt_middle );
        update();
        break;
    case draw_line_status:
        pt_judge_area( pt, m_pt_middle );
        update();
        break;
    case draw_rect_status:
        pt_judge_area( pt, m_pt_middle );
        update();
        break;
    case draw_circle_status:
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
            pt_move_ana_point( pt );
        } else if( ms_type_name == "line" ) {
            pt_move_ana_line( pt );
        } else if( ms_type_name == "rect" ) {
            pt_move_ana_rect( pt );
        } else if( ms_type_name == "circle" ) {
            pt_move_ana_circle( pt );
        }
    }
}

void WndIrImage::release_ir_template_ana( QPoint pt )
{
    if( mp_delete_ana_label->get_delete_status() ) {
        delete_shape_mouse_release( pt );
        mp_delete_ana_label->set_delete_status( false );
    } else {
        if( me_draw_status != none_draw_status ) {
            //qDebug()<<" shape_append_list ";
            shape_append_list( pt );//新增分析
        } else {
            shape_move_or_change_size( pt );//移动或者改变size
        }
    }

    ms_type_name = "";
}

void WndIrImage::delete_ana_shape( QPoint pt )
{
    if( mp_delete_ana_label->get_delete_status() ) {
        setCursor( Qt::CrossCursor );
    }
}

void WndIrImage::delete_ana(AnaInfo &ana_info)
{
    QList<AnaInfo>::iterator it;
    int i;
    for( i=0, it=m_list_anainfo.begin(); it!=m_list_anainfo.end(); it++, i++ ) {
         if( (*it).n_sign_id == ana_info.n_sign_id ) {
             //删除分析
             QString str_shape;
             shape_to_string( str_shape, (*it) );
             bool b_delete_status = yf_ir_image_ex1_delete_ana( mh_ir_image, str_shape.toStdString() );
             //if( b_delete_status ) {
                 m_list_anainfo.removeAt( i );
                 mp_delete_ana_label->set_delete_status( false );
                 mp_delete_ana_label->setHidden( true );
             //}
             update();
             break;
         }
    }
}

void WndIrImage::add_ana( AnaInfo &ana_info )
{
    QString str_shape;
    shape_to_string( str_shape, ana_info );
    bool b_add_status = yf_ir_image_ex1_add_ana( mh_ir_image, str_shape.toStdString() );
    if( b_add_status ) {
        m_list_anainfo.push_back( ana_info );
    }
    update();
}

void WndIrImage::change_ana( AnaInfo &ana_info_new, AnaInfo &ana_info_old )
{
    QString str_shape;
    shape_to_string( str_shape, ana_info_old );
    bool b_delete_status = yf_ir_image_ex1_delete_ana( mh_ir_image, str_shape.toStdString() );//删除老的分析
    if( b_delete_status ) {
        shape_to_string( str_shape, ana_info_new );
        yf_ir_image_ex1_add_ana( mh_ir_image, str_shape.toStdString() );//新增修改后的分析
    }
}

void WndIrImage::delete_shape_mouse_release( QPoint pt )
{
    //在点、线 上点击鼠标，则删除该图形
    AnaInfo ana_info;
    if ( !pt_in_ana( pt, me_ana_move, ana_info ) ) {
        qDebug()<<" delete_shape_mouse_release 1";
        return;
    }

    setCursor( Qt::ArrowCursor );

    delete_ana( ana_info );
    update();
}

void WndIrImage::set_ui_type(int n_type)
{
    //mn_ui_type = n_type;
}

QString WndIrImage::get_ana_draw_text( const AnaInfo &ana_info, const float &f_temp, const bool &b_has_temp )
{
    QString str_text;

    if ( ana_info.str_name.isEmpty() ) {

        if( ana_info.type_name == "point" ){
            str_text = "P";
        } else if( ana_info.type_name == "line" ) {
            str_text = "L";
        } else if( ana_info.type_name == "rect" ) {
            str_text = "R";
        } else if( ana_info.type_name == "circle" ) {
            str_text = "E";
        }

        if( ana_info.n_sign_id<10 ){
            str_text = str_text + QString("0%1").arg( ana_info.n_sign_id );
        } else {
            str_text = str_text + QString("%1").arg( ana_info.n_sign_id ) ;
        }

    } else {
        str_text = ana_info.str_name;
    }

    if ( b_has_temp ) {
        str_text += " Max:" + QString::number( f_temp, '.', 1 );
    }

    return str_text;
}

void WndIrImage::set_ir_ana_num( int n_num )
{
    mp_ir_label->set_ir_ana_num( n_num );
    mp_ir_label->resize( mp_ir_label->get_wnd_width(), LABEL_HEIGHT );
}

void WndIrImage::set_toolbar_hidden( bool b_hidden )
{
    mp_ir_label->setHidden( b_hidden );
    if ( b_hidden )mp_delete_ana_label->setHidden( b_hidden );
}

void WndIrImage::set_draw_line_cfg(QColor clr, int n_line_width, int n_line_style)
{
    m_clr_ana  = clr;
    mn_line_style = n_line_style;
    mn_line_width = n_line_width;
}

void WndIrImage::delete_preset_all_ana( int n_group )
{
    QList<AnaInfo>::iterator it = m_list_anainfo.begin();
    for( ; it != m_list_anainfo.end(); ++it ) {
        if ( it->b_preset_ana ) {
            delete_ana( *it );
        }
    }

    update();
}

int WndIrImage::get_sign_id()
{
    QMutexLocker lc( &m_mutex_sign );
    return ++mn_sign_id;
}

void WndIrImage::shape_to_string( QString &str_shape, const AnaInfo &anainfo )
{
    int n_start_x = anainfo.point_start.x();
    int n_start_y = anainfo.point_start.y();
    int n_end_x = anainfo.point_end.x();
    int n_end_y = anainfo.point_end.y();
    if( anainfo.type_name == "point" ) {
        str_shape = "P:" + QString::number( n_end_x ) + "," + QString::number( n_end_y ) + ";-";
    } else if( anainfo.type_name == "line" ) {
        str_shape = "L:" + QString::number( n_start_x ) + "," + QString::number( n_start_y ) + ";"
                + QString::number( n_end_x ) + "," + QString::number( n_end_y ) + ";-";
    } else if( anainfo.type_name == "rect" ) {
        str_shape = "R:" + QString::number( n_start_x ) + "," + QString::number( n_start_y ) + ";"
                + QString::number( n_end_x ) + "," + QString::number( n_end_y ) + ";-";
    } else if( anainfo.type_name == "circle" ) {
        str_shape = "E:" + QString::number( n_start_x ) + "," + QString::number( n_start_y ) + ";"
                + QString::number( n_end_x ) + "," + QString::number( n_end_y ) + ";-";
    }
}

bool WndIrImage::string_to_shape( const QString &str_shape, AnaInfo &anainfo )
{
    QStringList str_list_temp;
    QString str_temp;
    QStringList str_list = str_shape.split(':');
    if ( str_list.size() != 2 ) return false;
    QString str_type = str_list.at( 0 );

    if ( str_type == "P" ) {
        anainfo.type_name = "point";
    } else if ( str_type == "L" ) {
        anainfo.type_name = "line";
    } else if ( str_type == "R" ) {
        anainfo.type_name = "rect";
    } else if ( str_type == "E" ) {
        anainfo.type_name = "circle";

    } else {
        return false;
    }
    str_temp = str_list.at( 1 );
    str_list = str_temp.split(';');
    if ( str_list.size() == 2 ) {

        str_temp = str_list.at( 0 );
        str_list_temp = str_temp.split(',');
        if ( str_list_temp.size() != 2 ) return false;

        anainfo.point_start = QPoint( str_list_temp.at(0).toInt(), str_list_temp.at(1).toInt() );
        anainfo.point_end = anainfo.point_start;

    } else if ( str_list.size() == 3 ) {
        str_temp = str_list.at( 0 );
        str_list_temp = str_temp.split(',');
        if ( str_list_temp.size() != 2 ) return false;

        anainfo.point_start = QPoint( str_list_temp.at(0).toInt(), str_list_temp.at(1).toInt() );

        str_temp = str_list.at( 1 );
        str_list_temp = str_temp.split(',');
        if ( str_list_temp.size() != 2 ) return false;

        anainfo.point_end = QPoint( str_list_temp.at(0).toInt(), str_list_temp.at(1).toInt() );

    } else {
        return false;
    }

    return true;
}


YHANDLE WndIrImage::get_ir_handle( )
{
    return mh_ir_image;
}

bool WndIrImage::open_ir_image( QString str_path )
{
    string s_image_path ( str_path.toLocal8Bit() );
    m_str_ir_path = str_path;

    QDateTime dt = QDateTime::currentDateTime();
    if ( !yf_ir_image_ex1_load_file( mh_ir_image, s_image_path ) ) {
        set_toolbar_hidden( true );
        m_ir_image = QImage();
        return false;
    }
    qDebug() << QDateTime::currentDateTime().toMSecsSinceEpoch() - dt.toMSecsSinceEpoch();

    if ( mb_reset_preset_ana ) {
        yf_ir_image_ex1_delete_all_anas( mh_ir_image );

        add_preset_ana();
        add_matrix_ana();
    }
    add_normal_ana();
    mb_reset_preset_ana = false;

    if ( update_ir_image() ) {
        set_toolbar_hidden( false );
        return true;
    } else {
        set_toolbar_hidden( true );
        return false;
    }
}

bool WndIrImage::update_ir_image()
{
    unsigned short us_width, us_height;
    if ( !yf_ir_image_ex1_get_image_resolution( mh_ir_image, us_width, us_height ) ) return false;

    unsigned int dw_size;
    unsigned char *p_ir_bits = new unsigned char[ us_width*us_height*4 ];
    if( !yf_ir_image_ex1_get_bits( mh_ir_image, p_ir_bits, dw_size ) ) {
        if( p_ir_bits ) {
            delete[] p_ir_bits;
            p_ir_bits = NULL;
        }
        return false; //获取当前图像像素数据所占的字节大小
    } else {
    }
    m_ir_image = QImage( p_ir_bits, us_width, us_height, QImage::Format_RGB32 ).copy();//赋值
    if( p_ir_bits ) {
        delete[] p_ir_bits;
        p_ir_bits = NULL;
    }

    update();
    return true;
}

//获取调色板温度
bool WndIrImage::get_adjust_temp( float &f_level, float &f_level_span )
{
    return yf_ir_image_ex1_get_adjust_temp( mh_ir_image, f_level, f_level_span );
}

bool WndIrImage::set_adjust_temp( float f_level, float f_level_span )
{
    return yf_ir_image_ex1_set_adjust_temp( mh_ir_image, f_level, f_level_span );
}

//获取调色板类型
bool WndIrImage::get_adjust_type( YFAdjustType &adjust_type )
{
    return yf_ir_image_ex1_get_adjust_type( mh_ir_image, adjust_type );
}

bool WndIrImage::set_adjust_type( YFAdjustType adjust_type )
{
    return yf_ir_image_ex1_set_adjust_type( mh_ir_image, adjust_type );
}

//获取色标颜色
bool WndIrImage::get_adjust_color( ColorIndex &color_index )
{
    return yf_ir_image_ex1_get_color_index( mh_ir_image, color_index );
}

bool WndIrImage::set_adjust_color( ColorIndex color_index )
{
    return yf_ir_image_ex1_set_color_index( mh_ir_image, color_index );
}

bool WndIrImage::get_adjust_max_temp( float &f_level_span )
{
    unsigned short w_x, w_y;
    return yf_ir_image_ex1_get_max_temp( mh_ir_image, w_x, w_y, f_level_span );
}

bool WndIrImage::get_adjust_min_temp( float &f_level )
{
    unsigned short w_x, w_y;
    return yf_ir_image_ex1_get_min_temp( mh_ir_image, w_x, w_y, f_level );
}

bool WndIrImage::init_image()
{
    open_ir_image( "" );
}

void WndIrImage::set_group_num( int n_group_num )
{
    mn_group_num = n_group_num;
}

//设置预置点分析信息
void WndIrImage::set_preset_ana_info( QVector<AnaItem> & vct_ana )
{
    mb_reset_preset_ana = true;
    //m_list_anainfo.clear();
    m_list_pre_anainfo.clear();
    m_list_matrix_anainfo.clear();

    QVector<AnaItem>::iterator it = vct_ana.begin();
    for( ; it!=vct_ana.end(); it++ ) {
        AnaInfo ana_info;
        AnaItem ana_item = *it;
        QStringList str_ana_item = ana_item.str_name.split( "_" );

        if( str_ana_item[0] == "matrix" ) {
            string_to_matrix_list( ana_item.n_id_ana, ana_item.str_ana, str_ana_item[1] );
            continue;
        }
        string_to_shape( ana_item.str_ana, ana_info );
        ana_info.n_sign_id = ana_item.n_id_ana;
        ana_info.str_name = ana_item.str_name;
        ana_info.b_preset_ana = true;
        m_list_pre_anainfo.push_back( ana_info );
    }
}

bool WndIrImage::string_to_matrix_list( int n_ana_id, const QString &str_ana, QString str_color_index )
{
    QColor cl;
    if( str_color_index == "0" ) {
        cl = QColor( Qt::green );
    } else if( str_color_index == "1" ) {
        cl = QColor( Qt::cyan );
    } else if( str_color_index == "2" ) {
        cl = QColor( Qt::blue );
    } else if( str_color_index == "3" ) {
        cl = QColor( Qt::yellow );
    }

    cl.setAlpha( DRAW_COLOR_ALPHA );

    AnaInfo ana_info;
    bool ret = true;
    QStringList str_list_temp = str_ana.split( "-" );
    for( int i=0; i<str_list_temp.size()-1; i++ ) {
        QStringList str_list = str_list_temp[i].split(":")[1].split(";");
        QStringList str_list_pt_start = str_list[0].split( "," );
        QStringList str_list_pt_end = str_list[1].split( "," );
        ana_info.point_start = QPoint( str_list_pt_start.at(0).toInt(), str_list_pt_start.at(1).toInt() );
        ana_info.point_end = QPoint( str_list_pt_end.at(0).toInt(), str_list_pt_end.at(1).toInt() );
        ana_info.n_sign_id = n_ana_id;
        ana_info.type_name = "rect";
        m_list_matrix_anainfo.push_back( ana_info );
    }
    m_map_matrix_ana_clr.insert( n_ana_id, cl );
    //update();

    return ret;
}

//添加预置点分析
void WndIrImage::add_preset_ana( )
{
    QList<AnaInfo>::iterator it = m_list_pre_anainfo.begin();
    for( ; it!=m_list_pre_anainfo.end(); it++ ) {
        AnaInfo &ana_info = *it;
        QString str_shape = "";
        shape_to_string( str_shape, ana_info );
        bool b_add_status = yf_ir_image_ex1_add_ana( mh_ir_image, str_shape.toStdString() );
        if( b_add_status ) {

        }
    }
}

void WndIrImage::add_matrix_ana()
{
    QList<AnaInfo>::iterator it = m_list_matrix_anainfo.begin();
    for( ; it!=m_list_matrix_anainfo.end(); it++ ) {
        AnaInfo &ana_info = *it;
        QString str_shape="";
        shape_to_string( str_shape, ana_info );
        bool b_add_status = yf_ir_image_ex1_add_ana( mh_ir_image, str_shape.toStdString() );
        if( b_add_status ) {

        }
    }
}

void WndIrImage::add_normal_ana()
{
    QList< AnaInfo >::iterator it = m_list_anainfo.begin();
    for( ; it != m_list_anainfo.end(); it++ ) {
        AnaInfo &ana_info = *it;
        QString str_shape("");
        shape_to_string( str_shape, ana_info );
        bool b_add_status = yf_ir_image_ex1_add_ana( mh_ir_image, str_shape.toStdString() );
    }
}

//清除分析按下状态
void WndIrImage::clear_ana_press_status()
{
    me_mouse_press_status = none_press_status;
    //mb_press_ctrl = false;
    mp_ir_label->clear_down_status();
}

bool WndIrImage::get_mouse_press_status()
{
    return mb_press_ctrl;
}

void WndIrImage::set_mouse_press_status( bool b_press_ctrl )
{
    mb_press_ctrl = b_press_ctrl;
}

