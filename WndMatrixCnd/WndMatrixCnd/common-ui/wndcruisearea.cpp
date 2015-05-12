#include "wndcruisearea.h"
#include "ui_wndcruisearea.h"
#include "datamode.h"
#include <QMessageBox>
#include "../common-funtion/msclientlog.h"

WndCruiseArea::WndCruiseArea( QWidget *parent, int n_funtion ) :
    QWidget(parent),
    ui(new Ui::WndCruiseArea)
    , mn_group_number( 0 )
    , mn_funtion_map( n_funtion )
    , mb_preset_display( true )
    , mn_get_pic_count( 0 )
{
    ui->setupUi(this);
    init_ctrl();
    init_conn();
}

WndCruiseArea::~WndCruiseArea()
{
    delete ui;
}

void WndCruiseArea::init_ctrl()
{    
    ui->id_widget_image->set_funtion( mn_funtion_map );

    ui->id_lab_group_name->setAlignment( Qt::AlignCenter );
    ui->id_lab_group_name->setStyleSheet( "border:1px solid white;color:white" );
    QFont ft;
    ft.setPointSize(12);
    ui->id_lab_group_name->setFont( ft );

}

void WndCruiseArea::init_conn()
{
    connect( ui->id_btn_cruise_start, SIGNAL( clicked() ), this, SLOT( slot_btn_clicked() ) );
    connect( ui->id_combox_cruise_num, SIGNAL( currentIndexChanged( int ) ), this, SLOT( slot_combo_cruise_change( int ) ) );

    connect( ui->id_widget_image, SIGNAL( sig_call_preset( int,int ) ), this, SLOT( slot_call_preset( int, int ) ) );
}

void WndCruiseArea::resizeEvent( QResizeEvent *event )
{
    move_widget();
}

void WndCruiseArea::move_widget()
{
    QRect rct_group_name, rct_combox, rct_btn, rct_image;
    rct_group_name = QRect( 0, 0, width(), ui->id_lab_group_name->height() );
    rct_combox = QRect( rct_group_name.x(), rct_group_name.y()+rct_group_name.height(), width()-ui->id_btn_cruise_start->width()
                        , ui->id_combox_cruise_num->height() );
    rct_btn = QRect( rct_combox.x()+rct_combox.width(), rct_combox.y(), ui->id_btn_cruise_start->width(), ui->id_combox_cruise_num->height() );
    rct_image = QRect( rct_combox.x(), rct_combox.y()+rct_combox.height(), width(), height()-rct_combox.y()-rct_combox.height() );

    move_group_name( rct_group_name );
    move_combox( rct_combox );
    move_btn( rct_btn );
    move_image( rct_image );
}

void WndCruiseArea::move_group_name( QRect rct )
{
    ui->id_lab_group_name->move( rct.x(), rct.y() );
    ui->id_lab_group_name->resize( rct.width(), rct.height() );
}

void WndCruiseArea::move_combox( QRect rct )
{
    ui->id_combox_cruise_num->move( rct.x(), rct.y() );
    ui->id_combox_cruise_num->resize( rct.width(), rct.height() );
}

void WndCruiseArea::move_btn( QRect rct )
{
    ui->id_btn_cruise_start->move( rct.x(), rct.y() );
    ui->id_btn_cruise_start->resize( rct.width(), rct.height() );
}

void WndCruiseArea::move_image( QRect rct )
{
    ui->id_widget_image->move( rct.x(), rct.y() );
    ui->id_widget_image->resize( rct.width(), rct.height() );
}

void WndCruiseArea::get_cruise_area_info()
{
    ui->id_widget_image->set_arrive_preset( -1, -1 );
    mb_preset_display = false;
    get_preset_pts( mn_group_number );
}

