#include "wndpanrail.h"
#include <QPalette>
#include <QPainter>
#include <QGraphicsOpacityEffect>
#include <QDebug>
#include "cbasemsg.h"

#define SPACE 30
//云台的按钮
//#define BTN_WIDTH 60
//#define BTN_HEIGHT 40
#define PAN_WIDTH 220
#define PAN_HEIGHT 160
#define HEIGHT_SET_AREA 100
//预置点的按钮
#define PRE_BOX_TITLE_HEIGHT 12
#define PRE_LAB_WIDTH 60
#define PRE_BTN_WIDTH 100
#define PRE_HEIGHT 30

WndPanRail::WndPanRail(QWidget *parent) :
    QWidget(parent)
    ,mp_btn_left( NULL )
    ,mp_btn_lefttop( NULL )
    ,mp_btn_top( NULL )
    ,mp_btn_righttop( NULL )
    ,mp_btn_right( NULL )
    ,mp_btn_rightbottom( NULL )
    ,mp_btn_bottom( NULL )
    ,mp_btn_leftbottom( NULL )
    ,mp_slider_ver( NULL )
    ,mp_btn_ver_top( NULL )
    ,mp_btn_ver_bottom( NULL )
    ,mp_slider_hor( NULL )
    ,mp_btn_hor_left( NULL )
    ,mp_btn_hor_right( NULL )
    ,mp_box_set_pre( NULL )
    ,mp_lab_rail_pre( NULL )
    ,mp_edit_rail_pre( NULL )
    ,mp_lab_pan_pre( NULL )
    ,mp_edit_pan_pre( NULL )
    ,mp_btn_set_rail_pre( NULL )
    ,mp_btn_set_pan_pre( NULL )
    ,mb_rail_hidden( false )
    ,mb_pan_hidden( false )
    ,mb_rail_lefttop_rightbottom( true )
    ,mb_rail_ver_hor( false )
    ,mb_box_hidden( true )
    ,mn_group_num(0)
{
    setMouseTracking( true );

    //resize( 300, 340 );

    init_widget();
    update_ctrl();
    qDebug()<<"WndPanRail"<<width()<<height();

}

WndPanRail::~WndPanRail()
{

}

void WndPanRail::init_widget()
{
    mp_btn_left = new QPushButton( this );
    mp_btn_left->setText( "左" );
    mp_btn_lefttop = new QPushButton( this );
    mp_btn_lefttop->setText( "左上" );
    mp_btn_top = new QPushButton( this );
    mp_btn_top->setText( "上" );
    mp_btn_righttop = new QPushButton( this );
    mp_btn_righttop->setText( "右上" );
    mp_btn_right = new QPushButton( this );
    mp_btn_right->setText( "右" );
    mp_btn_rightbottom = new QPushButton( this );
    mp_btn_rightbottom->setText( "右下" );
    mp_btn_bottom = new QPushButton( this );
    mp_btn_bottom->setText( "下" );
    mp_btn_leftbottom = new QPushButton( this );
    mp_btn_leftbottom->setText( "左下" );

    mp_slider_ver = new QSlider( this );
    mp_btn_ver_top = new QPushButton( this );
    mp_btn_ver_bottom = new QPushButton( this );
    mp_btn_ver_top->setText( "▲" );
    mp_btn_ver_bottom->setText( "▼" );
    mp_slider_hor = new QSlider( Qt::Horizontal, this );
    mp_btn_hor_left = new QPushButton( this );
    mp_btn_hor_right = new QPushButton( this );
    mp_btn_hor_left->setText( "<" );
    mp_btn_hor_right->setText( ">" );

    mp_box_set_pre = new QGroupBox( this );
    mp_box_set_pre->setTitle( "设置预置点" );
    mp_lab_rail_pre = new QLabel( mp_box_set_pre );
    mp_lab_rail_pre->setText( "导轨预置点" );
    mp_edit_rail_pre = new QLineEdit( mp_box_set_pre );
    mp_btn_set_rail_pre = new QPushButton( mp_box_set_pre );
    mp_btn_set_rail_pre->setText( "设置导轨预置点" );
    mp_lab_pan_pre = new QLabel( mp_box_set_pre );
    mp_lab_pan_pre->setText( "云台预置点" );
    mp_edit_pan_pre = new QLineEdit( mp_box_set_pre );
    mp_btn_set_pan_pre = new QPushButton( mp_box_set_pre );
    mp_btn_set_pan_pre->setText( "设置云台预置点" );
}


