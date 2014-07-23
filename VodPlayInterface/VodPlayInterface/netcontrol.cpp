#include "netcontrol.h"
#include <QtEndian>
#include <QDebug>
#include <QDateTime>
#include <QXmlInputSource>
#include <QMessageBox>
#include "vodplayinterface.h"

extern VodPlayInterface * vod_ir_play_window;

NetControl::NetControl( const char *s_server_ip, int i_server_port )
    : NetCapture( s_server_ip, i_server_port )
    , mn_seq_cmd( 0 )
    , mp_connect_playback( 0 )
    , mn_group_num_playback( -1 )
    , mb_connected_playback( false )
    , mb_pause_playback( true )
    , mn_session_playback( 0 )
{
    mp_recv_buf = NULL;
    mi_recv_len = 0;
    mi_latest_recv = 0;
}

NetControl::~NetControl()
{
    if( mp_recv_buf ) {
        delete []mp_recv_buf;
        mp_recv_buf = NULL;
    }
}

void NetControl::little_to_big( MsgHeader &head )
{
    head.msg_type = qToBigEndian ( head.msg_type );
    head.seq_id = qToBigEndian ( head.seq_id );
    head.total_len = qToBigEndian ( head.total_len );
}

void NetControl::big_to_little( MsgHeader &head )
{
    head.msg_type = qFromBigEndian ( head.msg_type );
    head.seq_id = qFromBigEndian ( head.seq_id );
    head.total_len = qFromBigEndian ( head.total_len );
}

int NetControl::send_add_group( int n_sn_num, QString str_sns )
{
    GroupCfgAddReq group_add;
    memset( &group_add, 0 , sizeof( GroupCfgAddReq ) );

    int n_cmd = ++mn_seq_cmd;
    group_add.num = n_sn_num;
    group_add.num = qToBigEndian( group_add.num );
    strcpy( group_add.members, str_sns.toLatin1().data() );

    int n_send_len = sizeof( group_add.header ) + sizeof( group_add.num ) + strlen( group_add.members );
    group_add.header.msg_type = IR_MONITOR_GROUP_CFG_ADD_REQ;
    group_add.header.seq_id = n_cmd;
    group_add.header.total_len = n_send_len;

    little_to_big( group_add.header );

    send_data( (char *)&group_add, n_send_len );

    m_map_group_ctrl_reqs.insert( n_cmd, -1 );
    return n_cmd;
}

int NetControl::send_group_add_req( const QList<QString>& devices )
{
    char s_members[1024];
    memset( s_members, 0, sizeof( s_members ) );
    int i_pos = 0;
    for( int i = 0; i < devices.size(); i++ ) {
        memcpy( s_members + i_pos, devices[i].toLatin1().data(), devices[i].length() );
        i_pos += devices[i].length();
        if( i != ( devices.size() - 1 ) ) {
            memcpy( s_members + i_pos, ";", 1 );
            i_pos += 1;
        }
    }

    GroupCfgAddReq group_add_req;
    group_add_req.header.msg_type = IR_MONITOR_GROUP_CFG_ADD_REQ;
    group_add_req.header.seq_id = ++mn_seq_cmd;
    group_add_req.header.total_len = sizeof( group_add_req.header ) + sizeof( group_add_req.num ) + strlen( s_members );
    int i_send_len = group_add_req.header.total_len;

    little_to_big( group_add_req.header );
    group_add_req.num = devices.size();
    group_add_req.num = qToBigEndian( group_add_req.num );
    strcpy( group_add_req.members, s_members );
    return send_data( (char *)&group_add_req, i_send_len );
}


int NetControl::send_del_group( int n_group )
{
    GroupCfgDelReq group_del;
    memset( &group_del, 0 , sizeof( GroupCfgDelReq ) );

    int n_cmd = ++mn_seq_cmd;
    group_del.header.msg_type = IR_MONITOR_GROUP_CFG_DEL_REQ;
    group_del.header.seq_id = n_cmd;
    group_del.header.total_len = sizeof( GroupCfgDelReq );

    little_to_big( group_del.header );

    group_del.id = n_group;
    group_del.id = qToBigEndian( group_del.id );

    send_data( (char *)&group_del, sizeof( GroupCfgDelReq ) );

    m_map_group_ctrl_reqs.insert( n_cmd, -1 );
    return n_cmd;
}

