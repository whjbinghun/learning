#include "tab_array_ana.h"
#include <QStandardItemModel>
#include <QScrollBar>
#include <QAbstractItemView>
#include <QScrollArea>
#include <QHeaderView>
#include <QMessageBox>

tab_array_ana::tab_array_ana( QWidget *parent, ANA_TYPE_E ana_type ) :
    QWidget(parent)
   ,m_ana_type( ana_type )
   ,mp_add_btn(NULL)
   ,mp_table_widget(NULL)
{
    init_table_widget();
}

tab_array_ana::~tab_array_ana()
{
    clear_preset_alarm();
    if( mp_add_btn ){
        delete mp_add_btn;
        mp_add_btn = NULL;
    }
    if( mp_table_widget ){
        delete mp_table_widget;
        mp_table_widget = NULL;
    }
}

void tab_array_ana::init_table_widget()
{
    mp_table_widget = new QTableWidget( this );
    mp_table_widget->move( 0, 0 );
    if( m_ana_type == TAB_PRESET_ANA ){     //预置点
        mp_table_widget->resize( width() - 190, height() - 250 );
        mp_table_widget->setColumnCount( 3 );
        mp_add_btn = new QPushButton( this );
        mp_add_btn->setText("+");
        mp_add_btn->setFont(QFont("Times", 25, QFont::Bold));
        mp_add_btn->setFlat(true);
        mp_add_btn->move( 0, 227 );
        mp_add_btn->resize( width() - 190, 25 );
        mp_add_btn->show();
        connect( mp_add_btn, SIGNAL( clicked() ), this, SLOT( slot_add_btn() ) );
    } else if( m_ana_type == TAB_REAL_ANA ){  //实时
        mp_table_widget->resize( width()-385, height()-270 );
        mp_table_widget->setColumnCount( 1 );
    }

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


void tab_array_ana::insert_alarm_group( AlarmGroup *p_alarm_group )
{
    QString str_condition;
    str_condition = p_alarm_group->mv_conds[0].ms_atype + " ";
    str_condition += p_alarm_group->mv_conds[0].mn_compare + " ";
    str_condition += "%1 ";
    str_condition = str_condition.arg( p_alarm_group->mv_conds[0].mf_value );

    int n_row = mp_table_widget->rowCount();
    mp_table_widget->insertRow( n_row );//行号
    QTableWidgetItem *p_table_item = new QTableWidgetItem;
    p_table_item->setData( Qt::UserRole, (int)p_alarm_group );
    p_table_item->setText( str_condition );
    mp_table_widget->setItem( n_row, 0, p_table_item );

    if( m_ana_type == TAB_PRESET_ANA ){
        QPushButton *p_edit_btn = new QPushButton( mp_table_widget );
        p_edit_btn->setFont(QFont("Times", 12, QFont::Bold));
        p_edit_btn->setText("编辑");
        p_edit_btn->setFlat( true );
        mp_table_widget->setCellWidget( n_row, 1, p_edit_btn );
        connect( p_edit_btn, SIGNAL( clicked() ), this, SLOT( slot_edit_tab() ) );

        QPushButton *p_delete_btn = new QPushButton( mp_table_widget );
        p_delete_btn->setFont(QFont("Times", 12, QFont::Bold));
        p_delete_btn->setText("删除");
        p_delete_btn->setFlat( true );
        mp_table_widget->setCellWidget( n_row, 2, p_delete_btn );
        connect( p_delete_btn, SIGNAL( clicked() ), this, SLOT( slot_delete_tab() ) );
    }

    set_default_tab_width();
}


void tab_array_ana::set_default_tab_width()
{
    if( m_ana_type == TAB_PRESET_ANA ){
        mp_table_widget->setColumnWidth( 0, mp_table_widget->width() - 100 );
        mp_table_widget->setColumnWidth( 1, 48 );
        mp_table_widget->setColumnWidth( 2, 48 );
    } else if( m_ana_type == TAB_REAL_ANA ){
        mp_table_widget->setColumnWidth( 0, mp_table_widget->width() );
    }
}

//修改报警条件
void tab_array_ana::slot_edit_tab()
{
    QObject *p_sender = QObject::sender();//在槽内获取信号的发送者
    int n_row = mp_table_widget->rowCount();
    for( int i=0; i < n_row; i++ ) {
        QWidget *p_widget= mp_table_widget->cellWidget( i, 1 );//获得widget
        if( p_widget == p_sender ) { //按钮指针地址相同，表示是同一个按钮
            QTableWidgetItem *p_selected_item = mp_table_widget->item( i, 0 );
            AlarmGroup *p_alarm_group= (AlarmGroup* )p_selected_item->data(Qt::UserRole).toInt();
            if( p_alarm_group ){
                emit sig_edit_alarm_info( p_alarm_group );
            }
            break;
        }
    }
}


void tab_array_ana::slot_add_btn()
{
    emit sig_add_alarm_info();
}

//删除报警条件
void tab_array_ana::slot_delete_tab()
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
void tab_array_ana::delete_object_btn( int n_row )
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
void tab_array_ana::clear_preset_alarm()
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

void tab_array_ana::delete_alarm_group( int n_alarm_id )
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

//s_alarm_condition格式例如: Amatrix_green_0_480[AVG] > 0
void tab_array_ana::split_string_to_alarm_group( QString s_alarm_condition, int n_alarm_id, QString &s_name, AlarmGroup *p_alarm_group )
{
 //   QStringList s_alarm_cond_list;
    AlarmCondition alarm_cond;
    p_alarm_group->n_alarm_id = n_alarm_id;
    p_alarm_group->str_name = s_name;
    int n_index = s_alarm_condition.indexOf( ' ', 0 );
    alarm_cond.ms_atype = s_alarm_condition.left( n_index );
    s_alarm_condition.remove( 0, n_index+1 );
    n_index = s_alarm_condition.indexOf( ' ', 0 );
    alarm_cond.mn_compare = s_alarm_condition.left( n_index );
    s_alarm_condition.remove( 0, n_index+1 );
    alarm_cond.mf_value = s_alarm_condition.toFloat();
    alarm_cond.mn_oper = "";
    alarm_cond.ms_btype = "";
    p_alarm_group->str_and_or = "";
    p_alarm_group->mv_conds.push_back( alarm_cond );
}


void tab_array_ana::alarm_cond_init( QStringList s_alarm_cond_list, AlarmGroup *p_alarm_group )
{
    int n_index = -1;
    QString str_alarm = "", str_item = "";
    AlarmCondition alarm_cond;
    for( int i = 0; i<s_alarm_cond_list.size(); i++ ){
        str_alarm = s_alarm_cond_list.at( i );
        n_index = str_alarm.indexOf( ' ', 0 );
        if( n_index == 0 ){  //去掉前面一空格
            str_alarm.remove( 0, n_index+1 );
            n_index = str_alarm.indexOf( ' ', 0 );
        }
        alarm_cond.ms_atype = str_alarm.left( n_index );
        str_alarm.remove( 0, n_index+1 );
        n_index = str_alarm.indexOf( ' ', 0 );
        str_item = str_alarm.left( n_index );
        if( str_item == "+" || str_item == "-" ){
            alarm_cond.mn_oper = str_item;
            str_alarm.remove( 0, n_index+1 );
        }else{
            alarm_cond.mn_oper = "";
            alarm_cond.ms_btype = "";
            alarm_cond.mn_compare = str_item;
            str_alarm.remove( 0, n_index+1 );
            n_index = str_alarm.indexOf( ' ', 0 );
            if( n_index < 0 ){  //已是最后一项
                alarm_cond.mf_value = str_alarm.toFloat();
                p_alarm_group->mv_conds.push_back(alarm_cond);
                break;
            }else{
                alarm_cond.mf_value = str_alarm.left( n_index ).toFloat();
                p_alarm_group->mv_conds.push_back(alarm_cond);
                continue;
            }
        }
        n_index = str_alarm.indexOf( ' ', 0 );
        alarm_cond.ms_btype = str_alarm.left( n_index );
        str_alarm.remove( 0, n_index+1 );
        n_index = str_alarm.indexOf( ' ', 0 );
        alarm_cond.mn_compare = str_alarm.left( n_index );
        str_alarm.remove( 0, n_index+1 );
        n_index = str_alarm.indexOf( ' ', 0 );
        if( n_index < 0 ){  //已是最后一项
            alarm_cond.mf_value = str_alarm.toFloat();
            p_alarm_group->mv_conds.push_back(alarm_cond);
            break;
        }else{
            alarm_cond.mf_value = str_alarm.left( n_index ).toFloat();
            p_alarm_group->mv_conds.push_back(alarm_cond);

        }
    }
}


void tab_array_ana::alarm_group_to_stringlist( AlarmGroup &alarm_group, QStringList &str_alarm_list )
{
    for( vector<AlarmCondition>::iterator it_alarm = alarm_group.mv_conds.begin(); it_alarm != alarm_group.mv_conds.end(); it_alarm++ ){
        str_alarm_list.push_back( it_alarm->ms_atype );
        str_alarm_list.push_back( it_alarm->ms_btype );
    }
}

void tab_array_ana::alarm_group_to_stringlist( AlarmGroup *p_alarm_group, QStringList &str_alarm_list )
{
    for( vector<AlarmCondition>::iterator it_alarm = p_alarm_group->mv_conds.begin(); it_alarm != p_alarm_group->mv_conds.end(); it_alarm++ ){
        str_alarm_list.push_back( it_alarm->ms_atype );
        str_alarm_list.push_back( it_alarm->ms_btype );
    }
}

void tab_array_ana::alarm_group_to_string(AlarmGroup *p_alarm_group,QString &str_condition )
{
    vector<AlarmCondition>::iterator it_alarm = p_alarm_group->mv_conds.begin();
    str_condition = it_alarm->ms_atype + " ";
    str_condition += it_alarm->mn_compare + " ";
    str_condition += "%1 ";
    str_condition = str_condition.arg( it_alarm->mf_value );
}


void tab_array_ana::alarm_group_to_string(AlarmGroup &alarm_group,QString &str_condition )
{
    vector<AlarmCondition>::iterator it_alarm = alarm_group.mv_conds.begin();
    str_condition = it_alarm->ms_atype + " ";
    str_condition += it_alarm->mn_compare + " ";
    str_condition += "%1 ";
    str_condition = str_condition.arg( it_alarm->mf_value );
}

