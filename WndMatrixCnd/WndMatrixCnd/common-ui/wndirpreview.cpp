#include "wndirpreview.h"
#include <QPainter>
#include <QGraphicsOpacityEffect>
#include <QDialog>
#include <QtMath>
//#include <windef.h>
#include <QDockWidget>
#include <QWidgetAction>
#include <QListView>
#include <QDebug>
#include "datachartmode.h"
#include "dlgalarmconfig.h"
#include "../common-funtion/dataconfig.h"
#include "wndcharttrend.h"
#include "limits.h"
#include <QMessageBox>
#include "msanacolorsetting.h"
#include "msclientdefine.h"
#include <QMenu>
#include "../common-funtion/msclientlog.h"

#define LABEL_HEIGHT 32
#define SPACE_IR  20


QMutex WndIrPreview::m_mutex_sign;
int WndIrPreview::mn_sign_id = 0;
QMap<int, bool> WndIrPreview::m_map_no_preset_id;

WndIrPreview::WndIrPreview(QWidget *parent,int n_ui_type,  bool b_hide_warn, bool b_hide_calibration) :
    QMainWindow(parent)
    ,mn_group_number( 0 )
    ,mn_select_sign_id( -1 )
    ,ms_type_name( "" )
    ,me_draw_status( none_draw_status )
    ,me_mouse_press_status( none_press_status )
    ,ms_ir_sn( "" )
    ,mb_modify_status( false )
    ,mn_rail_type( 0 )
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
    ,mb_dynamic_toolbar( false )
    ,mn_ui_type( n_ui_type )
   // ,mp_btn_ir_matrix_analysis(NULL)
    ,mn_alarm_flag(0)
    ,mb_ir_connected( true )
    ,mn_rail_no( -1 )
    ,mn_ptz_no( -1 )
    ,mb_matrix_show( false )
    ,mb_press_ctrl( false )
{
   setWindowFlags(Qt::Widget);

   set_org_sz( 640, 480 );
   setMouseTracking( true );

   mp_left = new QPushButton(this);
   mp_left->setWindowOpacity(0.3);
   mp_left->resize(30,30);
   mp_left->setIcon(QIcon(":image/image/left.png"));
   mp_left->hide();

   mp_right= new QPushButton(this);
   mp_right->setWindowOpacity(0.3);
   mp_right->resize(30,30);
   mp_right->hide();
   mp_right->setIcon(QIcon(":image/image/right.png"));

   mp_top= new QPushButton(this);
   mp_top->setWindowOpacity(0.3);
   mp_top->resize(30,30);
   mp_top->hide();
   mp_top->setIcon(QIcon(":image/image/top.png"));

   mp_bottom= new QPushButton(this);
   mp_bottom->setWindowOpacity(0.3);
   mp_bottom->resize(30,30);
   mp_bottom->hide();
   mp_bottom->setIcon(QIcon(":image/image/buttom.png"));

   mp_lefttop= new QPushButton(this);
   mp_lefttop->setWindowOpacity(0.3);
   mp_lefttop->resize(30,30);
   mp_lefttop->hide();
   mp_lefttop->setIcon(QIcon(":image/image/top_left.png"));

   mp_leftbottom= new QPushButton(this);
   mp_leftbottom->setWindowOpacity(0.3);
   mp_leftbottom->resize(30,30);
   mp_leftbottom->hide();
   mp_leftbottom->setIcon(QIcon(":image/image/left_buttom.png"));

   mp_righttop= new QPushButton(this);
   mp_righttop->setWindowOpacity(0.3);
   mp_righttop->resize(30,30);
   mp_righttop->hide();
   mp_righttop->setIcon(QIcon(":image/image/top_right.png"));

   mp_rightbottom= new QPushButton(this);
   mp_rightbottom->setWindowOpacity(0.3);
   mp_rightbottom->resize(30,30);
   mp_rightbottom->hide();
   mp_rightbottom->setIcon(QIcon(":image/image/right_bottom.png"));

   mp_btn_hor_rail_left = new QPushButton( this );
   mp_btn_hor_rail_left->setWindowOpacity( 0.3 );
   mp_btn_hor_rail_left->resize(30,30);
   mp_btn_hor_rail_left->setIcon(QIcon(":image/image/left.png"));
   mp_btn_hor_rail_left->hide();

   mp_btn_hor_rail_right = new QPushButton( this );
   mp_btn_hor_rail_right->setWindowOpacity( 0.3 );
   mp_btn_hor_rail_right->resize(30,30);
   mp_btn_hor_rail_right->setIcon(QIcon(":image/image/right.png"));
   mp_btn_hor_rail_right->hide();

   mp_btn_ver_rail_top = new QPushButton( this );
   mp_btn_ver_rail_top->setWindowOpacity( 0.3 );
   mp_btn_ver_rail_top->resize(30,30);
   mp_btn_ver_rail_top->setIcon(QIcon(":image/image/top.png"));
   mp_btn_ver_rail_top->hide();

   mp_btn_ver_rail_buttom = new QPushButton( this );
   mp_btn_ver_rail_buttom->setWindowOpacity( 0.3 );
   mp_btn_ver_rail_buttom->resize(30,30);
   mp_btn_ver_rail_buttom->setIcon(QIcon(":image/image/buttom.png"));
   mp_btn_ver_rail_buttom->hide();

   //ana_label
   init_ir_widget();
   //me_mouse_press_status = (Ir_Ana_Label::enum_press_status)mp_ir_label->get_mouse_press_status();

   init_connect();
   init_no_preset_sign_id_map();

   if( MSCClientType::Instance()->get_client_type() == ClientMonitor ){ //如果是实时客户端,则开启线程
       mp_thread = new WndIrThread( this );
       mp_thread->start();
   }

   //setFocusPolicy( Qt::StrongFocus );//添加键盘事件之前的设置
}

WndIrPreview::~WndIrPreview()
{
    if( MSCClientType::Instance()->get_client_type() == ClientMonitor ){
        if( mp_thread ){
            mp_thread->set_exit( true );
            mp_thread->quit();
            mp_thread->wait();
            delete mp_thread;
            mp_thread = NULL;
        }
    }
    uninit_no_preset_sign_id_map();
}

void  WndIrPreview::on_add_point()
{
    bool b_selected_preset = true;
    if( MSCClientType::Instance()->get_client_type() == ClientPresetSet ){
        emit sig_selected_preset( b_selected_preset );
    }
    if( b_selected_preset ){
        me_mouse_press_status=point_status;
        mp_delete_ana_label->set_delete_status( false );
    }

}
void  WndIrPreview::on_add_line()
{
    bool b_selected_preset = true;
    if( MSCClientType::Instance()->get_client_type() == ClientPresetSet ){
        emit sig_selected_preset( b_selected_preset );
    }
    if( b_selected_preset ){
        me_mouse_press_status=line_status;
        mp_delete_ana_label->set_delete_status( false );
    }

}

void  WndIrPreview::on_add_rect()
{
    bool b_selected_preset = true;
    if( MSCClientType::Instance()->get_client_type() == ClientPresetSet ){
        emit sig_selected_preset( b_selected_preset );
    }
    if( b_selected_preset ){
        me_mouse_press_status=rect_status;
        mp_delete_ana_label->set_delete_status( false );
    }

 }

void WndIrPreview::slot_clear_btn_click()
{
    bool b_selected_preset = false;
    if( MSCClientType::Instance()->get_client_type() == ClientPresetSet ){
        emit sig_selected_preset( b_selected_preset );
        if( !b_selected_preset ){
           mp_ir_label->clear_down_status();
        }
    }
}

void  WndIrPreview::on_add_circle()
{

}

void WndIrPreview::on_focus_out()
{
    DeviceGroup  dev_group;
    if( ! CDataMode::Instance()->get_group_info(mn_group_number, dev_group) ){
        return ;
    }

    QString str_sn = dev_group.dev_ir.str_sn;

    focus_out( str_sn );

    clear_ana_press_status();
}

void WndIrPreview::on_focus_in()
{
    DeviceGroup  dev_group;
    if( ! CDataMode::Instance()->get_group_info(mn_group_number, dev_group) ){
        return ;
    }

    QString str_sn = dev_group.dev_ir.str_sn;

    focus_in( str_sn );
    clear_ana_press_status();
}

void WndIrPreview::on_correct_ir()
{
    DeviceGroup  dev_group;
    if( ! CDataMode::Instance()->get_group_info(mn_group_number, dev_group) ){
        return ;
    }

    QString str_sn = dev_group.dev_ir.str_sn;

    correct_ir( str_sn );
    clear_ana_press_status();
}

void WndIrPreview::on_alarm_cfg()
{
    me_mouse_press_status = warn_status;

    QString s_sn;

    DeviceGroup  devGroup;
    if( ! CDataMode::Instance()->get_group_info(mn_group_number,devGroup) ){
        return ;
    }
    s_sn = devGroup.dev_ir.str_sn;

    DlgAlarmConfig dlg(s_sn, mb_realtime? 0:1,this);


    if( mb_realtime ) {
        s_sn +="_real";
    } else {
        s_sn +="_record";
    }

    QMap< int, AnaInfo > map_ana_info;
    CDataMode::Instance()->get_ana_info( s_sn, map_ana_info );

    QStringList s_list;
    QMap< int, AnaInfo >::iterator iter = map_ana_info.begin();
    while( iter !=  map_ana_info.end() ){
        QString s_info = iter.value().type_name;
        s_info+="_%1";
        s_info = s_info.arg( iter.value().n_sign_id);
        s_list.append( s_info );
        ++iter;
    }
    dlg.set_ana_info( s_list );
    dlg.exec();

    clear_ana_press_status();
}

void WndIrPreview::slot_ptz_pressed()
{
   QObject*  pSender = QObject::sender();
   int  nDir = 0;
   if( pSender == mp_left) {
        nDir = PtzLeft;
   }
   else if( pSender == mp_right) {
       nDir = PtzRight;
   }
   else if( pSender == mp_top ) {
       nDir = PtzUp;
    }
    else if( pSender == mp_bottom ) {
       nDir = PtzDown;
     }
    else if( pSender == mp_lefttop ) {
       nDir = PtzLeftUp;
    }
    else if( pSender == mp_leftbottom) {
       nDir = PtzLeftDwon;
    }
    else if( pSender == mp_righttop) {
       nDir = PtzRightUp;
    }
    else if( pSender == mp_rightbottom) {
       nDir = PtzRightDown;
   } else  {
       return ;
   }
   
   ptz_ctrl(mn_group_number, (PtzDirect)nDir);
}

void WndIrPreview::slot_ptz_release()
{
    QObject*  pSender = QObject::sender();

     ptz_ctrl(mn_group_number,PtzStop);
}

void WndIrPreview::slot_rail_release()
{
    rail_ctrl( mn_group_number, RailStop );
}

//删除
void  WndIrPreview::on_delete()
{
   clear_ana_press_status();

   QString s_sn;
   if( !get_ir_sn(s_sn) ){
       return ;
   }

   QMap< int, AnaInfo > map_ana_info ;
   CDataMode::Instance()->get_ana_info( s_sn, map_ana_info );
   QMap< int, AnaInfo >::iterator it = map_ana_info.begin();
   for( int i = 0; it != map_ana_info.end();  it++, i++ ) {
        if( it.value().n_sign_id == mn_select_sign_id ) {
            //删除分析
            delete_single_ana( it.value() );
            mp_delete_ana_label->set_delete_status( false );
            mp_delete_ana_label->setHidden( true );
            update();
            break;
        }
   }

}

