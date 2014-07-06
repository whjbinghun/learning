#include <qlogging.h>
#include <string.h>
#include <arpa/inet.h>
#include "control-protocol.h"
#include "data-trans.h"

Controller::Controller( int i_port )
{
    mb_init = false;
    mi_port = i_port;
}

Controller::~Controller()
{
}

int Controller::init()
{
    int i_ret = DataTrans::instance()->start_listen( mi_port );
    if( i_ret < 0 ) {
        qDebug( "start_listen error:%d", i_ret );
        return i_ret;
    }

    i_ret = pthread_attr_init( &mo_thread_attr );
    //i_ret = pthread_create( &mo_thread_id, &mo_thread_attr, &handle_epoll_event, &mi_ep_fd );

    mb_init = true;

    return 0;
}

int Controller::parse_data( char *data, int len )
{
    if( len < sizeof(MsgHeader) ) {
        return -1;
    }

    size_t pos = 0;
    unsigned int msg_type;
    memcpy( &msg_type, data + pos, sizeof(msg_type) );
    msg_type = ntohl( msg_type );
    pos += sizeof( msg_type );

    CtrlReq ctrl_req;
    switch( msg_type ) {
    case IR_MONITOR_XML_DATA_REQ:
        ctrl_req.req_type = IR_MONITOR_XML_DATA_REQ;
        parse_xml_req( data, len, ctrl_req.req.xmlreq );
        break;
    case IR_MONITOR_LINK_HEART_REQ:
        ctrl_req.req_type = IR_MONITOR_LINK_HEART_REQ;
        parse_heart_req( data, len, ctrl_req.req.heartbeatreq );
        break;
    case IR_MONITOR_VID_DATA_PLAY_REQ:
        ctrl_req.req_type = IR_MONITOR_VID_DATA_PLAY_REQ;
        parse_vod_play_req( data, len, ctrl_req.req.vodplayreq );
        break;
    case IR_MONITOR_VID_DATA_STOP_REQ:
        ctrl_req.req_type = IR_MONITOR_VID_DATA_STOP_REQ;
        parse_vod_stop_req( data, len, ctrl_req.req.vodstopreq );
        break;
    default:
        break;
    }

    return 0;
}

int Controller::parse_xml_req( char *data, int len, XmlReq &xml_req)
{
    if( len < sizeof( XmlReq ) )
    {
        return -1;
    }

    size_t pos = sizeof( xml_req.header.msg_type );

    xml_req.header.msg_type = IR_MONITOR_XML_DATA_REQ;
    memcpy( &xml_req.header.total_len, data + pos, sizeof(xml_req.header.total_len ) );
    xml_req.header.total_len = ntohl( xml_req.header.total_len );
    if( xml_req.header.total_len != sizeof(XmlReq) ) {
        qDebug( "error xml req total len:%d", xml_req.header.total_len );
    }
    pos += sizeof( xml_req.header.total_len );
    memcpy( &xml_req.header.seq_id, data + pos, sizeof(xml_req.header.total_len ) );
    xml_req.header.seq_id = ntohl( xml_req.header.seq_id );
    pos += sizeof( xml_req.header.seq_id );
    memcpy( &xml_req.client_id, data + pos, sizeof( xml_req.client_id ) );
    xml_req.client_id = ntohl( xml_req.client_id );
    pos += sizeof( xml_req.client_id );

    return 0;
}

int Controller::pack_xml_resp( XmlReq xml_req )
{
    XmlResp xml_resp;
    xml_resp.header.msg_type = IR_MONITOR_XML_DATA_RESP;
    xml_resp.header.seq_id = xml_req.header.seq_id;
    xml_resp.result = 1;
    xml_resp.xml_str = "xxx";
    xml_resp.header.total_len = sizeof( MsgHeader ) + sizeof( xml_resp.result ) + xml_resp.xml_str.length();

    return 0;
}

