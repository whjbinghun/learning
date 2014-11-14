#include "wndcruisearea.h"
#include "ui_wndcruisearea.h"

WndCruiseArea::WndCruiseArea(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WndCruiseArea)
{
    ui->setupUi(this);
}

WndCruiseArea::~WndCruiseArea()
{
    delete ui;
}

void WndCruiseArea::resizeEvent( QResizeEvent *event )
{

}

void WndCruiseArea::move_widget()
{
    QRect rct_group_name, rct_combox, rct_btn, rct_image;
    rct_group_name = QRect( 0, 0, width(), ui->id_lab_group_name->height() );
    rct_combox = QRect( rct_group_name.x(), rct_group_name.y()+rct_group_name.height(), width()-ui->id_btn_cruise_start->width()
                        , ui->id_combox_cruise_num->height() );
    rct_btn = QRect( rct_combox.x()+rct_combox.width(), rct_combox.y(), ui->id_btn_cruise_start->width(),ui->id_combox_cruise_num->height() );
    rct_image = QRect( rct_combox.x(), rct_combox.y()+rct_combox.height(), width(), height()-ui->id_combox_cruise_num->height() );

    move_group_name( rct_group_name );
    move_combox( rct_combox );
    move_btn( rct_btn );
    move_image( rct_btn );
}

void WndCruiseArea::move_group_name( QRect rct )
{
    ui->id_lab_group_name->move( rct.x(), rct.y() );
    ui->id_lab_group_name->resize( rct.width(), rct.height() );
}

void WndCruiseArea::move_combox( QRect rct )
{
    ui->id_combox_cruise_num->move( rct.x(), rct.y() );
    ui->id_combox_cruise_num->resize( rct.width(), rct.height() );
}

void WndCruiseArea::move_btn( QRect rct )
{
    ui->id_btn_cruise_start->move( rct.x(), rct.y() );
    ui->id_btn_cruise_start->resize( rct.width(), rct.height() );
}

void WndCruiseArea::move_image( QRect rct )
{
    ui->id_widget_image->move( rct.x(), rct.y() );
    ui->id_widget_image->resize( rct.width(), rct.height() );
}


