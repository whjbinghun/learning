#include "fullshowtest.h"
#include "ui_fullshowtest.h"
#include <QPainter>

FullShowTest::FullShowTest(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::FullShowTest)
{
    ui->setupUi(this);
    init_connect();
}

FullShowTest::~FullShowTest()
{
    delete ui;
}

void FullShowTest::init_connect()
{
    connect( ui->id_btn_full_show, SIGNAL(clicked()), this, SLOT( slot_show_full() ) );
    connect( ui->id_btn_normal, SIGNAL( clicked()), this, SLOT( slot_show_normal() ) );

    connect( ui->id_full_widget, SIGNAL( sig_show_normal()), this, SLOT( slot_show_normal()) );
}

void FullShowTest::slot_show_full()
{
    ui->id_widget->setWindowFlags( Qt::Window );
    ui->id_full_widget->setParent( ui->id_widget );
    ui->id_widget->showFullScreen();

    int n_width = ui->id_widget->width();
    int n_height = ui->id_widget->height();
    QRect rct_ir;
    if( n_width>=n_height*4/3.0 ) {
        rct_ir = QRect( (n_width-n_height*4/3.0)/2, 0, n_height*4/3.0, n_height );
    } else {
        rct_ir = QRect( 0, (n_height-n_width*3/4.0)/2, n_width, n_width*3/4.0 );
    }
    ui->id_full_widget->move( rct_ir.x(), rct_ir.y() );
    ui->id_full_widget->resize( rct_ir.width(), rct_ir.height() );

}

void FullShowTest::slot_show_normal()
{

    ui->id_widget->setWindowFlags( Qt::SubWindow );
    ui->id_full_widget->setParent( ui->centralWidget );

    ui->id_widget->showNormal();
    ui->id_full_widget->showNormal();

    ui->id_widget->hide();

    ui->id_full_widget->move(100, 100 );
    ui->id_full_widget->resize( 200, 200 );
}

void FullShowTest::paintEvent( QPaintEvent *event )
{
    QPainter draw( this );

    //draw.drawImage( );

    draw.end();
}

