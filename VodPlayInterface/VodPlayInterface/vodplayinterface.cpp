#include "vodplayinterface.h"
#include "ui_vodplayinterface.h"
#include <QDesktopWidget>
#include <QBitmap>
#include <QPainter>
#include <QThread>
#include <QMessageBox>
#include <QGraphicsOpacityEffect>

#include <QDebug>
#include "wndpreview.h"

#define TITLE_BAR_HEIGHT 50
//配置界面和分析界面左边窗口的宽度
#define CONF_INTERFACE_WIDTH 400
#define CONF_ANALY_WIDTH 240
#define CONF_ANALY_HEIGHT 240

#define SPACE 10
//改变鼠标状态距离边框的位置
#define MOUSE_SPACE 5

extern VodPlayInterface * vod_ir_play_window;


VodPlayInterface::VodPlayInterface(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::VodPlayInterface)
{
    ui->setupUi( this );


    init_title_bar();
    init_mainwindow();

    init_window_connect();
    init_mouse_tracking();
    //connect( &m_connect_ctrl, SIGNAL( sig_vi_image_connect( QImage&, int& ) ), this, SLOT( slot_set_vi_image(QImage&,int&) ) );

    mp_netcontrol = new NetControl( "192.168.0.98", 5555 );
    if( !mp_netcontrol->connect_server() ) {
        QMessageBox::information( this, "ERROR", "failed to connect server!" );
    } else {
        init_net_timer();
        mp_netcontrol->send_update_xml();
    }

    ui->id_monitor_frame_1->set_net_ctrl( mp_netcontrol );
    ui->id_monitor_frame_1->set_group_number( 0 );

    ui->id_monitor_frame_2->set_net_ctrl( mp_netcontrol );
    ui->id_monitor_frame_2->set_group_number( 1 );

    QGraphicsOpacityEffect *effect=new QGraphicsOpacityEffect( this );
    effect->setOpacity( 0.8 );
    ui->id_config_label_move->setGraphicsEffect(effect);
    ui->id_config_label_move->hide();

    ui->id_config_interface->init_ctrl( ui, mp_netcontrol );
    ui->id_back_interface->init_ctrl( ui, mp_netcontrol );

    init_timer();
}

VodPlayInterface::~VodPlayInterface()
{
    delete ui;
}

void VodPlayInterface::init_mainwindow() {
    mb_left_btn_press = false;
    mb_max_win = false;
    ui->stackedWidget->addWidget( ui->id_monitor_interface );
    ui->stackedWidget->addWidget( ui->id_config_interface );
    ui->stackedWidget->addWidget( ui->id_analyse_interface );
    ui->stackedWidget->addWidget( ui->id_back_interface );

    //右键菜单
    ui->id_monitor_interface->setContextMenuPolicy( Qt::CustomContextMenu );
    connect( ui->id_monitor_interface, SIGNAL( customContextMenuRequested( const QPoint& ) ), this, SLOT( show_contextmenu( const QPoint& ) ) );
    mp_cmenu = NULL;
    mb_max_size = false;
}

void VodPlayInterface::init_window_connect()
{
    connect( this, SIGNAL( sig_display_monitor_interface() ), this, SLOT( slot_display_monitor_interface() ) );
    connect( this, SIGNAL( sig_display_config_interface() ), this, SLOT( slot_display_config_interface() ) );
    connect( this, SIGNAL( sig_display_analyse_interface() ), this, SLOT( slot_display_analyse_interface() ) );
    connect( this, SIGNAL( sig_display_back_interface() ), this, SLOT( slot_display_back_interface() ) );

    connect( this, SIGNAL( min_window() ), this, SLOT( showMinimized() ) );
    connect( this, SIGNAL( max_window() ), this, SLOT( slot_show_max_size() ) );

}

void VodPlayInterface::init_mouse_tracking()
{
    //在centralWidge和QMainWindow开启移动监视
    ui->centralWidget->setMouseTracking( true );
    setMouseTracking( true );
    ui->stackedWidget->setMouseTracking( true );
    ui->id_monitor_interface->setMouseTracking( true );
    ui->id_config_interface->setMouseTracking( true );
    ui->id_analyse_interface->setMouseTracking( true );
    ui->id_back_interface->setMouseTracking( true );

    ui->id_config_frame_group->setMouseTracking( true );
    ui->id_config_frame_ir->setMouseTracking( true );
    ui->id_config_frame_pan->setMouseTracking( true );
    ui->id_config_frame_vi->setMouseTracking( true );

}


/**********************************************************************************
Func    Name: title_bar_init
Descriptions: 隐藏标题栏，并创建标题栏，并对标题栏初始化
Input   para:
In&Out  Para:
Output  para:
Return value:
***********************************************************************************/
void VodPlayInterface::init_title_bar() {
    //隐藏标题栏
    setWindowFlags( Qt::FramelessWindowHint );
    //setWindowFlags( Qt::CustomizeWindowHint );
    //创建标题栏  类
    mp_title_bar = new TitleBar( this );
    mp_title_bar->setWindowIcon( QIcon( QPixmap( "../image/frame.jpg" ) ) );
    mp_title_bar->resize( ui->centralWidget->width(), 50 );
    mp_title_bar->setMouseTracking( true );


    //使用setmask过滤即可,窗口的最小size
    setMinimumWidth(850);
    setMinimumHeight(600);
}

void VodPlayInterface::init_timer() {
    //创建定时器
    QTimer *mp_update_timer = new QTimer( this );
    //将定时器超时信号与槽(功能函数)联系起来
    connect( mp_update_timer,SIGNAL( timeout() ), this, SLOT( update() ) );
    //开始运行定时器，定时时间间隔为1000ms
    mp_update_timer->start( 40 );
}

//鼠标按下事件
void VodPlayInterface::mousePressEvent( QMouseEvent *event ) {
    if ( event->button() == Qt::LeftButton ) {
        m_pt_press_global = event->globalPos();
        mb_left_btn_press = true;
    }
}
//鼠标移动事件
void VodPlayInterface::mouseMoveEvent( QMouseEvent *event ) {
    if( !mb_left_btn_press ) {
        me_direction = PointValid( event->x(), event->y() );
        if( this->isMaximized()== false ) {
            SetCursorStyle( me_direction );
        }
    } else {
        int nx_global = event->globalX();
        int ny_global = event->globalY();
        SetDrayMove( nx_global, ny_global, me_direction );
        m_pt_press_global = QPoint( nx_global, ny_global );
    }
}
//鼠标释放事件
void VodPlayInterface::mouseReleaseEvent( QMouseEvent *event ) {
    if ( event->button() == Qt::LeftButton ) {
        mb_left_btn_press = false;
        //m_eDirection = eNone;
    }
 }
