#include "autoconnectirvi.h"
#include "cbasemdl.h"
#include <qdebug.h>
#include <windows.h>
#include "../common-funtion/msclientlog.h"
#include <QThread>
#include <QTextCodec>


AutoConnectIrVi::AutoConnectIrVi() :
    mn_ir_time_id( -1 )
  , mn_vi_time_id( -1 )
{
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF8"));
    m_ir_connect_status_map.clear();
    m_vi_sub_connect_status_map.clear();
    m_vi_main_connect_status_map.clear();
    m_group_connect_map.clear();
    m_ir_reconnect_list.clear();
    m_vi_reconnect_list.clear();
    mp_ir_timer = new QTimer(this);
    connect( mp_ir_timer, SIGNAL( timeout() ), this, SLOT( slot_ir_connect_timer() ) );
    mp_vi_timer = new QTimer(this);
    connect( mp_vi_timer, SIGNAL( timeout() ), this, SLOT( slot_vi_connect_timer() ) );
}

AutoConnectIrVi::~AutoConnectIrVi()
{
    stop_ir_timer();
    stop_vi_timer();
    if( mp_ir_timer ){
        delete mp_ir_timer;
        mp_ir_timer = NULL;
    }
    if( mp_vi_timer ){
        delete mp_vi_timer;
        mp_vi_timer = NULL;
    }
}


void AutoConnectIrVi::connect_device( int n_group_num, DeviceType device_type, bool b_sub_stream )
{
    IR_VI_CONNECT_STATUS_E connect_status;
    QString str_group_name = "";
    str_group_name = get_group_str_name( n_group_num );
    if( device_type == DeviceIR ){
        connect_status = get_ir_connect_status( n_group_num );
        if( connect_status == CONNECTING || connect_status == CONNECTED )  return; //若处于正在重连中,则不再进行连接
        MS_LOGER_INFO( "红外连接中......,组别名为:%s", str_group_name.toStdString().data() );
        update_group_connect_status( n_group_num, true );
        update_ir_connect_status( n_group_num, CONNECTING, false );
    }else if( device_type == DeviceVI ){
        connect_status = get_vi_connect_status( n_group_num, b_sub_stream );
        if( connect_status == CONNECTING || connect_status == CONNECTED )  return;
        if( b_sub_stream ){
            update_vi_sub_connect_status( n_group_num, CONNECTING, false );  //false表示不是主动断开通知的
        }else{
            update_vi_main_connect_status( n_group_num, CONNECTING, false );
        }
        MS_LOGER_INFO( "可见光连接中......,组别名为:%s, 码流为:%s", str_group_name.toStdString().data(), ( b_sub_stream? "子码流":"主码流" ) );
    }
    connect_group_device( n_group_num, device_type, b_sub_stream );
}

//更新红外map状态
void AutoConnectIrVi::update_ir_connect_status( int n_group_num, IR_VI_CONNECT_STATUS_E ir_connect_status, bool b_self_disconnect )
{
    map< int, IR_VI_STATUS_S >::iterator it_ir_status = m_ir_connect_status_map.find( n_group_num );
    if( it_ir_status != m_ir_connect_status_map.end() ){
        it_ir_status->second.connect_status = ir_connect_status;
        it_ir_status->second.b_self_disconnect = b_self_disconnect;
    }
}

//更新可见光子码流map状态
void AutoConnectIrVi::update_vi_sub_connect_status( int n_group_num, IR_VI_CONNECT_STATUS_E ir_connect_status, bool b_self_disconnect )
{
    map< int, IR_VI_STATUS_S >::iterator it_vi_status = m_vi_sub_connect_status_map.find( n_group_num );
    if( it_vi_status != m_vi_sub_connect_status_map.end() ){
        it_vi_status->second.connect_status = ir_connect_status;
        it_vi_status->second.b_self_disconnect = b_self_disconnect;
    }
}

