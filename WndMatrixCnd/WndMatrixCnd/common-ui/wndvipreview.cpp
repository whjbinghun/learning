#include <QPainter>
#include "wndvipreview.h"
#include "datamode.h"
#include <QFileDialog>
#include "cbasemsg.h"
#include "mscclienttype.h"
#include "../common-funtion/msclientlog.h"

#define SPACE 20
#define LABEL_HEIGHT 32
#define BTN_WIDTH_HEIGHT 35
#define NSPACE 5

WndViPreview::WndViPreview(QWidget *parent) :
    QFrame(parent)
  , mb_backplay( false )
  , mp_timer_update( 0 )
  , mp_tool_label( NULL )
  , mb_dynamic_toolbar( false )
  , m_pt_wnd_start( QPoint( 0, 0 ) )
  , m_pt_wnd_end( QPoint( 0, 0 ) )
  , mb_sub_streaming( true )
  , mb_connecting( true )
  , mb_vi_connected( true )
  , ms_pan_sn( "" )
  , ms_rail_sn( "" )
  , mp_ptz_rail_view( NULL )
  , mn_rail( -1 )
  , mb_full_screen( false )
  , mb_sub_stream_connected( false )
  , mb_main_stream_connected( false )
  , m_str_group_name( "" )
{
    init_widget();
    init_connect();
    init_pan_rail_btn();
}

WndViPreview::~WndViPreview()
{
}

void WndViPreview::init_pan_rail_btn()
{
    mp_btn_left = NULL;
    mp_btn_right = NULL;
    mp_btn_top = NULL;
    mp_btn_buttom = NULL;
    mp_btn_lefttop = NULL;
    mp_btn_leftbuttom = NULL;
    mp_btn_righttop = NULL;
    mp_btn_rightbuttom = NULL;

    mp_btn_hor_rail_left = NULL;
    mp_btn_hor_rail_right = NULL;
    mp_btn_ver_rail_top = NULL;
    mp_btn_ver_rail_buttom = NULL;

    if( MSCClientType::Instance()->get_client_type() == ClientPresetSet ) {
        mp_ptz_rail_view = new CPtzRailView();
        add_pan_btn();
        add_rail_btn();
        for( int i=0; i<m_list_btn.size(); i++ ) {
            QGraphicsOpacityEffect *p_effect = new QGraphicsOpacityEffect();
            m_list_effect.push_back( p_effect );
        }

        for( int i=0; i<m_list_rail_btn.size(); i++ ) {
            QGraphicsOpacityEffect *p_effect = new QGraphicsOpacityEffect();
            m_list_rail_effect.push_back( p_effect );
        }
        set_btn_pan_transparency( 0.2f );
        set_btn_rail_transparency( 0.2f );
        init_btn_connect();
    }
}

void WndViPreview::init_connect()
{
}