//鼠标双击事件
void VodPlayInterface::mouseDoubleClickEvent( QMouseEvent *event ) {
    if ( event->button() == Qt::LeftButton && event->y()<= mp_title_bar->height() ) {
        if ( !mp_title_bar->is_click_in_btn( event->x(), event->y() ) ) {
            return ;
        }
        //如果me_direction处于eTop的位置，则不具有双击放大窗口的效果
        switch ( me_direction ) {
        case eTop:
        case eTopRight:
        case eLeft:
        case eLeftTop:
        case eRight:
        case eRightBottom:
        case eBottomLeft:
        case eBottom:
            return ;
        }
        if( !mb_max_win ) {
            //m_rect_restore_window = geometry();
            //setGeometry( qApp->desktop()->availableGeometry() );
            /*if( me_direction == eTop ) {
                m_rect_restore_window = geometry();
                setGeometry( ui->centralWidget->x(), 0, ui->centralWidget->width(), qApp->desktop()->height() );
            } else {*/
                showMaximized();
            //}
        } else {
           /* if( me_direction == eTop ) {
                setGeometry( m_rect_restore_window );
            } else {
            //setGeometry( m_rect_restore_window );*/
            showNormal();
            //}
        }
        mb_max_win = !mb_max_win;
        qDebug()<<"etop"<<me_direction;
        /*if( me_direction==eTop ) {
            setGeometry( qApp->desktop()->height() ) ;
        }*/
    }
}

