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
}