int NetControl::send_change_group( int n_group, int n_sn_num, QString str_sns )
{
    GroupCfgChgReq group_chg;
    memset( &group_chg, 0 , sizeof( GroupCfgChgReq ) );
    strcpy( group_chg.members, str_sns.toLatin1().data() );

    int n_cmd = ++mn_seq_cmd;
    group_chg.header.msg_type = IR_MONITOR_GROUP_CFG_CHG_REQ;
    group_chg.header.seq_id = n_cmd;


    int n_send_len = sizeof( group_chg.header ) + sizeof( group_chg.num ) + sizeof( group_chg.id ) + strlen( group_chg.members );
    group_chg.header.total_len = n_send_len;

    little_to_big( group_chg.header );

    group_chg.id = n_group;
    group_chg.id = qToBigEndian( group_chg.id );

    group_chg.num = n_sn_num;
    group_chg.num = qToBigEndian( group_chg.num );
    send_data( (char *)&group_chg, n_send_len );

    m_map_group_ctrl_reqs.insert( n_cmd, -1 );
    return n_cmd;
}

void NetControl::clear_group_ctrl_req()
{
    m_map_group_ctrl_reqs.clear();
}

void NetControl::get_complete_group_req( int &n_complete, int &n_failure )
{
    QMap< int, int >::iterator it = m_map_group_ctrl_reqs.begin();
    int n_complete_count(0);
    int n_failure_count(0);
    for ( ; it != m_map_group_ctrl_reqs.end(); ++it ) {
       if ( it.value() > 0 ) ++n_complete_count;
       if ( it.value() == 0 ) ++n_failure_count;
    }
    n_complete = n_complete_count;
    n_failure = n_failure_count;
}

bool NetControl::is_update_xml(int n_cmd)
{
    QMap<int, int>::iterator it = m_map_update_xml_reqs.find( n_cmd );
    return it == m_map_update_xml_reqs.end();
}

int NetControl::send_update_xml()
{
    XmlReq xml_req;
    int n_cmd = ++mn_seq_cmd;
    xml_req.header.msg_type = IR_MONITOR_XML_DATA_REQ;
    xml_req.header.seq_id = n_cmd;
    xml_req.header.total_len = sizeof(XmlReq);
    little_to_big( xml_req.header );
    xml_req.client_id = 0;
    //xml_req.client_id = qToBigEndian( xml_req.client_id );
    send_data( (char *)&xml_req, sizeof( xml_req ) );

    m_map_update_xml_reqs.insert( n_cmd, 0 );
    return n_cmd;
}

int NetControl::send_heart_tag()
{
    HeartBeatReq hb_req;
    hb_req.msg_type = IR_MONITOR_LINK_HEART_REQ;
    hb_req.seq_id = ++mn_seq_cmd;
    hb_req.total_len = sizeof( HeartBeatReq );
    little_to_big( hb_req );
    return send_data( (char *)&hb_req, sizeof( hb_req ) );
}

int NetControl::send_vod_play_req( const char *s_sn, const DeviceInfo &device_info, const int &n_group_num  )
{
    if( !s_sn || device_info.str_ip.isEmpty() ) {
        return -1;
    }


    VodPlayReq vod_play_req;
    memset( &vod_play_req, 0, sizeof(VodPlayReq) );
    strcpy( vod_play_req.device_sn, s_sn );

    vod_play_req.header.msg_type = IR_MONITOR_VID_DATA_PLAY_REQ;
    vod_play_req.header.seq_id = ++mn_seq_cmd;
    vod_play_req.header.total_len = sizeof( vod_play_req.header ) + strlen( vod_play_req.device_sn );

    int i_send_len = vod_play_req.header.total_len;
    little_to_big( vod_play_req.header );

    DeviceInfoSimple device_simple;
    device_simple.n_class = device_info.n_class;
    device_simple.n_group_num = n_group_num;
    device_simple.str_sn = device_info.str_sn;
    m_map_device_reqs.insert( mn_seq_cmd, device_simple );

    int n_send_len = send_data( (char *)&vod_play_req, i_send_len );

    return n_send_len;
}

int NetControl::send_vod_stop_req( const char *s_sn )
{
    VodStopReq vod_stop_req;
    vod_stop_req.header.msg_type = IR_MONITOR_VID_DATA_STOP_REQ;
    vod_stop_req.header.seq_id = ++mn_seq_cmd;
    vod_stop_req.header.total_len = sizeof( vod_stop_req.header ) + strlen( s_sn );
    int i_send_len = vod_stop_req.header.total_len;

    little_to_big( vod_stop_req.header );
    strcpy( vod_stop_req.device_sn, s_sn );
    return send_data( (char *)&vod_stop_req, i_send_len );
}

void NetControl::on_receive()
{
    mi_latest_recv = QDateTime::currentDateTime().toTime_t();

    NetCapture::on_receive();

    get_recv_buf();

    parse_recv();
}

int NetControl::parse_heartbeat( const MsgHeader &head )
{
    HeartBeatResp hb_resp;
    memcpy( &hb_resp, mp_recv_buf, head.total_len );
    big_to_little( hb_resp );
    return 0;
}