void VodPlayInterface::resizeEvent( QResizeEvent *event ) {
    mn_stackwindow_width = ui->centralWidget->width();
    mn_stackwindow_height = ui->centralWidget->height() - TITLE_BAR_HEIGHT;
    //标题栏
    mp_title_bar->move( 0, 0 );
    mp_title_bar->resize( ui->centralWidget->width(), TITLE_BAR_HEIGHT );
    qDebug()<<"主窗口的宽度："<<ui->centralWidget->width();
    //stackwindow
    ui->stackedWidget->move( 0, TITLE_BAR_HEIGHT );
    ui->stackedWidget->resize( mn_stackwindow_width, mn_stackwindow_height );
    //监控主界面
    int width = mn_stackwindow_width;
    int height = mn_stackwindow_height;
    ui->id_monitor_interface->move( 0, 0 );
    ui->id_monitor_interface->resize( mn_stackwindow_width, mn_stackwindow_height );

    ui->id_monitor_frame_1->move( 0, 0 );
    ui->id_monitor_frame_1->resize( width/2, height/4 );
    ui->id_monitor_frame_2->move( ui->id_monitor_frame_1->width(), 0 );
    ui->id_monitor_frame_2->resize( width/2, height/4 );
    ui->id_monitor_frame_3->move( 0, ui->id_monitor_frame_1->height() );
    ui->id_monitor_frame_3->resize( width/2, height/4 );
    ui->id_monitor_frame_4->move( ui->id_monitor_frame_3->width(), ui->id_monitor_frame_2->height() );
    ui->id_monitor_frame_4->resize( width/2, height/4 );
    ui->id_monitor_frame_5->move( 0, ui->id_monitor_frame_1->height()+ui->id_monitor_frame_3->height() );
    ui->id_monitor_frame_5->resize( width/2, height/4 );
    ui->id_monitor_frame_6->move( ui->id_monitor_frame_5->width(), ui->id_monitor_frame_2->height()+ui->id_monitor_frame_4->height() );
    ui->id_monitor_frame_6->resize( width/2, height/4 );
    ui->id_monitor_frame_7->move( 0, ui->id_monitor_frame_1->height()+ui->id_monitor_frame_3->height()+ui->id_monitor_frame_5->height() );
    ui->id_monitor_frame_7->resize( width/2, height-( ui->id_monitor_frame_1->height()+ui->id_monitor_frame_3->height()+ui->id_monitor_frame_5->height() ) );
    ui->id_monitor_frame_8->move( ui->id_monitor_frame_7->width(), ui->id_monitor_frame_2->height()+ui->id_monitor_frame_4->height()+ui->id_monitor_frame_6->height() );
    ui->id_monitor_frame_8->resize( width/2, height-( ui->id_monitor_frame_2->height()+ui->id_monitor_frame_4->height()+ui->id_monitor_frame_6->height() ) );

    ui->id_monitor_interface->setStyleSheet( "border:2px solid black" );
    ui->id_monitor_frame_1->setStyleSheet( "border:1px solid black" );
    ui->id_monitor_frame_2->setStyleSheet( "border:1px solid black" );
    ui->id_monitor_frame_3->setStyleSheet( "border:1px solid black" );
    ui->id_monitor_frame_4->setStyleSheet( "border:1px solid black" );
    ui->id_monitor_frame_5->setStyleSheet( "border:1px solid black" );
    ui->id_monitor_frame_6->setStyleSheet( "border:1px solid black" );
    ui->id_monitor_frame_7->setStyleSheet( "border:1px solid black" );
    ui->id_monitor_frame_8->setStyleSheet( "border:1px solid black" );
    //设置自动填充背景
    //mp_packet_window_1->setAutoFillBackground(true);

    //配置界面
    ui->id_config_interface->move( 0, 0 );
    ui->id_config_interface->resize( mn_stackwindow_width, mn_stackwindow_height );
    ui->id_config_frame_group->move( SPACE, SPACE );
    ui->id_config_frame_group->resize( CONF_INTERFACE_WIDTH, mn_stackwindow_height-2*SPACE-SPACE*4 );
    ui->id_config_btn_save->move( SPACE, mn_stackwindow_height-SPACE*4);
    ui->id_config_btn_save->resize( CONF_INTERFACE_WIDTH, SPACE*3 );
    ui->id_config_frame_ir->move( CONF_INTERFACE_WIDTH+2*SPACE, SPACE );
    ui->id_config_frame_ir->resize( mn_stackwindow_width-3*SPACE-CONF_INTERFACE_WIDTH, ( mn_stackwindow_height-4*SPACE )/3 );
    ui->id_config_label_ir->move( CONF_INTERFACE_WIDTH+2*SPACE, SPACE );
    ui->id_config_frame_vi->move( CONF_INTERFACE_WIDTH+2*SPACE, ( mn_stackwindow_height-4*SPACE )/3 + 2*SPACE );
    ui->id_config_frame_vi->resize( mn_stackwindow_width-3*SPACE-CONF_INTERFACE_WIDTH, ( mn_stackwindow_height-4*SPACE )/3 );
    ui->id_config_label_vi->move( CONF_INTERFACE_WIDTH+2*SPACE, ( mn_stackwindow_height-4*SPACE )/3 + 2*SPACE );
    ui->id_config_frame_pan->move( CONF_INTERFACE_WIDTH+2*SPACE, ( mn_stackwindow_height-4*SPACE )*2/3 + 3*SPACE );
    ui->id_config_frame_pan->resize( mn_stackwindow_width-3*SPACE-CONF_INTERFACE_WIDTH, ( mn_stackwindow_height-4*SPACE )/3 );
    ui->id_config_label_pan->move( CONF_INTERFACE_WIDTH+2*SPACE, ( mn_stackwindow_height-4*SPACE )*2/3 + 3*SPACE );

    //ui->id_config_interface
    //ui->id_config_widget_group->setStyleSheet( "border:1px solid black" );
    //ui->id_config_widget_ir->setStyleSheet( "border:1px solid black" );
    //ui->id_config_widget_vi->setStyleSheet( "border:1px solid black" );
    //ui->id_config_widget_pan->setStyleSheet( "border:1px solid black" );
    //ui->id_config_frame_group->setStyleSheet( "border:1px solid black" );
    //ui->id_config_frame_ir->setStyleSheet( "border:1px solid black" );
    //ui->id_config_frame_vi->setStyleSheet( "border:1px solid black" );
    //ui->id_config_frame_pan->setStyleSheet( "border:1px solid black" );
    //ui->id_config_interface->setStyleSheet( "border:1px solid black" );

    //分析界面
    ui->id_analyse_interface->move( 0, 0 );
    ui->id_analyse_interface->resize( mn_stackwindow_width, mn_stackwindow_height );
    width = mn_stackwindow_width - 3*SPACE-CONF_ANALY_WIDTH;
    height = mn_stackwindow_height - 3*SPACE;
    ui->id_analyse_group_pan->move( SPACE, SPACE );
    ui->id_analyse_group_pan->resize( CONF_ANALY_WIDTH, CONF_ANALY_HEIGHT );
    ui->id_analyse_group_connect_status->move( SPACE, CONF_ANALY_HEIGHT+2*SPACE );
    ui->id_analyse_group_connect_status->resize( CONF_ANALY_WIDTH, height-CONF_ANALY_HEIGHT );
    ui->id_analyse_widget_connect_status->move( SPACE, SPACE*2 );
    ui->id_analyse_widget_connect_status->resize( CONF_ANALY_WIDTH-2*SPACE, height-CONF_ANALY_HEIGHT-3*SPACE );
    //假设宽比为3:2,
    if( width >= height*8/5 ) {
        ui->id_analyse_frame_ir->move( CONF_ANALY_WIDTH+2*SPACE, SPACE );
        ui->id_analyse_frame_ir->resize( height*4/5, height*3/5 );
        ui->id_analyse_frame_vi->move( CONF_ANALY_WIDTH+2*SPACE+height*4/5, SPACE );
        ui->id_analyse_frame_vi->resize( height*4/5, height*3/5 );

        ui->tabWidget->move( CONF_ANALY_WIDTH+2*SPACE, height*3/5+2*SPACE );
        ui->tabWidget->resize( height*8/5, height*2/5 );

    } else {
        ui->id_analyse_frame_ir->move( CONF_ANALY_WIDTH+2*SPACE, SPACE );
        ui->id_analyse_frame_ir->resize( width/2, width*3/8 );
        qDebug()<<"analyse:"<<width/2<<mn_stackwindow_width<<mn_stackwindow_height;
        ui->id_analyse_frame_vi->move( CONF_ANALY_WIDTH+2*SPACE+width/2, SPACE );
        ui->id_analyse_frame_vi->resize( width/2, width*3/8 );

        ui->tabWidget->move( CONF_ANALY_WIDTH+2*SPACE, width*3/8+2*SPACE );
        ui->tabWidget->resize( width, height-width*3/8 );

    }
    //ui->id_analyse_interface->setStyleSheet( "border:1px solid black" );
    //回放界面
    ui->id_back_interface->move( 0, 0 );
    ui->id_back_interface->resize( mn_stackwindow_width, mn_stackwindow_height );
    ui->id_back_group_select->move( SPACE, SPACE );
    ui->id_back_group_select->resize( CONF_ANALY_WIDTH, CONF_ANALY_HEIGHT );
    ui->id_back_group_connect_status->move( SPACE, 2*SPACE+CONF_ANALY_HEIGHT );
    ui->id_back_group_connect_status->resize( CONF_ANALY_WIDTH, height-CONF_ANALY_HEIGHT );
    ui->id_back_widget_connect_status->move( SPACE, SPACE*2 );
    ui->id_back_widget_connect_status->resize( CONF_ANALY_WIDTH-2*SPACE, height-CONF_ANALY_HEIGHT-3*SPACE );
    //假设宽比为3:2,
    if( width >= height*8/5 ) {
        ui->id_back_frame_ir->move( CONF_ANALY_WIDTH+2*SPACE, SPACE );
        ui->id_back_frame_ir->resize( height*4/5, height*3/5 );
        ui->id_back_frame_vi->move( CONF_ANALY_WIDTH+2*SPACE+height*4/5, SPACE );
        ui->id_back_frame_vi->resize( height*4/5, height*3/5 );

        ui->id_back_btn_start->move( CONF_ANALY_WIDTH+2*SPACE, height*3/5+ui->id_back_btn_start->height()/2 );
        ui->id_back_btn_stop->move( CONF_ANALY_WIDTH+2*SPACE+ui->id_back_btn_start->width(), height*3/5+ui->id_back_btn_start->height()/2 );
        ui->id_back_btn_first_frame->move( CONF_ANALY_WIDTH+2*SPACE+ui->id_back_btn_start->width()*2, height*3/5+ui->id_back_btn_start->height()/2 );
        ui->id_back_btn_pre_frame->move( CONF_ANALY_WIDTH+2*SPACE+ui->id_back_btn_start->width()*3, height*3/5+ui->id_back_btn_start->height()/2 );
        ui->id_back_btn_next_frame->move( CONF_ANALY_WIDTH+2*SPACE+ui->id_back_btn_start->width()*4, height*3/5+ui->id_back_btn_start->height()/2 );
        ui->id_back_btn_last_frame->move( CONF_ANALY_WIDTH+2*SPACE+ui->id_back_btn_start->width()*5, height*3/5+ui->id_back_btn_start->height()/2 );
        ui->id_back_btn_play_speed->move( CONF_ANALY_WIDTH+2*SPACE+ui->id_back_btn_start->width()*6, height*3/5+ui->id_back_btn_start->height()/2 );

        ui->id_back_slider_play->move( CONF_ANALY_WIDTH+2*SPACE+ui->id_back_btn_start->width()*7, height*3/5+ui->id_back_btn_start->height()/2 );
        ui->id_back_slider_play->resize( height*8/5-ui->id_back_btn_start->width()*7-ui->id_back_btn_replay->width(), ui->id_back_btn_start->height() );
        ui->id_back_btn_replay->move( CONF_ANALY_WIDTH+2*SPACE+ui->id_back_btn_start->width()*7+ui->id_back_slider_play->width(), height*3/5+ui->id_back_btn_start->height()/2 );

        ui->tabWidget_2->move( CONF_ANALY_WIDTH+2*SPACE, height*3/5+2*SPACE+ui->id_back_btn_start->height() );
        ui->tabWidget_2->resize( height*8/5, height*2/5-ui->id_back_btn_start->height() );

    } else {
        ui->id_back_frame_ir->move( CONF_ANALY_WIDTH+2*SPACE, SPACE );
        ui->id_back_frame_ir->resize( width/2, width*3/8 );
        ui->id_back_frame_vi->move( CONF_ANALY_WIDTH+2*SPACE+width/2, SPACE );
        ui->id_back_frame_vi->resize( width/2, width*3/8 );

        ui->id_back_btn_start->move( CONF_ANALY_WIDTH+2*SPACE, width*3/8+ui->id_back_btn_start->height()/2 );
        ui->id_back_btn_stop->move( CONF_ANALY_WIDTH+2*SPACE+ui->id_back_btn_start->width(), width*3/8+ui->id_back_btn_start->height()/2 );
        ui->id_back_btn_first_frame->move( CONF_ANALY_WIDTH+2*SPACE+ui->id_back_btn_start->width()*2, width*3/8+ui->id_back_btn_start->height()/2 );
        ui->id_back_btn_pre_frame->move( CONF_ANALY_WIDTH+2*SPACE+ui->id_back_btn_start->width()*3, width*3/8+ui->id_back_btn_start->height()/2 );
        ui->id_back_btn_next_frame->move( CONF_ANALY_WIDTH+2*SPACE+ui->id_back_btn_start->width()*4, width*3/8+ui->id_back_btn_start->height()/2 );
        ui->id_back_btn_last_frame->move( CONF_ANALY_WIDTH+2*SPACE+ui->id_back_btn_start->width()*5, width*3/8+ui->id_back_btn_start->height()/2 );
        ui->id_back_btn_play_speed->move( CONF_ANALY_WIDTH+2*SPACE+ui->id_back_btn_start->width()*6, width*3/8+ui->id_back_btn_start->height()/2 );

        ui->id_back_slider_play->move( CONF_ANALY_WIDTH+2*SPACE+ui->id_back_btn_start->width()*7, width*3/8+ui->id_back_btn_start->height()/2 );
        ui->id_back_slider_play->resize( width-ui->id_back_btn_start->width()*7-ui->id_back_btn_replay->width(), ui->id_back_btn_start->height() );
        ui->id_back_btn_replay->move( CONF_ANALY_WIDTH+2*SPACE+ui->id_back_btn_start->width()*7+ui->id_back_slider_play->width(), width*3/8+ui->id_back_btn_start->height()/2 );

        ui->tabWidget_2->move( CONF_ANALY_WIDTH+2*SPACE, width*3/8+2*SPACE+ui->id_back_btn_start->height() );
        ui->tabWidget_2->resize( width, height-width*3/8-ui->id_back_btn_start->height() );

    }
    //ui->id_back_interface->setStyleSheet( "border:1px solid black" );


    //生成一张位图
    QBitmap objBitmap( size() );
    //QPainter用于在位图上绘画
    QPainter painter( &objBitmap );
    //填充位图矩形框(用白色填充)
    //painter.fillRect( rect(), Qt::white );
    painter.setBrush( QColor( 0, 0, 0 ) );
    //在位图上画圆角矩形(用黑色填充)  因为this->rect()是在改变的，所以应该把画圆角的代码放在resize事件中
    painter.drawRoundedRect( this->rect(), 10, 10 );
    //使用setmask过滤即可
    setMask( objBitmap );
}

