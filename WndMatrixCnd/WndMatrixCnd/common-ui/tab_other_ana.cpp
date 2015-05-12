#include "tab_other_ana.h"
#include <QStandardItemModel>
#include <QHeaderView>

tab_other_ana::tab_other_ana(QWidget *parent) :
    QWidget(parent)
  , mp_add_btn(NULL)
  , mp_table_widget(NULL)
{
   init_table_widget();
}

tab_other_ana::~tab_other_ana()
{
    clear_no_preset_alarm();
    if( mp_add_btn ){
        delete mp_add_btn;
        mp_add_btn = NULL;
    }
    if( mp_table_widget ){
        delete mp_table_widget;
        mp_table_widget = NULL;
    }
}

void tab_other_ana::init_table_widget()
{
    mp_table_widget = new QTableWidget( this );
    mp_table_widget->move( 0, 0 );
    mp_table_widget->resize( width() - 385, height() - 290 );
    mp_table_widget->setColumnCount( 3 );
    mp_add_btn = new QPushButton( this );
    mp_add_btn->setText("+");
    mp_add_btn->setFont(QFont("Times", 20, QFont::Bold));
    mp_add_btn->setFlat(true);
    mp_add_btn->move( 0, height() - 290 );
    mp_add_btn->resize( width() - 385, 25 );
    mp_add_btn->show();
    connect( mp_add_btn, SIGNAL( clicked() ), this, SLOT( slot_add_btn() ) );

    mp_table_widget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    QStringList strList;
    strList << tr( "报警条件" )<<tr( "编辑" )<< tr( "删除" );
    mp_table_widget->setHorizontalHeaderLabels( strList ); // 设置表格控件，列表头内容
    mp_table_widget->setSelectionMode(QAbstractItemView::SingleSelection); //  设置选择的模式为单选择
    mp_table_widget->setSelectionBehavior(QAbstractItemView::SelectRows);  //  设置选择行为时每次选择一行
    mp_table_widget->setAlternatingRowColors( true );//设置隔一行变一颜色，即：一灰一白
    mp_table_widget->setEditTriggers( QAbstractItemView::NoEditTriggers );//不能对表格内容进行修改
    mp_table_widget->setSelectionBehavior( QAbstractItemView::SelectItems ); //一项选中的方式
    QHeaderView* headerView = mp_table_widget->verticalHeader(); //去掉前面的行号
    headerView->setHidden(true);
    headerView = mp_table_widget->horizontalHeader();
    headerView->setHidden(true);
    mp_table_widget->show();
}



void tab_other_ana::insert_alarm_group(AlarmGroup *p_alarm_group)
{   
    QString str_condition = "", str_end = " ";
    for( int m=0; m<p_alarm_group->mv_conds.size();m++ ){
        if( p_alarm_group->mv_conds[m].mn_oper == "-" ){
            str_condition += "abs(";
            str_end = ") ";
        }
        str_condition += p_alarm_group->mv_conds[m].ms_atype + " ";
        if( p_alarm_group->mv_conds[m].ms_btype != "" ){
            str_condition += p_alarm_group->mv_conds[m].mn_oper + " ";
            str_condition += p_alarm_group->mv_conds[m].ms_btype + str_end;
        }
        str_condition += p_alarm_group->mv_conds[m].mn_compare + " ";
        str_condition += "%1 ";
        str_condition = str_condition.arg( p_alarm_group->mv_conds[m].mf_value );
        str_condition += p_alarm_group->str_and_or + " ";
        str_end = " ";
    }
    str_condition = str_condition.left( str_condition.length() - p_alarm_group->str_and_or.length() - 2 );

    int n_row = mp_table_widget->rowCount();
    mp_table_widget->insertRow( n_row );//行号
    QTableWidgetItem *p_table_item = new QTableWidgetItem;
    p_table_item->setData( Qt::UserRole, (int)p_alarm_group );
    p_table_item->setText( str_condition );
    mp_table_widget->setItem( n_row, 0, p_table_item );

    QPushButton *p_edit_btn = new QPushButton( mp_table_widget );
    p_edit_btn->setFont(QFont("Times", 10, QFont::Bold));
    p_edit_btn->setText("编辑");
    p_edit_btn->setFlat( true );
    mp_table_widget->setCellWidget( n_row, 1, p_edit_btn );
    connect( p_edit_btn, SIGNAL( clicked() ), this, SLOT( slot_edit_tab() ) );

    QPushButton *p_delete_btn = new QPushButton( mp_table_widget );
    p_delete_btn->setFont(QFont("Times", 10, QFont::Bold));
    p_delete_btn->setText("删除");
    p_delete_btn->setFlat( true );
    mp_table_widget->setCellWidget( n_row, 2, p_delete_btn );
    connect( p_delete_btn, SIGNAL( clicked() ), this, SLOT( slot_delete_tab() ) );

    set_default_tab_width();
}