//更新可见光主码流map状态
void AutoConnectIrVi::update_vi_main_connect_status( int n_group_num, IR_VI_CONNECT_STATUS_E ir_connect_status, bool b_self_disconnect )
{
    map< int, IR_VI_STATUS_S >::iterator it_vi_status = m_vi_main_connect_status_map.find( n_group_num );
    if( it_vi_status != m_vi_main_connect_status_map.end() ){
        it_vi_status->second.connect_status = ir_connect_status;
        it_vi_status->second.b_self_disconnect = b_self_disconnect;
    }
}


//返回当前红外连接状态
IR_VI_CONNECT_STATUS_E AutoConnectIrVi::get_ir_connect_status( int n_group_num )
{
    map< int, IR_VI_STATUS_S >::iterator it_ir_status = m_ir_connect_status_map.find( n_group_num );
    if( it_ir_status != m_ir_connect_status_map.end() ){
        return it_ir_status->second.connect_status;
    }
    return NO_STATUS;
}


//返回当前可见光连接状态
IR_VI_CONNECT_STATUS_E AutoConnectIrVi::get_vi_connect_status( int n_group_num, int n_stream )
{
    map< int, IR_VI_STATUS_S >::iterator it_vi_status;
    if( n_stream == StreamSub ){
        it_vi_status = m_vi_sub_connect_status_map.find( n_group_num );
        if( it_vi_status != m_vi_sub_connect_status_map.end() ){
            return it_vi_status->second.connect_status;
        }
    } else if( n_stream == StreamMain ){
        it_vi_status = m_vi_main_connect_status_map.find( n_group_num );
        if( it_vi_status != m_vi_main_connect_status_map.end() ){
            return it_vi_status->second.connect_status;
        }
    }
    return NO_STATUS;
}

//返回红外是否主动断开
bool AutoConnectIrVi::get_ir_self_disconnect( int n_group_num )
{
    map< int, IR_VI_STATUS_S >::iterator it_ir_status = m_ir_connect_status_map.find( n_group_num );
    if( it_ir_status != m_ir_connect_status_map.end() ){
        return it_ir_status->second.b_self_disconnect;
    }
    return true;
}


//返回可见光是否主动断开
bool AutoConnectIrVi::get_vi_self_disconnect( int n_group_num, int stream_type )
{
    map< int, IR_VI_STATUS_S >::iterator it_ir_status;
    if( stream_type == StreamSub ){
        it_ir_status = m_vi_sub_connect_status_map.find( n_group_num );
        return it_ir_status->second.b_self_disconnect;
    }else if( stream_type == StreamMain ){
        it_ir_status = m_vi_main_connect_status_map.find( n_group_num );
        return it_ir_status->second.b_self_disconnect;
    }
    return true;
}


void AutoConnectIrVi::dis_connect_device( int n_group_num, DeviceType device_type, bool b_sub_stream,
                                          IR_VI_CONNECT_STATUS_E ir_connect_status /* = NO_STATUS */, bool b_self_disconnect /*=true*/ )
{
    QString str_group_name = "";
    str_group_name = get_group_str_name( n_group_num );
    disconnect_group_device( n_group_num, device_type, b_sub_stream );
    if( device_type == DeviceIR ){
        MS_LOGER_INFO( "红外断开连接,组别名为:%s", str_group_name.toStdString().data() );
        update_ir_connect_status( n_group_num, ir_connect_status, b_self_disconnect );
    }else if( device_type == DeviceVI ){
        if( b_sub_stream ){
            update_vi_sub_connect_status( n_group_num, ir_connect_status, b_self_disconnect );
        }else{
            update_vi_main_connect_status( n_group_num, ir_connect_status, b_self_disconnect );
        }
        MS_LOGER_INFO( "可见光断开连接,组别名为:%s, 码流为:%s", str_group_name.toStdString().data(), ( b_sub_stream? "子码流":"主码流" ) );
        emit sig_vi_disconnect_status( n_group_num, b_sub_stream ); //发送断开可见光状态
    }
}