void VodPlayInterface::SetCursorStyle( VodPlayInterface::Direction direction )
{
    //设置上下左右以及右上、右下、左上、坐下的鼠标形状
    switch( direction )
    {
    case eTop:
    case eBottom:
        setCursor( Qt::SizeVerCursor );
        break;
    case eRight:
    case eLeft:
        setCursor( Qt::SizeHorCursor );
        break;
    case eTopRight:
    case eBottomLeft:
        setCursor( Qt::SizeBDiagCursor );
        break;
    case eRightBottom:
    case eLeftTop:
        setCursor( Qt::SizeFDiagCursor );
        break;
    default:
        setCursor( Qt::ArrowCursor );
        break;
    }
}

void VodPlayInterface::SetDrayMove( int nx_global, int ny_global, VodPlayInterface::Direction direction )
{
    //计算偏差
    int ndX = nx_global - m_pt_press_global.x();
    int ndY = ny_global - m_pt_press_global.y();
    //qDebug()<<"0000"<<nx_global<<ny_global<<m_pt_press_global.x()<<m_pt_press_global.y()<<ndX<<ndY;
    //获得主窗口位置信息
    QRect rectWindow = geometry();
    bool b_top_change = false;
    bool b_right_change = false;
    bool b_left_change = false;
    bool b_bottom_change = false;
    QRect rect_temp = rectWindow;


    if( direction == eTop ) {
       //ndY>0 往下移动
       if ( ndY > 0 ) {
             rect_temp.setTop( rect_temp.top()+ndY );
            if ( rect_temp.height()<=minimumHeight() ) {
                b_top_change = false;
            } else {
                b_top_change = true;
            }
        } else {
            if ( ny_global > rect_temp.top() ) { //当鼠标的当前位置大于窗口的top时，不做操作
                b_top_change = false;
            } else {
                b_top_change = true;
            }
        }
    } else if( direction == eBottom ) {
        if ( ndY > 0 ) {
            if ( ny_global < rectWindow.bottom() ) {
                b_bottom_change = false;
            } else {
               b_bottom_change = true;
            }
         } else {
            rect_temp.setBottom( rect_temp.bottom()+ndY );
            if ( rect_temp.height()<=minimumHeight() ) {
                b_bottom_change = false;
            } else {
               b_bottom_change = true;
            }
         }
    } else if( direction == eRight ) {
        //ndX>0往右移动
        if ( ndX > 0 ) {
            if ( nx_global < rectWindow.right() ) {
                b_right_change = false;
            } else {
                b_right_change = true;
            }
         } else {
            rect_temp.setRight( rect_temp.right()+ndX );
            if ( rect_temp.width()<=minimumWidth() ) {
                b_right_change = false;
            } else {
                b_right_change = true;
            }
         }
    } else if( direction == eLeft ) {
        if( ndX>0 ) {
            rect_temp.setLeft( rect_temp.left()+ndX );
            if( rect_temp.width()<= minimumWidth() ){
                b_left_change = false;
            } else {
                b_left_change = true;
            }
        } else {
            if( nx_global > rectWindow.left() ) {
                b_left_change = false;
            } else {
                b_left_change = true;
            }
        }
    } else if( direction == eTopRight ) {
        //ndY>0 往下移动
        if ( ndY > 0 ) {
            rect_temp.setTop( rect_temp.top()+ndY );
            if ( rect_temp.height()<=minimumHeight() ) {
                b_top_change = false;
            } else {
                //rectWindow = rct_temp;
                b_top_change = true;
            }
        } else {
             if ( ny_global > rectWindow.top() ) {
                 b_top_change = false;
             } else {
                 //rectWindow = rct_temp;
                 b_top_change = true;
             }
        }
        if( ndX >0 ) {
            if ( nx_global < rectWindow.right() ) {
                b_right_change = false;
            } else {
                b_right_change = true;
                //rectWindow.setRight( rectWindow.right()+ndX );
            }
        } else {
            rect_temp.setRight( rect_temp.right()+ndX );
            if ( rect_temp.width()<=minimumWidth() ) {
                b_right_change = false;
            } else {
                b_right_change = true;
                //rectWindow = rct_temp;
            }
        }
    } else if( direction == eBottomLeft ) {
        if ( ndY > 0 ) {
            if ( ny_global < rectWindow.bottom() ) {
                b_bottom_change = false;
            } else {
                b_bottom_change = true;
            }
         } else {
            rect_temp.setBottom( rect_temp.bottom()+ndY );
            if ( rect_temp.height()<=minimumHeight() ) {
                b_bottom_change = false;
            } else {
                b_bottom_change = true;
            }
         }
        if( ndX>0 ) {
            rect_temp.setLeft( rect_temp.left()+ndX );
            if( rect_temp.width()<= minimumWidth() ){
                b_left_change = false;
            } else {
                b_left_change = true;
            }
        } else {
            if( nx_global > rectWindow.left() ) {
                b_left_change = false;
            } else {
                b_left_change = true;
            }
        }
    } else if( direction == eRightBottom ) {
        //ndX>0往右
        if ( ndX > 0 ) {
            if ( nx_global < rectWindow.right() ) {
                b_right_change = false;
            } else {
                b_right_change = true;
            }
         } else {
            rect_temp.setRight( rect_temp.right()+ndX );
            if ( rect_temp.width()<=minimumWidth() ) {
                b_right_change = false;
            } else {
                b_right_change = true;
            }
         }
         if ( ndY > 0 ) {
            if ( ny_global < rectWindow.bottom() ) {
                b_bottom_change = false;
            } else {
                b_bottom_change = true;
            }
         } else {
            rect_temp.setBottom( rect_temp.bottom()+ndY );
            if ( rect_temp.height()<=minimumHeight() ) {
                b_bottom_change = false;
            } else {
                b_bottom_change = true;
            }
         }

    } else if( direction == eLeftTop ) {
        //ndY>0 往下移动
        if ( ndY > 0 ) {
             rect_temp.setTop( rect_temp.top()+ndY );
             if ( rect_temp.height()<=minimumHeight() ) {
                 b_top_change = false;
             } else {
                 b_top_change = true;
             }
         } else {
             if ( ny_global > rectWindow.top() ) {
                 b_top_change = false;
             } else {
                 b_top_change = true;
             }
         }
        if( ndX>0 ) {
            rect_temp.setLeft( rect_temp.left()+ndX );
            if( rect_temp.width()<= minimumWidth() ){
                b_left_change = false;
            } else {
                b_left_change = true;
            }
        } else {
            if( nx_global > rectWindow.left() ) {
                b_left_change = false;
            } else {
                b_left_change = true;
            }
        }
    }

    if( b_top_change ) {
        rectWindow.setTop( rectWindow.top() + ndY );
    }
    if( b_bottom_change ) {
        rectWindow.setBottom( rectWindow.bottom() + ndY );
    }
    if( b_right_change ) {
        rectWindow.setRight( rectWindow.right() + ndX );
    }
    if( b_left_change ) {
        rectWindow.setLeft( rectWindow.left() + ndX );
    }


    setGeometry( rectWindow );

}