int NetControl::parse_xml( const MsgHeader &head )
{
    XmlResp xml_resp;
    memset( &xml_resp, 0, sizeof( xml_resp ) );
    memcpy( &xml_resp, mp_recv_buf, head.total_len );

    QDomDocument document;
    QString error;
    int row = 0, column = 0;
    //setContent：设置内容
    QString s_xml_input = xml_resp.xml_buf;

    clear_device_group();

    analysis_xml( s_xml_input );

    send_connect_group_device();

    QMap<int, int>::iterator it = m_map_update_xml_reqs.find( head.seq_id );
    if ( it != m_map_update_xml_reqs.end() ) {
        m_map_update_xml_reqs.erase( it );
    }
    return 0;

    QXmlInputSource xml_input;
    xml_input.setData( s_xml_input );
    if(!document.setContent( &xml_input, false, &error, &row, &column))
    {
        //解析文件失败
        qDebug() << xml_resp.xml_buf;
        return -1;
    }

    if(document.isNull())
    {
        qDebug() << xml_resp.xml_buf;
        return -1;
    }

    QDomElement root = document.documentElement();

    //root_tag_name为persons
    QString root_tag_name = root.tagName();
    if(root.hasAttribute("name"))
    {
        //name为Qt
        QString name = root.attributeNode("name").value();
    }

    //获取id="1"的节点
    QDomElement person = root.firstChildElement();
    if(person.isNull())
    {
        qDebug() << xml_resp.xml_buf;
        return -1;
    }

    QString person_tag_name = person.tagName();

    //id为1
    QString id = person.attributeNode("id").value();

    //获取子节点，数目为2
    QDomNodeList list = root.childNodes();
    int count = list.count();
    qDebug()<<"11111"<<count;

    //listview 列表
    QStringList s_list;

    for(int i=0; i<count;i++)
    {
        QDomNode dom_node = list.item(i);
        QDomElement element = dom_node.toElement();

        //获取id值，等价
        QString id_1 = element.attributeNode("id").value();
        QString id_2 = element.attribute("id");
        qDebug()<<id_1<<id_2;

        //获取子节点，数目为4，包括：name、age、email、website
        QDomNodeList child_list = element.childNodes();
        int child_count = child_list.count();
        qDebug()<<"22222"<<child_count;

        DeviceInfo device_info;
        for(int j=0; j<child_count;j++)
        {
            QDomNode child_dom_node = child_list.item(j);
            QDomElement child_element = child_dom_node.toElement();
            QString child_tag_name = child_element.tagName();
            QString child__tag_value = child_element.text();
            qDebug()<<child_tag_name<<child__tag_value;
            /*if(child_tag_name=="sn"){
                s_list.append(child__tag_value);
            }*/
            if( child_tag_name == "class" ) {
                device_info.n_class = child__tag_value.toInt();
            } else if( child_tag_name == "type" ) {
                device_info.str_type = child__tag_value;
            } else if( child_tag_name == "sn" ) {
                device_info.str_sn = child__tag_value;
            }else if( child_tag_name == "ip" ) {
                device_info.str_ip = child__tag_value;
            }else if( child_tag_name == "port" ) {
                device_info.n_port = child__tag_value.toInt();
            }else if( child_tag_name == "mac" ){
                device_info.str_mac = child__tag_value;
            }
        }

        //mo_devices.insert( device_info.str_sn, device_info );
        //if( device_info.n_class == DEVICE_VISIBLELIGHT ) {
            //send_vod_play_req( device_info.str_sn.toLatin1().data(), device_info );
           // qCritical( "%s",device_info.s_sn.toLatin1().data() );
        //}
    }

    return 0;
}

int NetControl::parse_vod_play( const MsgHeader &head )
{
    VodPlayResp vod_play_resp;
    memset( &vod_play_resp, 0, sizeof(VodPlayResp) );
    memcpy( &vod_play_resp, mp_recv_buf, head.total_len );

    QString s_url = vod_play_resp.url_buf;
    QStringList url_list = s_url.split( ':' );
    if( url_list.size() < 2 ) {
        qDebug() << s_url;
        return -1;
    }

    QMap< int, DeviceInfoSimple >::iterator it = m_map_device_reqs.find( head.seq_id );
    if( it == m_map_device_reqs.end() ) {
        qDebug() << vod_play_resp.url_buf;
        return -1;
    }
    int i_class = (it.value()).n_class;
    int i_group_num = it.value().n_group_num;
    QString str_sn = it.value().str_sn;
    m_map_device_reqs.erase( it );

    //if( i_class == DEVICE_VISIBLELIGHT ) {
        //for( int i = 0; i < 2; i++ ) {
            play_vod( i_group_num, i_class, url_list[0].toLatin1().data(), url_list[1].toInt(), str_sn );
        //}
    //}


    return 0;
}