void WndPanRail::update_ctrl()
{
    update_ctrl_show();
    update_ctrl_pos();
}

void WndPanRail::update_ctrl_show()
{
    if ( mb_rail_hidden ) {
        show_hor_rail_ctrl( false );
        show_ver_rail_ctrl( false );
    } else {
        show_hor_rail_ctrl( !mb_rail_ver_hor );
        show_ver_rail_ctrl( mb_rail_ver_hor );
    }

    show_ptz_ctrl( !mb_pan_hidden );
    show_box_ctrl( !mb_box_hidden );
}

void WndPanRail::update_ctrl_pos()
{
    QRect rct_rail, rct_ptz, rct_set;

    if ( mb_rail_hidden ) {
        rct_rail = QRect( 0, 0, 0, 0 );
        rct_ptz = QRect( 0, 0, width(), height() );
        //rct_ptz = QRect( 0, 0, width(), height() - HEIGHT_SET_AREA );
        //rct_set = QRect( 0, height() - HEIGHT_SET_AREA, width(), HEIGHT_SET_AREA );
    } else {
        if ( mb_rail_ver_hor ) {//垂直
            if ( mb_rail_lefttop_rightbottom ) {//左
                rct_rail = QRect( 0, 0, SPACE, height() );
                rct_ptz = QRect( SPACE*2, 0, width()-SPACE*2, height() );
            } else { //右
                rct_ptz = QRect( 0, 0, width()-SPACE*2, height() );
                rct_rail = QRect( width()-SPACE, 0, SPACE, height() );
            }
        } else { //水平
            if ( mb_rail_lefttop_rightbottom ) {//上
                rct_rail = QRect( 0, 0, width(), SPACE);
                rct_ptz = QRect( 0, SPACE*2, width(), height() - SPACE*2 );
                qDebug()<<"WndPanRail::update_ctrl_pos"<<width()<<height();
            } else {//下
                rct_ptz = QRect( 0, 0, width(), height() - SPACE*2 );
                rct_rail = QRect( 0, height() - SPACE, width(), SPACE );
            }
        }
        //rct_set = QRect( 0, height() - HEIGHT_SET_AREA, width(), HEIGHT_SET_AREA );
    }

    if ( mb_pan_hidden ) {
        rct_ptz = QRect( 0, 0, 0, 0 );
        if( !mb_rail_hidden && !mb_rail_ver_hor && mb_rail_lefttop_rightbottom ) {
            rct_set = QRect( 0, SPACE*2, width(), HEIGHT_SET_AREA );
        }
    }

    if ( mb_box_hidden ) {
        rct_set = QRect( 0, 0, 0, 0 );
    }


    move_rail_ctrl( rct_rail );
    move_ptz_ctrl( rct_ptz );
    move_set_ctrl( rct_set );
}

void WndPanRail::resizeEvent( QResizeEvent *event )
{
    update_ctrl_pos();
    qDebug()<<"WndPanRail::resizeEvent"<<width()<<height();

    //update_ctrl();
}

void WndPanRail::show_rail_type( int n_class )
{
    if( n_class == 3 ){
        mb_rail_ver_hor = false;
    }else if( n_class == 4 ){
        mb_rail_ver_hor = true;
    }
    update_ctrl();
}