VodPlayInterface::Direction VodPlayInterface::PointValid( double x, double y )
{
    Direction direction;
    if( !mp_title_bar->is_click_in_btn( x, y ) ) {
            direction = eNone;
    } else if( x>width()-10 && x<=width()+10 && y<10 && y>=-10 ) {
        direction = eTopRight;
    } else if( x<10 && x>=-10 && y>height()-10 && y<=height()+10 ) {
        direction = eBottomLeft;
    } else if( x>width()-10 && x<=width()+10 && y>height()-10 && y<=height()+10 ) {
        direction = eRightBottom;
    } else if( x<10 && x>=-10 && y<10 && y>=-10 ) {
        direction = eLeftTop;
    } else if( x>=10 && x<=width()-10 && y>=-10 && y<10 ) {
        direction = eTop;
    } else if( x>=10 &&x<=width()-10 && y>height()-10 && y<=height()+10 ) {
        direction = eBottom;
    } else if( x>=width()-10 && x<width()+10 && y>=10 && y<=height()-10 ) {
        direction = eRight;
    } else if( x>=-10 && x<10 && y>=10 && y<=height()-10 ) {
        direction = eLeft;
    } else {
        direction = eNone;
    }

    return direction;

}

void VodPlayInterface::paintEvent(QPaintEvent *event)
{
    QPainter painter( this );
    //标题栏背景图片
    painter.drawPixmap( 0, 0, ui->centralWidget->width(), TITLE_BAR_HEIGHT, QPixmap("../image/title_bar.jpg"));
    //整个窗口的背景图片
    //painter.drawPixmap( 0, 0, ui->centralWidget->width(), ui->centralWidget->height(), QPixmap("../image/frame.jpg"));

    QPainter draw;
    draw.begin( this );

    QMap<quint64,NetCaptureVod*> p_vods;
    mp_netcontrol->get_vods( p_vods );
    if( p_vods.size() > 4 ) {
        qDebug() << p_vods.size();
    }
    for( QMap<quint64,NetCaptureVod*>::iterator it = p_vods.begin(); it != p_vods.end(); it++ ) {
        quint64 i_key = it.key();
        NetCaptureVod *p_vod = it.value();

        int i_class = (int)(i_key >> 32);
        int i_no = (int)(i_key & 0x00000000FFFFFFFF);
        if( i_class == DEVICE_INFRARED ) {
            if( i_no == 1 ) {
                if( p_vod->m_vod_image.isNull() ) {
                    draw.fillRect( 0, TITLE_BAR_HEIGHT, ui->id_monitor_frame_1->width(), ui->id_monitor_frame_1->height(), QBrush( QColor( 200, 200, 200 ) ) );
                } else {
                    //draw.drawImage( QRect( 0, TITLE_BAR_HEIGHT, ui->id_monitor_frame_1->width()/2, ui->id_monitor_frame_1->height() ), p_vod->m_vod_image );
                }
            }
            if( i_no == 2 ) {
                if( p_vod->m_vod_image.isNull() ) {
                    draw.fillRect( 0, TITLE_BAR_HEIGHT + ui->id_monitor_frame_1->height(), ui->id_monitor_frame_3->width(), ui->id_monitor_frame_3->height(), QBrush( QColor( 200, 200, 200 ) ) );
                } else {
                    draw.drawImage( QRect( 0, TITLE_BAR_HEIGHT + ui->id_monitor_frame_1->height(), ui->id_monitor_frame_3->width()/2, ui->id_monitor_frame_3->height() ), p_vod->m_vod_image );
                }
            }
            if( i_no == 3 ) {
                if( p_vod->m_vod_image.isNull() ) {
                    draw.fillRect( 0, TITLE_BAR_HEIGHT + ui->id_monitor_frame_1->height() + ui->id_monitor_frame_3->height(), ui->id_monitor_frame_5->width(), ui->id_monitor_frame_5->height(), QBrush( QColor( 200, 200, 200 ) ) );
                } else {
                    draw.drawImage( QRect( 0, TITLE_BAR_HEIGHT + ui->id_monitor_frame_1->height() + ui->id_monitor_frame_3->height(), ui->id_monitor_frame_5->width()/2, ui->id_monitor_frame_5->height() ), p_vod->m_vod_image );
                }
            }
            if( i_no == 4 ) {
                if( p_vod->m_vod_image.isNull() ) {
                    draw.fillRect( 0, TITLE_BAR_HEIGHT + ui->id_monitor_frame_1->height() + ui->id_monitor_frame_3->height() + ui->id_monitor_frame_5->height(), ui->id_monitor_frame_7->width(), ui->id_monitor_frame_7->height(), QBrush( QColor( 200, 200, 200 ) ) );
                } else {
                    draw.drawImage( QRect( 0, TITLE_BAR_HEIGHT + ui->id_monitor_frame_1->height() + ui->id_monitor_frame_3->height() + ui->id_monitor_frame_5->height(), ui->id_monitor_frame_7->width()/2, ui->id_monitor_frame_7->height() ), p_vod->m_vod_image );
                }
            }

            if( i_no == 5 ) {
                if( p_vod->m_vod_image.isNull() ) {
                    draw.fillRect( ui->id_monitor_frame_1->width(), TITLE_BAR_HEIGHT, ui->id_monitor_frame_2->width(), ui->id_monitor_frame_2->height(), QBrush( QColor( 200, 200, 200 ) ) );
                } else {
                    draw.drawImage( QRect( ui->id_monitor_frame_1->width(), TITLE_BAR_HEIGHT, ui->id_monitor_frame_2->width()/2, ui->id_monitor_frame_2->height() ), p_vod->m_vod_image );
                }
            }
            if( i_no == 6 ) {
                if( p_vod->m_vod_image.isNull() ) {
                    draw.fillRect( ui->id_monitor_frame_3->width(), TITLE_BAR_HEIGHT + ui->id_monitor_frame_2->height(), ui->id_monitor_frame_4->width(), ui->id_monitor_frame_4->height(), QBrush( QColor( 200, 200, 200 ) ) );
                } else {
                    draw.drawImage( QRect( ui->id_monitor_frame_3->width(), TITLE_BAR_HEIGHT + ui->id_monitor_frame_2->height(), ui->id_monitor_frame_4->width()/2, ui->id_monitor_frame_4->height() ), p_vod->m_vod_image );
                }
            }
            if( i_no == 7 ) {
                if( p_vod->m_vod_image.isNull() ) {
                    draw.fillRect( ui->id_monitor_frame_5->width(), TITLE_BAR_HEIGHT + ui->id_monitor_frame_2->height() + ui->id_monitor_frame_4->height(), ui->id_monitor_frame_6->width(), ui->id_monitor_frame_6->height(), QBrush( QColor( 200, 200, 200 ) ) );
                } else {
                    draw.drawImage( QRect( ui->id_monitor_frame_5->width(), TITLE_BAR_HEIGHT + ui->id_monitor_frame_2->height() + ui->id_monitor_frame_4->height(), ui->id_monitor_frame_6->width()/2, ui->id_monitor_frame_6->height() ), p_vod->m_vod_image );
                }
            }
            if( i_no == 8 ) {
                if( p_vod->m_vod_image.isNull() ) {
                    draw.fillRect( ui->id_monitor_frame_7->width(), TITLE_BAR_HEIGHT + ui->id_monitor_frame_2->height() + ui->id_monitor_frame_4->height() + ui->id_monitor_frame_6->height(), ui->id_monitor_frame_8->width(), ui->id_monitor_frame_8->height(), QBrush( QColor( 200, 200, 200 ) ) );
                } else {
                    draw.drawImage( QRect( ui->id_monitor_frame_7->width(), TITLE_BAR_HEIGHT + ui->id_monitor_frame_2->height() + ui->id_monitor_frame_4->height() + ui->id_monitor_frame_6->height(), ui->id_monitor_frame_8->width()/2, ui->id_monitor_frame_8->height() ), p_vod->m_vod_image );
                }
            }
        }

        if( i_class == DEVICE_VISIBLELIGHT ) {
            if( i_no == 0 ) {
                if( p_vod->m_vod_image.isNull() ) {
                    draw.fillRect( 0, TITLE_BAR_HEIGHT, ui->id_monitor_frame_1->width(), ui->id_monitor_frame_1->height(), QBrush( QColor( 200, 200, 200 ) ) );
                } else {
                    //draw.drawImage( QRect( ui->id_monitor_frame_1->width()/2, TITLE_BAR_HEIGHT, ui->id_monitor_frame_1->width()/2, ui->id_monitor_frame_1->height() ), p_vod->m_vod_image );
                }
            }
            if( i_no == 1 ) {
                if( p_vod->m_vod_image.isNull() ) {
                    draw.fillRect( 0, TITLE_BAR_HEIGHT + ui->id_monitor_frame_1->height(), ui->id_monitor_frame_3->width(), ui->id_monitor_frame_3->height(), QBrush( QColor( 200, 200, 200 ) ) );
                } else {
                    //draw.drawImage( QRect( ui->id_monitor_frame_3->width()/2, TITLE_BAR_HEIGHT + ui->id_monitor_frame_1->height(), ui->id_monitor_frame_3->width()/2, ui->id_monitor_frame_3->height() ), p_vod->m_vod_image );
                }
            }
            if( i_no == 2 ) {
                if( p_vod->m_vod_image.isNull() ) {
                    draw.fillRect( 0, TITLE_BAR_HEIGHT + ui->id_monitor_frame_1->height() + ui->id_monitor_frame_3->height(), ui->id_monitor_frame_5->width(), ui->id_monitor_frame_5->height(), QBrush( QColor( 200, 200, 200 ) ) );
                } else {
                    draw.drawImage( QRect( ui->id_monitor_frame_5->width()/2, TITLE_BAR_HEIGHT + ui->id_monitor_frame_1->height() + ui->id_monitor_frame_3->height(), ui->id_monitor_frame_5->width()/2, ui->id_monitor_frame_5->height() ), p_vod->m_vod_image );
                }
            }
            if( i_no == 3 ) {
                if( p_vod->m_vod_image.isNull() ) {
                    draw.fillRect( 0, TITLE_BAR_HEIGHT + ui->id_monitor_frame_1->height() + ui->id_monitor_frame_3->height() + ui->id_monitor_frame_5->height(), ui->id_monitor_frame_7->width(), ui->id_monitor_frame_7->height(), QBrush( QColor( 200, 200, 200 ) ) );
                } else {
                    draw.drawImage( QRect( ui->id_monitor_frame_7->width()/2, TITLE_BAR_HEIGHT + ui->id_monitor_frame_1->height() + ui->id_monitor_frame_3->height() + ui->id_monitor_frame_5->height(), ui->id_monitor_frame_7->width()/2, ui->id_monitor_frame_7->height() ), p_vod->m_vod_image );
                }
            }
            if( i_no == 4 ) {
                if( p_vod->m_vod_image.isNull() ) {
                    draw.fillRect( ui->id_monitor_frame_1->width(), TITLE_BAR_HEIGHT, ui->id_monitor_frame_2->width(), ui->id_monitor_frame_2->height(), QBrush( QColor( 200, 200, 200 ) ) );
                } else {
                    draw.drawImage( QRect( ui->id_monitor_frame_1->width() + ui->id_monitor_frame_2->width()/2, TITLE_BAR_HEIGHT, ui->id_monitor_frame_2->width()/2, ui->id_monitor_frame_2->height() ), p_vod->m_vod_image );
                }
            }
            if( i_no == 5 ) {
                if( p_vod->m_vod_image.isNull() ) {
                    draw.fillRect( ui->id_monitor_frame_3->width(), TITLE_BAR_HEIGHT + ui->id_monitor_frame_2->height(), ui->id_monitor_frame_4->width(), ui->id_monitor_frame_4->height(), QBrush( QColor( 200, 200, 200 ) ) );
                } else {
                    draw.drawImage( QRect( ui->id_monitor_frame_3->width() + ui->id_monitor_frame_4->width()/2, TITLE_BAR_HEIGHT + ui->id_monitor_frame_2->height(), ui->id_monitor_frame_4->width()/2, ui->id_monitor_frame_4->height() ), p_vod->m_vod_image );
                }
            }
            if( i_no == 6 ) {
                if( p_vod->m_vod_image.isNull() ) {
                    draw.fillRect( ui->id_monitor_frame_5->width(), TITLE_BAR_HEIGHT + ui->id_monitor_frame_2->height() + ui->id_monitor_frame_4->height(), ui->id_monitor_frame_6->width(), ui->id_monitor_frame_6->height(), QBrush( QColor( 200, 200, 200 ) ) );
                } else {
                    draw.drawImage( QRect( ui->id_monitor_frame_5->width() + ui->id_monitor_frame_6->width()/2, TITLE_BAR_HEIGHT + ui->id_monitor_frame_2->height() + ui->id_monitor_frame_4->height(), ui->id_monitor_frame_6->width()/2, ui->id_monitor_frame_6->height() ), p_vod->m_vod_image );
                }
            }
            if( i_no == 7 ) {
                if( p_vod->m_vod_image.isNull() ) {
                    draw.fillRect( ui->id_monitor_frame_7->width(), TITLE_BAR_HEIGHT + ui->id_monitor_frame_2->height() + ui->id_monitor_frame_4->height() + ui->id_monitor_frame_6->height(), ui->id_monitor_frame_8->width(), ui->id_monitor_frame_8->height(), QBrush( QColor( 200, 200, 200 ) ) );
                } else {
                    draw.drawImage( QRect( ui->id_monitor_frame_7->width() + ui->id_monitor_frame_8->width()/2, TITLE_BAR_HEIGHT + ui->id_monitor_frame_2->height() + ui->id_monitor_frame_4->height() + ui->id_monitor_frame_6->height(), ui->id_monitor_frame_8->width()/2, ui->id_monitor_frame_8->height() ), p_vod->m_vod_image );
                }
            }
        }
        /*if( i_class == DEVICE_INFRARED ) {
            if( i_no == 1 ) {
                if( p_vod->m_vod_image.isNull() ) {
                    draw.fillRect( 0, TITLE_BAR_HEIGHT, ui->id_monitor_frame_1->width(), ui->id_monitor_frame_1->height(), QBrush( QColor( 200, 200, 200 ) ) );
                } else {
                    draw.drawImage( QRect( 0, TITLE_BAR_HEIGHT, ui->id_monitor_frame_1->width(), ui->id_monitor_frame_1->height() ), p_vod->m_vod_image );
                }
            }
            if( i_no == 2 ) {
                if( p_vod->m_vod_image.isNull() ) {
                    draw.fillRect( 0, TITLE_BAR_HEIGHT + ui->id_monitor_frame_1->height(), ui->id_monitor_frame_3->width(), ui->id_monitor_frame_3->height(), QBrush( QColor( 200, 200, 200 ) ) );
                } else {
                    draw.drawImage( QRect( 0, TITLE_BAR_HEIGHT + ui->id_monitor_frame_1->height(), ui->id_monitor_frame_3->width(), ui->id_monitor_frame_3->height() ), p_vod->m_vod_image );
                }
            }
            if( i_no == 3 ) {
                if( p_vod->m_vod_image.isNull() ) {
                    draw.fillRect( 0, TITLE_BAR_HEIGHT + ui->id_monitor_frame_1->height() + ui->id_monitor_frame_3->height(), ui->id_monitor_frame_5->width(), ui->id_monitor_frame_5->height(), QBrush( QColor( 200, 200, 200 ) ) );
                } else {
                    draw.drawImage( QRect( 0, TITLE_BAR_HEIGHT + ui->id_monitor_frame_1->height() + ui->id_monitor_frame_3->height(), ui->id_monitor_frame_5->width(), ui->id_monitor_frame_5->height() ), p_vod->m_vod_image );
                }
            }
            if( i_no == 4 ) {
                if( p_vod->m_vod_image.isNull() ) {
                    draw.fillRect( 0, TITLE_BAR_HEIGHT + ui->id_monitor_frame_1->height() + ui->id_monitor_frame_3->height() + ui->id_monitor_frame_5->height(), ui->id_monitor_frame_7->width(), ui->id_monitor_frame_7->height(), QBrush( QColor( 200, 200, 200 ) ) );
                } else {
                    draw.drawImage( QRect( 0, TITLE_BAR_HEIGHT + ui->id_monitor_frame_1->height() + ui->id_monitor_frame_3->height() + ui->id_monitor_frame_5->height(), ui->id_monitor_frame_7->width(), ui->id_monitor_frame_7->height() ), p_vod->m_vod_image );
                }
            }
        }

        if( i_class == DEVICE_VISIBLELIGHT ) {
            if( i_no == 0 ) {
                if( p_vod->m_vod_image.isNull() ) {
                    draw.fillRect( ui->id_monitor_frame_1->width(), TITLE_BAR_HEIGHT, ui->id_monitor_frame_2->width(), ui->id_monitor_frame_2->height(), QBrush( QColor( 200, 200, 200 ) ) );
                } else {
                    draw.drawImage( QRect( ui->id_monitor_frame_1->width(), TITLE_BAR_HEIGHT, ui->id_monitor_frame_2->width(), ui->id_monitor_frame_2->height() ), p_vod->m_vod_image );
                }
            }
            if( i_no == 1 ) {
                if( p_vod->m_vod_image.isNull() ) {
                    draw.fillRect( ui->id_monitor_frame_1->width(), TITLE_BAR_HEIGHT + ui->id_monitor_frame_2->height(), ui->id_monitor_frame_4->width(), ui->id_monitor_frame_4->height(), QBrush( QColor( 200, 200, 200 ) ) );
                } else {
                    draw.drawImage( QRect( ui->id_monitor_frame_1->width(), TITLE_BAR_HEIGHT + ui->id_monitor_frame_2->height(), ui->id_monitor_frame_4->width(), ui->id_monitor_frame_4->height() ), p_vod->m_vod_image );
                }
            }
            if( i_no == 2 ) {
                if( p_vod->m_vod_image.isNull() ) {
                    draw.fillRect( ui->id_monitor_frame_1->width(), TITLE_BAR_HEIGHT + ui->id_monitor_frame_2->height() + ui->id_monitor_frame_4->height(), ui->id_monitor_frame_6->width(), ui->id_monitor_frame_6->height(), QBrush( QColor( 200, 200, 200 ) ) );
                } else {
                    draw.drawImage( QRect( ui->id_monitor_frame_1->width(), TITLE_BAR_HEIGHT + ui->id_monitor_frame_2->height() + ui->id_monitor_frame_4->height(), ui->id_monitor_frame_6->width(), ui->id_monitor_frame_6->height() ), p_vod->m_vod_image );
                }
            }
            if( i_no == 3 ) {
                if( p_vod->m_vod_image.isNull() ) {
                    draw.fillRect( ui->id_monitor_frame_1->width(), TITLE_BAR_HEIGHT + ui->id_monitor_frame_2->height() + ui->id_monitor_frame_4->height() + ui->id_monitor_frame_6->height(), ui->id_monitor_frame_8->width(), ui->id_monitor_frame_8->height(), QBrush( QColor( 200, 200, 200 ) ) );
                } else {
                    draw.drawImage( QRect( ui->id_monitor_frame_1->width(), TITLE_BAR_HEIGHT + ui->id_monitor_frame_2->height() + ui->id_monitor_frame_4->height() + ui->id_monitor_frame_6->height(), ui->id_monitor_frame_8->width(), ui->id_monitor_frame_8->height() ), p_vod->m_vod_image );
                }
            }
        }*/
    }
    //红外显示//n_height*4/5+SPACE,SPACE
    //if( m_ir_image.isNull() ) {
        //draw.fillRect( 165, 50, 640, 480, QBrush( QColor( 200, 200, 200 )));
        //draw.fillRect( 0, TITLE_BAR_HEIGHT, ui->id_monitor_frame_1->width(), ui->id_monitor_frame_1->height(), QBrush( QColor( 200, 200, 200 ) ) );
    //} else {
        //QRect rect = ui->id_frame_ir->rect();
        //draw.drawImage( 165, 50, m_ir_image, 165, 50, ui->id_frame_ir->width(), ui->id_frame_ir->height() );
        //draw.drawImage( QRect( 0, TITLE_BAR_HEIGHT, ui->id_monitor_frame_1->width(), ui->id_monitor_frame_1->height() ), m_ir_image );
        //qDebug()<<"frame:"<<ui->id_frame_ir->width()<<ui->id_frame_ir->height()<<m_ir_image.width()<<m_ir_image.height();
    //}

    //可见光显示
    //if( mp_vi_image->isNull() ) {
        //draw.fillRect( ui->id_monitor_frame_1->width(), TITLE_BAR_HEIGHT, ui->id_monitor_frame_2->width(), ui->id_monitor_frame_2->height(), QBrush( QColor( 200, 200, 200 ) ) );
    //} else {
        //draw.drawImage( QRect( ui->id_monitor_frame_1->width(), TITLE_BAR_HEIGHT, ui->id_monitor_frame_2->width(), ui->id_monitor_frame_2->height() ), *mp_vi_image );
        //draw.drawImage(QRect(WIDTH_LIST_VIEW_DEVICE+SPACE+ui->id_textedit_device->width(), ui->id_main_tool_bar->height()+SPACE + ui->id_frame_ir->height(), ui->id_frame_ana->width(), ui->id_frame_ana->height()), m_vi_image);
    //}

    draw.end();

}