void NetControl::play_vod( int i_no, int i_class, const char *s_ip, int i_port, QString str_sn )
{
    QMutexLocker lock_net( &m_mutex_device_net );

     QMap< int, NetCaptureGroup*>::iterator it_vods = m_map_connect_videos.find( i_no );
     if ( it_vods == m_map_connect_videos.end() ) {
        NetCaptureGroup *p_group = new NetCaptureGroup;
        if ( !p_group->connect_vod( i_class, s_ip, i_port, str_sn ) ) {
            delete p_group;
            QMessageBox::information( vod_ir_play_window, "ERROR", "failed to connect server!" );
            return;
        } else {
            m_map_connect_videos.insert( i_no, p_group );
        }

     } else {
        if ( !it_vods.value()->connect_vod( i_class, s_ip, i_port, str_sn ) ) {
            QMessageBox::information( vod_ir_play_window, "ERROR", "failed to connect server!" );
        }
     }

     return;

     /*
    qCritical( "no:%d,class:%d,ip:%s,port:%d", i_no, i_class, s_ip, i_port );
    NetCaptureVod *vod = new NetCaptureVod( i_no, i_class, s_ip, i_port );
    if( !vod->connect_server() ) {
        //vod->disconnect_server();
        delete vod;
        QMessageBox::information( vod_ir_play_window, "ERROR", "failed to connect server!" );
    } else {
        quint64 i_key = i_class;
        i_key = i_key << 32;
        i_key += i_no;
        mo_vod_mutex.lock();
        mo_vods.insert( i_key, vod );
        mo_vod_mutex.unlock();
        qDebug() << s_ip << ":" << i_port << "connected!";
        qCritical( "success,no:%d,class:%d,ip:%s,port:%d", i_no, i_class, s_ip, i_port );
    }*/
}

void NetControl::get_vods( QMap<quint64,NetCaptureVod*> &p_vods )
{
    QMutexLocker lock ( &m_mutex_device_net );

    for( QMap<quint64,NetCaptureVod*>::iterator it = mo_vods.begin(); it != mo_vods.end(); it++ ) {
        p_vods.insert( it.key(), it.value() );
    }
}

NetCaptureGroup *NetControl::get_group_connect( int n_index )
{
    QMutexLocker lock_net( &m_mutex_device_net );

    if ( n_index >= m_map_connect_videos.size() ) return NULL;

    QMap< int, NetCaptureGroup*>::iterator it_vods = m_map_connect_videos.begin();
    for ( int n =0; it_vods != m_map_connect_videos.end(); ++it_vods, ++n ) {
        if ( n== n_index ) return it_vods.value();
    }

    return NULL;
}


bool NetControl::get_group_info( int n_index, DeviceGroup &dev_group )
{
    QMutexLocker lock( &m_mutex_device_grp );

    if ( n_index >= m_map_device_group.size() ) return false;

    QMap< int, DeviceGroup >::iterator it_vods = m_map_device_group.begin();
    for ( int n =0; it_vods != m_map_device_group.end(); ++it_vods, ++n ) {
        if ( n== n_index ) {
            dev_group = it_vods.value();
            return true;
        }
    }

    return true;
}

int NetControl::parse_vod_stop( const MsgHeader &head )
{
    VodStopResp vod_stop_resp;
    memcpy( &vod_stop_resp, mp_recv_buf, head.total_len );

    return 0;
}

int NetControl::parse_group_cfg_add( const MsgHeader &head )
{
    GroupCfgAddResp group_cfg_add_resp;
    memset( &group_cfg_add_resp, 0, sizeof(GroupCfgAddResp) );
    memcpy( &group_cfg_add_resp, mp_recv_buf, head.total_len );

    group_cfg_add_resp.id = qFromBigEndian( group_cfg_add_resp.id );

    set_group_resp_map( head.seq_id
                        , group_cfg_add_resp.result == 1? group_cfg_add_resp.id : 0 );


    return 0;
}

int NetControl::parse_group_cfg_del( const MsgHeader &head )
{
    GroupCfgDelResp group_cfg_del_resp;
    memset( &group_cfg_del_resp, 0, sizeof(GroupCfgDelResp) );
    memcpy( &group_cfg_del_resp, mp_recv_buf, head.total_len );

    set_group_resp_map( head.seq_id, group_cfg_del_resp.result );

    return 0;
}

int NetControl::parse_group_cfg_chg( const MsgHeader &head )
{
    GroupCfgChgResp group_cfg_chg_resp;
    memset( &group_cfg_chg_resp, 0, sizeof(GroupCfgChgResp) );
    memcpy( &group_cfg_chg_resp, mp_recv_buf, head.total_len );

    set_group_resp_map( head.seq_id, group_cfg_chg_resp.result );

    return 0;
}

