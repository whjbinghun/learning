#ifndef SUSQLITE_H
#define SUSQLITE_H
#include <QObject>
#include <QString>

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
    bool delete_data();
    bool query_all_data();
    bool query_data();

private:
    bool mb_sql_open;
    QString ms_data_base_name;
    QString ms_sql_table_name;
};

#endif // SUSQLITE_H