void VodPlayInterface::slot_display_monitor_interface()
{
    ui->stackedWidget->setCurrentIndex( 0 );
    mp_title_bar->mp_btn_config_interface->show();
    mp_title_bar->mp_btn_analyse_interface->hide();
    mp_title_bar->mp_btn_backplay_interface->show();
    mp_title_bar->mp_btn_monitor_interface->hide();
}

void VodPlayInterface::slot_display_config_interface()
{
    //设置按钮的字体颜色
    // mp_title_bar->->setStyleSheet("color:black");
    //mp_button_new->setStyleSheet("color:red");
    ui->stackedWidget->setCurrentIndex( 1 );
    ui->id_config_interface->update_group_info();

    mp_title_bar->mp_btn_config_interface->hide();
    mp_title_bar->mp_btn_analyse_interface->hide();
    mp_title_bar->mp_btn_backplay_interface->hide();
    mp_title_bar->mp_btn_monitor_interface->show();

}

void VodPlayInterface::slot_display_analyse_interface()
{
    ui->stackedWidget->setCurrentIndex( 2 );
    mp_title_bar->mp_btn_config_interface->hide();
    mp_title_bar->mp_btn_analyse_interface->hide();
    mp_title_bar->mp_btn_backplay_interface->hide();
    mp_title_bar->mp_btn_monitor_interface->show();
}