void WndViPreview::paintEvent( QPaintEvent *event )
{
    bool b_has_group( false );
    QImage*  p_image = NULL, *p_sub_image = NULL, *p_main_image = NULL;
    QImage   tmp, sub_image, main_image;

    DeviceGroup devGroup;
    b_has_group = CDataMode::Instance()->get_group_info( mn_group_number,devGroup );

    if( b_has_group ) {
        if ( mb_backplay ){
            p_image = CDataMode::Instance()->get_main_image( mn_group_number, 2, mb_backplay ? 2:1, tmp );
        }else{
            p_main_image = CDataMode::Instance()->get_main_image( mn_group_number, DeviceVI, mb_backplay ? 2:1, main_image );
            p_sub_image = CDataMode::Instance()->get_sub_image( mn_group_number, DeviceVI, sub_image );
            if ( !mb_sub_streaming ){
                p_image = p_main_image;
                tmp = main_image;
            }else{
                p_image = p_sub_image;
                tmp = sub_image;
            }

            if( mb_full_screen ){
                if( mb_main_stream_connected && p_main_image != NULL && !p_main_image->isNull() ){ //主码流已连接上,且有数据的情况下
                    mb_sub_streaming = false;
                    if( mb_sub_stream_connected ){
                        MS_LOGER_INFO( "组别名%s发送可见光断开子码流信号", m_str_group_name.toStdString().data() );
                        emit sig_disconnect_device( mn_group_number, DeviceVI, true );  //断开子码流
                    }
                }
            } else {
                if( mb_sub_stream_connected && p_sub_image != NULL && !p_sub_image->isNull() ){
                    mb_sub_streaming = true;
                    if( mb_main_stream_connected ){
                        MS_LOGER_INFO( "组别名%s发送可见光断开主码流信号", m_str_group_name.toStdString().data() );
                        emit sig_disconnect_device( mn_group_number, DeviceVI, false ); //断开主码流
                    }
                }
            }
        }
    }

    QPainter draw;
    draw.begin( this );

    QRect rct_vi;
    if ( width()*3/4 > height() ) {
        rct_vi = QRect( 0, 0, height()*4/3, height() );
    } else {
        rct_vi = QRect( 0, 0, width(), width()*3/4  );
    }

    rct_vi = QRect( 0, 0, width(), height() );
    draw.setPen( Qt::white );
    if( mb_vi_connected ) {
        if ( b_has_group && devGroup.dev_vi.str_sn != "" ) {
            if( p_image != NULL ) {
                if( p_image->isNull() ) {
                    draw.fillRect( rct_vi, QBrush( QColor( 200, 200, 200 ) ) );
#ifdef QT_DEBUG
                    if ( mb_sub_streaming )
                        draw.drawText( rct_vi, Qt::AlignCenter, "正在连接可见光子码流......" );
                    else
                        draw.drawText( rct_vi, Qt::AlignCenter, "正在连接可见光主码流......" );
#else
                    draw.drawText( rct_vi, Qt::AlignCenter, "正在连接可见光" );
#endif
                } else {
                    draw.drawImage( rct_vi, tmp );
                }
            } else {
                draw.fillRect( rct_vi, QBrush( QColor( 200, 200, 200 ) ) );
                draw.drawText( rct_vi, Qt::AlignCenter, "正在连接可见光" );
            }
        } else {
            draw.fillRect( rct_vi, QBrush( QColor( 200, 200, 200 ) ) );
            draw.drawText( rct_vi, Qt::AlignCenter, mb_backplay?"未选择播放视频":"未配置可见光" );
        }
    } else {
       // draw.drawImage( rct_vi, tmp );
        draw.fillRect( rct_vi, QBrush( QColor( 200, 200, 200 ) ) );
        draw.drawText( rct_vi, Qt::AlignCenter, "可见光断开,正在连接......" );
    }

    draw.drawRect( 0, 0, width()-1, height()-1 );
    draw.end();
}

void WndViPreview::set_group_number( int n_num )
{
    mn_group_number = n_num;
    m_str_group_name = get_group_str_name();
    if ( !mp_timer_update ) {
        mp_timer_update = new QTimer( this );
        connect( mp_timer_update, SIGNAL( timeout() ), this, SLOT( timer_update() ) );
    }

    if ( n_num > 0 ) {
        mp_timer_update->start( 40 );
    } else {
        mp_timer_update->stop();
        update();
    }

    DeviceGroup  dev_group;
    CDataMode::Instance()->get_group_info(mn_group_number,dev_group);
    if( dev_group.dev_vi.str_sn != "" && !mb_dynamic_toolbar ) {
        set_toolbar_hidden( false );
    } else {
        set_toolbar_hidden( true );
    }

    if( MSCClientType::Instance()->get_client_type() == ClientPresetSet ) {
        if( dev_group.dev_vi.b_have_ptz == true ) {
         ms_pan_sn = dev_group.dev_vi.str_sn;
        }

        if( dev_group.dev_ir.b_have_ptz == true ) {
         //ms_rail_sn = dev_group.dev_ir.str_sn;
        }
        if( ms_pan_sn == "" ) {
            ms_pan_sn = dev_group.dev_ptz.str_sn;
        }

        ms_rail_sn = dev_group.dev_rail.str_sn;
        if( ms_pan_sn != "" ) {
            set_pan_hidden( false );
            move_btn_pan();
        } else {
            set_pan_hidden( true );
        }
        if( ms_rail_sn != "" ) {
            mn_rail = dev_group.dev_rail.n_class;
            set_rail_hidden( false );
            move_btn_rail();
        } else {
            mn_rail = -1;
            set_rail_hidden( true );
        }
    }
}

