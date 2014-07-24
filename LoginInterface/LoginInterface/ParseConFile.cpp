#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QMap>
#include <QCoreApplication>
#include <QTextCodec>

#define SDTP_VER "SDTP_VER"
#define SDTP_SUB_VER "SDTP_SUB_VER"
#define SDTP_SRV_PORT "SDTP_SRV_PORT"
#define SDTP_SRV_LISTEN_MAX "SDTP_SRV_LISTEN_MAX" /* 最大监听数 */
#define SDTP_MAX_ERR_MSG "SDTP_MAX_ERR_MSG" /* 最大错误包数，接收到这么多的错误包就说明SDTP处理程序有问题了，该监听接口断开 */
#define SDTP_NO_MSG_DISCONN_TIME "SDTP_NO_MSG_DISCONN_TIME" /* 客户端不发包后的超时时间,单位为秒 */
#define SDTP_USER_NAME "SDTP_USER_NAME" /* SDTP用户名 */
#define SDTP_USER_PASSWD "SDTP_USER_PASSWD" /* SDTP用户名 */

/*
 * MONITOR_VER = 1
MONITOR_SUB_VER = 0
MONITOR_USER_NAME = user
MONITOR_USER_PASSWD = 123456

###################### SERVER INFO ########################
SERVER_HOST_IP = 192.168.0.200
SERVER_PORT = 5555
*/
static QMap<QString, QString> m_map_config;
QString ms_ver;
QString ms_sub_ver;
QString ms_user_name;
QString ms_user_pwd;
QString ms_host_ip;
QString ms_server_port;

bool isValidCfgLine( const char* src )
{
    qDebug()<<"isValidCfgLine"<<src[0];
    if( src[0] != '#' || src[0] != ' ' ) {
        return true;
    }
    return false;
}

bool parser_cfg( QString str, char*& key, char*& value )
{
    const char * p_str = str.toLatin1().data();
    bool is_key = true;
    char* p_key = key;
    char* p_value = value;
    *p_key = '\0';
    *p_value = '\0';

    if ( isValidCfgLine( str.toLatin1().data() ) ) {//src为有效的  ??  src开头不为#
        while ( '\0' != *p_str ) {
            if ( ' ' == *p_str || '\t' == *p_str || '\r' == *p_str || '\n' == *p_str ) {
                p_str++;
                continue;
            } else if ( '=' == *p_str ) {
                p_str++;
                is_key = false;
                *p_key++ = '\0';
                continue;
            }

            if ( is_key ) {
                *p_key++ = *p_str;//key
            } else {
                *p_value++ = *p_str;//value
            }
            p_str++;
        }
        *p_value++ = '\0';
        return true;
    } else {
        return false;
    }
    return true;
}

