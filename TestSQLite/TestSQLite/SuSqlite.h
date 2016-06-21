#ifndef SUSQLITE_H
#define SUSQLITE_H
#include <QObject>
#include <QString>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QtSql/QSqlDriver>
#include <QMap>

typedef struct _tagPersonInfo {
    int n_id;
    QString str_name;
    QString str_month;
}PersonInfo;

class SuSqlite
{
public:
    SuSqlite( QString str_data_base_name );
    ~SuSqlite();
public:
    //对表的操作
    bool create_table( QString str_table_name );
    bool insert_data( int n_id, QString str_name );
    bool update_data( int n_id, QString str_name );
    bool delete_data( int n_id );
    bool query_all_data();
    bool query_data( int n_id );

private:
    QSqlDatabase m_sql_db;
    QSqlQuery m_sql_query;

    bool mb_sql_open;
    QString ms_data_base_name;
    QString ms_sql_table_name;

    QMap<int, PersonInfo> m_map_person_info;
};

#endif // SUSQLITE_H