void WndViPreview::set_funtion(bool b_backplay)
{
    mb_backplay = b_backplay;
}

void WndViPreview::timer_update()
{
    if ( !isHidden() ) {
#ifdef QT_DEBUG
    if ( !mb_connecting ){
        return;
    }
#endif
        update();
    }
}

void WndViPreview::init_widget()
{
    mp_tool_label = new Vi_Ana_Label( this );
    QGraphicsOpacityEffect *effect = new QGraphicsOpacityEffect( this );
    effect->setOpacity( 0.5 );
    mp_tool_label->setGraphicsEffect( effect );
    mp_tool_label->hide();
}

void WndViPreview::resizeEvent( QResizeEvent *event )
{
    mp_tool_label->move( width()-mp_tool_label->get_wnd_width()-SPACE, SPACE );
    mp_tool_label->resize( mp_tool_label->get_wnd_width(), LABEL_HEIGHT );

    if( ms_pan_sn != "" ) {
        move_btn_pan();
    }
    if( ms_rail_sn != "" ) {
        move_btn_rail();
    }
}

void WndViPreview::tool_bar_function( int n_press_status, bool b_fullscreen_show, bool b_down_or_up )
{
    DeviceGroup devGroup;
    bool b_has_group = CDataMode::Instance()->get_group_info(mn_group_number,devGroup);

    if ( b_down_or_up ) {
        if( n_press_status == 0 ) {
            focus_in( devGroup.dev_vi.str_sn );
            emit sig_focus_in();
        } else if( n_press_status == 1 ) {
            focus_out( devGroup.dev_vi.str_sn );
            emit sig_focus_out();
        }
    } else {
        if( n_press_status == 0 ) {
            focus_in_stop( devGroup.dev_vi.str_sn );
            emit sig_focus_in_stop();
        } else if( n_press_status == 1 ) {
            focus_out_stop( devGroup.dev_vi.str_sn );
            emit sig_focus_out_stop();
        } else if ( n_press_status == 2 ) {
            capture_image();
        } else if( n_press_status == 3 ) {
            //释放时调用可见光全屏
            if( b_fullscreen_show ) {
                mb_full_screen = true;
                emit sig_show_fullscreen();
                // 连接主码流
                emit sig_connect_device( mn_group_number, DeviceVI, false );

            } else {
                mb_full_screen = false;
                emit sig_show_normal();
                emit sig_connect_device( mn_group_number, DeviceVI, true );
            }
        }
    }
}

//可见光抓拍图片
void WndViPreview::capture_image()
{
    //获取当前的image数据
    bool b_has_group( false );
    QImage*  p_image;
    QImage   tmp;

    DeviceGroup devGroup;
    b_has_group = CDataMode::Instance()->get_group_info( mn_group_number, devGroup );

    QString fileName = QFileDialog::getSaveFileName( this,
            tr("Open Config"),
            "",
            tr("Config Files (*.jpg)"));

    if ( !fileName.isNull() )
    {
        //fileName是文件名
        if( b_has_group && devGroup.dev_vi.str_sn != "" ) {
           if ( !mb_sub_streaming ){
               p_image = CDataMode::Instance()->get_main_image( mn_group_number, DeviceVI, 1, tmp );
           }else{
               p_image = CDataMode::Instance()->get_sub_image( mn_group_number, DeviceVI, tmp );
           }
           p_image->save( fileName, "JPG" );
        }
    }
}

void WndViPreview::set_toolbar_hidden( bool b_hidden )
{
    mp_tool_label->setHidden( b_hidden );
}


void WndViPreview::set_vi_stream_status( int n_stream_type, bool b_connected )
{
    if( n_stream_type == StreamSub ){
        mb_sub_stream_connected = b_connected;
    }else if( n_stream_type == StreamMain ){
        mb_main_stream_connected = b_connected;
    }
}


#ifdef QT_DEBUG
void WndViPreview::test_connect_main_stream()
{
 //   emit sig_connect_device( mn_group_number, DeviceVI, false );
    mb_connecting = true;
}

void WndViPreview::test_connect_sub_stream()
{
//    emit sig_connect_device( mn_group_number, DeviceVI, true );
    mb_connecting = true;
}