void AutoConnectIrVi::init_connect_status_map( int n_group_num, IR_VI_CONNECT_STATUS_E connect_status, bool b_self_disconnect, DeviceType device_type, StreamType stream_type )
{
    IR_VI_STATUS_S ir_vi_status;
    ir_vi_status.connect_status = connect_status;
    ir_vi_status.b_self_disconnect = b_self_disconnect;
    if( device_type == DeviceIR ){
        m_ir_connect_status_map[n_group_num] = ir_vi_status;
    }else if( device_type == DeviceVI ){
        if( stream_type == StreamSub ){
            m_vi_sub_connect_status_map[n_group_num] = ir_vi_status;
        }else if( stream_type == StreamMain ){
            m_vi_main_connect_status_map[n_group_num] = ir_vi_status;
        }
    }
}

void AutoConnectIrVi::slot_ir_connect_timer()
{
    QString str_group_name = "";
    list<int>::iterator it_group = m_ir_reconnect_list.begin();
    for( ; it_group != m_ir_reconnect_list.end(); it_group++ ){
        if( get_ir_connect_status( *it_group ) == DISCONNECT ){
            str_group_name = get_group_str_name( *it_group );
            MS_LOGER_INFO( "正在连接红外......,组别名为:%s", str_group_name.toStdString().data() );
            update_group_connect_status( *it_group, false );
            update_ir_connect_status( *it_group, CONNECTING, false );
            connect_group_device( *it_group, DeviceIR ); //红外只有主码流(默认)
        }
    }
    m_ir_reconnect_list.clear(); //只发一次,发完之后清空列表及关闭定时器
    stop_ir_timer();
}


void AutoConnectIrVi::slot_vi_connect_timer()
{
    QString str_group_name = "";
    list<VI_RECONNECT_S>::iterator it_group = m_vi_reconnect_list.begin();
    for( ; it_group != m_vi_reconnect_list.end(); it_group++ ){
        if( get_vi_connect_status( it_group->n_group_num, it_group->device_type ) == DISCONNECT ){
            if( it_group->device_type == StreamSub ){
                update_vi_sub_connect_status( it_group->n_group_num, CONNECTING, false );
            }else if( it_group->device_type == StreamMain ){
                update_vi_main_connect_status( it_group->n_group_num, CONNECTING, false );
            }
            str_group_name = get_group_str_name( it_group->n_group_num );
            MS_LOGER_INFO( "正在连接可见光......,组别名为:%s,码流为:%s", str_group_name.toStdString().data(), ( it_group->device_type? "子码流":"主码流" ) );
            connect_group_device( it_group->n_group_num, DeviceVI, it_group->device_type );
        }
    }
    m_vi_reconnect_list.clear();
    stop_vi_timer();
}


void AutoConnectIrVi::start_ir_timer()
{
    mp_ir_timer->start( 1000 );
    mn_ir_time_id = mp_ir_timer->timerId();
}


void AutoConnectIrVi::stop_ir_timer()
{
    mp_ir_timer->stop();
    mn_ir_time_id = -1;
}


void AutoConnectIrVi::start_vi_timer()
{
    mp_vi_timer->start( 1000 );
    mn_vi_time_id = mp_vi_timer->timerId();
}


void AutoConnectIrVi::stop_vi_timer()
{
    mp_vi_timer->stop();
    mn_vi_time_id = -1;
}

//要重连的红外放到列表中
void AutoConnectIrVi::put_ir_reconnect_group( int n_group_num )
{
    list<int>::iterator it_group = m_ir_reconnect_list.begin();
    for( ; it_group != m_ir_reconnect_list.end(); it_group++ ){
        if( *it_group == n_group_num ){
            return;
        }
    }
//    if( m_ir_reconnect_list.size() == 0 ){
//        start_ir_timer();
//    }
    m_ir_reconnect_list.push_back( n_group_num );
    if( mn_ir_time_id < 0 && m_ir_reconnect_list.size() > 0 ){  //mn_ir_time_id小于0说明定时器是关闭的
        start_ir_timer();
    }
}

//红外从重连列表中移除
void AutoConnectIrVi::remove_ir_reconnect_group( int n_group_num )
{
    list<int>::iterator it_group = m_ir_reconnect_list.begin();
    for( ; it_group != m_ir_reconnect_list.end(); ){
        if( *it_group == n_group_num ){
            it_group = m_ir_reconnect_list.erase( it_group );
        }else{
            it_group++;
        }
    }
    if( m_ir_reconnect_list.size() == 0 ){
        stop_ir_timer();
    }
}

