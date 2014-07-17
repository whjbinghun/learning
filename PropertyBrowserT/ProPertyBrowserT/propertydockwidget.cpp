#include "propertydockwidget.h"
#include <QDebug>

PropertyDockWidget::PropertyDockWidget(QWidget *parent) :
    QDockWidget(parent)
    ,mp_property_browser( NULL )
    ,mp_btn_template_data( NULL )
    ,mp_btn_diagram( NULL )
{
    mp_property_browser = new PropertyBrowserA( this );
    setWidget( mp_property_browser->get_tree_property_browser() );
    init_button();
}

PropertyDockWidget::~PropertyDockWidget()
{
    delete mp_property_browser;
    delete mp_btn_template_data;
    delete mp_btn_diagram;
}

void PropertyDockWidget::resizeEvent( QResizeEvent *event )
{
    int n_height = 20;
    mp_property_browser->move( 0, 0 );
    mp_property_browser->resize( width(), height() );
    mp_btn_template_data->move( 0, height()-n_height*2 );
    mp_btn_template_data->resize( width(), n_height );
    mp_btn_diagram->move( 0, height()-n_height );
    mp_btn_diagram->resize( width(), n_height );
    qDebug()<<"PropertyDockWidget::resizeEvent"<<width()<<height();
}

void PropertyDockWidget::init_button()
{
    mp_btn_template_data = new QPushButton( this );
    mp_btn_diagram = new QPushButton( this );
    mp_btn_template_data->setText( "复制线温度数据" );
    mp_btn_diagram->setText( "复制线曲线图" );
    connect( mp_btn_template_data, SIGNAL( clicked() ), this, SLOT( slot_copy_templdate_data() ) );
    connect( mp_btn_diagram, SIGNAL( clicked() ), this, SLOT( slot_copy_diagram() ) );
}

void PropertyDockWidget::slot_copy_templdate_data()
{

}

void PropertyDockWidget::slot_copy_diagram()
{

}
