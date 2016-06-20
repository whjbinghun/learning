#include "SuSqlite.h"
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QtSql/QSqlDriver>

SuSqlite::SuSqlite(QString str_data_base_name)
    : mb_sql_open( false )
{
    QSqlDatabase sql_db = QSqlDatabase::addDatabase( "QSQLITE" );
    sql_db.setDatabaseName( str_data_base_name );
    mb_sql_open = sql_db.open(); // 连接数据库, 然后就可以使用了.

}

SuSqlite::~SuSqlite()
{

}

bool SuSqlite::create_table( QString str_table_name )
{
//    QString str_create_table = QString( "create table %1 (id int primary key, name vchar" ).arg( str_table_name );
//    if( m_sql_query.exec( QObject::tr( str_create_table ) ) ) {
//        qDebug()<<"创建数据库表成功";
//        ms_sql_table_name = str_table_name;

//        return true;
//    }

//    return false;
}

bool SuSqlite::insert_data( int n_id, QString str_name )
{
//    QString str_insert = QString( "insert into %1 values( %2, %3 )" ).arg( ms_sql_table_name ).arg( n_id ).arg( str_name );
//    if( m_sql_query.exec( QObject::tr( str_insert ) ) ) {
//        qDebug()<<"插入数据成功";
//        return true;
//    }

//    return false;
}

bool SuSqlite::update_data( int n_id, QString str_name )
{
//    QString str_update = QString( "update into %1 values( %2, %3 )" ).arg( ms_sql_table_name ).arg( n_id ).arg( str_name );
//    if( m_sql_query.exec( QObject::tr( str_insert ) ) ) {
//        qDebug()<<"插入数据成功";
//        return true;
//    }

//    return false;
}

bool SuSqlite::delete_data()
{

}

bool SuSqlite::query_all_data()
{
//    QString str_query_all_data = QString( "select * from %1 " ).arg( ms_sql_table_name );
//    if( m_sql_query.exec( QObject::tr( str_query_all_data ) ) )   //尝试列出  表的所有记录
//    {
//        return true;
//    }

//    return false;
}

bool SuSqlite::query_data()
{

}

