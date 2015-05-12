#include "wndanachartcolor.h"
#include "ui_wndanachartcolor.h"
#include <QColorDialog>
#include "datachartmode.h"

WndAnaChartColor::WndAnaChartColor( WndChartTrend *chart_trend, int n_index , QWidget *parent ) :
    QDialog( parent ),
    ui(new Ui::WndAnaChartColor)
  , mstr_sn( "" )
  , mn_index( n_index )
  , mp_wnd_property( NULL )
{
    ui->setupUi(this);
    resize( 450, 360);
    setWindowTitle( "参数设置" );

    mp_wnd_property = new WndPropertyBrowser( chart_trend, 0 );
    ui->m_top_horizontal_layout->addWidget( mp_wnd_property );
    ui->m_top_horizontal_layout->setStretch( 1, 1 );

    mp_wnd_property->resize( 80, height() );//设置编辑框的宽度

    connect( ui->mp_ok, SIGNAL( clicked() ), this, SLOT( slot_btn_ok() ) );
    connect( ui->mp_cancel, SIGNAL( clicked() ), this, SLOT( slot_btn_cancel() ) );


    init_tabel_header( );


}

WndAnaChartColor::~WndAnaChartColor()
{
    delete ui;
}

void WndAnaChartColor::set_group_number( int n_group_number )
{
    DeviceGroup dev_group;
    CDataMode::Instance()->get_group_info( n_group_number, dev_group );

    QString str_sn = dev_group.dev_ir.str_sn+"_real";
    mp_wnd_property->init_wnd_ctrl( n_group_number, str_sn, mn_index );

}

void WndAnaChartColor::get_ana_chart_color_list( QMap< QString, QColor > & map_color )
{
    map_color = m_map_curr_ana_chart_color;
}

void WndAnaChartColor::set_ana_chart_color_list( QMap< QString, QColor > map_color )
{
    QTableWidgetItem* item = 0;
    int map_size = map_color.size();
    if ( map_size == 0 ){
            add_data( 0, 10 );
    }else{
        int n_index = 1;
        QMap< QString, QColor >::iterator it = map_color.begin();
        for( ; it != map_color.end(); ++it, ++n_index ){
            int row_count = ui->mt_ana_chart_color->rowCount();
            ui->mt_ana_chart_color->insertRow( row_count );
            ui->mt_ana_chart_color->setRowHeight( row_count, 20 );
            // 第二列
            item = new QTableWidgetItem;
            item->setData(Qt::DisplayRole, "曲线");
            item->setTextAlignment( Qt::AlignCenter );
            item->setText( QString("曲线%1").arg( n_index )); // it.key()
            ui->mt_ana_chart_color->setItem( row_count, 1, item );

            // 第三列
            item = new QTableWidgetItem;
            item->setBackgroundColor( it.value() );
            ui->mt_ana_chart_color->setItem( row_count, 2,  item );

            if ( n_index <= 10 ){
                continue;
            }

            // 第四列
            item = new QTableWidgetItem;
            item->setTextAlignment( Qt::AlignCenter );
            item->setText( "-" );
            ui->mt_ana_chart_color->setItem( row_count, 3,  item );
        }

        if ( map_size < 10 ){
            // 增加到10列
            add_data( map_size,  10 );
        }
    }



    int row_count = ui->mt_ana_chart_color->rowCount();
    ui->mt_ana_chart_color->insertRow( row_count );
    ui->mt_ana_chart_color->setRowHeight( row_count, 20 );

    // 第一列
    item = new QTableWidgetItem;
    item->setText( "+" );
    ui->mt_ana_chart_color->setFocus();
    ui->mt_ana_chart_color->setItem( row_count, 0, item );

    m_map_curr_ana_chart_color = map_color;
}

void WndAnaChartColor::slot_btn_ok()
{
    int rows = ui->mt_ana_chart_color->rowCount();
    m_map_curr_ana_chart_color.clear();

    QTableWidgetItem *	item = 0;
    for( int i = 0; i < rows - 1; i++ ){
        item = ui->mt_ana_chart_color->item( i, 1 );
        QString str_name = QString( "chart%1" ).arg( i + 100 + 1 ) ;// item->text();

        item = ui->mt_ana_chart_color->item( i, 2 );
        QColor  color = item->backgroundColor();

        m_map_curr_ana_chart_color[ str_name ] = color;
    }
    accept();
}

void WndAnaChartColor::slot_btn_cancel()
{
    reject();
}