//确定是否有导轨以及导轨位置
void WndPanRail::set_rail_exist( bool b_rail_hidden, bool b_rail_ver_hor, bool b_rail_lefttop_or_rightbottom )
{
    mb_rail_hidden = b_rail_hidden;
    mb_rail_ver_hor = b_rail_ver_hor;
    mb_rail_lefttop_rightbottom = b_rail_lefttop_or_rightbottom;
}

void WndPanRail::set_pan_exit( bool b_pan_hidden )
{
    mb_pan_hidden = b_pan_hidden;
}

void WndPanRail::set_box_exit( bool b_box_hidden )
{
    mb_box_hidden = b_box_hidden;
}

//设置云台有效化
void WndPanRail::set_pan_effective( bool b_pan_effect )
{
    mp_btn_left->setEnabled( b_pan_effect );
    mp_btn_lefttop->setEnabled( b_pan_effect );
    mp_btn_top->setEnabled( b_pan_effect );
    mp_btn_righttop->setEnabled( b_pan_effect );
    mp_btn_right->setEnabled( b_pan_effect );
    mp_btn_rightbottom->setEnabled( b_pan_effect );
    mp_btn_bottom->setEnabled( b_pan_effect );
    mp_btn_leftbottom->setEnabled( b_pan_effect );
}

//设置导轨有效化
void WndPanRail::set_rail_effective( bool b_rail_effect )
{
    if( mb_rail_ver_hor ) {
        mp_btn_ver_top->setEnabled( b_rail_effect );
        mp_btn_ver_bottom->setEnabled( b_rail_effect );
        mp_slider_ver->setEnabled( b_rail_effect );
    } else {
        mp_btn_hor_left->setEnabled( b_rail_effect );
        mp_btn_hor_right->setEnabled( b_rail_effect );
        mp_slider_hor->setEnabled( b_rail_effect );
    }
}


void WndPanRail::set_rail_visiable(bool b_visiable)
{
    if( b_visiable ) {
        if ( mb_rail_ver_hor ){
            mp_btn_ver_top->setHidden( false );
            mp_btn_ver_bottom->setHidden( false );
            mp_slider_ver->setHidden( false );
        } else {
            mp_btn_hor_left->setHidden( false );
            mp_btn_hor_right->setHidden( false );
            mp_slider_hor->setHidden( false );
        }
    } else {
        if ( mb_rail_ver_hor ){
            mp_btn_ver_top->setHidden( true );
            mp_btn_ver_bottom->setHidden( true );
            mp_slider_ver->setHidden( true );
        } else {
            mp_btn_hor_left->setHidden( true );
            mp_btn_hor_right->setHidden( true );
            mp_slider_hor->setHidden( true );
        }
    }
}

//设置窗口透明度
void WndPanRail::set_widget_transparency( float f_level )
{
    QGraphicsOpacityEffect *effect=new QGraphicsOpacityEffect( this );
    effect->setOpacity( f_level );
    setGraphicsEffect(effect);
}

void WndPanRail::paintEvent( QPaintEvent *event )
{
    QPainter draw( this );
    //draw.drawPixmap( 0, 0, 340, 480, QPixmap( "E:/learning-master/PanRail/caption_logo.bmp" ) );
    draw.end();
}