//清除所有分析
void WndIrPreview::clear_all_ana()
{
    QString s_sn = "";
    QMap< int , AnaInfo > ana_info_map;
    QMap< int , AnaInfo >::iterator it_ana;
    if( !get_ir_sn( s_sn ) )  return;
    CDataMode::Instance()->get_ana_info( s_sn, ana_info_map );
    if( ana_info_map.size() == 0 )  return;
    if( QMessageBox::Ok != QMessageBox::information( this, "提示", "确定要删除所有分析吗？", QMessageBox::Ok | QMessageBox::Cancel ) ) {
        return ;
    }

    me_mouse_press_status = none_press_status;
    mp_ir_label->clear_down_status();
    for( it_ana = ana_info_map.begin(); it_ana != ana_info_map.end(); it_ana++ ) {
        delete_single_ana( it_ana.value() );
    }
    mp_delete_ana_label->set_delete_status( false );
    mp_delete_ana_label->setHidden( true );
    update();
}


void WndIrPreview::resizeEvent( QResizeEvent *event )
{
    qDebug() << "WndIrPreview resizeEvent width:"<<width()<<"; height:"<<height();
    mf_sz_width = width()/(float)mn_org_width;
    mf_sz_height = height()/(float)mn_org_height;

    int nRectWidth  = this->rect().width();
    int nRectHeight = this->rect().height();

    int nBtnWidth =  30;
    int nBtnHeight = 30;

    mp_lefttop->move( nRectWidth-93,nRectHeight-93);
    mp_left->move( nRectWidth-93,nRectHeight-62);
    mp_leftbottom->move( nRectWidth-93,nRectHeight-31);

    mp_top->move( nRectWidth-62,nRectHeight-93);
    //mp_stop->move( nRectWidth-62,nRectHeight-62);
    mp_bottom->move(  nRectWidth-62,nRectHeight-31);

    mp_righttop->move( nRectWidth-31,nRectHeight-93);
    mp_right->move(  nRectWidth-31,nRectHeight-62);
    mp_rightbottom->move( nRectWidth-31,nRectHeight-31);

    mp_btn_hor_rail_left->move( 1, this->rect().height()-31 );
    mp_btn_hor_rail_right->move( 31, this->rect().height()-31 );

    mp_btn_ver_rail_top->move( 1, this->rect().height()-62 );
    mp_btn_ver_rail_buttom->move( 1, this->rect().height()-31 );

    mp_ir_label->move( SPACE_IR, SPACE_IR );
    mp_ir_label->resize( mp_ir_label->get_wnd_width(), LABEL_HEIGHT );
    mp_delete_ana_label->move( width()-SPACE_IR*2-1, SPACE_IR );
    mp_delete_ana_label->resize( LABEL_HEIGHT, LABEL_HEIGHT );

    mw_matrix_widget->setGeometry( 0, 0, width(), height());
    //mp_btn_ir_matrix_analysis->move( SPACE_IR+mp_ir_label->get_wnd_width()-1, SPACE_IR );
    //mp_btn_ir_matrix_analysis->resize( LABEL_HEIGHT*3, LABEL_HEIGHT );
}

void WndIrPreview::paintEvent( QPaintEvent *event )
{
    bool b_has_group( false );
    QImage*  p_image = NULL;
    QImage   tmp;

    mn_alarm_flag = !mn_alarm_flag;
    DeviceGroup devGroup;
    b_has_group = CDataMode::Instance()->get_group_info( mn_group_number, devGroup );

    if( b_has_group ) {
       p_image = CDataMode::Instance()->get_main_image( mn_group_number, DeviceIR, mb_realtime?1:2, tmp);
    }

    QPainter draw;
    draw.begin( this );

    QPixmap bmp_pix(  width()-1, height()-1 );
    QPainter m_painter_buf( &bmp_pix );
    m_painter_buf.initFrom( this );


    QRect rct_ir( 0, 0, width(), height() );
    m_painter_buf.setPen( Qt::white );

    if( mb_ir_connected ) {
        if ( b_has_group && devGroup.dev_ir.str_sn != "" ) {
            if( p_image!=NULL ) {
                if( p_image->isNull() ) {
                    m_painter_buf.fillRect( rct_ir, QBrush( QColor( 200, 200, 200 ) ) );
                    m_painter_buf.drawText( rct_ir, Qt::AlignCenter, "正在连接红外" );
                } else {
                    m_painter_buf.drawImage( rct_ir, tmp );
                    QString s_path;
                    QDir  dir;
                    QString s_sn = devGroup.dev_ir.str_sn;
                    if( mb_realtime ) {
                        s_sn+="_real";
                    } else {
                        s_sn+="_record";
                    }

                    if(  CDataMode::Instance()->get_capture(s_sn,s_path) && dir.exists(s_path) ) {
                        QString sfile = s_path+"/"+s_sn;
                        QString sdate = QDate::currentDate().toString("yyyy-MM-dd");
                        sfile = sfile+sdate;
                        sfile= sfile+ QTime::currentTime().toString(tr("-hh-mm-ss"));
                        sfile+=".png";
                        if( ! p_image->save(sfile,"PNG") ) {
                            qDebug()<<" capture failed "<<sfile;
                        }
                    }
                }
            } else {
                m_painter_buf.fillRect( rct_ir, QBrush( QColor( 200, 200, 200 ) ) );
                m_painter_buf.drawText( rct_ir, Qt::AlignCenter, "正在连接红外" );
            }
        } else {
            m_painter_buf.fillRect( rct_ir, QBrush( QColor( 200, 200, 200 ) ) );
            m_painter_buf.drawText( rct_ir, Qt::AlignCenter, mb_realtime?"未配置红外":"未选择播放视频" );
        }
    } else {
        m_painter_buf.fillRect( rct_ir, QBrush( QColor( 200, 200, 200 ) ) );
        m_painter_buf.drawText( rct_ir, Qt::AlignCenter, "红外断开,正在连接......" );
    }

    draw_anas( m_painter_buf );
    draw_alarm_matrix_ana( m_painter_buf );
    draw.drawPixmap(0, 0, width()-1,  height()-1, bmp_pix );

    draw.setPen( Qt::white );
    draw.drawRect( 0, 0, width()-1, height()-1 );
    draw.end();
}