int WndCruiseArea::onResponse( CBaseMsg& reqMsg,CBaseMsg& rspMsg)
{
    CPtzRailView::onResponse(reqMsg,rspMsg);

    switch( rspMsg.m_nCmd ) {
    case PresetCallPreset:
    {
        int n_group = reqMsg.map_msgdata[MSGDATA_GROUPNUM].value<int>();
        int n_ptz = reqMsg.map_msgdata[MSGDATA_PTZ_PRESET].value<int>();
        int n_rail= reqMsg.map_msgdata[MSGDATA_RAIL_PRESET].value<int>();

        if ( n_group == mn_group_number ) {
            emit sig_update_ir_ana( n_rail, n_ptz );
        }
    }
        break;
    case PresetGetInfo:
    {
        int n_group = reqMsg.map_msgdata[MSGDATA_GROUPNUM].value<int>();
        if (  n_group == mn_group_number ) {
            QString str_group_name = "";
            str_group_name = get_group_str_name( mn_group_number );
            if ( rspMsg.m_nResult== ResultOk ) {
                MS_LOGER_INFO( "组别名为:%s获取预置点信息成功,发送获取图片命令......", str_group_name.toStdString().data() );
                get_pic( mn_group_number );
            }else {
                MS_LOGER_INFO( "组别名为:%s获取预置点信息失败,再次发送获取预置点信息命令......", str_group_name.toStdString().data() );
                get_preset_pts( mn_group_number );
            }
        }
    }
        break;
    case PicGet:
    {
        int n_group = reqMsg.map_msgdata[MSGDATA_GROUPNUM].value<int>();
        if (  n_group == mn_group_number ) {
            QString str_group_name = "";
            str_group_name = get_group_str_name( mn_group_number );
            if ( rspMsg.m_nResult== ResultOk ) {
                MS_LOGER_INFO( "组别名为:%s获取图片成功......", str_group_name.toStdString().data() );
                mn_get_pic_count = 0;
                update_preset_map();
            } else {
                if( mn_get_pic_count < 10 ){
                    MS_LOGER_INFO( "组别名为:%s获取图片失败, 第%d次发送获取图片命令", str_group_name.toStdString().data(), ( mn_get_pic_count + 1 ) );
                    get_pic( mn_group_number );
                    mn_get_pic_count++;
                    break;
                } else {
                    ui->id_widget_image->set_status_get( false );
                }
            }
            MS_LOGER_INFO( "组别名为:%s发送获取巡检状态命令......", str_group_name.toStdString().data() );
            get_cruise_info( mn_group_number );
        }
    }
        break;
    case CruiseGetInfo:
    {
        int n_group = reqMsg.map_msgdata[MSGDATA_GROUPNUM].value<int>();
        if (  n_group == mn_group_number ) {
            QString str_group_name = "";
            str_group_name = get_group_str_name( mn_group_number );
            enable_all_ctrl( true );
            if ( rspMsg.m_nResult == ResultOk ) {
                QMap<int, CruiseSchemeInfo> map_scheme;
                CDataMode::Instance()->get_cruise_scheme( mn_group_number, map_scheme );
                update_combo_cruise( map_scheme );
                if ( map_scheme.isEmpty() ) {
                    ui->id_combox_cruise_num->setEnabled( false );
                    ui->id_btn_cruise_start->setEnabled( false );
                }
              ////  emit sig_update_start_cruise();
                MS_LOGER_INFO( "组别名为:%s获取巡检状态成功......", str_group_name.toStdString().data() );
                get_cruise_cur_preset( mn_group_number );

            } else {
                MS_LOGER_INFO( "组别名为:%s没有巡检方案......", str_group_name.toStdString().data() );
                ui->id_combox_cruise_num->addItem( "没有巡检方案" );
                ui->id_combox_cruise_num->setCurrentIndex( 0 );
                ui->id_combox_cruise_num->setEnabled( false );
                ui->id_btn_cruise_start->setEnabled( false );
            }
        }
        break;
    }
    case CruiseStart:
    {
        qDebug() << "CruiseStart";
        int n_group = reqMsg.map_msgdata[MSGDATA_GROUPNUM].value<int>();
        int n_cruise = reqMsg.map_msgdata[MSGDATA_INDEX].value<int>();
        if (  n_group == mn_group_number ) {
            if ( rspMsg.m_nResult == ResultOk ) {
              //  mb_cruiseing = true;
                update_cruise_ctrl( true );
            } else {
                QMessageBox::information( this, "提示", "开始巡检方案失败！" );
            }
        }
        break;
    }
    case CruiseStop:
    {
        qDebug() << "CruiseStop";
        int n_group = reqMsg.map_msgdata[MSGDATA_GROUPNUM].value<int>();
        int n_cruise = reqMsg.map_msgdata[MSGDATA_INDEX].value<int>();
        if (  n_group == mn_group_number ) {
            if ( rspMsg.m_nResult == ResultOk ) {
                update_cruise_ctrl( false );
            } else {
                QMessageBox::information( this, "提示", "停止巡检失败!" );
            }
        }
        break;
    }
    case CruiseCurPreset:
    {
        int n_ptz, n_rail;
        if ( rspMsg.m_nResult == ResultOk ) {
            int n_group = reqMsg.map_msgdata[MSGDATA_GROUPNUM].value<int>();
            n_ptz = rspMsg.map_msgdata[MSGDATA_PTZ_PRESET].value<int>();
            n_rail= rspMsg.map_msgdata[MSGDATA_RAIL_PRESET].value<int>();
            if( mb_preset_display ){
                emit sig_cruise_selected_preset( n_rail, n_ptz );
            }else{
                mb_preset_display = true;
            }

        }
        break;
    }
    }

    return 0;
}