int NetControl::parse_playback(const MsgHeader &head)
{
    PlaybackControlResp playback_resp;
    memset( &playback_resp, 0, sizeof(playback_resp) );
    memcpy( &playback_resp, mp_recv_buf, head.total_len );

    int n_cmd_replay =  qFromBigEndian( playback_resp.n_ctrl_cmd );
    int n_result = qFromBigEndian( playback_resp.result );
    int n_session = qFromBigEndian( playback_resp.n_session );

    if ( n_result == 1 ) {
        switch ( n_cmd_replay )
        {
        case IR_MONITOR_REPLAY_SETUP_RESP:
        {
            int n_result = parse_playback_connect( playback_resp.members );
            m_map_playback_connect_reqs.insert( IR_MONITOR_REPLAY_SETUP
                                                , n_result!=1?0:1 );
            mn_session_playback = n_session;
            return 0;
        }
        case IR_MONITOR_REPLAY_PLAY_RESP:
            mb_connected_playback = true;
            mb_pause_playback = false;
            break;
        case IR_MONITOR_REPLAY_PAUSE_RESP:
            mb_pause_playback = true;
            break;
        case IR_MONITOR_REPLAY_STOP_RESP:
            mb_connected_playback = false;
            mb_pause_playback = true;
            mn_session_playback = 0;
            break;
        }
    }

    m_map_playback_connect_reqs.insert( n_cmd_replay - 0x1000, n_result );
    return 0;
}

int NetControl::parse_playback_connect(char *p_url)
{
    unsigned int n_url_num(0);
    memcpy( &n_url_num, p_url, 4 );p_url+=4;
    n_url_num = qFromBigEndian( n_url_num );
    if ( n_url_num > 9 ) { return 0; }

    int n_group_num(0);
    NetCaptureGroup *p_group(NULL);

    for( int n=0; n<n_url_num; ++n ) {
        unsigned int n_url_len(0);
        unsigned int n_sn_len(0);
        char sz_url[1024];
        char sz_sn[1024];
        memset( sz_url, 0, 1024 );
        memset( sz_sn, 0, 1024 );

        memcpy( &n_url_len, p_url, 4 ); p_url+=4;
        n_url_len = qFromBigEndian( n_url_len );
        memcpy( sz_url, p_url, n_url_len ); p_url+= n_url_len;

        memcpy( &n_sn_len, p_url, 4 ); p_url+=4;
        n_sn_len = qFromBigEndian( n_sn_len );
        memcpy( sz_sn, p_url, n_sn_len ); p_url+=n_sn_len;

        QString str_url( sz_url ), str_sn( sz_sn );
        QStringList lst_str = str_url.split( ':' );
        int n_class;

        if ( lst_str.size() < 2 ) { return 0; }
        if ( !get_device_group_info( str_sn, n_group_num, n_class ) ) {
            return 0;
        }

        if ( mp_connect_playback != NULL ) {
            if ( !mp_connect_playback->connect_vod( n_class
                    , str_url.split(':')[0].toLatin1().data(), str_url.split(':')[1].toInt(), str_sn ) ) {
                QMessageBox::information( vod_ir_play_window, "ERROR", "failed to connect server!" );
                return 0;
            }
        } else {
            p_group = new NetCaptureGroup;
            if ( !p_group->connect_vod( n_class
                                 , str_url.split(':')[0].toLatin1().data(), str_url.split(':')[1].toInt(), str_sn ) ) {
                QMessageBox::information( vod_ir_play_window, "ERROR", "failed to connect server!" );
                delete p_group;
                return 0;
            }
        }
    }

    if ( mp_connect_playback == NULL ) {
        mp_connect_playback = p_group;
    }
    mp_connect_playback->init_replay();
    mn_group_num_playback = n_group_num;
    return 1;
}

void NetControl::set_group_resp_map( const int &n_seq, const int & n_result )
{
    QMap< int, int >::iterator it = m_map_group_ctrl_reqs.find( n_seq );

    if ( it == m_map_group_ctrl_reqs.end() ) return;
    it.value() = n_result;
}

void NetControl::get_group_resp_map( const int & n_seq, int &n_result )
{
    QMap< int, int >::iterator it = m_map_group_ctrl_reqs.find( n_seq );

    if ( it == m_map_group_ctrl_reqs.end() ) return;
    n_result = it.value();
}

