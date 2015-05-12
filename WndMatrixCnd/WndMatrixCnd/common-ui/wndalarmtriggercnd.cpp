#include "wndalarmtriggercnd.h"
#include <QHeaderView>
#include <QScrollBar>
#include <QToolButton>

WndAlarmTriggerCnd::WndAlarmTriggerCnd(QWidget *parent) :
    QTableWidget(parent)
    ,mn_current_alarm_cnd_row( -1 )
{
    init_table_header();
    connect( this, SIGNAL( clicked(QModelIndex)), this, SLOT( slot_cruise_list_clicked( QModelIndex ) ) );
}

WndAlarmTriggerCnd::~WndAlarmTriggerCnd()
{
    //delete ui;
}

void WndAlarmTriggerCnd::insert_alarm_group( AlarmGroup *p_alarm_group, AnaType ana_type )
{
    QString str_condition = "", str_end = " ";
    QString str_ana_type = "";
    for( int m = 0; m < p_alarm_group->mv_conds.size(); m++ ){
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
    int n_row = 0;
    if( ana_type == ANA_COM ) {
        str_ana_type = "普通";
    } else if( ana_type == ANA_MATRIX ) {
        str_ana_type = "阵列";
        n_row = rowCount();
    } else if( ana_type == ANA_INTERIM ) {
        str_ana_type = "临时";
        n_row = rowCount();
    } else {
        str_ana_type = "未知";
        n_row = rowCount();
    }
    insertRow( n_row );//行号
    setRowHeight( n_row, 22 );

    setItem( n_row, 0, new QTableWidgetItem( str_ana_type ) );

    QTableWidgetItem *p_table_item = new QTableWidgetItem;
    p_table_item->setData( Qt::UserRole, (int)p_alarm_group );
    p_table_item->setText( str_condition );
    setItem( n_row, 1, p_table_item );

    if ( !p_alarm_group->b_preset ){
        QToolButton *p_edit_btn = new QToolButton( this );
        p_edit_btn->setText("编辑");
        //p_edit_btn->setFlat( true );
        setCellWidget( n_row, 2, p_edit_btn );
        connect( p_edit_btn, SIGNAL( clicked() ), this, SLOT( slot_edit_tab() ) );

        QToolButton *p_delete_btn = new QToolButton( this );
        p_delete_btn->setText("删除");
        //p_delete_btn->setFlat( true );
        setCellWidget( n_row, 3, p_delete_btn );
        connect( p_delete_btn, SIGNAL( clicked() ), this, SLOT( slot_delete_tab() ) );
    }
    update_cruise_list_cur_clr( n_row );
    mn_current_alarm_cnd_row = n_row;
    resize_scoll( ana_type );
}

void WndAlarmTriggerCnd::clear_all_alarm_condition()
{
    int n_row = rowCount();
    for( int i = 0; i < n_row; i++ ) {
        QTableWidgetItem *p_selected_item = item( 0, 1 );
        AlarmGroup *p_alarm_group = ( AlarmGroup* )p_selected_item->data(Qt::UserRole).toInt();
        if( p_alarm_group ){

            if ( !p_alarm_group->b_preset ){
                delete_object_btn( 0 );
            }


            delete p_alarm_group;
            p_alarm_group = NULL;
        }
        removeRow( 0 );
    }
}

void WndAlarmTriggerCnd::clear_preset_alarm_condition()
{
    int i = rowCount() - 1;
    while ( i >= 0 ){
        QTableWidgetItem *p_selected_item = item( i, 1 );
        AlarmGroup *p_alarm_group = ( AlarmGroup* )p_selected_item->data( Qt::UserRole ).toInt();
        if( p_alarm_group ){
            if ( p_alarm_group->b_preset ){
                delete p_alarm_group;
                p_alarm_group = NULL;
                removeRow( i );

                i = rowCount() - 1;
                continue;
            }
        }

        --i;
    }
}

void WndAlarmTriggerCnd::delete_object_btn( int n_row )
{
    QPushButton* p_btn = NULL;
    p_btn = (QPushButton*)cellWidget( n_row, 2 );
    if( p_btn ){
        delete p_btn;
        p_btn = NULL;
    }
    p_btn = (QPushButton*)cellWidget( n_row, 3 );
    if( p_btn ){
        delete p_btn;
        p_btn = NULL;
    }
}

void WndAlarmTriggerCnd::delete_alarm_group( int n_alarm_id )
{
    int n_row = rowCount();
    for( int i = 0; i < n_row; i++ ) {
        QTableWidgetItem *p_selected_item = item( i, 1 );
        AlarmGroup *p_alarm_group= (AlarmGroup* )p_selected_item->data(Qt::UserRole).toInt();
        if( p_alarm_group->n_alarm_id == n_alarm_id ){

            if ( !p_alarm_group->b_preset ){
                delete_object_btn( i );
            }

            delete p_alarm_group;
            p_alarm_group = NULL;

//            delete p_selected_item;
//            p_selected_item = NULL;


            removeRow( i );
            break;
        }
    }
}

void WndAlarmTriggerCnd::split_string_to_alarm_group(QString s_alarm_condition, int n_alarm_id, QString &s_name, AlarmGroup *p_alarm_group)
{
    QStringList s_alarm_cond_list;
    p_alarm_group->n_alarm_id = n_alarm_id;
    p_alarm_group->str_name = s_name;
    s_alarm_condition.remove("abs(");
    s_alarm_condition.remove(')');
    int n_alarm_index = s_alarm_condition.indexOf( "OR", 0 );
    if( n_alarm_index > 0 ){
        p_alarm_group->str_and_or = "OR";
        s_alarm_cond_list = s_alarm_condition.split( "OR" );
    } else {
        p_alarm_group->str_and_or = "AND";
        s_alarm_cond_list = s_alarm_condition.split( "AND" );
    }
    alarm_cond_init( s_alarm_cond_list, p_alarm_group );

}

void WndAlarmTriggerCnd::alarm_cond_init( QStringList s_alarm_cond_list, AlarmGroup *p_alarm_group )
{
    int n_index;
    QString str_alarm, str_item;
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

void WndAlarmTriggerCnd::alarm_group_to_string(AlarmGroup *p_alarm_group, QString &str_condition)
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

//void WndAlarmTriggerCnd::resizeEvent(QResizeEvent *event)
//{
//    //init_table_header();
//    move_header();
//}

void WndAlarmTriggerCnd::slot_edit_tab()
{
    QObject *p_sender = QObject::sender();//在槽内获取信号的发送者
    int n_row = rowCount();
    for( int i=0; i < n_row; i++ ) {
        QWidget *p_widget= cellWidget( i, 2 );//获得widget
        if( p_widget == p_sender ) { //按钮指针地址相同，表示是同一个按钮
            QTableWidgetItem *p_selected_item = item( i, 1 );
            AlarmGroup *p_alarm_group = (AlarmGroup* )p_selected_item->data(Qt::UserRole).toInt();
            QString str_ana_type = item( i, 0 )->text();
            if( p_alarm_group ){
                emit sig_edit_alarm_info( p_alarm_group, str_ana_type );
            }
            break;
        }
    }
}

void WndAlarmTriggerCnd::slot_delete_tab()
{
    QObject *p_sender = QObject::sender();//在槽内获取信号的发送者
    int n_row = rowCount();
    for( int i=0; i < n_row; i++ ) {
        QWidget *p_widget= cellWidget( i, 3 );//获得widget
        if( p_widget == p_sender ) { //按钮指针地址相同，表示是同一个按钮
            QTableWidgetItem *p_selected_item = item( i, 1 );
            AlarmGroup *p_alarm_group= (AlarmGroup* )p_selected_item->data(Qt::UserRole).toInt();
            if( p_alarm_group ){
                emit sig_delete_alarm_info( p_alarm_group );
            }
            break;
        }
    }
}

void WndAlarmTriggerCnd::slot_add_btn()
{
    emit sig_add_alarm_info();
}

void WndAlarmTriggerCnd::init_table_header()
{
    int w  = width();
    setColumnCount( 4 );
    QStringList header;
    header<<"类型"<<"内容"<<""<<"";
    setHorizontalHeaderLabels(header);


//    setColumnCount( header.size() );
//    setHorizontalHeaderLabels( header ); // 设置表格控件，列表头内容
    //this->verticalHeader()->setVisible( false );    // 隐藏列表头
    //ui->id_table_cruise_list->setAlternatingRowColors( true );                     // 设置隔一行变一颜色，即：一灰一白
    setEditTriggers( QAbstractItemView::NoEditTriggers );  // 不能对表格内容进行修改
    setSelectionBehavior( QAbstractItemView::SelectRows ); // 整行选中的方式
    //ui->id_table_cruise_list->horizontalHeader()->setStretchLastSection( true );     // 设置充满表宽度

    horizontalHeader()->setSectionResizeMode( 0, QHeaderView::Interactive );
    horizontalHeader()->setSectionResizeMode( 1, QHeaderView::Interactive );
    horizontalHeader()->setSectionResizeMode( 2, QHeaderView::Fixed );
    horizontalHeader()->resizeSection( 0, 40 );
    horizontalHeader()->resizeSection( 2, 30 );
    horizontalHeader()->resizeSection( 3, 30 );
    //setColumnWidth( 1, 200 );

    //ui->id_table_cruise_list->horizontalHeader()->resizeSection( 1, 30 );;

    setSelectionMode( QAbstractItemView::NoSelection );    //. 不能选中

    verticalHeader()->setHidden( false );//显示自带的行号

    //move_header();
}

void WndAlarmTriggerCnd::update_cruise_list_cur_clr( int n_cur )
{
    int n_row_count = rowCount();
    int n_col_count = columnCount();
    QTableWidgetItem *p_item(NULL);

    for ( int n_row = 0; n_row < n_row_count; ++n_row ) {

        for( int n_col = 0; n_col < n_col_count; ++n_col ) {
            p_item = item( n_row, n_col );
            if ( p_item ) {
                if ( n_cur == n_row ) {
                    p_item->setBackgroundColor( QColor( 90,167,45 ) );
                    p_item->setTextColor( QColor( 255,255,255 ) );
                } else {
                    p_item->setBackgroundColor( QColor( 255,255,255 ) );
                    p_item->setTextColor( QColor( 0,0,0 ) );
                }
            }
        }
    }
}

void WndAlarmTriggerCnd::set_current_row( int n_current_row )
{
    mn_current_alarm_cnd_row = n_current_row;
}

int WndAlarmTriggerCnd::get_current_row()
{
    return mn_current_alarm_cnd_row;
}

void WndAlarmTriggerCnd::slot_cruise_list_clicked( QModelIndex index )
{
    int n_row = index.row();
    update_cruise_list_cur_clr( n_row );
    mn_current_alarm_cnd_row = n_row;
}

void WndAlarmTriggerCnd::resize_scoll( bool b_matrix_ana )
{
    int n_size = rowCount();

    //verticalScrollBar()->setMaximum( n_size*rowHeight( 0 ) + this->horizontalHeader()->height() );
    //verticalScrollBar()->setPageStep( n_size*rowHeight( 0 ) + this->horizontalHeader()->height() );
    if( b_matrix_ana ) {
        verticalScrollBar()->setSliderPosition( n_size*rowHeight( 0 ) + this->horizontalHeader()->height() );
    } else {
        verticalScrollBar()->setSliderPosition( 0 );
    }
    update();
}

void WndAlarmTriggerCnd::move_header()
{
    setColumnWidth( 0, 40 );
    setColumnWidth( 1, width() - 200 );
    setColumnWidth( 2, 40 );
    setColumnWidth( 3, 40 );
}


