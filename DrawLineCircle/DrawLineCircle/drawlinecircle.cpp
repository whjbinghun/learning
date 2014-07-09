#include "drawlinecircle.h"
#include "ui_drawlinecircle.h"
#include <QDebug>

DrawLineCircle::DrawLineCircle(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::DrawLineCircle)
{
    ui->setupUi(this);
    ui->id_ir_frame->set_ir_slider( ui->id_ir_slider );
    set_slider();
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
    ui->id_slider->move( 0, 0 );
}

void DrawLineCircle::set_slider()
{
    //设置QSlider的范围
    ui->id_slider->setRange( 0, 400 );
    //在拖动条上按一下改变的值
    //mp_slider_play->setPageStep( 10 );
    //设置显示刻度的位置
    //mp_ir_slider->setTickPosition( QSlider::TicksRight );
    connect( ui->id_slider, SIGNAL( valueChanged(int) ), this, SLOT( slot_slider_value_change(int) ) );
    /*QTimer *slider_timer = new QTimer( this );
    //开始运行定时器，定时时间间隔为1000ms
    connect( slider_timer, SIGNAL( timeout() ), this, SLOT( slot_slider_value() ) );
    timer->start( 50 );*/
}

void DrawLineCircle::slot_slider_value_change( int n_pos )
{
    ui->id_slider->setValue( n_pos );
    qDebug()<<"DrawLineCircle::slot_slider_value_change"<<n_pos<< QCursor::pos();
}

void DrawLineCircle::mousePressEvent( QMouseEvent *event )
{
    if ( event->button() == Qt::LeftButton && this->isEnabled() ) {
        qDebug()<<"DrawLineCircle::mousePressEvent"<< event->x();
    }
}

void DrawLineCircle::mouseMoveEvent( QMouseEvent *event )
{
    qDebug()<<"DrawLineCircle::mouseMoveEvent"<< event->x();
}

void DrawLineCircle::mouseReleaseEvent( QMouseEvent *event )
{
    if( event->button() == Qt::LeftButton && this->isEnabled() ) {

    }
}