void WndViPreview::test_disconnect_main_stream()
{
//    emit sig_disconnect_device( mn_group_number, DeviceVI, false );
    mb_connecting = false;
}

void WndViPreview::test_disconnect_sub_stream()
{
 //   emit sig_disconnect_device( mn_group_number, DeviceVI, true );
    mb_connecting = false;
}

#endif

void WndViPreview::enterEvent( QEvent * e )
{
    if ( mb_dynamic_toolbar ) {
        DeviceGroup  dev_group;

        CDataMode::Instance()->get_group_info( mn_group_number, dev_group );
        if( dev_group.dev_vi.str_sn != "" && mp_tool_label->isHidden() ) {
            set_toolbar_hidden( false );
        }
    }

    if( MSCClientType::Instance()->get_client_type() == ClientPresetSet ) {
        if( ms_pan_sn != "" ) {
            set_btn_pan_transparency( 0.8f );
        }

        if( ms_rail_sn != "" ) {
            set_btn_rail_transparency( 0.8f );
        }
    }
}

void WndViPreview::leaveEvent( QEvent * e )
{
    if ( mb_dynamic_toolbar ) {
        if(  !mp_tool_label->isHidden() ) {
            set_toolbar_hidden( true );
        }
    }

    if( MSCClientType::Instance()->get_client_type() == ClientPresetSet ) {
        if( ms_pan_sn != "" ) {
            set_btn_pan_transparency( 0.2f );
        }

        if( ms_rail_sn != "" ) {
            set_btn_rail_transparency( 0.2f );
        }
    }
}

void WndViPreview::keyPressEvent( QKeyEvent  *event )
{
    if( event->key()==Qt::Key_Escape && mp_tool_label->get_fullscreen_status() )
    {
        vi_recovery_show();
    }
    QFrame::keyPressEvent( event );
}

void WndViPreview::mouseDoubleClickEvent( QMouseEvent *event )
{
    if( mp_tool_label->get_fullscreen_status() ) {
        vi_recovery_show();
    } else {
        QFrame::mouseDoubleClickEvent( event );
    }
}

bool WndViPreview::event( QEvent *e )
{
    if ( e->type() == QEvent::Close ){
        e->ignore();
        return true;
    }

    return QFrame::event( e );
}


void WndViPreview::vi_recovery_show()
{
    mp_tool_label->set_fullscreen_status( false );
    mp_tool_label->set_fullscreen_text();
    emit sig_show_normal();
    // 连接次码流
    emit sig_connect_device( mn_group_number, DeviceVI, true );
  //  mp_ir_iv_connect->connect_device( mn_group_number, DeviceVI, true );
//    mb_sub_streaming = true;
}

void WndViPreview::add_pan_btn()
{
    int n_btn_width = BTN_WIDTH_HEIGHT;
    mp_btn_left = new QPushButton(this);
    mp_btn_left->setStyleSheet(tr("border-width: 0px;"));
    mp_btn_left->resize(n_btn_width, n_btn_width);
    mp_btn_left->setIcon(QIcon(":image/image/left.png"));
    m_list_btn.push_back( mp_btn_left );

    mp_btn_right= new QPushButton(this);
    mp_btn_right->setStyleSheet(tr("border-width: 0px;"));
    mp_btn_right->resize(n_btn_width, n_btn_width);
    mp_btn_right->setIcon(QIcon(":image/image/right.png"));
    m_list_btn.push_back( mp_btn_right );

    mp_btn_top= new QPushButton(this);
    mp_btn_top->setStyleSheet(tr("border-width: 0px;"));
    mp_btn_top->resize(n_btn_width, n_btn_width);
    mp_btn_top->setIcon(QIcon(":image/image/top.png"));
    m_list_btn.push_back( mp_btn_top );

    mp_btn_buttom= new QPushButton(this);
    mp_btn_buttom->setStyleSheet(tr("border-width: 0px;"));
    mp_btn_buttom->resize(n_btn_width, n_btn_width);
    mp_btn_buttom->setIcon(QIcon(":image/image/buttom.png"));
    m_list_btn.push_back( mp_btn_buttom );

    mp_btn_lefttop= new QPushButton(this);
    mp_btn_lefttop->setStyleSheet(tr("border-width: 0px;"));
    mp_btn_lefttop->resize(n_btn_width, n_btn_width);
    mp_btn_lefttop->setIcon(QIcon(":image/image/top_left.png"));
    m_list_btn.push_back( mp_btn_lefttop );

    mp_btn_leftbuttom= new QPushButton(this);
    mp_btn_leftbuttom->setStyleSheet(tr("border-width: 0px;"));
    mp_btn_leftbuttom->resize(n_btn_width, n_btn_width);
    mp_btn_leftbuttom->setIcon(QIcon(":image/image/left_buttom.png"));
    m_list_btn.push_back( mp_btn_leftbuttom );

    mp_btn_righttop= new QPushButton(this);
    mp_btn_righttop->setStyleSheet(tr("border-width: 0px;"));
    mp_btn_righttop->resize(n_btn_width, n_btn_width);
    mp_btn_righttop->setIcon(QIcon(":image/image/top_right.png"));
    m_list_btn.push_back( mp_btn_righttop );

    mp_btn_rightbuttom= new QPushButton(this);
    mp_btn_rightbuttom->setStyleSheet(tr("border-width: 0px;"));
    mp_btn_rightbuttom->resize(n_btn_width, n_btn_width);
    mp_btn_rightbuttom->setIcon(QIcon(":image/image/right_bottom.png"));
    m_list_btn.push_back( mp_btn_rightbuttom );

    //set_btn_pan_transparency( 0.2f );
    set_pan_hidden( true );
}

