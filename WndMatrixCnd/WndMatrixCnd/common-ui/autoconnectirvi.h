#ifndef AUTOCONNECTIRVI_H
#define AUTOCONNECTIRVI_H

#include "cloginview.h"
#include "cmainview.h"
#include "datamode.h"
#include <QTimer>
#include <QWidget>
#include <map>
#include <list>

using namespace std;

typedef enum{
    NO_STATUS,
    CONNECTING, //正在连接
    CONNECTED,  //已连接上
    DISCONNECT, //断开
}IR_VI_CONNECT_STATUS_E; //红外,可见光工作状态


typedef struct {
    IR_VI_CONNECT_STATUS_E connect_status;
    bool b_self_disconnect;  //是否为主动断开
} IR_VI_STATUS_S;

typedef struct{
    int n_group_num;
    int device_type;
}VI_RECONNECT_S;

class AutoConnectIrVi : public QObject, public CMainView
{
    Q_OBJECT
public:
    AutoConnectIrVi();
    ~AutoConnectIrVi();
    void connect_device(int n_group_num, DeviceType device_type, bool b_sub_stream = false );
    //红外,可见光连接状态初始化
    void init_connect_status_map( int n_group_num, IR_VI_CONNECT_STATUS_E connect_status, bool b_self_disconnect,
                                                   DeviceType device_type, StreamType stream_type );
    void dis_connect_device(int n_group_num, DeviceType device_type, bool b_sub_stream,
                            IR_VI_CONNECT_STATUS_E ir_connect_status = NO_STATUS, bool b_self_disconnect = true );
    void update_ir_connect_status( int n_group_num, bool b_wnd_exist );
    void update_ir_connect_status( int n_group_num, IR_VI_CONNECT_STATUS_E ir_connect_status, bool b_self_disconnect );
    void update_vi_sub_connect_status( int n_group_num, bool b_wnd_exist );
    void update_vi_sub_connect_status( int n_group_num, IR_VI_CONNECT_STATUS_E ir_connect_status, bool b_self_disconnect );
    void update_vi_main_connect_status( int n_group_num, bool b_wnd_exist );
    void update_vi_main_connect_status( int n_group_num, IR_VI_CONNECT_STATUS_E ir_connect_status, bool b_self_disconnect );
    void put_ir_reconnect_group( int n_group_num );
    void remove_ir_reconnect_group(int n_group_num );
    void put_vi_reconnect_group(int n_group_num, int type_stream );
    void remove_vi_reconnect_group( int n_group_num, int n_stream );
    void clear_all_reconnect_list();
    void onNotify( CBaseMsg& msg , int n_group_num );
    void update_group_connect_status( int n_group_num, bool b_self_connect );
    bool get_group_connect_status( int n_group_num );
    QString get_group_str_name(int n_group_num );
    int onResponse( CBaseMsg& reqMsg,CBaseMsg& rspMsg );
private:
    void start_ir_timer();
    void stop_ir_timer();
    void start_vi_timer();
    void stop_vi_timer();
    IR_VI_CONNECT_STATUS_E get_ir_connect_status( int n_group_num );//返回红外连接状态
    IR_VI_CONNECT_STATUS_E get_vi_connect_status( int n_group_num, int n_stream ); //返回可见光连接状态
    bool get_ir_self_disconnect( int n_group_num ); //返回红外是否主动断开
    bool get_vi_self_disconnect(int n_group_num, int stream_type ); //返回可见光是否主动断开
private:
    QTimer *mp_ir_timer;
    QTimer *mp_vi_timer;
    int mn_ir_time_id;
    int mn_vi_time_id;

    map< int, IR_VI_STATUS_S > m_ir_connect_status_map; //红外连接状态
    map< int, IR_VI_STATUS_S > m_vi_sub_connect_status_map; //可见光子码流连接状态
    map< int, IR_VI_STATUS_S > m_vi_main_connect_status_map;//可见光主码流连接状态
    list<int> m_ir_reconnect_list;  //红外重连列表
    list<VI_RECONNECT_S> m_vi_reconnect_list; //可见光重连列表
    map< int, bool > m_group_connect_map; //用于存放主动去连接还是断线之后再去连接红外
signals:
    void sig_connect_status( int n_group_num, DeviceType device_type, bool b_connected, int type_stream, bool b_self_connect = false );
    void sig_vi_disconnect_status( int n_group_number, bool b_sub_stream );
private slots:
    void slot_ir_connect_timer();
    void slot_vi_connect_timer();
};

#endif // AUTOCONNECTIRVI_H