int NetControl::parse_recv()
{
    while( NULL != mp_recv_buf && mi_recv_len > sizeof( MsgHeader ) ) {

        MsgHeader head;
        memcpy( &head, mp_recv_buf, sizeof( head ) );
        big_to_little( head );

        if( mi_recv_len < head.total_len ) {
            return -1;
        }

        int i_ret = 0;
        switch ( head.msg_type ) {
        case IR_MONITOR_XML_DATA_RESP:
            i_ret = parse_xml( head ); break;
        case IR_MONITOR_LINK_HEART_RESP:
            i_ret = parse_heartbeat( head ); break;
        case IR_MONITOR_VID_DATA_PLAY_RESP:
            i_ret = parse_vod_play( head ); break;
        case IR_MONITOR_VID_DATA_STOP_RESP:
            i_ret = parse_vod_stop( head ); break;
        case IR_MONITOR_GROUP_CFG_ADD_RESP:
            i_ret = parse_group_cfg_add( head ); break;
        case IR_MONITOR_GROUP_CFG_DEL_RESP:
            i_ret = parse_group_cfg_del( head ); break;
        case IR_MONITOR_GROUP_CFG_CHG_RESP:
            i_ret = parse_group_cfg_chg( head ); break;
        case IR_MONITOR_REPLAY_DATA_RESP:
            i_ret = parse_playback( head ); break;
        default:
            i_ret = -1;
            break;
        }

        if( 0 == i_ret && mi_recv_len > head.total_len ) {
            int i_remain_len = mi_recv_len - head.total_len;
            char *remain_recv_buf = new char[i_remain_len];
            memcpy( remain_recv_buf, mp_recv_buf + head.total_len, i_remain_len );

            delete []mp_recv_buf;

            mp_recv_buf = remain_recv_buf;
            mi_recv_len = i_remain_len;
        } else {
            delete []mp_recv_buf;
            mp_recv_buf = NULL;
            mi_recv_len = 0;

            if( 0 != i_ret ) {
                disconnect_server();
                connect_server();
            }
        }
    }

    return 0;
}

void NetControl::get_recv_buf()
{
    int i_new_recv_len = 0;
    for( int i = 0; i < m_data_list.size(); i++ ) {
        i_new_recv_len += m_data_list[i].len;
    }

    if( i_new_recv_len > 0 ) {
        int i_total_recv_len = mi_recv_len + i_new_recv_len;
        char *total_recv_buf = new char[i_total_recv_len];
        int i_copy_len = 0;

        if( mi_recv_len > 0 ) {
            memcpy( total_recv_buf, mp_recv_buf, mi_recv_len );
            delete []mp_recv_buf;
            mp_recv_buf = NULL;
            i_copy_len = mi_recv_len;
            mi_recv_len = 0;
        }

        while( m_data_list.size() > 0 ) {
            NetworkData nd = m_data_list.front();
            memcpy( total_recv_buf + i_copy_len, nd.buf, nd.len );
            i_copy_len += nd.len;

            delete []nd.buf;
            m_data_list.pop_front();
        }

        mp_recv_buf = total_recv_buf;
        mi_recv_len = i_copy_len;
    }
}

void NetControl::clear_device_group()
{
    QMutexLocker lock ( &m_mutex_device_grp );

    m_list_device_ir_no_group.clear();
    m_list_device_vi_no_group.clear();
    m_map_device_group.clear();
}

bool NetControl::analysis_xml( QString str_xml )
{
    QDomDocument  dom_xml;

    QString str_error;
    int n_row = 0, n_column = 0;
    QXmlInputSource xml_input;
    xml_input.setData( str_xml );
    if( !dom_xml.setContent( &xml_input, false, &str_error, &n_row, &n_column) ) {
        return false;
    }

    QDomElement doc_elem = dom_xml.documentElement();

    QString str_tmp = doc_elem.tagName();
    if ( "dev_list" != str_tmp ) return false;

    QString str_change = doc_elem.attribute("change");
    //if ( mstr_xml_change == str_change ) { return false; }
    mstr_xml_change = str_change;

    bool b_root = doc_elem.hasChildNodes();
    if ( !b_root ) { return false; }

    QDomNodeList doc_node_list = doc_elem.childNodes();
    analysis_xml_device( doc_node_list );

    return true;
}