void WndViPreview::add_rail_btn()
{
    int n_btn_width = BTN_WIDTH_HEIGHT;
    int n_btn_height = BTN_WIDTH_HEIGHT;
    //if( n_rail == RAIL_H ) {
        mp_btn_hor_rail_left = new QPushButton( this );
        mp_btn_hor_rail_left->resize( n_btn_width, n_btn_height );
        mp_btn_hor_rail_left->setIcon(QIcon(":image/image/left.png"));

        mp_btn_hor_rail_right = new QPushButton( this );
        mp_btn_hor_rail_right->resize( n_btn_width, n_btn_height );
        mp_btn_hor_rail_right->setIcon(QIcon(":image/image/right.png"));
    //} else if( n_rail == RAIL_V ) {
        mp_btn_ver_rail_top = new QPushButton( this );
        mp_btn_ver_rail_top->resize( n_btn_width, n_btn_height );
        mp_btn_ver_rail_top->setIcon(QIcon(":image/image/top.png"));

        mp_btn_ver_rail_buttom = new QPushButton( this );
        mp_btn_ver_rail_buttom->resize( n_btn_width, n_btn_height );
        mp_btn_ver_rail_buttom->setIcon(QIcon(":image/image/buttom.png"));
    //}
        m_list_rail_btn.push_back( mp_btn_hor_rail_left );
        m_list_rail_btn.push_back( mp_btn_hor_rail_right );
        m_list_rail_btn.push_back( mp_btn_ver_rail_top );
        m_list_rail_btn.push_back( mp_btn_ver_rail_buttom );

        //set_btn_rail_transparency( 0.2f );
        set_rail_hidden( true );
}