int WndCruiseArea::onNotify(CBaseMsg& msg)
{
    switch( msg.m_nCmd ) {
    case NotifyCruiseStop:
    {
        update_cruise_ctrl( false );

       // mb_cruiseing = false;
        break;
    }
    case NotifyGetToPreset:
    {
      //  mb_cruiseing = true;
        qDebug() << "NotifyGetToPreset";
        int n_group= msg.map_msgdata[MSGDATA_GROUPNUM].toInt();
        int n_rail = msg.map_msgdata[MSGDATA_RAIL_PRESET].value<int>();
        int n_ptz = msg.map_msgdata[MSGDATA_PTZ_PRESET].value<int>();
        if ( mn_group_number == n_group ) {
            ui->id_widget_image->set_arrive_preset( n_rail, n_ptz );
        }
        break;
    }
    case NotifyLeavePreset:
    {
        qDebug() << "NotifyLeavePreset";
        int n_group= msg.map_msgdata[MSGDATA_GROUPNUM].toInt();
        int n_rail = msg.map_msgdata[MSGDATA_RAIL_PRESET].value<int>();
        int n_ptz = msg.map_msgdata[MSGDATA_PTZ_PRESET].value<int>();
        if ( mn_group_number == n_group ) {
            emit sig_del_preset_all_ana( n_group );
        }
        break;
    }
    default:
        break;
    }

    return 0;
}


//获取组号对应的别名
QString WndCruiseArea::get_group_str_name( int n_group_num )
{
    QMap< int, DeviceGroup >::iterator it_device;
    QMap< int, DeviceGroup > device_group_map;
    QString str_group_name;
    CDataMode::Instance()->get_group_device( device_group_map );
    if( ( it_device = device_group_map.find( n_group_num ) ) != device_group_map.end() ){
        if( it_device.value().str_name != "" ){
            str_group_name = it_device.value().str_name;
            return str_group_name;
        }
    }
    str_group_name =  "组号-" + QString::number( n_group_num );
    return str_group_name;
}


void WndCruiseArea::set_cruise_selected_preset( int n_rail, int n_ptz )
{
    QList< PresetInfo * > &preset_info_list = ui->id_widget_image->get_preset_info();
    for( QList< PresetInfo * >::iterator it_preset = preset_info_list.begin(); it_preset != preset_info_list.end(); ++it_preset ){
        if( (*it_preset)->n_rail_preset_no == n_rail && (*it_preset)->n_ptz_preset_no == n_ptz ){
            ui->id_widget_image->get_double_preset_info() = *it_preset;
 //           emit sig_cruise_selected_preset( n_rail, n_ptz );
        }
    }
}


void WndCruiseArea::set_group_num( int n_num )
{
    if ( n_num == mn_group_number ) return;

    mn_group_number = n_num;

    DeviceGroup device_group;
    CDataMode::Instance()->get_group_info( mn_group_number, device_group );
    if( device_group.str_name != "" ) {
        ui->id_lab_group_name->setText( device_group.str_name );
    } else {
        ui->id_lab_group_name->setText( n_num<=0?"未设置组":("组号-"+QString::number( n_num )) );
    }

    enable_all_ctrl( false );
    clear_all_ctrl_info();

    if ( n_num > 0 ) {
        get_preset_pts( n_num );
    } else {
        ui->id_widget_image->set_status_get( false );
    }

    DeviceGroup dev_group;
    if( CDataMode::Instance()->get_group_info( mn_group_number, dev_group ) ) {
        get_pelete_clr( dev_group.dev_ir.str_sn );
    }
}

void WndCruiseArea::set_widget_hidden( bool b_hidden )
{
    ui->id_btn_cruise_start->setHidden( b_hidden );
    ui->id_combox_cruise_num->setHidden( b_hidden );
    ui->id_lab_group_name->setHidden( b_hidden );
    ui->id_widget_image->setHidden( b_hidden );
}

void WndCruiseArea::slot_btn_clicked()
{
    int n_cruise = ui->id_combox_cruise_num->currentData().toInt();
    if ( ui->id_btn_cruise_start->text() == "开始" ) {
        start_cruise( mn_group_number, n_cruise );
    } else {
        strop_cruise( mn_group_number, n_cruise );
    }
}

