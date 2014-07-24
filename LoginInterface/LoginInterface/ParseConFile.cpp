#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QMap>
#include <QCoreApplication>
#include <QTextCodec>

//#include<string.h>
//#include<stdio.h>

static QMap<QString, QString> m_map_config;
QString ms_ver;
QString ms_sub_ver;
QString ms_user_name;
QString ms_user_pwd;
QString ms_host_ip;
QString ms_server_port;

bool is_valid_cfg_line( const char* src )
{
    if( src[0] != '#' && src[0] != ' '  ) {
        return true;
    }
    return false;
}

bool parser_cfg( const char* str, char*& key, char*& value )
{
    const char * p_str = str;
    bool is_key = true;
    char* p_key = key;
    char* p_value = value;
    *p_key = '\0';
    *p_value = '\0';

    if ( is_valid_cfg_line( str ) ) {//src为有效的  ??  src开头不为#
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
    QString str_file_name = strfile+"/client.cfg";
    qDebug()<<"load_cfg:"<<str_file_name;

    char *p_buff = buff;
    char* p_key = key_buff;
    char* p_value = value_buff;

    QFile file( str_file_name );
    if(!file.open(QIODevice::ReadWrite | QIODevice::Text)) {
        return -1;
    }

    QTextStream stream( &file );
    QTextCodec *codec = QTextCodec::codecForName( "UTF8" );
    stream.setCodec( codec );
    QString str_buff;
    QByteArray ba;

    do {
        memset( buff, 0, 1024 );
        str_buff = stream.readLine( 1024 );
        int len = str_buff.size();
        ba = str_buff.toLatin1();
        p_buff = ba.data();
        if ( parser_cfg( p_buff, p_key, p_value ) ) {
            m_map_config.insert( QString( QLatin1String( key_buff ) ) , QString( QLatin1String( value_buff ) ) );
            //qDebug()<<"parser_cfg"<<QString( QLatin1String( key_buff ) ) << QString( QLatin1String( value_buff ) )<<","<<key_buff<<","<<value_buff;
        }

    } while (!stream.atEnd());

    QMap<QString, QString>::iterator itr = m_map_config.begin();
    for ( ; itr != m_map_config.end(); itr++ ) {
        //qDebug()<<"for"<<itr.key()<<itr.value();
    }

    /********************** MONITOR INFO ********************/
    itr = m_map_config.find( "MONITOR_VER" );
    if(itr != m_map_config.end()){
        ms_ver = itr.value();
    }else{
        return -1;
    }

    itr = m_map_config.find( "MONITOR_SUB_VER" );
    if( itr != m_map_config.end() ) {
        ms_sub_ver = itr.value();
    }else{
        return -1;
    }

    itr = m_map_config.find( "MONITOR_USER_NAME" );
    if(itr != m_map_config.end()){
        ms_user_name = itr.value();
    }else{
        return -1;
    }

    itr = m_map_config.find( "MONITOR_USER_PASSWD" );
    if( itr != m_map_config.end() ) {
        ms_user_pwd = itr.value();
    }else{
        return -1;
    }


    /************************ SERVER INFO *************************/
    itr = m_map_config.find( "SERVER_HOST_IP" );
    if( itr != m_map_config.end() ){
        ms_host_ip = itr.value();
    }else{
        return -1;
    }

    itr = m_map_config.find( "SERVER_PORT" );
    if(itr != m_map_config.end()){
        ms_server_port = itr.value();
    }else{
        return -1;
    }

    qDebug()<<"all"<<ms_ver<<ms_sub_ver<<ms_user_name<<ms_user_pwd<<ms_host_ip<<ms_server_port;

    file.close();

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
void write_file();
void save_login_info()
{
    m_map_config.clear();
    m_map_config.insert( "MONITOR_VER", "1");
    m_map_config.insert( "MONITOR_SUB_VER", "0" );
    m_map_config.insert( "MONITOR_USER_NAME", ms_user_name);
    m_map_config.insert( "MONITOR_USER_PASSWD", ms_user_pwd );
    m_map_config.insert( "SERVER_HOST_IP", ms_host_ip );
    m_map_config.insert( "SERVER_PORT", ms_server_port );
    write_file();
}

void write_file()
{
    QString strfile;
    strfile = QCoreApplication::applicationDirPath();
    QString str_file_name = strfile+"/client.cfg";

    qDebug()<<"load_cfg:"<<str_file_name;

    QFile file( str_file_name );
    file.remove();

    if(!file.open(QIODevice::ReadWrite | QIODevice::Text)) {
        return;
    }

    QTextStream write_stream( &file );
    QTextCodec *codec = QTextCodec::codecForName( "UTF8" );
    write_stream.setCodec( codec );
    write_stream<<"###################### MONITOR INFO #######################"<<"\n";
    QMap<QString, QString>::iterator it = m_map_config.find( "MONITOR_VER" );
    write_stream<<"MONITOR_VER = "<<it.value()<<"\n";
    it = m_map_config.find( "MONITOR_SUB_VER" );
    write_stream<<"MONITOR_SUB_VER = "<<it.value()<<"\n";
    it = m_map_config.find( "MONITOR_USER_NAME" );
    write_stream<<"MONITOR_USER_NAME = "<<it.value()<<"\n";
    it = m_map_config.find( "MONITOR_USER_PASSWD" );
    write_stream<<"MONITOR_USER_PASSWD = "<<it.value()<<"\n";
    write_stream<<"###################### SERVER INFO ########################"<<"\n";
    it = m_map_config.find( "SERVER_HOST_IP" );
    write_stream<<"SERVER_HOST_IP = "<<it.value()<<"\n";
    it = m_map_config.find( "SERVER_PORT" );
    write_stream<<"SERVER_PORT = "<<it.value()<<"\n";
    write_stream.flush();
    file.close();

}
