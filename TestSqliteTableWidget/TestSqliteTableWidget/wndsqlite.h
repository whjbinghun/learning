#ifndef WNDSQLITE_H
#define WNDSQLITE_H

#include <QWidget>
#include "SuSqlite.h"

namespace Ui {
class wndSqlite;
}

class wndSqlite : public QWidget
{
    Q_OBJECT

public:
    explicit wndSqlite(QWidget *parent = 0);
    ~wndSqlite();
public:
    static bool creat_connection();
    void insert_table_from_sql( int n_id, QString str_name );
public slots:
    void slot_commit_sql_data();
    void slot_cancel_sql_data();
    void slot_insert_sql_data();
    void slot_del_sql_data();
    void slot_ascend_sql_data();
    void slot_descend_sql_data();
    void slot_query_sql_data();
    void slot_return_table();
private:
    void init_table();
    void init_connect();
private:
    Ui::wndSqlite *ui;

    static SuSqlite *mp_sqlite;
};

#endif // WNDSQLITE_H