void WndPanRail::move_ptz_ctrl( QRect rct )
{
    if ( mb_pan_hidden ) return;
    int n_x = rct.x();
    int n_y = rct.y();

    int n_btn_widget = (rct.width()-SPACE*2)/3;
    int n_btn_height = (rct.height()-SPACE*2)/3;

    mp_btn_left->move( n_x, n_y+SPACE+n_btn_height );
    mp_btn_left->resize( n_btn_widget, n_btn_height );
    mp_btn_lefttop->move( n_x, n_y );
    mp_btn_lefttop->resize( n_btn_widget, n_btn_height );
    mp_btn_top->move( n_x+SPACE+n_btn_widget, n_y );
    mp_btn_top->resize( n_btn_widget, n_btn_height );
    mp_btn_righttop->move( n_x+SPACE*2+n_btn_widget*2, n_y );
    mp_btn_righttop->resize( n_btn_widget, n_btn_height );
    mp_btn_right->move( n_x+SPACE*2+n_btn_widget*2, n_y+SPACE+n_btn_height);
    mp_btn_right->resize( n_btn_widget, n_btn_height );
    mp_btn_rightbottom->move( n_x+SPACE*2+n_btn_widget*2, n_y+SPACE*2+n_btn_height*2 );
    mp_btn_rightbottom->resize( n_btn_widget, n_btn_height );
    mp_btn_bottom->move( n_x+SPACE+n_btn_widget, n_y+SPACE*2+n_btn_height*2 );
    mp_btn_bottom->resize( n_btn_widget, n_btn_height );
    mp_btn_leftbottom->move( n_x, n_y+SPACE*2+n_btn_height*2 );
    mp_btn_leftbottom->resize( n_btn_widget, n_btn_height );
}

void WndPanRail::move_set_ctrl( QRect rct )
{
    mp_box_set_pre->move( rct.x(), rct.y() );
    mp_box_set_pre->resize( rct.width(), rct.height() );
    int n_height = PRE_HEIGHT;
    int n_width_space = (rct.width()-PRE_LAB_WIDTH*2-PRE_BTN_WIDTH)/4;
    int n_height_space = (rct.height()-n_height*2-PRE_BOX_TITLE_HEIGHT)/3;
    if( !mb_rail_hidden ) {
        mp_lab_rail_pre->move( n_width_space, PRE_BOX_TITLE_HEIGHT+n_height_space );
        mp_lab_rail_pre->resize( PRE_LAB_WIDTH, n_height );
        mp_edit_rail_pre->move( n_width_space*2+PRE_LAB_WIDTH, PRE_BOX_TITLE_HEIGHT+n_height_space );
        mp_edit_rail_pre->resize( PRE_LAB_WIDTH, n_height );
        mp_btn_set_rail_pre->move( n_width_space*3+PRE_LAB_WIDTH*2, PRE_BOX_TITLE_HEIGHT+n_height_space );
        mp_btn_set_rail_pre->resize( PRE_BTN_WIDTH, n_height );
        if( !mb_pan_hidden ) {
            mp_lab_pan_pre->move( n_width_space, PRE_BOX_TITLE_HEIGHT+n_height_space*2 + n_height );
            mp_lab_pan_pre->resize( PRE_LAB_WIDTH, n_height );
            mp_edit_pan_pre->move( n_width_space*2+PRE_LAB_WIDTH, PRE_BOX_TITLE_HEIGHT+n_height_space*2 + n_height);
            mp_edit_pan_pre->resize( PRE_LAB_WIDTH, n_height );
            mp_btn_set_pan_pre->move( n_width_space*3+PRE_LAB_WIDTH*2, PRE_BOX_TITLE_HEIGHT+n_height_space*2 + n_height);
            mp_btn_set_pan_pre->resize( PRE_BTN_WIDTH, n_height );
        }
    } else {
        if( !mb_pan_hidden ) {
            mp_lab_pan_pre->move( n_width_space, PRE_BOX_TITLE_HEIGHT+n_height_space );
            mp_lab_pan_pre->resize( PRE_LAB_WIDTH, n_height );
            mp_edit_pan_pre->move( n_width_space*2+PRE_LAB_WIDTH, PRE_BOX_TITLE_HEIGHT+n_height_space );
            mp_edit_pan_pre->resize( PRE_LAB_WIDTH, n_height );
            mp_btn_set_pan_pre->move( n_width_space*3+PRE_LAB_WIDTH*2, PRE_BOX_TITLE_HEIGHT+n_height_space );
            mp_btn_set_pan_pre->resize( PRE_BTN_WIDTH, n_height );
        }
    }
}

