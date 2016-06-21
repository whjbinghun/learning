#include "testsqlite.h"
#include "ui_testsqlite.h"
#include <QSqlTableModel>
#include <QMessageBox>
#include <QSqlQuery>
#include <QStandardItemModel>
#include <QtSql/QSqlError>
#include <QtSql/QSqlDriver>

TestSQLite::TestSQLite(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TestSQLite)
    , mp_sql_model( NULL )
{
    ui->setupUi(this);

    init_table();
    creat_connection();
    init_connect();

    mp_sql_model = new QSqlTableModel(this);
    mp_sql_model->setTable( "student" );
    mp_sql_model->setEditStrategy( QSqlTableModel::OnManualSubmit );
    mp_sql_model->select(); //选取整个表的所有行
    mp_sql_model->removeColumn( 1 ); //不显示name属性列,如果这时添加记录，则该属性的值添加不上
    ui->id_tab_list->setModel( mp_sql_model );
    // ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);   //使其不可编辑

}

TestSQLite::~TestSQLite()
{
    delete ui;
}

void TestSQLite::init_table()
{
    //准备数据模型
    QStandardItemModel *p_student_model = new QStandardItemModel();
    p_student_model->setHorizontalHeaderItem(0, new QStandardItem(QObject::tr("id")));
    p_student_model->setHorizontalHeaderItem(1, new QStandardItem(QObject::tr("name")));
    //利用setModel()方法将数据模型与QTableView绑定
    ui->id_tab_list->setModel( p_student_model );
}

void TestSQLite::init_connect()
{
    connect( ui->id_btn_commit, SIGNAL( clicked() ), this, SLOT( slot_commit_sql_data() ) );
    connect( ui->id_btn_cancel, SIGNAL( clicked() ), this, SLOT( slot_cancel_sql_data() ) );
    connect( ui->id_btn_add, SIGNAL( clicked() ), this, SLOT( slot_insert_sql_data() ) );
    connect( ui->id_btn_delete, SIGNAL( clicked() ), this, SLOT( slot_del_sql_data() ) );
    connect( ui->id_btn_ascend, SIGNAL( clicked() ), this, SLOT( slot_ascend_sql_data() ) );
    connect( ui->id_btn_descend, SIGNAL( clicked() ), this, SLOT( slot_descend_sql_data() ) );
    connect( ui->id_btn_query, SIGNAL( clicked() ), this, SLOT( slot_query_sql_data() ) );
    connect( ui->id_btn_return_table, SIGNAL( clicked() ), this, SLOT( slot_return_table() ) );
}

void TestSQLite::slot_commit_sql_data()
{
    mp_sql_model->database().transaction(); //开始事务操作
    if ( mp_sql_model->submitAll() ) {
        mp_sql_model->database().commit(); //提交
    } else {
        mp_sql_model->database().rollback(); //回滚
        QMessageBox::warning( this, tr( "tableModel" ), tr( "数据库错误: %1" ).arg( mp_sql_model->lastError().text() ) );
    }
}

void TestSQLite::slot_cancel_sql_data()
{
    mp_sql_model->revertAll();
}

void TestSQLite::slot_insert_sql_data()
{
    int rowNum = mp_sql_model->rowCount(); //获得表的行数
    mp_sql_model->insertRow( rowNum ); //添加一行
    mp_sql_model->setData( mp_sql_model->index( rowNum, 0 ), rowNum );
    mp_sql_model->setData( mp_sql_model->index( rowNum, 1 ), "开心" );
    //model->submitAll(); //可以直接提交
}

void TestSQLite::slot_del_sql_data()
{
    int curRow = ui->id_tab_list->currentIndex().row();
    if( curRow <= 0 ) return;

    //获取选中的行
    mp_sql_model->removeRow( curRow );
    //删除该行
    int ok = QMessageBox::warning( this, tr("删除当前行!"), tr( "你确定删除当前行吗？" ), QMessageBox::Yes,QMessageBox::No );
    if(ok == QMessageBox::No)
    {
       mp_sql_model->revertAll(); //如果不删除，则撤销
    }
    else mp_sql_model->submitAll(); //否则提交，在数据库中删除该行
}

//升序
void TestSQLite::slot_ascend_sql_data()
{
    mp_sql_model->setSort( 0, Qt::AscendingOrder ); //id属性，即第0列，升序排列
    mp_sql_model->select();
}

//降序
void TestSQLite::slot_descend_sql_data()
{
    mp_sql_model->setSort( 0, Qt::DescendingOrder );
    mp_sql_model->select();
}

void TestSQLite::slot_query_sql_data()
{
    QString str_name = ui->id_edit_name->text();
    mp_sql_model->setFilter( QObject::tr( "name = '%1' ").arg( str_name ) ); //根据姓名进行筛选
    //mp_sql_model->setFilter( QObject::tr( "id > '%1' ").arg( 3 ) ); //根据id大于某个值进行筛选
    mp_sql_model->select(); //显示结果


}

void TestSQLite::slot_return_table()
{
    mp_sql_model->setTable( "student" );   //重新关联表
    mp_sql_model->select();   //这样才能再次显示整个表的内容
    mp_sql_model->removeColumn( 1 ); //不显示name属性列,如果这时添加记录，则该属性的值添加不上

}

bool TestSQLite::creat_connection()
{
    QSqlDatabase db = QSqlDatabase::addDatabase( "QSQLITE" );
    db.setDatabaseName( "database.db" );
    if( !db.open() ) return false;
    QSqlQuery query;
    query.exec( QObject::tr( "create table student (id int primary key, name vchar, month vchar)" ) );

    query.exec( QObject::tr( "insert into student values (0,'刘明', '5')" ) );
    query.exec( QObject::tr( "insert into student values (1,'陈刚', '6')" ) );
    query.exec( QObject::tr( "insert into student values (2,'王红', '7')" ) );

    //db.close();
    return true;
}




