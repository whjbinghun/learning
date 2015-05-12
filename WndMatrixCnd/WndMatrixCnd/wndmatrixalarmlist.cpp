#include "wndmatrixalarmlist.h"
#include <QHeaderView>
#include "wndcolor.h"
#include <QColorDialog>
#include <QMessageBox>
#include <QToolTip>

WndMatrixAlarmList::WndMatrixAlarmList(QWidget *parent) :
    QTableWidget(parent)
    ,mn_cur_martix_list_row( -1 )
    ,mp_lab_show_alarm_cnd( NULL )
    ,mn_eq_width( 0 )
{
    init_ctrl();
    init_connect();
}

WndMatrixAlarmList::~WndMatrixAlarmList()
{

}

void WndMatrixAlarmList::init_lab( QLabel *p_lab )
{
    mp_lab_show_alarm_cnd = p_lab;
    mp_lab_show_alarm_cnd->hide();
}

void WndMatrixAlarmList::init_ctrl()
{
    //mp_lab_show_alarm_cnd = new QLabel( this );
    //mp_lab_show_alarm_cnd->topLevelWidget();

    //ui->id_pre_edit_preset_list->hide();
    //connect( ui->id_pre_edit_preset_list, SIGNAL( returnPressed() ), this, SLOT( slot_preset_return_pressed() ) );
    //ui->id_pre_edit_preset_list->installEventFilter( this );

    setColumnCount( 4 ); // 设置表格控件列大小
    QStringList str_list;
    str_list << tr("编号")<< tr( "矩阵名" )<<tr( "颜色" )<< tr( "报警条件" ) ;
    setHorizontalHeaderLabels( str_list ); // 设置表格控件，列表头内容
    verticalHeader()->setVisible(false);   //隐藏列表头
    setAlternatingRowColors( true );//设置隔一行变一颜色，即：一灰一白
    setEditTriggers( QAbstractItemView::NoEditTriggers );//不能对表格内容进行修改
    setSelectionBehavior( QAbstractItemView::SelectRows ); //整行选中的方式

    QHeaderView *p_header_view = horizontalHeader();
    if ( p_header_view ){
        p_header_view->setDefaultAlignment( Qt::AlignCenter );
        p_header_view->setTextElideMode( Qt::ElideRight );
        p_header_view->setStretchLastSection( true );
    }

    setSelectionMode( QAbstractItemView::NoSelection );

    update_alarm_list_width();

}

void WndMatrixAlarmList::init_connect()
{
    connect( this, SIGNAL(  itemDoubleClicked(QTableWidgetItem *) ), this\
             , SLOT( slot_tab_martix_double_clicked( QTableWidgetItem* ) ) );

    connect( this, SIGNAL(  currentItemChanged(QTableWidgetItem*,QTableWidgetItem*) )
             , this, SLOT( slot_tab_martix_item_change( QTableWidgetItem*,QTableWidgetItem* ) ) );
}

void WndMatrixAlarmList::add_martix_tab_list( MartixInfo &martix_info )
{
    add_martix_tab( martix_info );
    m_list_martix_alarm.push_back( &martix_info );
}

void WndMatrixAlarmList::add_martix_tab( MartixInfo &martix_info )
{
    int n_row = rowCount();

    insertRow( n_row );//行号
    setItem( n_row, 0, new QTableWidgetItem( QString::number( n_row ) ) );//第n_preset_row_num+1行，第1列
    setItem( n_row, 1, new QTableWidgetItem( martix_info.str_martix_name ) );

    WndColor *p_widget_color = new WndColor( this );
    p_widget_color->set_color( martix_info.clr_martix );
    p_widget_color->set_row( n_row );
    connect( p_widget_color, SIGNAL( sig_color_dialog() ), this, SLOT( slot_color_dialog() ) );

    setCellWidget( n_row, 2, p_widget_color );
    setItem( n_row, 3, new QTableWidgetItem( martix_info.str_martix_alarm_cnd ) );

    set_martix_list_cur( n_row );

}

void WndMatrixAlarmList::del_martix_tab( /*MartixInfo &martix_info*/ )
{
    //删除选中的矩阵
    if ( QMessageBox::Ok != QMessageBox::information( this, "提示", "确定要删除矩阵吗？", QMessageBox::Ok | QMessageBox::Cancel ) ) {
          return ;
      }

    QList<MartixInfo*>::iterator it_martix = m_list_martix_alarm.begin();
    if( mn_cur_martix_list_row>= 0 ) {
        //QString str_martix_name = ui->id_tablewidget_matrix_cnd->item( mn_cur_martix_list_row, 1 )->text();
        int n_row = 0;
        for( ; it_martix != m_list_martix_alarm.end(); it_martix++, n_row++ ) {
            if( mn_cur_martix_list_row == n_row ) {
                m_list_martix_alarm.erase( it_martix );
                update_martix_alarm_list();
                break;
            }
        }
    }
}

void WndMatrixAlarmList::resizeEvent( QResizeEvent *event )
{
    update_alarm_list_width();
}

