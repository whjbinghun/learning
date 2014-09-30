#include "iranavifocuse.h"
#include "ui_iranavifocuse.h"

#define SPACE 20

IrAnaViFocuse::IrAnaViFocuse(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::IrAnaViFocuse)
{
    ui->setupUi(this);
}

IrAnaViFocuse::~IrAnaViFocuse()
{
    delete ui;
}

void IrAnaViFocuse::resizeEvent( QResizeEvent *event )
{
    move_widget();
}

void IrAnaViFocuse::move_widget()
{
    int n_width = width();
    int n_height = height();

    QRect rct( 0, 0, n_width, n_height );
    int n_btn_width = (n_width-SPACE*2)/3;
    int n_btn_height = (n_height-SPACE*2)/3;
    ui->id_btn_point->move( rct.x(), rct.y() );
    ui->id_btn_point->resize( n_btn_width, n_btn_height );
    ui->id_btn_line->move( rct.x()+n_btn_width+SPACE, rct.y() );
    ui->id_btn_line->resize( n_btn_width, n_btn_height );
    ui->id_btn_rect->move( rct.x()+(n_btn_width+SPACE)*2, rct.y() );
    ui->id_btn_rect->resize( n_btn_width, n_btn_height );
    ui->id_btn_circle->move( rct.x(), rct.y()+n_btn_height+SPACE );
    ui->id_btn_circle->resize( n_btn_width, n_btn_height );
    ui->id_btn_ir_focus_far->move( rct.x()+n_btn_width+SPACE, rct.y()+n_btn_height+SPACE );
    ui->id_btn_ir_focus_far->resize( n_btn_width, n_btn_height );
    ui->id_btn_ir_focus_near->move( rct.x()+(n_btn_width+SPACE)*2, rct.y()+n_btn_height+SPACE );
    ui->id_btn_ir_focus_near->resize( n_btn_width, n_btn_height );
    ui->id_btn_delete_shape->move( rct.x(), rct.y()+(n_btn_height+SPACE)*2 );
    ui->id_btn_delete_shape->resize( n_btn_width, n_btn_height );
    ui->id_btn_vi_focus_far->move( rct.x()+n_btn_width+SPACE, rct.y()+(n_btn_height+SPACE)*2 );
    ui->id_btn_vi_focus_far->resize( n_btn_width, n_btn_height );
    ui->id_btn_vi_focus_near->move( rct.x()+(n_btn_width+SPACE)*2, rct.y()+(n_btn_height+SPACE)*2 );
    ui->id_btn_vi_focus_near->resize( n_btn_width, n_btn_height );
}