void VodPlayInterface::slot_display_back_interface()
{
    ui->stackedWidget->setCurrentIndex( 3 );
    ui->id_back_interface->update_ctrl_info();
    mp_title_bar->mp_btn_config_interface->hide();
    mp_title_bar->mp_btn_analyse_interface->hide();
    mp_title_bar->mp_btn_backplay_interface->hide();
    mp_title_bar->mp_btn_monitor_interface->show();
}



void VodPlayInterface::show_contextmenu( const QPoint& pos ) {
    if( mp_cmenu ) {//保证同时只存在一个menu，及时释放内存
        delete mp_cmenu;
        mp_cmenu = NULL;
    }
    mp_cmenu = new QMenu( ui->id_monitor_interface );

    //QAction *p_action_monitor_widget = mp_cmenu->addAction( "监控主窗口" );
    //QAction *p_action_config_widget = mp_cmenu->addAction( "配置窗口" );
    QAction *p_action_analyse_widget = mp_cmenu->addAction( "分析窗口" );
    QAction *p_action_analyse_back = mp_cmenu->addAction( "回放窗口" );


    //connect( p_action_monitor_widget, SIGNAL( triggered( bool ) ), this, SLOT( slot_display_monitor_interface() ) );
    //connect( p_action_config_widget, SIGNAL( triggered( bool ) ), this, SLOT( slot_display_config_interface() ) );
    connect( p_action_analyse_widget, SIGNAL( triggered( bool ) ), this, SLOT( slot_display_analyse_interface() ) );
    connect( p_action_analyse_back, SIGNAL( triggered( bool ) ), this, SLOT( slot_display_back_interface() ) );

    mp_cmenu->exec( QCursor::pos() );//在当前鼠标位置显示
    //cmenu->exec(pos)是在viewport显示
}

