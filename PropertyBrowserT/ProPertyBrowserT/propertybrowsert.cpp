#include "propertybrowsert.h"
#include "ui_propertybrowsert.h"
#include <QDebug>

PropertyBrowserT::PropertyBrowserT(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PropertyBrowserT)
    ,mp_property_browser( NULL )
    ,mp_btn_template_data( NULL )
    ,mp_btn_diagram( NULL )
{
    ui->setupUi(this);

    init_button();
    mp_property_browser = new PropertyBrowserA( this );

}

PropertyBrowserT::~PropertyBrowserT()
{
    delete mp_btn_template_data;
    delete mp_btn_diagram;

    delete ui;
}

void PropertyBrowserT::resizeEvent(QResizeEvent *event)
{
    int n_height = 20;
    mp_btn_template_data->move( 0, 0 );
    mp_btn_template_data->resize( width(), n_height );
    mp_btn_diagram->move( 0, n_height );
    mp_btn_diagram->resize( width(), n_height );

    mp_property_browser->move( 0, n_height*2 );
    mp_property_browser->resize( ui->centralWidget->width(), ui->centralWidget->height()-n_height*2 );
    qDebug()<<"PropertyBrowserT::resizeEvent"<<ui->centralWidget->width()<<ui->centralWidget->height();
}

void PropertyBrowserT::init_button()
{
    mp_btn_template_data = new QPushButton( this );
    mp_btn_diagram = new QPushButton( this );
    mp_btn_template_data->setText( "复制线温度数据" );
    mp_btn_diagram->setText( "复制线曲线图" );
    connect( mp_btn_template_data, SIGNAL( clicked() ), this, SLOT( slot_copy_templdate_data() ) );
    connect( mp_btn_diagram, SIGNAL( clicked() ), this, SLOT( slot_copy_diagram() ) );
}

void PropertyBrowserT::slot_copy_templdate_data()
{

}

void PropertyBrowserT::slot_copy_diagram()
{

}