void WndIrPreview::draw_anas(QPainter &draw)
{
    if ( mw_matrix_widget->isVisible() ){
        return;
    }

    draw.drawRect( 0, 0, width()-1, height()-1 );

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

    QString s_sn;
    if( !get_ir_sn(s_sn) ){
        return ;
    }

    QMap< int, AnaInfo > map_new_ana_info;
    if( MSCClientType::Instance()->get_client_type() == ClientMonitor ){
        QMutexLocker lc(&m_ana_info_mutex);
        map_new_ana_info = m_list_ana_info;
    }

    QMap< int, AnaInfo > map_original_ana_info;
    CDataMode::Instance()->get_ana_info( s_sn, map_original_ana_info );
    if( MSCClientType::Instance()->get_client_type() == ClientMonitor ){

        QMap< int, AnaInfo >::iterator it_original = map_original_ana_info.begin();
        for( ; it_original != map_original_ana_info.end(); ++it_original ){

            QMap< int, AnaInfo >::iterator it_new = map_new_ana_info.begin();
            for( ; it_new != map_new_ana_info.end(); ++it_new ){
                if( it_original.value().n_sign_id == it_new.value().n_sign_id ){
                    it_original.value().b_alarm = it_new.value().b_alarm;
                    break;
                }
            }
        }
    }

    QMap< int, AnaInfo >::iterator it = map_original_ana_info.begin();
    for( ; it != map_original_ana_info.end(); it++ ) {
        QString str_ana;
        TempAnaBuf temp_ana;

        AnaInfo &ana_info = it.value();

        WndIrPreview::shape_to_string( str_ana,  ana_info );
        bool b_has_temp = get_ana_now_temp( str_ana, temp_ana );
        if ( ana_info.str_name.left(6) == "matrix" ){
            continue;
        }

        if ( !ana_info.b_show ) continue;
        QPen pen;

        if ( ana_info.b_preset_ana ) {
            pen.setColor( m_clr_ana );
            pen.setStyle( (Qt::PenStyle)mn_line_style );
            pen.setWidth( mn_line_width );
        } else {
            pen.setColor( QColor( 0, 255, 255 ) );
        }

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


        if( MSCClientType::Instance()->get_client_type() == ClientMonitor ){
            if( ana_info.b_alarm ){
                if( mn_alarm_flag ){
                     draw.setPen( QColor( 255, 0, 255 ) );
                } else {
                     draw.setPen( QColor( 0, 255, 255 ) );
                }
            }
        }


        QPoint pt_draw_text;

        QString str_text = get_ana_draw_text( ana_info, temp_ana.max_temp.f_temp, b_has_temp );

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

        // 显示文本的宽度按字符串的实际长度
        int str_text_len = draw.fontMetrics().width( str_text );
        draw.drawText( pt_draw_text.x(), pt_draw_text.y(), str_text_len, temp_height, Qt::AlignLeft,  str_text );
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

void WndIrPreview::update_preset_alarm_map(QMap<int, PresetAlarmCondition > &preset_alarm_map )
{
    QMutexLocker lc(&m_preset_alarm_mutex);
    m_preset_alarm_map = preset_alarm_map;
}

void WndIrPreview::update_no_preset_alarm_map(QMap<int, PresetAlarmCondition > &no_preset_alarm_map )
{
    QMutexLocker lc(&m_no_preset_alarm_mutex);
    m_no_preset_alarm_map = no_preset_alarm_map;
}

void WndIrPreview::clear_alarm_map()
{
    {
        QMutexLocker lc(&m_preset_alarm_mutex);
        m_preset_alarm_map.clear();
    }
    {
        QMutexLocker lc(&m_no_preset_alarm_mutex);
        m_no_preset_alarm_map.clear();
    }
}

void WndIrPreview::clear_matrix_alarm_list()
{
    QMutexLocker lc(&m_matrix_ana_mutex);
    m_matrix_ana_list.clear();
}

//矩阵分析字符串
void WndIrPreview::set_matrix_ana_list(QList<QString> ana_info_list )
{
    QMutexLocker lc(&m_matrix_ana_mutex);
    m_matrix_ana_list = ana_info_list;
}


void WndIrPreview::draw_alarm_matrix_ana( QPainter &draw )
{
    QList<QString> matrix_ana_list;
    QString str_ana, str_start_x, str_start_y, str_end_x, str_end_y;
    int n_index = -1;
    QRectF rect;
    QColor q_color;
    if( mn_alarm_flag ){
        q_color = QColor( 255, 0, 255, 125 );
    } else {
        q_color = QColor( 0, 255, 255, 125 );
    }
    {
        QMutexLocker lc(&m_matrix_ana_mutex);
        matrix_ana_list = m_matrix_ana_list;
    }
    for( QList<QString>::iterator it = matrix_ana_list.begin(); it != matrix_ana_list.end(); ++it ){
        str_ana = *it;
        str_ana.remove( 0, 2 );//去掉"R:"
        n_index = str_ana.indexOf( ',', 0 );
        str_start_x = str_ana.left( n_index );
        str_ana.remove( 0, n_index+1 );

        n_index = str_ana.indexOf( ';', 0 );
        str_start_y = str_ana.left( n_index );
        str_ana.remove( 0, n_index+1 );

        n_index = str_ana.indexOf( ',', 0 );
        str_end_x = str_ana.left( n_index );
        str_ana.remove( 0, n_index+1 );

        n_index = str_ana.indexOf( ';', 0 );
        str_end_y = str_ana.left( n_index );

        draw.setPen( QColor( 0, 0, 0 ) );
        rect = QRectF( str_start_x.toInt()*mf_sz_width, str_start_y.toInt()*mf_sz_height,
                       ( str_end_x.toInt() - str_start_x.toInt() ) * mf_sz_width, ( str_end_y.toInt() - str_start_y.toInt() ) * mf_sz_height );
        draw.fillRect( rect, q_color );
        draw.drawRect( rect );

    }
}

void WndIrPreview::delete_all_ana()
{
    QString s_sn;
    if( !get_ir_sn(s_sn) ){
        return ;
    }

    // 不能调用clear_chart
//    CDataChartMode::Instance()->clear_chart( s_sn );
    CDataMode::Instance()->clear_ana_info( s_sn );
}

void WndIrPreview::mousePressEvent( QMouseEvent *event )
{
    if ( event->button() == Qt::LeftButton ) {
        QPointF pt_press_local = event->localPos();
        press_ir_template_ana( QPoint( event->x(), event->y() ) );
    } else if ( event->button() == Qt::RightButton ) {

        AnaInfo ana_info;
        AnaMove ana_move;
        QPoint pt_global = mapToGlobal( event->pos() );
        if ( pt_in_ana( event->pos(), ana_move, ana_info ) ) {
            QString s_name;
            this->get_ana_name(ana_info.n_sign_id,s_name);
            emit signal_ana_menu( ana_info.n_sign_id, pt_global,s_name );
        } else if( MSCClientType::Instance()->get_client_type() == ClientPresetSet ){
            QString s_sn = "";
            QMap< int , AnaInfo > ana_info_map;
            if( !get_ir_sn( s_sn ) )  return;
            CDataMode::Instance()->get_ana_info( s_sn, ana_info_map );
            if( ana_info_map.size() == 0 )  return;
            QMenu menu(this);
            menu.addAction( tr("清除所有分析") );
            QAction *p_act = menu.exec( pt_global );
            if ( NULL == p_act ) return;
            clear_all_ana();
        }
    }

}

void WndIrPreview::mouseMoveEvent( QMouseEvent *event )
{
    /*int n_size = m_list_pic_preview_group.size();
    if( event->x()>SPACE && event->x()<=width()-SPACE*2 && \
            event->y()>SPACE+n_size*((width()-SPACE*3)*3/8+SPACE*2+SPACE)-mn_scroll_pos
            && event->y()<=SPACE+n_size*((width()-SPACE*3)*3/8+SPACE*2+SPACE)+SPACE*4-mn_scroll_pos ) {
        mb_move_add_image = true;
    } else {
        mb_move_add_image = false;
    }
    update();*/
    delete_ana_shape( QPoint( event->x(), event->y() ) );
    //红外温度分析
    move_ir_template_ana( QPoint( event->x(), event->y() ) );
}

void WndIrPreview::mouseReleaseEvent( QMouseEvent *event )
{
    /*if ( event->button() == Qt::LeftButton ) {
        if( mb_press_add_image == true ) {
            add_new_group();
            mb_press_add_image = false;
        }
    }*/
    if ( event->button() == Qt::LeftButton ) {
        qDebug()<<"WndIrPreview::mouseReleaseEvent";
        release_ir_template_ana( QPoint( event->x(), event->y() ) );
        if( !mb_press_ctrl ) {
            mp_ir_label->clear_down_status();
        }
        update();
    }
}

void WndIrPreview::normalized_pt( QPoint &pt1, QPoint &pt2 )
{
    QRect rct( pt1, pt2 );
    rct = rct.normalized();//得出对称点，topleft点和bottomRight
    pt1 = rct.topLeft();
    pt2 = rct.bottomRight();
}

void WndIrPreview::draw_add_shape( QPoint pt )
{
    pt_judge_area( pt, m_pt_start );
    m_pt_middle = pt;
    me_draw_status = (enum_draw_status)me_mouse_press_status;
}

bool WndIrPreview::pt_in_ana( const QPoint &pt, AnaMove &ana_move, AnaInfo &ana_info )
{
    QString s_sn;
    if( !get_ir_sn(s_sn) )
    {
        return false;
    }

    QMap< int, AnaInfo > map_ana_info;
    CDataMode::Instance()->get_ana_info( s_sn, map_ana_info );

    QMap< int, AnaInfo >::iterator it = map_ana_info.begin();
    for( ; it != map_ana_info.end(); ++it ) {
        AnaInfo &temp_ana_info = it.value();
        if ( !temp_ana_info.b_ctrl ) { continue; }
        if ( !temp_ana_info.b_show ) { continue; }
        if ( !mb_preset_setting && temp_ana_info.b_preset_ana ) { continue; }

        if( temp_ana_info.type_name == "point" ) {
            ana_move = pt_in_ana_point( pt, temp_ana_info );
            if( ana_move != move_null ) {
                ana_info = temp_ana_info;
                return true;
            }
        } else if ( temp_ana_info.type_name == "line" ) {
           ana_move = pt_in_ana_line( pt, temp_ana_info );
           if( ana_move != move_null ) {
               ana_info = temp_ana_info;
               return true;
           }
        } else if ( temp_ana_info.type_name == "rect" ) {
            ana_move = pt_in_ana_rect( pt, temp_ana_info );
            if( ana_move != move_null ) {
                ana_info = temp_ana_info;
                return true;
            }
        } else if ( temp_ana_info.type_name == "circle" ) {
            ana_move = pt_in_ana_circle( pt, temp_ana_info );
            if( ana_move != move_null ) {
                ana_info = temp_ana_info;
                return true;
            }
        }
    }
//    if ( ana_move == move_null ) return false;
//    if ( it == map_ana_info.end() ) return false;
//    //如果it== m_list_anainfo.end()，那么就不能用下面的语句进行赋值，否则会报错
//    ana_info.n_sign_id = it->n_sign_id;
//    ana_info.type_name = it->type_name;
//    ana_info.point_end = it->point_end;
//    ana_info.point_start = it->point_start;
    return false;
}

//对点的分析
AnaMove WndIrPreview::pt_in_ana_point(const QPoint &pt, const AnaInfo &ana_info)
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
AnaMove WndIrPreview::pt_in_ana_line( const QPoint &pt, const AnaInfo &ana_info )
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

AnaMove WndIrPreview::pt_in_ana_rect( const QPoint &pt, const AnaInfo &ana_info )
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

AnaMove WndIrPreview::pt_in_ana_circle( const QPoint &pt, const AnaInfo &ana_info )
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

void WndIrPreview::get_pt_ana_shape( AnaInfo &ana_info, const QPoint &pt )
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

void WndIrPreview::pt_move_ana_point( const QPoint &pt )
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

void WndIrPreview::pt_move_ana_line( const QPoint &pt )
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

void WndIrPreview::pt_move_ana_rect( const QPoint &pt )
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

void WndIrPreview::pt_move_ana_circle( const QPoint &pt )
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
void WndIrPreview::pt_judge_area( const QPoint &pt, QPoint &pt_area )
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
void WndIrPreview::judge_area( QPoint &pt_area )
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
void WndIrPreview::pt_move_self_boundary( QPoint &pt_start, QPoint &pt_end )
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
void WndIrPreview::pt_circle_move_self_boundary( QPoint &pt_circle_center, QPoint &pt_end )
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


void WndIrPreview::set_cursor_pos()
{/*
    RECT ir_frame_rect;
    ir_frame_rect.left = (LONG)this->geometry().left();
    ir_frame_rect.right = (LONG)this->geometry().right();
    ir_frame_rect.top = (LONG)this->geometry().top();
    ir_frame_rect.bottom = (LONG)this->geometry().bottom(); */
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

void WndIrPreview::start_area( QPoint &pt_start, int &n_r )
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

void WndIrPreview::shape_append_list( QPoint pt )
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

    if( !mb_press_ctrl ) {
        me_mouse_press_status = none_press_status ;
    }
    me_draw_status = none_draw_status;


    AnaInfo ana_info;
    ana_info.type_name = str_type_name;
    ana_info.n_sign_id = WndIrPreview::get_sign_id();
    ana_info.b_preset_ana = mb_preset_setting;
    ana_info.point_start = pt_start;
    ana_info.point_end = pt_end;

    add_ana( ana_info );
}

void WndIrPreview::shape_move_or_change_size( QPoint pt )
{
    QString s_sn;
    if( !get_ir_sn(s_sn) )
    {
        return ;
    }

    ///.获取所有分析
    QMap< int, AnaInfo > map_ana_info;
    CDataMode::Instance()->get_ana_info( s_sn, map_ana_info );

    QMap< int, AnaInfo >::iterator it = map_ana_info.begin();
    for( ; it != map_ana_info.end(); it++ ) {
        AnaInfo & ana_info = it.value();
        int n_x = pt.x()-m_press_pt.x();
        int n_y = pt.y()-m_press_pt.y();
        if( mn_select_sign_id == ana_info.n_sign_id ) {
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

            // 如果开始、结束两点没变动continue
            if ( ( ana_info.point_start == m_pt_release_start ) && ( ana_info.point_end == m_pt_release_end ) ){
                continue;
            }

            QString str_sn;
            DeviceGroup  devGroup;
            CDataMode::Instance()->get_group_info(mn_group_number,devGroup);
            str_sn = devGroup.dev_ir.str_sn;

            mb_modify_status = true;
            AnaInfo temp_ana_info;
            temp_ana_info.b_ctrl = ana_info.b_ctrl;
            temp_ana_info.b_preset_ana = ana_info.b_preset_ana;
            temp_ana_info.b_show = ana_info.b_show;
            temp_ana_info.str_name = ana_info.str_name;
            temp_ana_info.n_sign_id = ana_info.n_sign_id;
            temp_ana_info.point_start = m_pt_release_start;
            temp_ana_info.point_end = m_pt_release_end;
            temp_ana_info.b_preset_ana = mb_preset_setting;
            temp_ana_info.type_name = ana_info.type_name;

            if ( mb_realtime ) {
                if ( temp_ana_info.b_preset_ana ){

                    QString str_temp = str_sn + "_real";
                    CDataMode::Instance()->del_ana_info( str_temp, ana_info );
                    CDataMode::Instance()->add_ana_info( str_temp, temp_ana_info );

                    AnaItem ana_item;
                    ana_item.str_name = temp_ana_info.str_name;
                    shape_to_string( ana_item.str_ana, temp_ana_info );
                    emit sig_preset_ana_change( temp_ana_info.n_sign_id, ana_item );


                } else {
                    change_ana(mb_realtime?1:2, mn_group_number, str_sn, ana_info, temp_ana_info );
                }
            } else {
                emit sig_change_ana_to_server( mn_group_number, ana_info, temp_ana_info );
            }

            break;
        }
    }


}

void WndIrPreview::set_org_sz( int width, int height )
{
    mn_org_width = width;
    mn_org_height = height;
}

//判断pt_area点是否在原始区域内
void WndIrPreview::judge_org_area( QPoint &pt_area )
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

void WndIrPreview::press_ir_template_ana( QPoint pt )
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

void WndIrPreview::move_ir_template_ana( QPoint pt )
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

void WndIrPreview::release_ir_template_ana( QPoint pt )
{
    if( mp_delete_ana_label->get_delete_status() ) {
        delete_shape_mouse_release( pt );
        mp_delete_ana_label->set_delete_status( false );
    } else {
        if( me_draw_status != none_draw_status ) {
            //qDebug()<<" shape_append_list ";
            shape_append_list( pt );
        } else {
            shape_move_or_change_size( pt );
        }
    }

    ms_type_name = "";
}

void WndIrPreview::delete_ana_shape( QPoint pt )
{
    if( mp_delete_ana_label->get_delete_status() ) {
        setCursor( Qt::CrossCursor );
    }
}

//void WndIrPreview::delete_ana( AnaInfo &ana_info )
//{
//    emit signal_ana_delete( ana_info.n_sign_id );
//    delete_ana_in_data( ana_info );

//    delete_ana_to_server( ana_info );
//}

void WndIrPreview::delete_local_ana( AnaInfo &ana_info )
{
    emit signal_ana_delete( ana_info.n_sign_id );
    //emit sig_alarm_cnd_delete( ana_info.n_sign_id );
    delete_ana_in_data( ana_info );
}


void WndIrPreview::delete_ana_in_data( AnaInfo &ana_info )
{
    QString str_sn;
    if( !get_ir_sn( str_sn ) ) return;

    CDataMode::Instance()->del_ana_info( str_sn, ana_info );
}

void WndIrPreview::clear_matrix_ana_info()
{
    mw_matrix_widget->clear_matrix_ana_info();
}

void WndIrPreview::delete_ana_to_chart(AnaInfo &ana_info)
{
    QString str_sn;
    if( !get_ir_sn( str_sn ) ) return;

    // 预置分析自动加到预置点分析图表中
//    if ( ana_info.b_preset_ana ){
//        CDataChartMode::Instance()->remove_ana( str_sn, MS_PRESET_CHART_INDEX, ana_info.n_sign_id, ANA_TYPE_MAX);
    //        }
}

QList<AnaInfo> WndIrPreview::get_preset_ana_info()
{
    QList< AnaInfo > ret_list;
    QMap< int, AnaInfo > map_ana_info;
    if ( get_ana_info( map_ana_info ) ){
        QMap< int, AnaInfo >::iterator it = map_ana_info.begin();
        for( ; it != map_ana_info.end(); ++it ) {
            AnaInfo& ana_info = it.value();
            if ( ana_info.b_preset_ana ) {
                ret_list.push_back( ana_info );
            }
        }
    }
    return ret_list;
}

QList<AnaInfo> WndIrPreview::get_no_preset_ana_info(  )
{
    QList<AnaInfo> ret_list;
    QMap< int, AnaInfo > map_no_preset_ana_info;
    if ( get_ana_info( map_no_preset_ana_info ) ){
        QMap< int, AnaInfo >::iterator it = map_no_preset_ana_info.begin();
        for( ; it != map_no_preset_ana_info.end(); ++it ) {
            AnaInfo& ana_info = it.value();
            if ( !ana_info.b_preset_ana ) {
                ret_list.push_back( ana_info );
            }
        }
    }

    return ret_list;
}

void WndIrPreview::load_ana( QList<AnaInfo> &ana_list )
{
    QString str_sn;
    if( !get_ir_sn( str_sn ) ) return;
   // add_ana_list_to_server( ana_list );//对新加的分析从服务器端取温度
    add_ana_list_in_data( ana_list );

    if ( MSCClientType::Instance()->get_client_type() == ClientMonitor ){
        add_ana_list_to_chart( ana_list );
        CDataChartMode::Instance()->load_backup_temp_data( str_sn, MS_PRESET_CHART_INDEX );
    }
}

void WndIrPreview::load_no_preset_ana( QList<AnaInfo> &no_preset_ana_list )
{
    QString str_sn;
    if( !get_ir_sn( str_sn ) ) return;
    add_no_preset_ana_list_to_server( no_preset_ana_list );//对新加的分析从服务器端取温度
    add_ana_list_in_data( no_preset_ana_list );

    if ( MSCClientType::Instance()->get_client_type() == ClientMonitor ){
        add_ana_list_to_chart( no_preset_ana_list );
        CDataChartMode::Instance()->load_backup_temp_data( str_sn, MS_NO_PRESET_CHART_INDEX );
    }
}

void WndIrPreview::add_no_preset_ana_list_to_server( QList<AnaInfo> &no_preset_ana_list )
{
    DeviceGroup  dev_group;
    CDataMode::Instance()->get_group_info( mn_group_number, dev_group );

    if ( !no_preset_ana_list.isEmpty() ){
        CIrPreview::add_ana(  mb_realtime?1:2, mn_group_number, dev_group.dev_ir.str_sn, no_preset_ana_list );
    }
}

void WndIrPreview::add_ana_list_in_data(QList<AnaInfo> &ana_list)
{
    QString str_sn;
    if( !get_ir_sn( str_sn ) ){ return; }

    QList<AnaInfo>::iterator it = ana_list.begin();
    for(; it != ana_list.end(); it++){
        CDataMode::Instance()->add_ana_info( str_sn, *it );
    }

}

void WndIrPreview::add_ana_list_to_chart( QList<AnaInfo> &ana_list )
{
    QList<AnaInfo>::iterator it = ana_list.begin();
    for(; it != ana_list.end(); it++){
        add_ana_to_chart(*it);
    }
}

void WndIrPreview::delete_ana_to_server( AnaInfo &ana_info )
{
    if ( mb_realtime ) {
        DeviceGroup  dev_group;
        CDataMode::Instance()->get_group_info( mn_group_number, dev_group );

        if ( ana_info.b_preset_ana ){
            emit sig_preset_ana_delete( ana_info.n_sign_id );
        } else {
            QList< AnaInfo > list_ana_info;
            list_ana_info.push_back( ana_info );
            del_ana( mb_realtime?1:2, mn_group_number, dev_group.dev_ir.str_sn, list_ana_info );
        }
    } else {
        emit sig_delete_ana_to_server( mn_group_number, ana_info );
    }
}

void WndIrPreview::delete_ana_to_server( QList<AnaInfo> &list_ana_info )
{
    DeviceGroup  dev_group;
    CDataMode::Instance()->get_group_info( mn_group_number, dev_group );

    QList< AnaInfo > list_no_preset_ana;

    QList< AnaInfo >::iterator it = list_ana_info.begin();
    for( ; it != list_ana_info.end(); ++it ){
        if ( (*it).b_preset_ana )
            continue;

        list_no_preset_ana.push_back( *it );
    }

    if ( !list_no_preset_ana.isEmpty() ){
        del_ana( mb_realtime?1:2, mn_group_number, dev_group.dev_ir.str_sn, list_no_preset_ana );
    }
}

void WndIrPreview::add_ana( AnaInfo &ana_info )
{
    add_ana_in_data( ana_info );
    add_ana_to_server( ana_info );
    add_ana_to_chart( ana_info );
}

void WndIrPreview::add_ana_to_server( AnaInfo &ana_info )
{
    if ( mb_realtime ) {
        DeviceGroup  dev_group;
        CDataMode::Instance()->get_group_info( mn_group_number, dev_group );

        if ( ana_info.b_preset_ana ){
            // 增加预置点分析
            AnaItem ana_item;
            ana_item.str_name = ana_info.str_name;
            shape_to_string( ana_item.str_ana, ana_info );
            emit sig_preset_ana_add( ana_info.n_sign_id, ana_item );
        } else {
            QList< AnaInfo > list_ana;
            list_ana.push_back( ana_info );
            // 获取温度分析
            CIrPreview::add_ana(  mb_realtime?1:2, mn_group_number, dev_group.dev_ir.str_sn, list_ana );
        }
    } else {
        emit sig_add_ana_to_server( mn_group_number, ana_info );
    }
}

void WndIrPreview::add_ana_in_data( AnaInfo &ana_info )
{
    QString str_sn;
    if( !get_ir_sn( str_sn ) ){ return; }
    CDataMode::Instance()->add_ana_info( str_sn, ana_info );
}

void WndIrPreview::add_ana_to_chart(AnaInfo &ana_info)
{
    if ( !mb_realtime ) return;

    QString str_sn;
    if( !get_ir_sn( str_sn ) ){ return; }
    // 预置分析自动加到预置点分析图表中
    if ( ana_info.b_preset_ana ){
        if ( ana_info.str_name.left(6) != "matrix"){
            ChartPoints* p_point = CDataChartMode::Instance()->add_ana( str_sn, MS_PRESET_CHART_INDEX, ana_info.n_sign_id, ANA_TYPE_MAX);
            if( p_point != NULL ) {
                if (ana_info.str_name.trimmed().isEmpty()){
                    p_point->set_name( QString::number( ana_info.n_sign_id ));
                }else{
                    p_point->set_name( ana_info.str_name );
                }
            }
        }
    }else{
        ChartPoints* p_point = CDataChartMode::Instance()->add_ana( str_sn, MS_NO_PRESET_CHART_INDEX, ana_info.n_sign_id, ANA_TYPE_MAX);
        if( p_point != NULL ) {
            if (ana_info.str_name.trimmed().isEmpty()){
                p_point->set_name( QString::number( ana_info.n_sign_id ));
            }else{
                p_point->set_name( ana_info.str_name );
            }
        }
    }
}

int WndIrPreview::get_preset_sign_id( int n_sign_id )
{
    return 1000 + n_sign_id;
}

int WndIrPreview::get_rail_no()
{
    return mn_rail_no;
}

int WndIrPreview::get_ptz_no()
{
    return mn_ptz_no;
}

void WndIrPreview::set_rail_no( const int n_rail_no )
{
    mn_rail_no = n_rail_no;
}

void WndIrPreview::set_ptz_no( const int n_ptz_no )
{
    mn_ptz_no = n_ptz_no;
}

bool WndIrPreview::get_matrix_show()
{
    return mb_matrix_show;
}

void WndIrPreview::set_matrix_show( bool b_matrix_show )
{
    mb_matrix_show = b_matrix_show;
}


void WndIrPreview::delete_shape_mouse_release( QPoint pt )
{
    //在点、线 上点击鼠标，则删除该图形
    AnaInfo ana_info;
    if ( !pt_in_ana( pt, me_ana_move, ana_info ) ) {
        qDebug()<<" delete_shape_mouse_release 1";
        return;
    }

    setCursor( Qt::ArrowCursor );

    delete_single_ana( ana_info );
    update();
}

void WndIrPreview::add_list_pt( AnaInfo anainfo )
{
    QString s_sn;
    if( !get_ir_sn(s_sn) )
    {
        qDebug()<<"WndIrPreview::add_list_pt return false";
        return ;
    }
     qDebug()<<"WndIrPreview::add_list_pt return  s_sn"<<s_sn;
     CDataMode::Instance()->add_ana_info(s_sn,anainfo);
}

bool WndIrPreview::get_ir_sn(QString & s_sn)
{
    DeviceGroup  devGroup;
    CDataMode::Instance()->get_group_info(mn_group_number,devGroup);

    s_sn = devGroup.dev_ir.str_sn;

    if( s_sn.isEmpty() ) return false;

    if( mb_realtime ) {
        s_sn +="_real";
    } else {
        s_sn +="_record";
    }

    return true;
}

MatrixWidget *WndIrPreview::get_matrix_widget()
{
    return mw_matrix_widget;
}

void WndIrPreview::get_matrix_color_name_by_ana_id(int n_ana_id, QString &out_color_name)
{
    if (mw_matrix_widget){
        char color_name[255] = {0};
        mw_matrix_widget->get_matrix_color_by_ana_id( n_ana_id, color_name);
        out_color_name = color_name;
    }
}

int WndIrPreview::get_ana_id_by_matrix_color(const QString matrix_color)
{
    if ( mw_matrix_widget ){
        return mw_matrix_widget->get_ana_id_by_matrix_color( matrix_color );
    }
}


void WndIrPreview::delete_list_pt( int signed_id, bool b_sig )
{
    QString s_sn ;

    if( !get_ir_sn(s_sn)) {
          return ;
    }
    AnaInfo ana;
    ana.n_sign_id = signed_id;

    CDataMode::Instance()->del_ana_info(s_sn,ana);

    if(  b_sig ) {
         emit signal_ana_delete( signed_id );
         emit sig_alarm_cnd_delete( signed_id );
    }
}

bool WndIrPreview::get_ana_now_temp( QString str_ana, TempAnaBuf &temp_info )
{
    QString s_sn;
    if( !get_ir_sn(s_sn) ) {
        return false;
    }

    return  CDataChartMode::Instance()->get_ana_now_temp(s_sn,str_ana,temp_info);
}
void WndIrPreview::set_ui_type(int n_type)
{
    show_ptz_guidrail(false);
    //mn_ui_type = n_type;
}

void WndIrPreview::set_rail_type(int n_rail)
{
    mn_rail_type = n_rail;
}

void WndIrPreview::slot_temp_ana()
{

}
bool WndIrPreview::get_ana_info(QString str_ana, int &n_id, QString &str_name,QString& s_type)
{
    QString s_sn;
    if( !get_ir_sn(s_sn) ) {
        return false;
    }

    return  CDataMode::Instance()->get_ana_info(s_sn,str_ana,n_id,str_name,s_type);
}

bool WndIrPreview::get_ana_name(int n_id, QString &str_name)
{
    QString s_sn;
    if( !get_ir_sn(s_sn) ) {
        return false;
    }
    return  CDataMode::Instance()->get_ana_abbreviation_name( s_sn,n_id, str_name);
}

QString WndIrPreview::get_ana_draw_text( const AnaInfo &ana_info, const float &f_temp, const bool &b_has_temp )
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

//        int temp_sign_id = 0;
//        if ( ana_info.b_preset_ana ){
//            temp_sign_id = ana_info.n_sign_id - 1000;
//        }else{
//            temp_sign_id = ana_info.n_sign_id;
//        }

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

void WndIrPreview::show_ptz_guidrail(bool bshow)
{
    //if( mb_realtime ) return ;

    if ( bshow ) {
        mp_left->show();
        mp_right->show();
        mp_top->show();
        mp_bottom->show();
        mp_lefttop->show();
        mp_leftbottom->show();
        mp_righttop->show();
        mp_rightbottom->show();
        //mp_stop->show();
        if( mn_rail_type == 3) {
           mp_btn_hor_rail_left->show();
           mp_btn_hor_rail_right->show();
        } else if( mn_rail_type == 4 ){
            mp_btn_ver_rail_top->show();
            mp_btn_ver_rail_buttom->show();
        }
    } else {
        //mp_stop->hide();
        mp_left->hide();
        mp_right->hide();
        mp_top->hide();
        mp_bottom->hide();
        mp_lefttop->hide();
        mp_leftbottom->hide();
        mp_righttop->hide();
        mp_rightbottom->hide();

        mp_btn_hor_rail_left->hide();
        mp_btn_hor_rail_right->hide();
        mp_btn_ver_rail_top->hide();
        mp_btn_ver_rail_buttom->hide();
    }
}

void WndIrPreview::init_connect()
{
    connect( mp_left, SIGNAL( pressed() ), this, SLOT( slot_ptz_pressed() ) );
    connect( mp_left, SIGNAL( released() ), this, SLOT( slot_ptz_release() ) );
    connect( mp_lefttop, SIGNAL( pressed() ), this, SLOT( slot_ptz_pressed() ) );
    connect( mp_lefttop, SIGNAL( released() ), this, SLOT( slot_ptz_release() ) );
    connect( mp_top, SIGNAL( pressed() ), this, SLOT( slot_ptz_pressed() ) );
    connect( mp_top, SIGNAL( released() ), this, SLOT( slot_ptz_release() ) );
    connect( mp_righttop, SIGNAL( pressed() ), this, SLOT( slot_ptz_pressed() ) );
    connect( mp_righttop, SIGNAL( released() ), this, SLOT( slot_ptz_release() ) );
    connect( mp_right, SIGNAL( pressed() ), this, SLOT( slot_ptz_pressed() ) );
    connect( mp_right, SIGNAL( released() ), this, SLOT( slot_ptz_release() ) );
    connect( mp_rightbottom, SIGNAL( pressed() ), this, SLOT( slot_ptz_pressed() ) );
    connect( mp_rightbottom, SIGNAL( released() ), this, SLOT( slot_ptz_release() ) );
    connect( mp_bottom, SIGNAL( pressed() ), this, SLOT( slot_ptz_pressed() ) );
    connect( mp_bottom, SIGNAL( released() ), this, SLOT( slot_ptz_release() ) );
    connect( mp_leftbottom, SIGNAL( pressed() ), this, SLOT( slot_ptz_pressed() ) );
    connect( mp_leftbottom, SIGNAL( released() ), this, SLOT( slot_ptz_release() ) );

    connect( mp_btn_hor_rail_left, SIGNAL(pressed()), this, SLOT( slot_rail_move_left_or_top() ) );
    connect( mp_btn_hor_rail_right, SIGNAL(pressed()), this, SLOT( slot_rail_move_right_or_buttom() ) );
    connect( mp_btn_ver_rail_top, SIGNAL(pressed()), this, SLOT( slot_rail_move_left_or_top() ) );
    connect( mp_btn_ver_rail_buttom, SIGNAL(pressed()), this, SLOT( slot_rail_move_right_or_buttom() ) );

    connect( mp_btn_hor_rail_left, SIGNAL(released()), this, SLOT( slot_rail_release() ) );
    connect( mp_btn_hor_rail_right, SIGNAL(released()), this, SLOT( slot_rail_release() ) );
    connect( mp_btn_ver_rail_top, SIGNAL(released()), this, SLOT( slot_rail_release() ) );
    connect( mp_btn_ver_rail_buttom, SIGNAL(released()), this, SLOT( slot_rail_release() ) );

    //connect( mp_stop, SIGNAL( clicked() ), this, SLOT( slot_pan_stop() ) );

    //红外工具栏
    connect( mp_ir_label, SIGNAL( sig_add_point() ), this, SLOT( on_add_point() ) );
    connect( mp_ir_label, SIGNAL( sig_add_line() ), this, SLOT( on_add_line() ) );
    connect( mp_ir_label, SIGNAL( sig_add_rect() ), this, SLOT( on_add_rect() ) );
    connect( mp_ir_label, SIGNAL( sig_modify_far() ), this, SLOT( on_focus_out() ) );
    connect( mp_ir_label, SIGNAL( sig_modify_near() ), this, SLOT( on_focus_in() ) );
    connect( mp_ir_label, SIGNAL( sig_modify_calibration() ), this, SLOT( on_correct_ir() ) );
    connect( mp_ir_label, SIGNAL( sig_modify_warning() ), this, SLOT( on_alarm_cfg() ) );
    connect( mp_ir_label, SIGNAL( sig_add_matrix_ana() ), this, SLOT( slot_on_matrix_btn_clicked() ) );
    connect( mp_ir_label, SIGNAL( sig_clear_btn_click() ), this, SLOT( slot_clear_btn_click() ) );
    connect( mp_delete_ana_label, SIGNAL( sig_delete_shape() ), this, SLOT( on_delete() ) );


    connect( mw_matrix_widget, SIGNAL(matrix_widget_hide()), this, SLOT( slot_matrix_widget_hide()) );
    connect( mw_matrix_widget, SIGNAL( sig_update_alarm_info() ), this, SLOT( slot_update_alarm_info() ) );
    //connect( mp_btn_ir_matrix_analysis, SIGNAL( clicked() ), this, SLOT( slot_on_matrix_btn_clicked() ) );
}

void WndIrPreview::slot_rail_move_left_or_top()
{
    rail_ctrl( mn_group_number, RailLeft );
}

void WndIrPreview::slot_rail_move_right_or_buttom()
{
    rail_ctrl( mn_group_number, RailRight );
}

void WndIrPreview::slot_on_matrix_btn_clicked()
{
    slot_matrix_widget_show();
    mw_matrix_widget->set_sn_info(mb_realtime?1:2, ms_ir_sn);
    mw_matrix_widget->show();
    mb_matrix_show = true;

    clear_ana_press_status();
}

void WndIrPreview::slot_matrix_widget_show()
{
    mp_ir_label->setHidden( true );
    mp_delete_ana_label->setHidden( true );
    //mp_btn_ir_matrix_analysis->setHidden( true );
}

void WndIrPreview::slot_matrix_widget_hide()
{
    if (mp_ir_label->isHidden()){
        mp_ir_label->setHidden( false );
    }
    if ( mn_select_sign_id >= 0 ){
        mp_delete_ana_label->setHidden( false );
    }
    //if (mp_btn_ir_matrix_analysis->isHidden()){
     //   mp_btn_ir_matrix_analysis->setHidden( false );
    //}
    mb_matrix_show = false;
}


void WndIrPreview::slot_update_alarm_info()
{
    emit sig_update_alarm_info();
}


void WndIrPreview::set_group_number( int n_num )
{
    if ( mn_group_number != n_num ) {
        delete_all_ana();
        if( mn_select_sign_id >= 0 ) {
            mp_delete_ana_label->setHidden( true );
            mn_select_sign_id = -1;
        }
        mw_matrix_widget->hide();
        mb_matrix_show = false;
    }

    mn_group_number = n_num;
    if ( n_num == 0 ){
        mn_rail_no = -1;
        mn_ptz_no = -1;
    }

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

    DeviceGroup  devGroup;
    CDataMode::Instance()->get_group_info(mn_group_number,devGroup);
    if( devGroup.dev_ir.str_sn != "" && !mb_dynamic_toolbar ) {
        set_toolbar_hidden( false );
        //mp_btn_ir_matrix_analysis->setHidden( false );
    } else {
        set_toolbar_hidden( true );
        //mp_btn_ir_matrix_analysis->setHidden( true );
    }
}

void WndIrPreview::set_preset_param( int n_rail, int n_ptz )
{
    mn_rail_no = n_rail;
    mn_ptz_no = n_ptz;
    if ( mw_matrix_widget ){
        mw_matrix_widget->set_preset_params(mn_group_number, n_rail, n_ptz);
    }
}

void WndIrPreview::set_preset_setting(bool b_preset)
{
    mb_preset_setting = b_preset;
    if (mb_preset_setting){
        DeviceGroup  devGroup;
        CDataMode::Instance()->get_group_info(mn_group_number,devGroup);
        if( devGroup.dev_ir.str_sn != ""  && !mb_dynamic_toolbar ){
            //mp_btn_ir_matrix_analysis->show();
        }
    }else{
        //mp_btn_ir_matrix_analysis->hide();
    }
}

void WndIrPreview::timer_update()
{
    if ( !isHidden() ) {
        update();
    }
}


void WndIrPreview::init_ir_widget()
{
    mp_ir_label = new Ir_Ana_Label( this, mb_hide_warn, mb_hide_calibration );
    QGraphicsOpacityEffect *effect = new QGraphicsOpacityEffect( this );
    effect->setOpacity( 0.5 );
    mp_ir_label->setGraphicsEffect( effect );

    mp_delete_ana_label = new DeleteLabel( this );
    QGraphicsOpacityEffect *effect_delete = new QGraphicsOpacityEffect( this );
    effect_delete->setOpacity( 0.5 );
    mp_delete_ana_label->setGraphicsEffect( effect_delete );

    // 矩阵分析按钮
    //mp_btn_ir_matrix_analysis = new Ir_Matrix_Analysis_Button( "增加阵列分析", this );
    //QGraphicsOpacityEffect *effect_matrix = new QGraphicsOpacityEffect( this );
   // effect_matrix->setOpacity( 0.5 );
    //mp_btn_ir_matrix_analysis->setGraphicsEffect( effect_matrix );
    //mp_btn_ir_matrix_analysis->hide();


    mw_matrix_widget = new MatrixWidget( this );
    QGraphicsOpacityEffect *effect_matrix_widget = new QGraphicsOpacityEffect( this );
    effect_matrix_widget->setOpacity( 0.5 );
    mw_matrix_widget->setGraphicsEffect( effect_matrix_widget );
    mw_matrix_widget->hide();
    mb_matrix_show = false;

    set_toolbar_hidden( true );
    mp_delete_ana_label->setHidden( true );

}

void WndIrPreview::set_ir_ana_num( int n_num )
{
    mp_ir_label->set_ir_ana_num( n_num );
    mp_ir_label->resize( mp_ir_label->get_wnd_width(), LABEL_HEIGHT );
}

void WndIrPreview::set_toolbar_hidden( bool b_hidden )
{
    mp_ir_label->setHidden( b_hidden );
    //mp_delete_ana_label->setHidden( b_hidden );
}

void WndIrPreview::set_draw_line_cfg(QColor clr, int n_line_width, int n_line_style)
{
    m_clr_ana  = clr;
    mn_line_style = n_line_style;
    mn_line_width = n_line_width;
}

bool WndIrPreview::get_ana_info( QMap<int, AnaInfo> &map_ana_info )
{
    QString str_sn;
    if( !get_ir_sn( str_sn ) ) return false;

    return CDataMode::Instance()->get_ana_info( str_sn, map_ana_info );
}

void WndIrPreview::load_preset_all_ana( const QList<AnaItem> &ana_item_list, QMap<int, int> &map_sign_id_to_ana_id )
{
    mw_matrix_widget->matrix_clear();

    QList<AnaInfo> ana_list;
    QList<AnaItem>::const_iterator it = ana_item_list.begin();
    for(; it != ana_item_list.end(); it++){
        AnaInfo ana_info;
        // 如果是矩阵分析进行操作
        if (it->str_name.left(6)=="matrix")
        {
            mw_matrix_widget->string_to_matrix_list( it->n_id_ana, it->str_ana, ClientPresetSet );
        } else {
            if ( MSCClientType::Instance()->get_client_type() == ClientMonitor ){
                // 实时的需要加入图表中， 图表中显示历史数据， 是以分析号作为区分
                ana_info.n_sign_id = it->n_id_ana; // 预置点标志id填写为分析id;    WndIrPreview::get_sign_id();
            }else{
                ana_info.n_sign_id = get_sign_id();
            }

            ana_info.b_preset_ana = true;
            ana_info.str_name = it->str_name;
            if ( WndIrPreview::string_to_shape( it->str_ana, ana_info ) ) {
                ana_list.push_back( ana_info );
                // 窗口号与分析号的对应关系
                map_sign_id_to_ana_id[ana_info.n_sign_id] = it->n_id_ana;
            }
        }
    }

    // load分析
    if (ana_list.size() > 0){
        load_ana( ana_list );
    }

    // 刷新窗口
    update();
}

void WndIrPreview::load_preset_all_ana( int n_group, int n_rail, int n_ptz )
{
    if ( mn_group_number != n_group ) return;

    // 保存导轨、云台号
    mn_rail_no = n_rail;
    mn_ptz_no = n_ptz;

    mw_matrix_widget->matrix_clear();
    PresetInfo preset_info;
    if ( !CDataMode::Instance()->get_preset_info( n_group, n_rail, n_ptz, preset_info ) ) return;

    QList< AnaInfo > ana_list;
    ana_list.clear();

    // 预置点分析
    QVector < AnaItem >::iterator it = preset_info.vct_anas.begin();
    for ( ; it != preset_info.vct_anas.end(); ++it  ) {
        AnaInfo ana_info;
        // 如果是矩阵分析进行操作
        if ( it->str_name.left(6) =="matrix" )
        {
             mw_matrix_widget->string_to_matrix_list( it->n_id_ana, it->str_ana, ClientPresetSet );
            // 将矩阵分析也加入add_ana， 为了判断报警
            QStringList str_ana_list = it->str_ana.split( '-' );
            QString str_ana;
            for( int i = 0; i<str_ana_list.size(); i++ ){
                ana_info.b_preset_ana = true;
                ana_info.str_name = it->str_name;
                ana_info.n_sign_id = WndIrPreview::get_matrix_sign_id();
                str_ana = str_ana_list.at( i );
                str_ana = str_ana + '-';
                if ( WndIrPreview::string_to_shape( str_ana, ana_info ) ){
                    ana_list.push_back( ana_info );
                }
            }
        }else{
            ana_info.n_sign_id = get_preset_sign_id( it->n_id_ana ); // 预置点标志id填写为分析id; WndIrPreview::get_sign_id();
            ana_info.b_preset_ana = true;
            ana_info.str_name = it->str_name;

            if ( WndIrPreview::string_to_shape( it->str_ana, ana_info ) ) {
                ana_list.push_back( ana_info );
            }
        }
    }

    // 在调用load_ana之前， 取出非预置点分析,  因为调用load_ana()会清除所有分析。
//    QMap< int, AnaInfo > map_ana_info;
//    if ( get_ana_info( map_ana_info ) ){
//        QMap< int, AnaInfo >::iterator it = map_ana_info.begin();
//        for( ; it != map_ana_info.end(); ++it ) {
//            if ( !it.value().b_preset_ana ) {
//                ana_list.push_back( it.value() );
//            }
//        }
//    }

    if ( !ana_list.isEmpty() ){
        load_ana( ana_list );
    }

    emit sig_preset_info( mn_group_number, mn_rail_no, mn_ptz_no );
}

void WndIrPreview::load_all_ana( int n_group, int n_rail, int n_ptz, QList< AnaInfo > list_no_preset_ana_info )
{
    if ( mn_group_number != n_group ) return;


    // 保存导轨、云台号
    mn_rail_no = n_rail;
    mn_ptz_no = n_ptz;

    mw_matrix_widget->matrix_clear();

    QList< AnaInfo > ana_list;
    ana_list.clear();

    PresetInfo preset_info;
    if ( CDataMode::Instance()->get_preset_info( n_group, n_rail, n_ptz, preset_info ) ){
        // 预置点分析
        QVector < AnaItem >::iterator it = preset_info.vct_anas.begin();
        for ( ; it != preset_info.vct_anas.end(); ++it  ) {
            AnaInfo ana_info;
            // 如果是矩阵分析进行操作
            if ( it->str_name.left(6) =="matrix" )
            {
                mw_matrix_widget->string_to_matrix_list( it->n_id_ana, it->str_ana, ClientPresetSet );
                // 将矩阵分析也加入add_ana， 为了判断报警
                QStringList str_ana_list = it->str_ana.split( '-' );
                QString str_ana;
                for( int i = 0; i<str_ana_list.size(); i++ ){
                    ana_info.b_preset_ana = true;
                    ana_info.str_name = it->str_name;
                    ana_info.n_sign_id = WndIrPreview::get_matrix_sign_id();
                    str_ana = str_ana_list.at( i );
                    str_ana = str_ana + '-';
                    if ( WndIrPreview::string_to_shape( str_ana, ana_info ) ){
                        ana_list.push_back( ana_info );
                    }
                }
            }else{
                ana_info.n_sign_id = get_preset_sign_id( it->n_id_ana ); // 预置点标志id填写为分析id; WndIrPreview::get_sign_id();
                ana_info.b_preset_ana = true;
                ana_info.str_name = it->str_name;

                if ( WndIrPreview::string_to_shape( it->str_ana, ana_info ) ) {
                    ana_list.push_back( ana_info );
                }
            }
        }
    }

    if ( !ana_list.isEmpty() ){
        load_ana( ana_list );
    }

    if ( !list_no_preset_ana_info.isEmpty() ){
        load_no_preset_ana( list_no_preset_ana_info );
    }

    emit sig_preset_info( mn_group_number, mn_rail_no, mn_ptz_no );
}

void WndIrPreview::delete_preset_all_ana( int n_group )
{
    if ( mn_group_number != n_group ) return;

    QList<AnaInfo> list_preset_ana_info = get_preset_ana_info();
    if ( list_preset_ana_info.size() == 0 ) return;

    // 本地删除
    QList<AnaInfo>::iterator it = list_preset_ana_info.begin();
    for( ; it != list_preset_ana_info.end(); ++it ) {
         delete_local_ana( *it );
    }

    // 服务上删除
    delete_ana_to_server( list_preset_ana_info );

    update();
}

void WndIrPreview::delete_no_preset_all_ana( int n_group )
{
    if ( mn_group_number != n_group ) return;

    QList<AnaInfo> list_no_preset_ana_info = get_no_preset_ana_info();
    if ( list_no_preset_ana_info.size() == 0 ) return;

    // 本地删除
    QList<AnaInfo>::iterator it = list_no_preset_ana_info.begin();
    for( ; it != list_no_preset_ana_info.end(); ++it ) {
        delete_local_ana( *it );
    }

    // 服务上删除
    delete_ana_to_server( list_no_preset_ana_info );

    update();
}

void WndIrPreview::delete_single_ana( AnaInfo &ana_info )
{
    emit signal_ana_delete( ana_info.n_sign_id );
    emit sig_alarm_cnd_delete( ana_info.n_sign_id );
    delete_ana_in_data( ana_info );
    delete_ana_to_server( ana_info );

    if ( !ana_info.b_preset_ana ){
        update_no_preset_sign_id_map( ana_info.n_sign_id, false );
    }
}

////// 实时客户端调用
//void WndIrPreview::add_preset_all_ana( int n_group, int n_rail, int n_ptz )
//{
//    if ( mn_group_number != n_group ) return;

//    mw_matrix_widget->matrix_clear();
//    PresetInfo preset_info;
//    if ( !CDataMode::Instance()->get_preset_info( n_group, n_rail, n_ptz, preset_info ) ) return;

//    QVector < AnaItem >::iterator it = preset_info.vct_anas.begin();
//    for ( ; it != preset_info.vct_anas.end(); ++it  ) {
//        AnaInfo ana_info;
//        // 如果是矩阵分析进行操作
//        if (it->str_name.left(6)=="matrix")
//        {
//            mw_matrix_widget->string_to_matrix_list( it->n_id_ana, it->str_ana, ClientPresetSet );
//            // 将矩阵分析也加入add_ana， 为了判断报警
//            QStringList str_ana_list = it->str_ana.split( '-' );
//            QString str_ana;
//            for( int i = 0; i<str_ana_list.size(); i++ ){
//                ana_info.b_preset_ana = true;
//                ana_info.str_name = it->str_name;
//                ana_info.n_sign_id = WndIrPreview::get_matrix_sign_id();
//                str_ana = str_ana_list.at( i );
//                str_ana = str_ana + '-';
//                if ( WndIrPreview::string_to_shape( str_ana, ana_info ) ){
//                    add_ana( ana_info );
//                }
//            }
//        }else{
//            ana_info.n_sign_id = get_preset_sign_id( it->n_id_ana ); // 预置点标志id填写为分析id; WndIrPreview::get_sign_id();
//            ana_info.b_preset_ana = true;
//            ana_info.str_name = it->str_name;

//            if ( WndIrPreview::string_to_shape( it->str_ana, ana_info ) ) {
//                add_ana( ana_info );
//            }
//        }
//    }
//}

void WndIrPreview::remove_all_no_perset_chart()
{
    QString s_sn;
    if( !get_ir_sn(s_sn) ){
        return ;
    }
    CDataChartMode::Instance()->remove_all_ana( s_sn, MS_NO_PRESET_CHART_INDEX );
}

int WndIrPreview::get_sign_id()
{
    QMutexLocker lc( &m_mutex_sign );
    if ( MSCClientType::Instance()->get_client_type() == ClientMonitor ){
        return get_no_preset_sigin_id();
    }
    return ++mn_sign_id;
}

int WndIrPreview::get_matrix_sign_id()
{
    QMutexLocker lc( &m_mutex_sign );
    return ++mn_sign_id + MAX_HALF_INT;
}

void WndIrPreview::shape_to_string( QString &str_shape, const AnaInfo &anainfo )
{
    //qDebug() << "shape_to_string: " << str_shape;
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

bool WndIrPreview::string_to_shape( const QString &str_shape, AnaInfo &anainfo )
{
   // qDebug() << "string_to_shape: " << str_shape;
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

void WndIrPreview::init_no_preset_sign_id_map()
{
    QMutexLocker lc( &m_mutex_sign );
    for( int i = 1; i < 999; i++ ){
        m_map_no_preset_id[ i ] = false;
    }
}



void WndIrPreview::uninit_no_preset_sign_id_map()
{
    QMutexLocker lc( &m_mutex_sign );
    m_map_no_preset_id.clear();
}

int WndIrPreview::get_no_preset_sigin_id()
{
    // QMutexLocker lc( &m_mutex_sign ); 不可用锁， 不然嵌套锁了
    int n_ret = 1;
    QMap<int, bool>::iterator it = m_map_no_preset_id.begin();
    for( ; it != m_map_no_preset_id.end(); ++it ){
        if ( !it.value() ){
            m_map_no_preset_id[ it.key() ] = true;
            n_ret = it.key();
            break;
        }
    }
    return n_ret;
}

void WndIrPreview::update_no_preset_sign_id_map(int n_no_preset_id, bool b_use)
{
    QMutexLocker lc( &m_mutex_sign );
    QMap<int, bool>::iterator it = m_map_no_preset_id.begin();
    for( ; it != m_map_no_preset_id.end(); ++it ){
        if ( it.key() == n_no_preset_id ){
            m_map_no_preset_id[ n_no_preset_id ] = b_use;
            break;
        }
    }
}

void WndIrPreview::enterEvent( QEvent * e )
{
    QMainWindow::enterEvent(e);

    if ( mb_dynamic_toolbar ) {
        DeviceGroup  dev_group;

        CDataMode::Instance()->get_group_info( mn_group_number, dev_group );
        if( dev_group.dev_ir.str_sn != "" && mp_ir_label->isHidden() ) {
            set_toolbar_hidden( false );
        }
    }

    if( mn_ui_type == 0 ) {
        show_ptz_guidrail( true );
    }

}

void WndIrPreview::leaveEvent( QEvent * e )
{
    QMainWindow::leaveEvent(e);

    if ( mb_dynamic_toolbar ) {
        if(  !mp_ir_label->isHidden() ) {
            set_toolbar_hidden( true );
        }
    }

    show_ptz_guidrail( false );

    clear_ana_press_status();
}

int WndIrPreview::onResponse( CBaseMsg& reqMsg, CBaseMsg& rspMsg )
{
    CIrPreview::onResponse( reqMsg, rspMsg );
//    switch( rspMsg.m_nCmd ) {
//    case ConfigAnaAdd:
//    {
//        if ( rspMsg.m_nResult== ResultOk ) {
//            QString str_sn;
//            if( !get_ir_sn( str_sn ) )  break;

//            QVariant var_ana = reqMsg.map_msgdata[MSGDATA_ANA];
//            QList< AnaInfo > list_ana = var_ana.value< QList< AnaInfo > >();

//            QList< AnaInfo >::iterator it = list_ana.begin();
//            for(; it != list_ana.end(); it++ ){
//                AnaInfo ana_info = *it;
//                AnaItem ana_item;
//                ana_item.str_name = ana_info.str_name;
//                shape_to_string( ana_item.str_ana, ana_info );

//                if( reqMsg.map_msgdata[MSGDATA_CHANGE_ANA].toBool() ){  //修改预置点分析
//                    emit sig_preset_ana_change( ana_info.n_sign_id, ana_item );
//                } else {
//                    emit sig_preset_ana_add( ana_info.n_sign_id, ana_item );
//                }
//            }
//        }
//        break;
//    }
//    case ConfigAnaDel:
//    {
//        if ( rspMsg.m_nResult== ResultOk ) {
//            bool b_change = reqMsg.map_msgdata[MSGDATA_CHANGE_ANA].toBool();
//            if ( !b_change ){
//                QVariant var_ana = reqMsg.map_msgdata[MSGDATA_ANA];
//                QList< AnaInfo > list_ana= var_ana.value< QList< AnaInfo > >();

//                QList< AnaInfo >::iterator it = list_ana.begin();
//                for(; it != list_ana.end(); it++ ){
//                    AnaInfo ana_info = *it;
//                    emit sig_preset_ana_delete( ana_info.n_sign_id );
//                }
//            }
//        }
//        break;
//    }
//    }
}

void WndIrPreview::set_ana_info_list( QMap< int, AnaInfo > ana_info_list )
{
    QMutexLocker lc( &m_ana_info_mutex );
    m_list_ana_info = ana_info_list;
}


bool WndIrPreview::get_sn(QString &s_sn)
{
    DeviceGroup  devGroup;
    CDataMode::Instance()->get_group_info(mn_group_number,devGroup);
    s_sn = devGroup.dev_ir.str_sn;
    if( s_sn.isEmpty() ) return false;
    s_sn +="_real";
    return true;
}

void WndIrPreview::add_no_preset_ana_info( QList<AnaInfo> &ana_list )
{
    for( QList<AnaInfo>::iterator it_ana = ana_list.begin(); it_ana != ana_list.end(); ++it_ana ){
        add_ana( *it_ana );
    }
}

void WndIrPreview::ir_wnd_update()
{
    if ( mn_group_number == -1 ) return;

    QList<AnaInfo> list_no_preset_ana_info = get_no_preset_ana_info();
    delete_no_preset_all_ana( mn_group_number );
    load_no_preset_ana( list_no_preset_ana_info );

    delete_preset_all_ana( mn_group_number );
    // 预置点不进行加载， 因为这时候已经不知道是那个预置点了
    // load_preset_all_ana( mn_group_number, mn_rail_no, mn_ptz_no );



//    QString s_sn;
//    if( !get_ir_sn(s_sn) ){
//        return ;
//    }

//    QMap< int, AnaInfo > map_ana_info;
//    CDataMode::Instance()->get_ana_info( s_sn, map_ana_info );
//    delete_all_ana();


//    QList< AnaInfo > list_ana;
//    QMap< int, AnaInfo >::iterator it = map_ana_info.begin();
//    for( ; it != map_ana_info.end(); ++it ){
//        list_ana.push_back( it.value() );
//    }
//    load_ana( list_ana );
}

//按下Ctrl键，可以画之前点击的分析图形
/*void WndIrPreview::keyPressEvent( QKeyEvent *p_key )
{
    if( p_key->key() == Qt::Key_Control ) {
        mb_press_ctrl = true;
    }
}

void WndIrPreview::keyReleaseEvent( QKeyEvent *p_key )
{
    if( p_key->key() == Qt::Key_Control ) {
        clear_ana_press_status();
        mb_press_ctrl = false;
    }
}*/

//清除分析按下状态
void WndIrPreview::clear_ana_press_status()
{
    me_mouse_press_status = none_press_status;
    //mb_press_ctrl = false;
    mp_ir_label->clear_down_status();
}

bool WndIrPreview::get_mouse_press_status()
{
    return mb_press_ctrl;
}

void WndIrPreview::set_mouse_press_status( bool b_press_ctrl )
{
    mb_press_ctrl = b_press_ctrl;
}



WndIrThread::WndIrThread( WndIrPreview *p_ir_wnd ):
    mb_exit( false )
  , mn_group_id( 0 )
{
    mp_ir_wnd = p_ir_wnd;
    AlarmJudge_Init();
}


WndIrThread::~WndIrThread()
{
    if( mn_group_id > 0 ){
        Delete_TempMap( mn_group_id );
    }
    AlarmJudge_UnInit();
}


void WndIrThread::run()
{
    while ( !mb_exit ){
        QString str_ana = "", s_sn = "", str_name = "", s_type = "";
        int n_wnd_id = -1, n_group_num = 0;
        THREE_MONITOR_TEMPERATURE_S ana_temp;
        QMap< int, AnaInfo > ana_info_list;
        QMap<QString, TempAnaBuf> ana_temp_map;
        QMap<QString, TempAnaBuf>::iterator it_ana_temp;
        list<THREE_MONITOR_TEMPERATURE_S> ana_temp_list;
        std::map<UINT32, std::list<THREE_MONITOR_TEMPERATURE_S> > preset_ana_temp_map;
        std::map<UINT32, std::list<THREE_MONITOR_TEMPERATURE_S> > no_preset_ana_temp_map;
        m_ana_info_list.clear();
        m_matrix_ana_list.clear();
        m_matrix_ana_map.clear();

        if( mp_ir_wnd && ( n_group_num = mp_ir_wnd->get_group_num() ) > 0 ){
            if( mn_group_id != n_group_num ){
                if( mn_group_id > 0 ){
                    Delete_TempMap( mn_group_id );
                    mn_group_id = n_group_num;
                } else {
                    mn_group_id = n_group_num;
                }
                mp_ir_wnd->set_matrix_ana_list( m_matrix_ana_list );
                mp_ir_wnd->set_ana_info_list( m_ana_info_list );
            }
        } else {
            if( n_group_num <= 0 && mn_group_id > 0 ){
                Delete_TempMap( mn_group_id );
                mn_group_id = 0;
                mp_ir_wnd->set_matrix_ana_list( m_matrix_ana_list );
                mp_ir_wnd->set_ana_info_list( m_ana_info_list );
            }
            msleep(1000);
            continue;
        }
        if( mp_ir_wnd && (!mp_ir_wnd->get_sn( s_sn )) ){
            msleep( 100 );
            continue;
        }
        if( mp_ir_wnd && ( !mp_ir_wnd->get_ana_info( ana_info_list )) ){
            msleep( 100 );
            continue;
        }

        if( !CDataChartMode::Instance()->get_ana_now_temp( s_sn, ana_temp_map ) ){
            msleep( 100 );
            continue;
        }

        QMap< int, AnaInfo >::iterator it = ana_info_list.begin();
        for( ; it != ana_info_list.end(); it++ ) {
            AnaInfo ana_info = it.value();
            WndIrPreview::shape_to_string( str_ana, ana_info );
            ana_temp_list.clear();
            if( (it_ana_temp = ana_temp_map.find( str_ana )) != ana_temp_map.end() ){
                ana_temp.ft_max = it_ana_temp.value().max_temp.f_temp;
                ana_temp.ft_avg = it_ana_temp.value().avg_temp.f_temp;
                ana_temp.ft_min = it_ana_temp.value().min_temp.f_temp;
                ana_temp_list.push_back( ana_temp );
                if( ana_info.b_preset_ana ){
                    if( ana_info.str_name.left(6) == "matrix" ){
                        if( CDataMode::Instance()->get_ana_info( s_sn, str_ana, n_wnd_id, str_name, s_type ) ){
                            m_matrix_ana_map[n_wnd_id] = str_ana;
                            preset_ana_temp_map[n_wnd_id] = ana_temp_list;
                        }
                    } else {
                        n_wnd_id = ana_info.n_sign_id;
                        preset_ana_temp_map[ ana_info.n_sign_id ] = ana_temp_list;
                    }
                } else {
                    no_preset_ana_temp_map[ ana_info.n_sign_id ] = ana_temp_list;
                }
            }
        }
        handle_preset_alarm( preset_ana_temp_map );
        handle_no_preset_alarm( no_preset_ana_temp_map );
        mp_ir_wnd->set_matrix_ana_list( m_matrix_ana_list );
        mp_ir_wnd->set_ana_info_list( m_ana_info_list );

        msleep(1000);
    }
}

void WndIrThread::init_ana_list()
{
    for( QMap< int, AnaInfo >::iterator it = m_ana_info_list.begin(); it != m_ana_info_list.end(); ++it ){
        it.value().b_alarm = false;
    }
}

void WndIrThread::handle_preset_alarm( std::map<UINT32, std::list<THREE_MONITOR_TEMPERATURE_S> > &preset_ana_temp_map )
{
    QMap<int, PresetAlarmCondition > preset_alarm_map;
    QMap<int, PresetAlarmCondition>::iterator it_preset_alarm;
    AlarmConditionInfo_S alarm_condtion_info;

    if( SetAlarmTempMap( mn_group_id, preset_ana_temp_map ) == -1 )  return;
    {
        QMutexLocker lc(&mp_ir_wnd->m_preset_alarm_mutex);
        preset_alarm_map = mp_ir_wnd->m_preset_alarm_map;
    }

    for( it_preset_alarm = preset_alarm_map.begin(); it_preset_alarm != preset_alarm_map.end(); it_preset_alarm++ ){
        alarm_condtion_info.str_alarm_condition = it_preset_alarm.value().str_condition.toStdString();
        alarm_condtion_info.str_alias = it_preset_alarm.value().str_name.toStdString();
        alarm_condtion_info.ui_alarm_no = it_preset_alarm.value().n_id_alarm;
        if( it_preset_alarm.value().str_name.left( 6 ) == "matrix" ){
           if( Judge_alarm( mn_group_id, alarm_condtion_info ) == 0 ){
               split_matrix_alarm_condition( it_preset_alarm.value() );
           }
       } else {
          if( Judge_alarm( mn_group_id, alarm_condtion_info ) == 0 ){
               split_general_alarm_condition( it_preset_alarm.value() );
          }
       }
   }
}


void WndIrThread::change_general_alarm_cond( QString &old_alarm_cond, QString &new_alarm_cond )
{
    QString str_alarm_cond = "", str_item = "", str_temp = "";
    int n_index = -1;
    QStringList s_alarm_cond_list;

    int n_alarm_index = old_alarm_cond.indexOf( "OR", 0 );
    if( n_alarm_index > 0 ){
        str_item = "OR";
        s_alarm_cond_list = old_alarm_cond.split( "OR" );
    } else {
        str_item = "AND";
        s_alarm_cond_list = old_alarm_cond.split( "AND" );
    }

    for( int i = 0; i < s_alarm_cond_list.size(); i++ ){
        str_alarm_cond = s_alarm_cond_list.at( i );
        if( str_alarm_cond.indexOf( '-', 0 ) > 0 ){
            n_index = str_alarm_cond.lastIndexOf( ']', -1 );
            str_temp = str_alarm_cond.left( n_index + 1 );
            str_alarm_cond.remove( 0, n_index + 1 );
            str_alarm_cond = " abs( " + str_temp  + " ) "+ str_alarm_cond;
        }
        new_alarm_cond += str_alarm_cond + str_item;
    }
    if( new_alarm_cond.left(1) == " " ){
        new_alarm_cond.remove( 0, 1 );
    }
    new_alarm_cond = new_alarm_cond.left( new_alarm_cond.length() - str_item.length() );
}


void WndIrThread::handle_no_preset_alarm( std::map<UINT32, std::list<THREE_MONITOR_TEMPERATURE_S> > &no_preset_ana_temp_map )
{
    QMap<int, PresetAlarmCondition > no_preset_alarm_map;
    QMap<int, PresetAlarmCondition>::iterator it_preset_alarm;
    AlarmConditionInfo_S alarm_condtion_info;
    QString new_alarm_cond = "";
    if( SetAlarmTempMap( mn_group_id, no_preset_ana_temp_map ) == -1 )  return;
    {
        QMutexLocker lc(&mp_ir_wnd->m_no_preset_alarm_mutex);
        no_preset_alarm_map = mp_ir_wnd->m_no_preset_alarm_map;
    }
    for( it_preset_alarm = no_preset_alarm_map.begin(); it_preset_alarm != no_preset_alarm_map.end(); it_preset_alarm++ ){
        new_alarm_cond = "";
        change_general_alarm_cond( it_preset_alarm.value().str_condition, new_alarm_cond );
        qDebug() << "非预置点报警条件" << new_alarm_cond;
        alarm_condtion_info.str_alarm_condition = new_alarm_cond.toStdString();
        alarm_condtion_info.str_alias = it_preset_alarm.value().str_name.toStdString();
        alarm_condtion_info.ui_alarm_no = it_preset_alarm.value().n_id_alarm;
        if( Judge_alarm( mn_group_id, alarm_condtion_info ) == 0 ){
            qDebug() << "报警.................";
            split_general_alarm_condition( it_preset_alarm.value() );
        }
    }
}


void WndIrThread::split_matrix_alarm_condition( PresetAlarmCondition &preset_alarm_cond )
{
    QMap<int, QString>::iterator it;
    int n_ana_id = split_num( preset_alarm_cond.str_condition );
    if( ( it = m_matrix_ana_map.find( n_ana_id ) ) != m_matrix_ana_map.end() ){
        m_matrix_ana_list.push_back( it.value() );
    }
}


void WndIrThread::split_general_alarm_condition( PresetAlarmCondition &preset_alarm_cond )
{
    QString s_alarm_condition = preset_alarm_cond.str_condition;
    QString str_alarm = "", str_item = "";
    int n_ana_id = -1;
    int n_index = -1;
    QStringList s_alarm_cond_list;

    int n_alarm_index = s_alarm_condition.indexOf( "OR", 0 );
    if( n_alarm_index > 0 ){
        s_alarm_cond_list = s_alarm_condition.split( "OR" );
    } else {
        s_alarm_cond_list = s_alarm_condition.split( "AND" );
    }
    for( int i = 0; i < s_alarm_cond_list.size(); i++ ){
        str_alarm = s_alarm_cond_list.at( i );
        n_index = str_alarm.indexOf( ' ', 0 );
        if( n_index == 0 ){  //去掉前面一空格
            str_alarm.remove( 0, n_index+1 );
            n_index = str_alarm.indexOf( ' ', 0 );
        }
        str_item = str_alarm.left( n_index );
        n_index = str_item.indexOf( '[', 0 );
        n_ana_id = split_num( str_item.left(n_index) );
        construct_ana_list( n_ana_id );

        n_index = str_alarm.indexOf( ' ', 0 );
        str_alarm.remove( 0, n_index+1 );
        n_index = str_alarm.indexOf( ' ', 0 );
        str_item = str_alarm.left( n_index );
        if( str_item == "+" || str_item == "-" ){  //后面还有一分析项
            str_alarm.remove( 0, n_index+1 );//同时去掉+-后的一空格
            n_index = str_alarm.indexOf( ' ', 0 );
            str_alarm = str_alarm.left( n_index );
            n_index = str_alarm.indexOf( '[', 0 );
            n_ana_id = split_num(str_alarm.left(n_index));
            construct_ana_list( n_ana_id );
        }
    }
}

//取出分析id号
int WndIrThread::split_num( QString str_ana_id )
{
    str_ana_id = str_ana_id.left( str_ana_id.indexOf( '[' ) );
    QByteArray ba = str_ana_id.toLatin1();
    char c_data;
    char *p_c = ba.data();
    int n_num = 0;
    while( *p_c != '\0' ){
        c_data = *p_c;
        if( c_data >='0' && c_data <='9' ) {
            n_num = n_num*10;
            n_num += c_data - '0';
        } else {
            n_num = 0;
        }
        p_c++;
    }
    return n_num;
}

void WndIrThread::construct_ana_list( int n_id )
{
    QMap< int, AnaInfo > ana_info_list;
    if( !mp_ir_wnd->get_ana_info( ana_info_list ) ){
        return;
    }

    QMap< int, AnaInfo >::iterator it = m_ana_info_list.find( n_id );
    if ( it != m_ana_info_list.end() ) return;


    QMap< int, AnaInfo >::iterator it_ana = ana_info_list.find( n_id );
    if ( it_ana != ana_info_list.end() ){
        AnaInfo& ana_info = it_ana.value();
        ana_info.b_alarm = true;
        m_ana_info_list[ n_id ] = ana_info;
    }

//    for( QList<AnaInfo>::iterator it_ana_info = ana_info_list.begin(); it_ana_info != ana_info_list.end(); ++it_ana_info ){
//        if( it_ana_info->n_sign_id == n_id ){
//            it_ana_info->b_alarm = true;
//            ana_info = *it_ana_info;
//            break;
//        }
//    }
}