void VodPlayInterface::slot_show_max_size()
{
    if( !mb_max_win ) {//如果窗口没有放大，那么记录当前的窗口大小，并放大窗口
        m_rect_restore_window = geometry();
        //setGeometry( qApp->desktop()->availableGeometry() );
        showMaximized();
    } else {
        //setGeometry( m_rect_restore_window );
        showNormal();
    }
    mb_max_win = !mb_max_win;
    /*if( mb_max_win == false ) {
        showMaximized();
        //showFullScreen();
        //mb_max_size = true;
        mb_max_win = true;
    } else {
        //还原窗口大小
        showNormal();
        //mb_max_size = false;
        mb_max_win = false;
    }*/
}

void VodPlayInterface::init_net_timer()
{
    mp_timer_heart_tag = new QTimer( this );
    mp_timer_update_xml = new QTimer( this );

    connect( mp_timer_heart_tag, SIGNAL( timeout() ), this, SLOT( send_heart_tag() ) );
    connect( mp_timer_update_xml, SIGNAL( timeout() ), this, SLOT( send_update_xml() ) );

    mp_timer_heart_tag->start( 1000 );
    mp_timer_update_xml->start( 10000 );
}

void VodPlayInterface::uninit_net_timer()
{
    if ( mp_timer_heart_tag ) {
        mp_timer_heart_tag->stop();
        delete mp_timer_heart_tag;
        mp_timer_heart_tag = 0;
    }

    if ( mp_timer_update_xml ) {
        mp_timer_update_xml->stop();
        delete mp_timer_update_xml;
        mp_timer_update_xml = 0;
    }
}

void VodPlayInterface::send_update_xml()
{
    if ( mp_netcontrol ) {
        mp_netcontrol->send_update_xml();
    }
}

void VodPlayInterface::send_heart_tag()
{
    if ( mp_netcontrol ) {
        //mp_netcontrol->send_heart_tag();
    }
}
