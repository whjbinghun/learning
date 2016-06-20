#ifndef TESTSQLITE_H
#define TESTSQLITE_H

#include <QWidget>

class QSqlTableModel;

namespace Ui {
class TestSQLite;
}

class TestSQLite : public QWidget
{
    Q_OBJECT

public:
    explicit TestSQLite(QWidget *parent = 0);
    ~TestSQLite();
public:
    static bool creat_connection();
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
    Ui::TestSQLite *ui;

    QSqlTableModel *mp_sql_model;
};

#endif // TESTSQLITE_H
