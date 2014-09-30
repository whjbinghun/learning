#include "panrailtest.h"
#include "ui_panrailtest.h"

PanRailTest::PanRailTest(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PanRailTest)
    ,mp_pan_rail_widget( NULL )
{
    ui->setupUi(this);
    //mp_pan_rail_widget = new WndPanRail( this );

}

PanRailTest::~PanRailTest()
{
    delete ui;
}

void PanRailTest::resizeEvent( QResizeEvent *event )
{
    ui->id_widget->move( 100, 100 );
    ui->id_widget->resize( 300, 400 );
    ui->id_widget->setStyleSheet( "border-width: 1px;border-style: solid;border-color: rgb(255, 170, 0);" );
}