void NetControl::analysis_xml_device( QDomNodeList& doc_node_list, int n_group_num)
{
    if ( doc_node_list.isEmpty() ) return;

    DeviceGroup device_group;
    for ( int n = 0; n< doc_node_list.count(); ++n ){
        QDomNode  doc_node = doc_node_list.at( n );
        QString str_node_name = doc_node.toElement().tagName();
        if ( doc_node.isNull() ||
             ( str_node_name != "dev_info" && str_node_name != "group" ) )
        {
            continue;
        }

        if ( str_node_name == "group" ) {
            int n_group_num = doc_node.toElement().attribute("num").toInt();
            QDomNodeList doc_dev_list = doc_node.childNodes();
            analysis_xml_device( doc_dev_list, n_group_num );
        } else {
            DeviceInfo device_info;

            QDomNode doc_node_child =  doc_node.firstChild();
            while ( !doc_node_child.isNull() ) {
                QString str_info_name = doc_node_child.toElement().tagName();
                QString str_info_value = doc_node_child.toElement().text();

                if( str_info_name == "class" ) {
                    device_info.n_class = str_info_value.toInt();
                } else if( str_info_name == "type" ) {
                    device_info.str_type = str_info_value;
                } else if( str_info_name == "sn" ) {
                    device_info.str_sn = str_info_value;
                }else if( str_info_name == "ip" ) {
                    device_info.str_ip = str_info_value;
                }else if( str_info_name == "port" ) {
                    device_info.n_port = str_info_value.toInt();
                }else if( str_info_name == "mac" ){
                    device_info.str_mac = str_info_value;
                } else {}

                qDebug() << str_info_name + ": " +  str_info_value;

                doc_node_child = doc_node_child.nextSibling();
            }

            if ( n_group_num > 0 ) {  //.分组
                if ( device_info.n_class == 0 ) {       //.可见光
                    device_group.dev_vi = device_info;
                } else {
                    device_group.dev_ir = device_info;
                }
            } else {
                QMutexLocker lock ( &m_mutex_device_grp );
                if ( device_info.n_class == 0 ) {       //.可见光
                    m_list_device_vi_no_group.push_back( device_info );
                } else {
                    m_list_device_ir_no_group.push_back( device_info );
                }
            }
        }
    }

    if ( n_group_num > 0 ) {  //.分组
        QMutexLocker lock ( &m_mutex_device_grp );
        m_map_device_group.insert( n_group_num, device_group );
    }
}

void NetControl::send_connect_group_device()
{
    QMutexLocker lock ( &m_mutex_device_grp );
    QMutexLocker lock_net( &m_mutex_device_net );

    QMap< int, DeviceGroup >::iterator it = m_map_device_group.begin();
    for ( ; it != m_map_device_group.end(); ++it ) {
        DeviceGroup &device_group = it.value();

        QMap< int, NetCaptureGroup*>::iterator it_vods = m_map_connect_videos.find( it.key() );
        if ( it_vods == m_map_connect_videos.end() ) {
            if ( !device_group.dev_ir.str_sn.isEmpty() ) {
                send_vod_play_req( device_group.dev_ir.str_sn.toLatin1().data(), device_group.dev_ir, it.key() );
            }
            if ( !device_group.dev_vi.str_sn.isEmpty() ) {
                send_vod_play_req( device_group.dev_vi.str_sn.toLatin1().data(), device_group.dev_vi, it.key() );
            }
        } else {
            NetCaptureGroup *p_group = it_vods.value();
            if ( p_group->get_net_ir_sn() != device_group.dev_ir.str_sn ) {
                p_group->disconnect_ir();
                if ( !device_group.dev_ir.str_sn.isEmpty() ) {
                    send_vod_play_req( device_group.dev_ir.str_sn.toLatin1().data(), device_group.dev_ir, it.key() );
                }
            }

            if ( p_group->get_net_vi_sn() != device_group.dev_vi.str_sn ) {
                p_group->disconnect_vi();

                if ( !device_group.dev_vi.str_sn.isEmpty() ) {
                    send_vod_play_req( device_group.dev_vi.str_sn.toLatin1().data(), device_group.dev_vi, it.key() );
                }
            }
        }
    }

    QMap< int, NetCaptureGroup*>::iterator it_vods = m_map_connect_videos.begin();
    for ( ; it_vods != m_map_connect_videos.end();  ) {
        QMap< int, DeviceGroup >::iterator it_group = m_map_device_group.find( it_vods.key() );
        if ( it_group ==m_map_device_group.end() ) {
            NetCaptureGroup *p_group = it_vods.value();
            p_group->disconnect_ir();
            p_group->disconnect_vi();
            delete p_group;
            it_vods = m_map_connect_videos.erase( it_vods );
        } else {
            ++it_vods;
        }
    }
}

void NetControl::get_group_device( QMap< int, DeviceGroup > &map_group  )
{
    QMutexLocker lock ( &m_mutex_device_grp );

    QMap< int, DeviceGroup >::iterator it = m_map_device_group.begin();
    for ( ; it != m_map_device_group.end(); ++it ) {
        map_group.insert( it.key(), it.value() );
    }
}

void NetControl::get_no_group_ir_device( QList< DeviceInfo > &list_ir )
{
    QMutexLocker lock ( &m_mutex_device_grp );

    QList< DeviceInfo >::iterator it = m_list_device_ir_no_group.begin();
    for ( ; it != m_list_device_ir_no_group.end(); ++it ) {
        list_ir.push_back( *it );
    }
}