void WndMatrixAlarmList::mouseMoveEvent( QMouseEvent *event )
{
    //循环使用报警列表，当鼠标在报警条件位置时，显示一个lab
    int n_num = this->rowCount();
    if( n_num == 0 ) return;

    for( int i=1; i<=n_num; i++ ) {
        QTableWidgetItem *p_table_item = item( i-1, 3 );
        QRect rct_item = this->visualItemRect( p_table_item );//相对当前窗口的区域
        int n_tab_head_height = this->horizontalHeader()->height();//tab表头的高度
        int n_x = rct_item.x();//+this->x();//rct_item.x:表示在tab中的位置，this->x():表示在整个窗口中，tab的位置
        int n_y = rct_item.y();//+n_tab_head_height;+this->y();//要加上项的高度

        if ( event->x()>= n_x && event->x() < n_x + rct_item.width() \
                && event->y()>= n_y && event->y() < n_y + rct_item.height() ) {
            QToolTip::showText( event->globalPos(), p_table_item->text(), this );
            //m_str_tooltip.clear();
        } else{
            QToolTip::hideText();
        }
    }
}

void WndMatrixAlarmList::slot_tab_martix_double_clicked( QTableWidgetItem *p_item )
{
    //修改矩阵名
    if ( 0 == p_item ) return;
    int n_row = p_item->row();
    if ( n_row < 0 ) return;

    QTableWidgetItem *p_table_item = this->item( n_row, 1 );
    QRect rct_item = this->visualItemRect( p_table_item );//相对当前窗口的区域

    int n_tab_head_height = this->horizontalHeader()->height();
    int n_x = rct_item.x()+this->x();
    int n_y = rct_item.y()+this->y()+n_tab_head_height;//要加上项的高度

    /*mp_edit_time->setValidator(new QRegExpValidator( QRegExp("[\\w]{1,31}"), this ));
    mp_edit_time->show();
    mp_edit_time->move( n_x, n_y );
    mp_edit_time->resize( rct_item.width(), rct_item.height() );
    mp_edit_time->setText( mp_cru_tab_widget_item->text() );
    mp_edit_time->setFocus();
    m_str_cruise_name = mp_edit_time->text();

    mn_edit_change = 2;
    */
}

void WndMatrixAlarmList::slot_tab_martix_item_change( QTableWidgetItem *current, QTableWidgetItem *previous )
{
    if (  0 == current ) {
        set_martix_list_cur( -1 );
    } else {
        int n_row = current->row();
        if ( m_list_martix_alarm.size() <= n_row ) {
            n_row--;
        }
        set_martix_list_cur( n_row );  //设置选中某一项背景色
    }
}

void WndMatrixAlarmList::slot_color_dialog()
{
    WndColor *p_color = (WndColor*)sender();

    if( p_color ) {
        QColor color = QColorDialog::getColor( p_color->get_color(), this );
        //如果用户在颜色对话框点击取消的话，得到的color是无效的
        if( color.isValid() ) {
            p_color->set_color( color );
            int n_row = p_color->get_row();
            m_list_martix_alarm[n_row]->clr_martix = color;
        }
    }
}

void WndMatrixAlarmList::update_martix_alarm_list()
{
    clear_martix_alarm_tab();

    QList< MartixInfo * >::iterator it_martix = m_list_martix_alarm.begin();

    for ( int n=0; it_martix != m_list_martix_alarm.end(); ++it_martix, ++n ) {
        MartixInfo martix_info;
        martix_info.str_martix_name = (*it_martix)->str_martix_name;
        martix_info.clr_martix = (*it_martix)->clr_martix;
        martix_info.str_martix_alarm_cnd = (*it_martix)->str_martix_alarm_cnd;
        add_martix_tab( martix_info );
    }

    selectRow( 0 );
    int n_row = currentRow();//如果列表中没有数据，返回为-1
    set_martix_list_cur( n_row );

}

void WndMatrixAlarmList::clear_martix_alarm_tab()
{
    clearContents();
    while ( rowCount() ) {
        removeRow( 0 );
    }
}

void WndMatrixAlarmList::set_martix_list_cur( int n_row )
{
    if( mn_cur_martix_list_row == n_row ) return;

    int n_row_count = this->rowCount();
    int n_low_count = this->columnCount();
    for ( int n=0; n<n_row_count; ++n ) {
        QTableWidgetItem *p_item(NULL);
        for ( int m=0; m<n_low_count; ++m ) {
            p_item = this->item( n, m );
            if ( p_item ) {
                if ( n == n_row ) {
                    p_item->setBackgroundColor( QColor( 90,167,45 ) );
                    p_item->setTextColor( QColor( 255,255,255 ) );
                } else {
                    p_item->setBackgroundColor( QColor( 255,255,255 ) );
                    p_item->setTextColor( QColor( 0,0,0 ) );
                }
            }
        }
    }

    mn_cur_martix_list_row = n_row;
}

void WndMatrixAlarmList::update_alarm_list_width()
{
    mn_eq_width = width()/5;
    for( int i=0; i<columnCount()-1; i++ ) {
        if( i != 3 ) {
            setColumnWidth( i, mn_eq_width );
        } else {
            setColumnWidth( i, mn_eq_width*2 );
        }
    }
}

