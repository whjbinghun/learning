#include "splitterproperty.h"
#include "ui_splitterproperty.h"

SplitterProperty::SplitterProperty(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SplitterProperty)
    ,mp_temp_ana( NULL )
{
    ui->setupUi(this);
    mp_temp_ana = new WndTempAna( this );

}

SplitterProperty::~SplitterProperty()
{
    delete ui;
}

void SplitterProperty::resizeEvent(QResizeEvent *event)
{
    mp_temp_ana->move( 0, 0 );
    mp_temp_ana->resize( width(), height() );
}