void WndCruiseArea::slot_combo_cruise_change( int n_index )
{
    if ( n_index < 0 ) return;

    int n_cruise = ui->id_combox_cruise_num->itemData( n_index ).toInt();

    CruiseSchemeInfo cru_info;
    if ( CDataMode::Instance()->get_cruise_scheme( mn_group_number, n_cruise, cru_info ) ) {
        ui->id_widget_image->update_scheme_info( cru_info );
    }
}

void WndCruiseArea::slot_call_preset( int n_rail, int n_ptz )
{
    call_preset( mn_group_number, n_rail, n_ptz );
}

bool WndCruiseArea::is_cruiseing()
{
    bool b_ret = false;
    QMap<int, CruiseSchemeInfo> map_scheme;
    CDataMode::Instance()->get_cruise_scheme( mn_group_number, map_scheme );
    QMap<int, CruiseSchemeInfo>::iterator it = map_scheme.begin();
    for( ; it != map_scheme.end(); it++ ){
        if( it.value().b_cruise ){
            b_ret = true;
            break;
        }
    }
    return b_ret;
}

void WndCruiseArea::enable_all_ctrl( bool b_enable )
{
    ui->id_combox_cruise_num->setEnabled( b_enable );
    ui->id_btn_cruise_start->setEnabled( b_enable );

    emit sig_enable_ctrl( b_enable );
}

void WndCruiseArea::clear_all_ctrl_info()
{
    ui->id_widget_image->clear_preset_info();
    ui->id_widget_image->set_status_get( true );

    ui->id_combox_cruise_num->clear();
    update_cruise_ctrl( false );
}

void WndCruiseArea::update_cruise_ctrl( bool b_cruise )
{
    ui->id_combox_cruise_num->setEnabled( !b_cruise );
    ui->id_btn_cruise_start->setText( b_cruise?"停止":"开始" );
}

void WndCruiseArea::update_map_preset( int n_rail, int n_ptz )
{
    ui->id_widget_image->set_arrive_preset( n_rail, n_ptz );
}

void WndCruiseArea::selected_preset( int n_rail, int n_ptz )
{
    PresetInfo preset_info;
    CDataMode::Instance()->get_preset_info( mn_group_number, n_rail, n_ptz, preset_info );
//    ui->id_widget_image->set_arrive_preset( n_rail, n_ptz );
    ui->id_widget_image->set_selected_preset( preset_info );
}

void WndCruiseArea::update_preset_map()
{
    QMap< int,  PresetInfo> map_preset_info;
    int n_buf_len( 0 ); unsigned char *p_pic_buf( 0 );
    CDataMode::Instance()->get_preset_info( mn_group_number, map_preset_info );
    if ( CDataMode::Instance()->get_preset_pic( mn_group_number, n_buf_len, 0 ) ) {
        p_pic_buf = new unsigned char[ n_buf_len ];
        CDataMode::Instance()->get_preset_pic( mn_group_number, n_buf_len, p_pic_buf );
    }

    ui->id_widget_image->set_status_get( false );
    ui->id_widget_image->update_preset_info( map_preset_info, n_buf_len, p_pic_buf );

    if ( p_pic_buf ) {
        delete[] p_pic_buf;
    }
}

void WndCruiseArea::update_combo_cruise( QMap<int, CruiseSchemeInfo> &map_cruise )
{
    ui->id_combox_cruise_num->clear();
    bool b_cruise = false;
    QMap<int, CruiseSchemeInfo>::iterator it = map_cruise.begin();
    for ( ; it != map_cruise.end(); ++it ) {
        ui->id_combox_cruise_num->addItem( it.value().str_crusie_name, it.value().n_cruise_num );
        if ( it.value().b_cruise ) {
            b_cruise = true;
            ui->id_combox_cruise_num->setCurrentIndex( ui->id_combox_cruise_num->count() - 1 );
        //    ui->id_widget_image->set_cruise_state( true );
            emit sig_cruise_state( true );
            break;
        }
    }
    update_cruise_ctrl( b_cruise );
}

void WndCruiseArea::set_cruise_state( bool b_cruise )
{
  //   ui->id_widget_image->set_cruise_state( b_cruise );
}

//获取选中的预置点
bool WndCruiseArea::get_cur_preset_no( int &n_rail, int &n_ptz )
{
    return ui->id_widget_image->get_cur_preset( n_rail, n_ptz );
}