void WndPanRail::move_rail_ctrl( QRect rct )
{
    if ( mb_rail_ver_hor ) {//垂直
        int n_btn_width = rct.width();
        int n_btn_height = SPACE;

        int n_slider_width = n_btn_width;
        int n_slider_hieght = rct.height() - n_btn_height* 2;

        mp_btn_ver_top->move( rct.left(), rct.top() );
        mp_btn_ver_top->resize( n_btn_width, n_btn_height );

        mp_slider_ver->move( rct.left(), rct.top() + n_btn_height );
        mp_slider_ver->resize( n_slider_width, n_slider_hieght );

        mp_btn_ver_bottom->move( rct.left(), rct.top() + n_btn_height + n_slider_hieght );
        mp_btn_ver_bottom->resize( n_btn_width, n_btn_height );
    } else {
        int n_btn_width = SPACE;
        int n_btn_height = rct.height();

        int n_slider_width = rct.width()-n_btn_width*2;
        int n_slider_hieght = n_btn_height;

        mp_btn_hor_left->move( rct.left(), rct.top() );
        mp_btn_hor_left->resize( n_btn_width, n_btn_height );

        mp_slider_hor->move( rct.left() + n_btn_width, rct.top());
        mp_slider_hor->resize( n_slider_width, n_slider_hieght );

        mp_btn_hor_right->move( rct.left() + n_btn_width + n_slider_width, rct.top() );
        mp_btn_hor_right->resize( n_btn_width, n_btn_height );
    }
}

void WndPanRail::show_hor_rail_ctrl( bool b_show )
{
    if( b_show ) {
        mp_btn_hor_left->show();
        mp_btn_hor_right->show();
        mp_slider_hor->show();
    } else {
        mp_btn_hor_left->hide();
        mp_btn_hor_right->hide();
        mp_slider_hor->hide();
    }
}

void WndPanRail::show_ver_rail_ctrl( bool b_show )
{
    if( b_show ) {
        mp_btn_ver_top->show();
        mp_btn_ver_bottom->show();
        mp_slider_ver->show();
    } else {
        mp_btn_ver_top->hide();
        mp_btn_ver_bottom->hide();
        mp_slider_ver->hide();
    }
}

void WndPanRail::show_ptz_ctrl( bool b_show )
{
    if( b_show ) {
        mp_btn_left->show();
        mp_btn_lefttop->show();
        mp_btn_top->show();
        mp_btn_righttop->show();
        mp_btn_right->show();
        mp_btn_rightbottom->show();
        mp_btn_bottom->show();
        mp_btn_leftbottom->show();
    } else {
        mp_btn_left->hide();
        mp_btn_lefttop->hide();
        mp_btn_top->hide();
        mp_btn_righttop->hide();
        mp_btn_right->hide();
        mp_btn_rightbottom->hide();
        mp_btn_bottom->hide();
        mp_btn_leftbottom->hide();
    }
}

void WndPanRail::show_box_ctrl( bool b_show )
{
    if( b_show ) {
        if( mb_pan_hidden ) {
            mp_lab_pan_pre->hide();
            mp_edit_pan_pre->hide();
            mp_btn_set_pan_pre->hide();
        } else {
            mp_lab_pan_pre->show();
            mp_edit_pan_pre->show();
            mp_btn_set_pan_pre->show();
        }

        if( mb_rail_hidden ) {
            mp_lab_rail_pre->hide();
            mp_edit_rail_pre->hide();
            mp_btn_set_rail_pre->hide();
        } else {
            mp_lab_rail_pre->show();
            mp_edit_rail_pre->show();
            mp_btn_set_rail_pre->show();
        }

        if ( mb_rail_hidden && mb_pan_hidden ) {
            mp_box_set_pre->hide();
        } else {
            mp_box_set_pre->show();
        }
    } else {
        mp_lab_rail_pre->hide();
        mp_edit_rail_pre->hide();
        mp_btn_set_rail_pre->hide();
        mp_lab_pan_pre->hide();
        mp_edit_pan_pre->hide();
        mp_btn_set_pan_pre->hide();
        mp_box_set_pre->hide();
    }
}