void WndViPreview::init_btn_connect()
{
    connect( mp_btn_left, SIGNAL( pressed() ), this, SLOT( slot_ptz_pressed() ) );
    connect( mp_btn_left, SIGNAL( released() ), this, SLOT( slot_ptz_release() ) );
    connect( mp_btn_lefttop, SIGNAL( pressed() ), this, SLOT( slot_ptz_pressed() ) );
    connect( mp_btn_lefttop, SIGNAL( released() ), this, SLOT( slot_ptz_release() ) );
    connect( mp_btn_top, SIGNAL( pressed() ), this, SLOT( slot_ptz_pressed() ) );
    connect( mp_btn_top, SIGNAL( released() ), this, SLOT( slot_ptz_release() ) );
    connect( mp_btn_righttop, SIGNAL( pressed() ), this, SLOT( slot_ptz_pressed() ) );
    connect( mp_btn_righttop, SIGNAL( released() ), this, SLOT( slot_ptz_release() ) );
    connect( mp_btn_right, SIGNAL( pressed() ), this, SLOT( slot_ptz_pressed() ) );
    connect( mp_btn_right, SIGNAL( released() ), this, SLOT( slot_ptz_release() ) );
    connect( mp_btn_rightbuttom, SIGNAL( pressed() ), this, SLOT( slot_ptz_pressed() ) );
    connect( mp_btn_rightbuttom, SIGNAL( released() ), this, SLOT( slot_ptz_release() ) );
    connect( mp_btn_buttom, SIGNAL( pressed() ), this, SLOT( slot_ptz_pressed() ) );
    connect( mp_btn_buttom, SIGNAL( released() ), this, SLOT( slot_ptz_release() ) );
    connect( mp_btn_leftbuttom, SIGNAL( pressed() ), this, SLOT( slot_ptz_pressed() ) );
    connect( mp_btn_leftbuttom, SIGNAL( released() ), this, SLOT( slot_ptz_release() ) );

    //if( mn_rail == RAIL_H ) {
        connect( mp_btn_hor_rail_left, SIGNAL(pressed() ), this, SLOT( slot_rail_move_left_or_top() ) );
        connect( mp_btn_hor_rail_right, SIGNAL(pressed()), this, SLOT( slot_rail_move_right_or_buttom() ) );
        connect( mp_btn_hor_rail_left, SIGNAL(released() ), this, SLOT( slot_rail_stop() ) );
        connect( mp_btn_hor_rail_right, SIGNAL(released()), this, SLOT( slot_rail_stop() ) );
    //} else if( mn_rail == RAIL_V ) {
        connect( mp_btn_ver_rail_top, SIGNAL(pressed()), this, SLOT( slot_rail_move_left_or_top() ) );
        connect( mp_btn_ver_rail_buttom, SIGNAL(pressed()), this, SLOT( slot_rail_move_right_or_buttom() ) );
        connect( mp_btn_ver_rail_top, SIGNAL(released()), this, SLOT( slot_rail_stop() ) );
        connect( mp_btn_ver_rail_buttom, SIGNAL(released()), this, SLOT( slot_rail_stop() ) );
    //}

}

void WndViPreview::move_btn_pan()
{
    int n_rect_width = width();
    int n_rect_height = height();
    int n_btn_width = BTN_WIDTH_HEIGHT;
    int n_space = NSPACE;
    //云台move
    //if( mb_have_ptz && !mb_group ) {
        mp_btn_lefttop->move( n_rect_width-n_btn_width*3-n_space, n_rect_height-n_btn_width*3-n_space);
        mp_btn_left->move( n_rect_width-n_btn_width*3-n_space, n_rect_height-n_btn_width*2-n_space);
        mp_btn_leftbuttom->move( n_rect_width-n_btn_width*3-n_space, n_rect_height-n_btn_width-n_space);

        mp_btn_top->move( n_rect_width-n_btn_width*2-n_space, n_rect_height-n_btn_width*3-n_space);
        //mp_stop->move( n_rect_width-62, n_rect_height-62);
        mp_btn_buttom->move(  n_rect_width-n_btn_width*2-n_space, n_rect_height-n_btn_width-n_space);

        mp_btn_righttop->move( n_rect_width-n_btn_width-n_space, n_rect_height-n_btn_width*3-n_space);
        mp_btn_right->move(  n_rect_width-n_btn_width-n_space, n_rect_height-n_btn_width*2-n_space);
        mp_btn_rightbuttom->move( n_rect_width-n_btn_width-n_space, n_rect_height-n_btn_width-n_space);
    //}
}

void WndViPreview::move_btn_rail()
{
    int n_space = NSPACE;
    int n_btn_width = BTN_WIDTH_HEIGHT;
    //int n_width = width();
    int n_height = height();
    if( mn_rail == RAIL_H ) {//水平
        mp_btn_hor_rail_left->move( n_space, n_height-n_btn_width-n_space );
        mp_btn_hor_rail_right->move( n_btn_width+n_space, n_height-n_btn_width-n_space );
    } else if( mn_rail == RAIL_V ) {//垂直
        mp_btn_ver_rail_top->move( n_space, n_height-n_btn_width*2-n_space );
        mp_btn_ver_rail_buttom->move( n_space, n_height-n_btn_width-n_space );
    }
}

