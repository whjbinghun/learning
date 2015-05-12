#include "cfgfileinfo.h"
#include <QDebug>
#include <QFile>
#include <QTextCodec>


CfgFileInfo::CfgFileInfo()
{

}

bool CfgFileInfo::is_valid_cfg_line( const char* src )
{
    if( src[0] != '#' && src[0] != ' '  ) {
        return true;
    }
    return false;
}

//解析配置文件
bool CfgFileInfo::parser_cfg( const char* str, char*& key, char*& value )
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

int CfgFileInfo::load_cfg( QString &str_file_name )
{
    char buff[1024];
    char key_buff[512];
    char value_buff[1024];

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
            qDebug()<<"parser_cfg"<<QString( QLatin1String( key_buff ) ) << QString( QLatin1String( value_buff ) )<<","<<key_buff<<","<<value_buff;
        }

    } while ( !stream.atEnd() );

    file.close();

    return 0;
}

bool CfgFileInfo::get_config_info( QString str_key, QString &str_value )
{
    QMap<QString, QString>::iterator itr = m_map_config.begin();
    itr = m_map_config.find( str_key );
    if(itr != m_map_config.end()){
        str_value = itr.value();
        return true;
    }

    return false;
}

bool CfgFileInfo::set_config_info( QString str_key, QString str_value )
{
    QMap<QString, QString>::iterator itr = m_map_config.begin();
    itr = m_map_config.find( str_key );
    if( itr != m_map_config.end() ){
        itr.value() = str_value;
    } else {
        m_map_config.insert( str_key, str_value );
    }

    return true;
}

void CfgFileInfo::save_cofig_file(QString &str_file_name )
{
    QFile file( str_file_name );
    file.remove();
    qDebug()<<"load_cfg:"<<str_file_name;
    if( !file.open( QIODevice::ReadWrite | QIODevice::Text ) ) {
        return;
    }

    QTextStream write_stream( &file );
    QTextCodec *codec = QTextCodec::codecForName( "UTF8" );
    write_stream.setCodec( codec );
    QMap<QString, QString>::iterator it = m_map_config.begin();
    for( ; it != m_map_config.end(); it++ ) {
        write_stream<<it.key()<<" = "<<it.value()<<"\n";
    }

    write_stream.flush();
    file.close();

}

void CfgFileInfo::test_map()
{
    QMap<QString, QString>::iterator it = m_map_config.begin();
    for( ; it != m_map_config.end(); it++ ) {
        qDebug()<<"CfgFileInfo::test_map"<<it.key()<<" = "<<it.value()<<"\n";
    }
}