void tab_other_ana::set_default_tab_width()
{
    mp_table_widget->resizeColumnsToContents();  //调整列宽
    mp_table_widget->resizeRowsToContents();

    if( mp_table_widget->columnWidth(0) < mp_table_widget->width() - 100 ) {
        mp_table_widget->setColumnWidth( 0, mp_table_widget->width() - 100 );
    }
    mp_table_widget->setColumnWidth( 1, 48 );
    mp_table_widget->setColumnWidth( 2, 48 );

}


//修改报警条件
void tab_other_ana::slot_edit_tab()
{
    QObject *p_sender = QObject::sender();//在槽内获取信号的发送者
    int n_row = mp_table_widget->rowCount();
    for( int i=0; i < n_row; i++ ) {
        QWidget *p_widget= mp_table_widget->cellWidget( i, 1 );//获得widget
        if( p_widget == p_sender ) { //按钮指针地址相同，表示是同一个按钮
            QTableWidgetItem *p_selected_item = mp_table_widget->item( i, 0 );
            AlarmGroup *p_alarm_group = (AlarmGroup* )p_selected_item->data(Qt::UserRole).toInt();
            if( p_alarm_group ){
                emit sig_edit_alarm_info( p_alarm_group );
            }
            break;
        }
    }
}


void tab_other_ana::slot_add_btn()
{
    emit sig_add_alarm_info();
}

//删除报警条件
void tab_other_ana::slot_delete_tab()
{
    QObject *p_sender = QObject::sender();//在槽内获取信号的发送者
    int n_row = mp_table_widget->rowCount();
    for( int i=0; i < n_row; i++ ) {
        QWidget *p_widget= mp_table_widget->cellWidget( i, 2 );//获得widget
        if( p_widget == p_sender ) { //按钮指针地址相同，表示是同一个按钮
            QTableWidgetItem *p_selected_item = mp_table_widget->item( i, 0 );
            AlarmGroup *p_alarm_group= (AlarmGroup* )p_selected_item->data(Qt::UserRole).toInt();
            if( p_alarm_group ){
                emit sig_delete_alarm_info( p_alarm_group );
            }
            set_default_tab_width();
            break;
        }
    }
}

//删除button对象
void tab_other_ana::delete_object_btn( int n_row )
{
    QPushButton* p_btn = NULL;
    p_btn = (QPushButton*)mp_table_widget->cellWidget( n_row, 1 );
    if( p_btn ){
        delete p_btn;
        p_btn = NULL;
    }
    p_btn = (QPushButton*)mp_table_widget->cellWidget( n_row, 2 );
    if( p_btn ){
        delete p_btn;
        p_btn = NULL;
    }

}


//清除所有报警条件
void tab_other_ana::clear_no_preset_alarm()
{
    int n_row = mp_table_widget->rowCount();
    for( int i = 0; i < n_row; i++ ) {
        QTableWidgetItem *p_selected_item = mp_table_widget->item( 0, 0 );
        AlarmGroup *p_alarm_group = ( AlarmGroup* )p_selected_item->data(Qt::UserRole).toInt();
        if( p_alarm_group ){
            delete p_alarm_group;
            p_alarm_group = NULL;
        }
        delete p_selected_item;
        p_selected_item = NULL;
        delete_object_btn( 0 );
        mp_table_widget->removeRow( 0 );
    }
    set_default_tab_width();
}

//根据报警条件id号删除报警条件
void tab_other_ana::delete_alarm_group( int n_alarm_id )
{
    int n_row = mp_table_widget->rowCount();
    for( int i = 0; i < n_row; i++ ) {
        QTableWidgetItem *p_selected_item = mp_table_widget->item( i, 0 );
        AlarmGroup *p_alarm_group= (AlarmGroup* )p_selected_item->data(Qt::UserRole).toInt();
        if( p_alarm_group->n_alarm_id == n_alarm_id ){
            delete p_alarm_group;
            p_alarm_group = NULL;
            delete p_selected_item;
            p_selected_item = NULL;
            delete_object_btn( i );
            mp_table_widget->removeRow( i );
            break;
        }
    }
}


void tab_other_ana::alarm_group_to_string(AlarmGroup *p_alarm_group, QString &str_condition )
{  
    for( vector<AlarmCondition>::iterator it_alarm = p_alarm_group->mv_conds.begin(); it_alarm != p_alarm_group->mv_conds.end();it_alarm++ ){
        str_condition += it_alarm->ms_atype + " ";
        if( it_alarm->ms_btype != "" ){
            str_condition += it_alarm->mn_oper + " ";
            str_condition += it_alarm->ms_btype + " ";
        }
        str_condition += it_alarm->mn_compare + " ";
        str_condition += "%1 ";
        str_condition = str_condition.arg( it_alarm->mf_value );
        str_condition += p_alarm_group->str_and_or + " ";
    }
    str_condition = str_condition.left( str_condition.length() - p_alarm_group->str_and_or.length() - 2 );
}