//要重连的可见光放到列表中
void AutoConnectIrVi::put_vi_reconnect_group( int n_group_num, int type_stream )
{
    list<VI_RECONNECT_S>::iterator it_group = m_vi_reconnect_list.begin();
    for( ; it_group != m_vi_reconnect_list.end(); it_group++ ){
        if( it_group->n_group_num == n_group_num && it_group->device_type == type_stream ){
            return;
        }
    }

//    if( m_vi_reconnect_list.size() == 0 ){
//        start_vi_timer();
//    }
    VI_RECONNECT_S vi_reconnect;
    vi_reconnect.n_group_num = n_group_num;
    vi_reconnect.device_type = type_stream;
    m_vi_reconnect_list.push_back( vi_reconnect );
    if( mn_vi_time_id < 0 && m_vi_reconnect_list.size() > 0 ){
        start_vi_timer();
    }
}

//可见光从重连列表中移除
void AutoConnectIrVi::remove_vi_reconnect_group( int n_group_num, int n_stream )
{
    list<VI_RECONNECT_S>::iterator it_group = m_vi_reconnect_list.begin();
    for( ; it_group != m_vi_reconnect_list.end(); ){
        if( it_group->n_group_num == n_group_num && it_group->device_type == n_stream  ){
            it_group = m_vi_reconnect_list.erase( it_group );
        } else {
            it_group++;
        }
    }
    if( m_vi_reconnect_list.size() == 0 ){
        stop_vi_timer();
    }
}

//清空所有要重连的项
void AutoConnectIrVi::clear_all_reconnect_list()
{
    m_ir_reconnect_list.clear();
    m_vi_reconnect_list.clear();
    stop_ir_timer();
    stop_vi_timer();

}

//设置切换组时去连接还是断开之后去连接
void AutoConnectIrVi::update_group_connect_status( int n_group_num, bool b_self_connect )
{
    m_group_connect_map[n_group_num] = b_self_connect;
}

//返回是否是切换组时去连接
bool AutoConnectIrVi::get_group_connect_status( int n_group_num )
{
    map< int, bool >::iterator it = m_group_connect_map.find( n_group_num );
    if( it != m_group_connect_map.end() ){
        return it->second;
    }
    return false;
}

//获取组号对应的别名
QString AutoConnectIrVi::get_group_str_name( int n_group_num )
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

void AutoConnectIrVi::onNotify( CBaseMsg& msg, int n_group_num )
{
    int n_stream = msg.map_msgdata[MSGDATA_TYPE].toInt(); //  StreamKey, StreamMain, StreamSub;
    int n_from_mdl = msg.m_nFromMdl;
    QString str_group_name = "";
    str_group_name = get_group_str_name( n_group_num );
    if( n_from_mdl == IrMonitorMdl ){   //红外:
        if( !get_ir_self_disconnect( n_group_num ) ){     //如果红外不是主动断开
            MS_LOGER_INFO( "红外断开通知, 组别名为:%s", str_group_name.toStdString().data() );
            update_ir_connect_status( n_group_num, DISCONNECT, false );
            put_ir_reconnect_group( n_group_num );
        }
    } else if( n_from_mdl == ViMonitorMdl ){  //可见光:
        if( !get_vi_self_disconnect( n_group_num, n_stream ) ){
            MS_LOGER_INFO( "可见光断开通知, 组别名为:%s, 可见光码流类别:%s", str_group_name.toStdString().data(), ( n_stream? "子码流":"主码流" ) );
            if( n_stream == StreamSub ){
                update_vi_sub_connect_status( n_group_num, DISCONNECT, false );
            }else if( n_stream == StreamMain ){
                update_vi_main_connect_status( n_group_num, DISCONNECT, false );
            }
            put_vi_reconnect_group( n_group_num, n_stream );
        }
    }

}