int load_cfg()
{
    QString strfile;
    strfile = QCoreApplication::applicationDirPath();

    char buff[1024];
    char key_buff[512];
    char value_buff[1024];
    QString str_file_name = strfile+"/../../LoginInterface/client.cfg";
    qDebug()<<"load_cfg:"<<str_file_name;

    const char *p_buff = buff;
    char* p_key = key_buff;
    char* p_value = value_buff;
    int iValue = 0;
    QString sValue = "";



    //LoggerPtr root_logger = Logger::getRootLogger();//日志
    QFile file( str_file_name );
    if(!file.open(QIODevice::ReadWrite | QIODevice::Text)) {
        //LOG4CXX_ERROR(root_logger, "Unable to open " << str_file_name << " for read");
        return -1;
    }

    QTextStream stream( &file );
    QTextCodec *codec = QTextCodec::codecForName( "UTF8" );
    stream.setCodec( codec );
    QString str_buff;
    do {
        str_buff.clear();
        memset( buff, 0, 1024 );
        str_buff = stream.readLine(  );
        p_buff = (char*)str_buff.toStdString().data();
        qDebug()<<"dfdkjfl dfsd f"<<p_buff<<str_buff<<p_buff[0];
        /*if ( parser_cfg( str_buff, p_key, p_value ) ) {
            m_map_config.insert( QString( QLatin1String( key_buff ) ) , QString( QLatin1String( value_buff ) ) );
            qDebug()<<"parser_cfg"<<QString( QLatin1String( key_buff ) ) << QString( QLatin1String( value_buff ) )<<","<<key_buff<<","<<value_buff;
        }*/

    } while (!stream.atEnd());

    QMap<QString, QString>::iterator itr = m_map_config.begin();
    for (; itr != m_map_config.end(); itr++) {
        //LOG4CXX_INFO(root_logger, "prop's(" << i++ << ") key=" << itr->first.c_str()
        //<< " ,value = " << itr->second.c_str());
        qDebug()<<"for"<<itr.key()<<itr.value();
    }

    /********************** MONITOR INFO ********************/
    itr = m_map_config.find( "MONITOR_VER" );
    if(itr != m_map_config.end()){
        ms_ver = itr.value();
    }else{
       // LOG4CXX_ERROR(root_logger, "Can't find MONITOR_PORT in app.cfg !");
        return -1;
    }

    itr = m_map_config.find( "MONITOR_SUB_VER" );
    if( itr != m_map_config.end() ) {
        ms_sub_ver = itr.value();
    }else{
        //LOG4CXX_ERROR(root_logger, "Can't find MONITOR_HOST_IP in app.cfg !");
        return -1;
    }

    itr = m_map_config.find( "MONITOR_USER_NAME" );
    if(itr != m_map_config.end()){
        ms_user_name = itr.value();
    }else{
       // LOG4CXX_ERROR(root_logger, "Can't find MONITOR_PORT in app.cfg !");
        return -1;
    }

    itr = m_map_config.find( "MONITOR_USER_PASSWD" );
    if( itr != m_map_config.end() ) {
        ms_user_pwd = itr.value();
    }else{
        //LOG4CXX_ERROR(root_logger, "Can't find MONITOR_HOST_IP in app.cfg !");
        return -1;
    }


    /************************ SERVER INFO *************************/
    itr = m_map_config.find( "SERVER_HOST_IP" );
    if( itr != m_map_config.end() ){
        ms_host_ip = itr.value();
    }else{
        //LOG4CXX_ERROR(root_logger, "Can't find IS_DEBUG in app.cfg !");
        return -1;
    }

    itr = m_map_config.find( "SERVER_PORT" );
    if(itr != m_map_config.end()){
        ms_server_port = itr.value();
    }else{
        //LOG4CXX_ERROR(root_logger, "Can't find "SDTP_VER" in app.cfg !");
        return -1;
    }

    qDebug()<<"所有"<<ms_ver<<ms_sub_ver<<ms_user_name<<ms_user_pwd<<ms_host_ip<<ms_server_port;

    return 0;
}

QString get_ver()
{
    return ms_ver;
}

QString get_sub_ver()
{
    return ms_sub_ver;
}

QString get_user_name()
{
    return ms_user_name;
}

QString get_user_pwd()
{
    return ms_user_pwd;
}

QString get_host_ip()
{
    return ms_host_ip;
}

QString get_server_port()
{
    return ms_server_port;
}



void set_ver( QString str_ver )
{
    ms_ver = str_ver;
}


void set_sub_ver( QString str_sub_ver )
{
    ms_sub_ver = str_sub_ver;
}


void set_user_name( QString str_user_name )
{
    ms_user_name = str_user_name;
}


void set_user_pwd( QString str_user_pwd )
{
    ms_user_pwd = str_user_pwd;
}


void set_host_ip( QString str_host_ip )
{
    ms_host_ip = str_host_ip;
}


void set_server_port( QString str_server_port )
{
    ms_server_port = str_server_port;
}

void save_login_info()
{
    QMap<QString, QString>::iterator itr = m_map_config.begin();
    itr = m_map_config.find( "MONITOR_VER" );
    if( itr != m_map_config.end() ) {
        //(*itr).set
    }
}