int Controller::parse_heart_req( char *data, int len, HeartBeatReq &heartbeat_req )
{
    if( len < sizeof( HeartBeatReq ) )
    {
        return -1;
    }

    size_t pos = sizeof( heartbeat_req.msg_type );

    heartbeat_req.msg_type = IR_MONITOR_LINK_HEART_REQ;
    memcpy( &heartbeat_req.total_len, data + pos, sizeof( heartbeat_req.total_len ) );
    if( heartbeat_req.total_len != sizeof( HeartBeatReq ) ) {
        qDebug( "error heartbeat req total len:%d", heartbeat_req.total_len );
    }
    pos += sizeof( heartbeat_req.total_len );
    memcpy( &heartbeat_req.seq_id, data + pos, sizeof(heartbeat_req.total_len ) );
    pos += sizeof( heartbeat_req.seq_id );

    return 0;
}

int Controller::pack_heart_resp( HeartBeatReq heartbeat_req )
{
    HeartBeatResp Heartbeat_resp;
    Heartbeat_resp.msg_type = IR_MONITOR_LINK_HEART_RESP;
    Heartbeat_resp.seq_id = heartbeat_req.seq_id;
    Heartbeat_resp.total_len = sizeof( HeartBeatResp );

    return 0;
}

int Controller::parse_vod_play_req( char *data, int len, VodPlayReq &vod_play_req )
{
    if( len < sizeof( VodPlayReq ) )
    {
        return -1;
    }

    size_t pos = sizeof( vod_play_req.header.msg_type );

    vod_play_req.header.msg_type = IR_MONITOR_VID_DATA_PLAY_REQ;
    memcpy( &vod_play_req.header.total_len, data + pos, sizeof(vod_play_req.header.total_len ) );
    if( vod_play_req.header.total_len != sizeof(VodPlayReq) ) {
        qDebug( "error vod play req total len:%d", vod_play_req.header.total_len );
    }
    pos += sizeof( vod_play_req.header.total_len );
    memcpy( &vod_play_req.header.seq_id, data + pos, sizeof(vod_play_req.header.total_len ) );
    pos += sizeof( vod_play_req.header.seq_id );
    memcpy( &vod_play_req.client_id, data + pos, sizeof( vod_play_req.client_id ) );
    pos += sizeof( vod_play_req.client_id );

    return 0;
}

int Controller::pack_vod_play_resp( VodPlayReq vod_play_req )
{
    VodPlayResp vod_play_resp;
    vod_play_resp.header.msg_type = IR_MONITOR_VID_DATA_PLAY_RESP;
    vod_play_resp.header.seq_id = vod_play_req.header.seq_id;
    vod_play_resp.result = 1;
    vod_play_resp.url_str = "xxx";
    vod_play_resp.header.total_len = sizeof( MsgHeader ) + sizeof( vod_play_resp.result ) + vod_play_resp.url_str.length();

    return 0;
}

int Controller::parse_vod_stop_req( char *data, int len, VodStopReq &vod_stop_req)
{
    if( len < sizeof( VodStopReq ) )
    {
        return -1;
    }

    size_t pos = sizeof( vod_stop_req.header.msg_type );

    vod_stop_req.header.msg_type = IR_MONITOR_VID_DATA_STOP_REQ;
    memcpy( &vod_stop_req.header.total_len, data + pos, sizeof(vod_stop_req.header.total_len ) );
    if( vod_stop_req.header.total_len != sizeof(VodStopReq) ) {
        qDebug( "error vod stop req total len:%d", vod_stop_req.header.total_len );
    }
    pos += sizeof( vod_stop_req.header.total_len );
    memcpy( &vod_stop_req.header.seq_id, data + pos, sizeof(vod_stop_req.header.total_len ) );
    pos += sizeof( vod_stop_req.header.seq_id );
    memcpy( &vod_stop_req.client_id, data + pos, sizeof( vod_stop_req.client_id ) );
    pos += sizeof( vod_stop_req.client_id );

    return 0;
}

int Controller::pack_vod_stop_resp( VodStopReq vod_play_req )
{
    VodStopResp vod_stop_resp;
    vod_stop_resp.header.msg_type = IR_MONITOR_VID_DATA_STOP_RESP;
    vod_stop_resp.header.seq_id = vod_play_req.header.seq_id;
    vod_stop_resp.result = 1;
    vod_stop_resp.header.total_len = sizeof( MsgHeader ) + sizeof( vod_stop_resp.result );

    return 0;
}