void WndViPreview::slot_ptz_pressed()
{
   QObject*  pSender = QObject::sender();
   int  nDir = 0;
   if( pSender == mp_btn_left) {
        nDir = PtzLeft;
   }
   else if( pSender == mp_btn_right) {
       nDir = PtzRight;
   }
   else if( pSender == mp_btn_top ) {
       nDir = PtzUp;
    }
    else if( pSender == mp_btn_buttom ) {
       nDir = PtzDown;
     }
    else if( pSender == mp_btn_lefttop ) {
       nDir = PtzLeftUp;
    }
    else if( pSender == mp_btn_leftbuttom) {
       nDir = PtzLeftDwon;
    }
    else if( pSender == mp_btn_righttop) {
       nDir = PtzRightUp;
    }
    else if( pSender == mp_btn_rightbuttom) {
       nDir = PtzRightDown;
   } else  {
       return ;
   }

   mp_ptz_rail_view->ptz_ctrl( ms_pan_sn, (PtzDirect) nDir);
}

void WndViPreview::slot_ptz_release()
{
    //QObject*  pSender = QObject::sender();
    mp_ptz_rail_view->ptz_ctrl( ms_pan_sn, PtzStop);
}

void WndViPreview::slot_rail_move_left_or_top()
{
    mp_ptz_rail_view->rail_ctrl( ms_rail_sn, RailLeft );
}

void WndViPreview::slot_rail_move_right_or_buttom()
{
    mp_ptz_rail_view->rail_ctrl( ms_rail_sn, RailRight );
}

void WndViPreview::slot_rail_stop()
{
    mp_ptz_rail_view->rail_ctrl( ms_rail_sn, RailStop );
}

void WndViPreview::set_pan_hidden( bool b_hidden )
{
    mp_btn_left->setHidden( b_hidden );
    mp_btn_right->setHidden( b_hidden );
    mp_btn_top->setHidden( b_hidden );
    mp_btn_buttom->setHidden( b_hidden );
    mp_btn_lefttop->setHidden( b_hidden );
    mp_btn_righttop->setHidden( b_hidden );
    mp_btn_leftbuttom->setHidden( b_hidden );
    mp_btn_rightbuttom->setHidden( b_hidden );
}

void WndViPreview::set_rail_hidden( bool b_hidden )
{
    if( mn_rail == RAIL_H ) {
        mp_btn_hor_rail_left->setHidden( b_hidden );
        mp_btn_hor_rail_right->setHidden( b_hidden );
    } else if( mn_rail == RAIL_V ) {
        mp_btn_ver_rail_top->setHidden( b_hidden );
        mp_btn_ver_rail_buttom->setHidden( b_hidden );
    } else {
        mp_btn_hor_rail_left->setHidden( b_hidden );
        mp_btn_hor_rail_right->setHidden( b_hidden );
        mp_btn_ver_rail_top->setHidden( b_hidden );
        mp_btn_ver_rail_buttom->setHidden( b_hidden );
    }
}

//按钮透明度
void WndViPreview::set_btn_pan_transparency( float f_level )
{
    for( int i=0; i< m_list_btn.size(); i++ ) {
        m_list_effect[i]->setOpacity( f_level );
        m_list_btn[i]->setGraphicsEffect( m_list_effect[i] );
    }
}

void WndViPreview::set_btn_rail_transparency( float f_level )
{
    for( int i=0; i< m_list_rail_btn.size(); i++ ) {
        m_list_rail_effect[i]->setOpacity( f_level );
        m_list_rail_btn[i]->setGraphicsEffect( m_list_rail_effect[i] );
    }
}


//获取组号对应的别名
QString WndViPreview::get_group_str_name()
{
    QMap< int, DeviceGroup >::iterator it_device;
    QMap< int, DeviceGroup > device_group_map;
    QString str_group_name;
    CDataMode::Instance()->get_group_device( device_group_map );
    if( ( it_device = device_group_map.find( mn_group_number ) ) != device_group_map.end() ){
        if( it_device.value().str_name != "" ){
            str_group_name = it_device.value().str_name;
            return str_group_name;
        }
    }
    str_group_name =  "组号-" + QString::number( mn_group_number );
    return str_group_name;
}