void NetControl::get_no_group_vi_device( QList< DeviceInfo > &list_vi )
{
    QMutexLocker lock ( &m_mutex_device_grp );

    QList< DeviceInfo >::iterator it = m_list_device_vi_no_group.begin();
    for ( ; it != m_list_device_vi_no_group.end(); ++it ) {
        list_vi.push_back( *it );
    }
}

bool NetControl::get_device_group_info(const QString &str_sn, int &n_group_num, int &n_class)
{
    QMutexLocker lock( &m_mutex_device_grp );

    QMap< int, DeviceGroup >::iterator it = m_map_device_group.begin();
    for ( ; it != m_map_device_group.end(); ++it ) {
        if ( it.value().dev_ir.str_sn == str_sn ) {
            n_group_num = it.key();
            n_class = 1;
            return true;
        }

        if ( it.value().dev_vi.str_sn == str_sn ) {
            n_group_num = it.key();
            n_class = 0;
            return true;
        }
    }

    return false;
}

void NetControl::clear_playback_connect_req()
{
    m_map_playback_connect_reqs.clear();
}

void NetControl::get_playback_complete_connect_req( const int &n_seq_cmd, int &n_result )
{
    QMap< int, int >::iterator it = m_map_playback_connect_reqs.find( n_seq_cmd );
    if ( it == m_map_playback_connect_reqs.end() )  return;

    n_result = it.value();
    return;
}

int NetControl::send_playback_connect( int n_group_num )
{
    return send_playback_cmd( n_group_num, IR_MONITOR_REPLAY_SETUP );
}

int NetControl::send_playback_play( int n_group_num, int n_speed, QString str_time_min, QString str_time_max )
{
    int n_cmd = ++mn_seq_cmd;
    PlaybackControlReq playback_play_req;

    playback_play_req.header.msg_type = IR_MONITOR_REPLAY_DATA_REQ;
    playback_play_req.header.seq_id = n_cmd;
    playback_play_req.header.total_len = sizeof( MsgHeader ) + (str_time_min.isEmpty()?16:44);

    playback_play_req.n_group_num = qToBigEndian( n_group_num );
    int n_replay_cmd = IR_MONITOR_REPLAY_PLAY;
    playback_play_req.n_ctrl_cmd = qToBigEndian ( n_replay_cmd );
    playback_play_req.n_play_speed = qToBigEndian ( n_speed );
    playback_play_req.n_session = qToBigEndian( mn_session_playback );

    int n_send_len = playback_play_req.header.total_len;

    little_to_big( playback_play_req.header );

    if ( !str_time_min.isEmpty() ) {
        memcpy( playback_play_req.sz_time, str_time_min.toStdString().c_str(), 14 );
        memcpy( playback_play_req.sz_time+14, str_time_max.toStdString().c_str(), 14 );
    }

    m_map_playback_connect_reqs.insert( IR_MONITOR_REPLAY_PLAY, -1 );

    send_data( ( char *)&playback_play_req, n_send_len );
    return n_replay_cmd;
}

int NetControl::send_playback_pause( int n_group_num )
{
    return send_playback_cmd( n_group_num, IR_MONITOR_REPLAY_PAUSE, mn_session_playback );
}

int NetControl::send_playback_disconnect( int n_group_num )
{
    if ( n_group_num <= 0 ) return 0;
    if ( mp_connect_playback ) {
        mp_connect_playback->disconnect_ir();
        mp_connect_playback->disconnect_vi();
        mn_group_num_playback = 0;
    }

    return send_playback_cmd( n_group_num, IR_MONITOR_REPLAY_STOP, mn_session_playback );
}

int NetControl::send_playback_cmd( int n_group_num, int n_replay_cmd, int n_session )
{
    int n_cmd = ++mn_seq_cmd;
    PlaybackControlReq playback_cmd_req;

    playback_cmd_req.header.msg_type = IR_MONITOR_REPLAY_DATA_REQ;
    playback_cmd_req.header.seq_id = n_cmd;
    playback_cmd_req.header.total_len = sizeof( MsgHeader ) + 8 + ( n_session==0?0:4 );

    playback_cmd_req.n_group_num = qToBigEndian( n_group_num );
    playback_cmd_req.n_ctrl_cmd = qToBigEndian( n_replay_cmd );
    playback_cmd_req.n_session = qToBigEndian( n_session );

    int n_send_len = playback_cmd_req.header.total_len;

    little_to_big( playback_cmd_req.header );
    m_map_playback_connect_reqs.insert( n_replay_cmd, -1 );

    send_data( ( char * )&playback_cmd_req, n_send_len );
    return n_replay_cmd;
}