int AutoConnectIrVi::onResponse( CBaseMsg& reqMsg,CBaseMsg& rspMsg )
{
    CMainView::onResponse( reqMsg, rspMsg );
    bool b_sub_stream = reqMsg.map_msgdata[ MSGDATA_POS ].toBool(); //  StreamKey, StreamMain, StreamSub;
    int n_stream = b_sub_stream ? StreamSub : StreamMain;
    int n_type = reqMsg.map_msgdata[MSGDATA_TYPE].toInt();
    int n_group_number = reqMsg.map_msgdata[MSGDATA_GROUPNUM].toInt();
    QString str_group_name = "";
    str_group_name = get_group_str_name( n_group_number );
    if( rspMsg.m_nCmd == StartRealPlay ){
        if ( rspMsg.m_nResult == ResultOk ) {
            if( n_type == DeviceIR ){  //红外:
                if( get_ir_connect_status( n_group_number ) == NO_STATUS ){ //若对应窗口已不存在,表示已断开,现在返回连接成功,再次发送断开命令
                    MS_LOGER_INFO( "红外对应窗口已不存在,返回连接成功,再次发送断开命令,组别名为:%s", str_group_name.toStdString().data() );
                    dis_connect_device( n_group_number, DeviceIR, false );
                } else {
                    MS_LOGER_INFO( "红外连接成功,组别名为:%s", str_group_name.toStdString().data() );
                    update_ir_connect_status( n_group_number, CONNECTED, false );
                    emit sig_connect_status( n_group_number, DeviceIR, true, StreamMain, get_group_connect_status(n_group_number) );
                }
                remove_ir_reconnect_group( n_group_number ); //从要重连的列表中删除
            } else if( n_type == DeviceVI ){  //可见光:
                if( get_vi_connect_status( n_group_number, n_stream ) == NO_STATUS ){ //若对应窗口已不存在,表示已断开,现在返回连接成功,再次发送断开命令
                    MS_LOGER_INFO( "可见光对应窗口已不存在,返回连接成功,再次发送断开命令,组别名为:%s,码流为:%s", str_group_name.toStdString().data(), ( n_stream? "子码流":"主码流" ) );
                    dis_connect_device( n_group_number, DeviceVI, n_stream );
                } else {
                    if( n_stream == StreamSub ){
                        update_vi_sub_connect_status( n_group_number, CONNECTED, false );
                    }else if( n_stream == StreamMain ){
                        update_vi_main_connect_status( n_group_number, CONNECTED, false );
                    }
                    MS_LOGER_INFO( "可见光连接成功,组别名为:%s, 码流为:%s", str_group_name.toStdString().data(), ( n_stream? "子码流":"主码流" ) );
                    emit sig_connect_status( n_group_number, DeviceVI, true, n_stream );
                }
                remove_vi_reconnect_group( n_group_number, n_stream );
            }
        } else {
            if( n_type == DeviceIR ){   //红外:
                if( get_ir_connect_status( n_group_number ) == CONNECTING ){
                    MS_LOGER_INFO( "红外连接失败,组别名为:%s", str_group_name.toStdString().data() );
                    update_ir_connect_status( n_group_number, DISCONNECT, false );
                    put_ir_reconnect_group( n_group_number ); //放入要重连的列表中
                    emit sig_connect_status( n_group_number, DeviceIR, false, false );
                }
            } else if( n_type == DeviceVI ){  //可见光:
                if( get_vi_connect_status( n_group_number, n_stream ) == CONNECTING ){
                    if( n_stream == StreamSub ){
                        update_vi_sub_connect_status( n_group_number, DISCONNECT, false );
                    }else if( n_stream == StreamMain ){
                        update_vi_main_connect_status( n_group_number, DISCONNECT, false );
                    }
                    MS_LOGER_INFO( "可见光连接失败,组别名为:%s,码流为:%s", str_group_name.toStdString().data(), ( n_stream? "子码流":"主码流" ) );
                    put_vi_reconnect_group( n_group_number, n_stream);
                    emit sig_connect_status( n_group_number, DeviceVI, false, n_stream );
                }
            }
        }
    }
}


