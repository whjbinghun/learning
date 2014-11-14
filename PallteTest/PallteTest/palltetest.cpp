#include "palltetest.h"
#include "ui_palltetest.h"
#include "wndpalette.h"
#include <QTimer>
#include <QDebug>

PallteTest::PallteTest(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PallteTest)
{
    ui->setupUi(this);
    init_pallte();
    //创建定时器
    mp_timer = new QTimer( this );
    //将定时器超时信号与槽(功能函数)联系起来
    connect( mp_timer, SIGNAL( timeout() ), this, SLOT( slot_send_data() ) );
    //开始运行定时器，定时时间间隔为1000ms
    mp_timer->start( 40 );
}

PallteTest::~PallteTest()
{
    delete ui;
}

void PallteTest::init_pallte()
{
    ui->id_widget_hor->setMouseTracking( true );
    ui->id_widget_hor->show();

    ui->id_widget_hor->init_palette( clrIron, true, 10.0, 50.0, 24.3, 35.4 );
    connect( ui->id_widget_hor, SIGNAL(sig_set_adjust_clr(int)), this, SLOT(slot_palette_change_clr(int)) );
    connect( ui->id_widget_hor, SIGNAL(sig_set_adjust_auto()), this, SLOT(slot_palette_auto()) );
    connect( ui->id_widget_hor, SIGNAL(sig_set_adjust_range(float,float)), this, SLOT(slot_palette_change_range(float,float)) );

    ui->id_widget_ver->setMouseTracking( true );
    ui->id_widget_ver->show();

    ui->id_widget_ver->init_palette( clrIron, true, 10.0, 50.0, 24.3, 35.4 );
    ui->id_widget_ver->set_hor_ver( false );
    ui->id_widget_ver->set_hor_ver_image( false );
    connect( ui->id_widget_ver, SIGNAL(sig_set_adjust_clr(int)), this, SLOT(slot_palette_change_clr(int)) );
    connect( ui->id_widget_ver, SIGNAL(sig_set_adjust_auto()), this, SLOT(slot_palette_auto()) );
    connect( ui->id_widget_ver, SIGNAL(sig_set_adjust_range(float,float)), this, SLOT(slot_palette_change_range(float,float)) );
}

void PallteTest::auto_send()
{
    int n_rand = qrand()%30;
    float f_min = n_rand;
    float f_range_min = f_min;
    float f_max = f_min + 20;
    float f_range_max = f_max;
    int n_type=0;
    ui->id_widget_hor->slot_update_adjust( n_type==0,f_min,f_max,f_range_min,f_range_max);
    ui->id_widget_ver->slot_update_adjust( n_type==0,f_min,f_max,f_range_min,f_range_max);
    qDebug()<<"PallteTest::auto_send"<<n_rand<<f_min<<f_max;
}

void PallteTest::slot_send_data()
{
    auto_send();
}

void PallteTest::slot_palette_change_clr( int clr )
{

}

void PallteTest::slot_palette_auto()
{
    ui->id_widget_hor->set_adjust_auto( true );
    mp_timer->start( 40 );
}

void PallteTest::slot_palette_change_range( float f_level, float f_levelspan )
{
    ui->id_widget_hor->set_adjust_auto( false );
    mp_timer->stop();
}
