#include "drawlinecircle.h"
#include "ui_drawlinecircle.h"

DrawLineCircle::DrawLineCircle(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::DrawLineCircle)
{
    ui->setupUi(this);
    ui->id_ir_frame->set_ir_slider( ui->id_ir_slider );
}

DrawLineCircle::~DrawLineCircle()
{
    delete ui;
}

void DrawLineCircle::resizeEvent( QResizeEvent *event )
{
    int SPACE = 50;
    int wnd_width = ui->centralWidget->width()-SPACE*2;
    int wnd_height = ui->centralWidget->height()-SPACE*2;

    ui->id_ir_frame->move( SPACE, SPACE );
    if( wnd_width>=height()*4/3 ) {
        ui->id_ir_frame->resize( wnd_height*4/3, wnd_height );
    } else {
        ui->id_ir_frame->resize( wnd_width, wnd_width*3/4 );
    }

}
