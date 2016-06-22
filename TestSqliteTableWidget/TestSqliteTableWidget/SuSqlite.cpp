#include "SuSqlite.h"
#include <QDebug>

SuSqlite::SuSqlite(QString str_data_base_name)
    : mb_sql_open( false )
{
    m_sql_db = QSqlDatabase::addDatabase( "QSQLITE" );
    m_sql_db.setDatabaseName( str_data_base_name );
}

SuSqlite::~SuSqlite()
{
    close_sql();
}

bool SuSqlite::open_sql()
{
    mb_sql_open = m_sql_db.open(); // 连接数据库, 然后就可以使用了.
    return mb_sql_open;
}

void SuSqlite::close_sql()
{
    if( mb_sql_open ) {
        m_sql_db.close();
        mb_sql_open = false;
    }
}

bool SuSqlite::create_table( QString str_table_name )
{
    ms_sql_table_name = str_table_name;

    QString str_create_table = QString( "create table %1 (id int primary key, name vchar)" ).arg( str_table_name );
    QSqlQuery sql_query;

    if( sql_query.exec( QObject::tr( str_create_table.toStdString().c_str() ) ) ) {
        qDebug()<<"创建数据库表成功";
        return true;
    }

    return false;
}

bool SuSqlite::insert_data( int n_id, QString str_name )
{
    QSqlQuery sql_query;

    QString str_insert = QString( "insert into %1 values( %2, '%3' )" ).arg( ms_sql_table_name ).arg( n_id ).arg( str_name );
    if( sql_query.exec( QObject::tr( str_insert.toStdString().c_str() ) ) ) {
        qDebug()<<"插入数据成功";
        return true;
    }

    return false;
}

bool SuSqlite::update_data( int n_id, QString str_name )
{
    QSqlQuery sql_query;

    QString str_update = QString( "update %1 set name = '%2' where id = %3" ).arg( ms_sql_table_name ).arg( str_name ).arg( n_id );
    if( sql_query.exec( QObject::tr( str_update.toStdString().c_str() ) ) ) {
        qDebug()<<"更新数据成功";
        return true;
    }

    return false;
}

bool SuSqlite::delete_data( int n_id )
{
    QSqlQuery sql_query;

    QString str_delete = QString( "delete from %1 where id = %2 " ).arg( ms_sql_table_name ).arg( n_id );
    if( sql_query.exec( QObject::tr( str_delete.toStdString().c_str() ) ) ) {
        qDebug()<<"删除数据成功";
        return true;
    }

    return false;
}

bool SuSqlite::query_all_data()
{
    QSqlQuery sql_query;

    QString str_query_all_data = QString( "select * from %1 " ).arg( ms_sql_table_name );
    if( sql_query.exec( QObject::tr( str_query_all_data.toStdString().c_str() ) ) )   //尝试列出  表的所有记录
    {
        while( sql_query.next() ) {//开始就先执行一次next()函数，那么query指向结果集的第一条记录
            PersonInfo person_info;
            person_info.n_id = sql_query.value( 0 ).toInt();
            person_info.str_name = sql_query.value( 1 ).toString();
            person_info.str_month = sql_query.value( 2 ).toString();
            m_map_person_info.insert( person_info.n_id, person_info );
        }

        PersonInfo person_info;
        m_st_person_info = person_info;
        return true;
    }

    return false;
}

bool SuSqlite::query_data( int n_id )
{
    QSqlQuery sql_query;

    QString str_query_data = QString( "select * from %1 where id = %2 " ).arg( ms_sql_table_name ).arg( n_id );
    if( sql_query.exec( QObject::tr( str_query_data.toStdString().c_str() ) ) )   //尝试列出  表的所有记录
    {
        qDebug()<<"查询数据成功";
        m_map_person_info.clear();
        sql_query.first();
        m_st_person_info.n_id = sql_query.value( 0 ).toInt();
        m_st_person_info.str_name = sql_query.value( 1 ).toString();
        m_st_person_info.str_month = sql_query.value( 2 ).toString();

        return true;
    }

    return false;
}

QMap<int, PersonInfo> &SuSqlite::get_person_info()
{
    return m_map_person_info;
}

