#include "panrail.h"
#include "ui_panrail.h"
#include <QPainter>
#include <QDebug>

PanRail::PanRail(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PanRail)
    ,mp_pan_rail( NULL )
{
    ui->setupUi(this);
    mp_pan_rail = new WndPanRail( ui->id_widget_2 );
    qDebug()<<"PanRail";
    //mp_pan_rail->setWindowOpacity( 0.3 );
    //mp_pan_rail->set_widget_transparency( 0.3 );
    /*QPalette pal = mp_pan_rail->palette();
    pal.setColor(QPalette::Background, QColor(100,100,100,0));
    mp_pan_rail->setPalette(pal);
    mp_pan_rail->setAutoFillBackground(true);
    */
}

PanRail::~PanRail()
{
    delete mp_pan_rail;
    delete ui;
}

void PanRail::resizeEvent( QResizeEvent *event )
{
    ui->id_widget_2->move( 500, 30 );
    ui->id_widget_2->resize( 260, 300 );
    mp_pan_rail->move( 0, 0 );
    mp_pan_rail->resize( 260, 300 );
}

void PanRail::paintEvent(QPaintEvent *event)
{
    QPainter draw( this );
    draw.drawPixmap( 0, 0, width(), height(), QPixmap( "E:/learning-master/PanRail/caption_background.bmp" ) );

    draw.end();
}

void PanRail::on_pushButton_clicked()
{
    static int i=1;
    switch (i) {
    case 1:
        mp_pan_rail->set_box_exit( true );
        break;
    case 2:
        mp_pan_rail->set_box_exit( false );
        break;
    default:
        break;
    }
    if( i==2 ) {
        i=0;
    }
    i++;
    mp_pan_rail->update_ctrl();
}

void PanRail::on_pushButton_2_clicked()
{
    static int i=1;
    switch (i) {
    case 1:
        mp_pan_rail->set_pan_exit( true );
        break;
    case 2:
        mp_pan_rail->set_pan_exit( false );
        break;
    default:
        break;
    }
    if( i==2 ) {
        i=0;
    }
    i++;
    mp_pan_rail->update_ctrl();
}

void PanRail::on_pushButton_3_clicked()
{
    static int i=1;

    switch (i) {
    case 1:
        mp_pan_rail->set_rail_exist( true, true, true );
        break;
    case 2:
        mp_pan_rail->set_rail_exist( true, true, false );
        break;
    case 3:
        mp_pan_rail->set_rail_exist( true, false, true );
        break;
    case 4:
        mp_pan_rail->set_rail_exist( true, false, false );
        break;
    case 5:
        mp_pan_rail->set_rail_exist( false, true, true );
        break;
    case 6:
        mp_pan_rail->set_rail_exist( false, true, false );
        break;
    case 7:
        mp_pan_rail->set_rail_exist( false, false, true );
        break;
    case 8:
        mp_pan_rail->set_rail_exist( false, false, false );
        break;
    default:
        break;
    }
    if( i==8 ) {
        i=0;
    }
    i++;
    mp_pan_rail->update_ctrl();
    //mp_pan_rail->set_rail_exist( false, false, true );
    //mp_pan_rail->set_widget_transparency( 0.5 );
}