void WndAnaChartColor::init_tabel_header()
{
    ui->mt_ana_chart_color->setColumnCount( 4 );
    QStringList header;
    header<<""<<"名称"<<"颜色"<<"";
    ui->mt_ana_chart_color->setHorizontalHeaderLabels(header);
    ui->mt_ana_chart_color->setColumnWidth( 0, 20 );
    ui->mt_ana_chart_color->setColumnWidth( 1, 60 );
    ui->mt_ana_chart_color->setColumnWidth( 2, 80 );
    ui->mt_ana_chart_color->setColumnWidth( 3, 20 );
}

QColor WndAnaChartColor::generate_rand_color()
{
    return QColor::fromHsl( rand() % 360, rand() % 256, rand() % 200 );
}

void WndAnaChartColor::add_data( int start_index, int rows )
{
    QTableWidgetItem* item = 0;
    for( int i = start_index; i < rows; ++i ){
        int row_count = ui->mt_ana_chart_color->rowCount();
        ui->mt_ana_chart_color->insertRow( row_count );
        ui->mt_ana_chart_color->setRowHeight( row_count, 20 );
        // 第二列
        item = new QTableWidgetItem;
        item->setData(Qt::DisplayRole, "曲线");
        item->setTextAlignment( Qt::AlignCenter );
        item->setText( QString("曲线%1").arg( i + 1 )); // it.key()
        ui->mt_ana_chart_color->setItem( row_count, 1, item );

        // 第三列
        item = new QTableWidgetItem;
        item->setBackgroundColor( generate_rand_color() );
        ui->mt_ana_chart_color->setItem( row_count, 2,  item );

        // 第四列, 10列不需要-号
//        item = new QTableWidgetItem;
//        item->setTextAlignment( Qt::AlignCenter );
//        item->setText( "-" );
//        ui->mt_ana_chart_color->setItem( row_count, 3,  item );
    }
}


void WndAnaChartColor::on_mt_ana_chart_color_cellClicked(int row, int column)
{
    if ( column == 2 && ( row != ui->mt_ana_chart_color->rowCount() - 1 )){
        QColorDialog color_dialog;
        if ( QDialog::Accepted == color_dialog.exec() ){
            QTableWidgetItem* item = new QTableWidgetItem;
            QColor color = color_dialog.currentColor();
            item->setBackgroundColor( color );
            ui->mt_ana_chart_color->setItem( row, column, item );
        }
    }else if ( column == 0 ){
        if ( row == ui->mt_ana_chart_color->rowCount() - 1 ){

            // 新增的标记列
            int row_count = ui->mt_ana_chart_color->rowCount();

            // 消除列1
            QTableWidgetItem* item = new QTableWidgetItem;
            item->setText( "" );
            ui->mt_ana_chart_color->setItem( row, column, item );

            // 增加列2
            item = new QTableWidgetItem;
            item->setText( QString("曲线%1").arg( row_count ) );
            item->setTextAlignment( Qt::AlignCenter );
            ui->mt_ana_chart_color->setItem( row, 1, item );

            item = new QTableWidgetItem;
            item->setTextAlignment( Qt::AlignCenter );

            item->setBackgroundColor( generate_rand_color() );
            ui->mt_ana_chart_color->setItem( row, 2, item );

            // 增加列4
            item = new QTableWidgetItem;
            item->setTextAlignment( Qt::AlignCenter );
            item->setText( "-" );
            ui->mt_ana_chart_color->setItem( row, 3, item );

            // --------------------------------------------------
            // 新增
            ui->mt_ana_chart_color->insertRow( row_count );
            ui->mt_ana_chart_color->setRowHeight( row_count, 20 );
            // 列1
            item = new QTableWidgetItem;
            item->setTextAlignment( Qt::AlignCenter );
            item->setText( "+" );
            ui->mt_ana_chart_color->setItem( row_count, 0, item );
        }
        // 删除列
    } else if ( column == 3 ){
        int row_count = ui->mt_ana_chart_color->rowCount();
        if ( ( row == row_count - 1 ) || ( row_count <= 11 ) || ( row < 10 ) ){
            return;
        }

        ui->mt_ana_chart_color->removeRow( row );

        // 重新计算名称

        int rows = ui->mt_ana_chart_color->rowCount();
        for( int i = 0; i < rows - 1; i++ ){

            QTableWidgetItem* item = new QTableWidgetItem;
            item->setTextAlignment( Qt::AlignCenter );
            item->setText( QString("曲线%1").arg( i + 1 ) );
            ui->mt_ana_chart_color->setItem( i, 1, item );


        }
    }
}

