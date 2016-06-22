#include "wndsqlite.h"
#include "ui_wndsqlite.h"
#include <QDebug>

SuSqlite* wndSqlite::mp_sqlite = NULL;


wndSqlite::wndSqlite(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::wndSqlite)
{
    ui->setupUi(this);

    init_table();
    init_connect();
    creat_connection();
}

wndSqlite::~wndSqlite()
{
    delete ui;
}

void wndSqlite::init_table()
{
//    //准备数据模型
//    QStandardItemModel *p_student_model = new QStandardItemModel();
//    p_student_model->setHorizontalHeaderItem(0, new QStandardItem(QObject::tr("id")));
//    p_student_model->setHorizontalHeaderItem(1, new QStandardItem(QObject::tr("name")));
//    //利用setModel()方法将数据模型与QTableView绑定
//    ui->id_tab_list->setModel( p_student_model );

    QStringList list_string;
    list_string << tr( "id" ) << tr( "name" ) ;

    ui->id_tab_list->setColumnCount( list_string.size() );
    ui->id_tab_list->setHorizontalHeaderLabels( list_string ); // 设置表格控件，列表头内容
    //ui->id_tab_list->verticalHeader()->setVisible( false );    // 隐藏列表头
    //ui->id_tab_list->setAlternatingRowColors( true );                     // 设置隔一行变一颜色，即：一灰一白
    ui->id_tab_list->setEditTriggers( QAbstractItemView::NoEditTriggers );  // 不能对表格内容进行修改
    ui->id_tab_list->setSelectionBehavior( QAbstractItemView::SelectRows ); // 整行选中的方式
    //ui->id_tab_list->horizontalHeader()->setStretchLastSection( true );     // 设置充满表宽度

    ui->id_tab_list->horizontalHeader()->resizeSection( 1, 50 );
    ui->id_tab_list->horizontalHeader()->setSectionResizeMode( 0, QHeaderView::Interactive );
    ui->id_tab_list->horizontalHeader()->setSectionResizeMode( 1, QHeaderView::Fixed );
    //ui->id_tab_list->horizontalHeader()->resizeSection( 1, 30 );;

    ui->id_tab_list->setSelectionMode( QAbstractItemView::NoSelection );    //. 不能选中

    //ui->id_tab_list->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    //ui->id_tab_list->setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOn );


}

void wndSqlite::init_connect()
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

void wndSqlite::slot_commit_sql_data()
{
//    mp_sql_model->database().transaction(); //开始事务操作
//    if ( mp_sql_model->submitAll() ) {
//        mp_sql_model->database().commit(); //提交
//    } else {
//        mp_sql_model->database().rollback(); //回滚
//        QMessageBox::warning( this, tr( "tableModel" ), tr( "数据库错误: %1" ).arg( mp_sql_model->lastError().text() ) );
//    }
}

void wndSqlite::slot_cancel_sql_data()
{
//    mp_sql_model->revertAll();
}

void wndSqlite::slot_insert_sql_data()
{
    int n_row_num = ui->id_tab_list->rowCount();
    ui->id_tab_list->insertRow( n_row_num );

    QTableWidgetItem *p_item = new QTableWidgetItem( QString::number( n_row_num ) );
    ui->id_tab_list->setItem( n_row_num, 0, p_item );

    ui->id_tab_list->setItem( n_row_num, 1, new QTableWidgetItem( "激活码" ) );

    mp_sqlite->insert_data( n_row_num, "激活码" );
}

void wndSqlite::slot_del_sql_data()
{
    int n_cur_row = ui->id_tab_list->currentRow();
    if( n_cur_row <= 0 ) return;

    int n_id = ui->id_tab_list->item( n_cur_row, 0 )->text().toInt();
    ui->id_tab_list->removeRow( n_cur_row );
    mp_sqlite->delete_data( n_id );
}

//升序
void wndSqlite::slot_ascend_sql_data()
{
//    mp_sql_model->setSort( 0, Qt::AscendingOrder ); //id属性，即第0列，升序排列
//    mp_sql_model->select();
}

//降序
void wndSqlite::slot_descend_sql_data()
{
//    mp_sql_model->setSort( 0, Qt::DescendingOrder );
//    mp_sql_model->select();
}

void wndSqlite::slot_query_sql_data()
{
//    QString str_name = ui->id_edit_name->text();
//    mp_sql_model->setFilter( QObject::tr( "name = '%1' ").arg( str_name ) ); //根据姓名进行筛选
//    //mp_sql_model->setFilter( QObject::tr( "id > '%1' ").arg( 3 ) ); //根据id大于某个值进行筛选
//    mp_sql_model->select(); //显示结果
}

void wndSqlite::slot_return_table()
{
//    mp_sql_model->setTable( "student" );   //重新关联表
//    mp_sql_model->select();   //这样才能再次显示整个表的内容
//    mp_sql_model->removeColumn( 1 ); //不显示name属性列,如果这时添加记录，则该属性的值添加不上

    while( ui->id_tab_list->rowCount() != 0 ) {
        ui->id_tab_list->removeRow( 0 );
    }

    mp_sqlite->query_all_data();
    QMap< int, PersonInfo> map_person_info = mp_sqlite->get_person_info();
    QMap< int, PersonInfo>::iterator it = map_person_info.begin();
    for( ; it != map_person_info.end(); it++ ) {
       insert_table_from_sql( it.key(), it.value().str_name );
    }


}

void wndSqlite::insert_table_from_sql( int n_id, QString str_name )
{
    int n_row_num = ui->id_tab_list->rowCount();
    ui->id_tab_list->insertRow( n_row_num );

    QTableWidgetItem *p_item = new QTableWidgetItem( QString::number( n_id ) );
    ui->id_tab_list->setItem( n_row_num, 0, p_item );

    ui->id_tab_list->setItem( n_row_num, 1, new QTableWidgetItem( str_name ) );
}

bool wndSqlite::creat_connection()
{
    mp_sqlite = new SuSqlite( "../TestSqliteTableWidget/database.db" );
    if( mp_sqlite->open_sql() ) {
        qDebug()<<"数据库文件database.db打开成功";
    } else {
        return false;
    }

    mp_sqlite->create_table( "student" );
    mp_sqlite->insert_data( 0, "刘明" );
    mp_sqlite->insert_data( 1, "陈刚" );
    mp_sqlite->insert_data( 2, "王红" );
    mp_sqlite->insert_data( 3, "红尘" );


    return true;
}
