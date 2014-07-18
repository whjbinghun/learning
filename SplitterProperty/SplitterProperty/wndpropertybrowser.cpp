#include "wndpropertybrowser.h"

WndPropertyBrowser::WndPropertyBrowser(QWidget *parent) :
    QWidget(parent)
    ,mp_property_browser( NULL )
    ,mp_btn_template_data( NULL )
    ,mp_btn_diagram( NULL )
{
    mp_property_browser = new PropertyBrowserA( this );
    init_button();
}

void WndPropertyBrowser::resizeEvent( QResizeEvent *event )
{
    int n_space = 20;
    mp_property_browser->resize( width(), height()-n_space*2 );
    mp_btn_template_data->move( 0, height()-n_space*2 );
    mp_btn_template_data->setFixedWidth( width() );
    mp_btn_diagram->move( 0, height()-n_space );
    mp_btn_diagram->setFixedWidth( width() );
}

void WndPropertyBrowser::init_button()
{
    mp_btn_template_data = new QPushButton( this );
    mp_btn_diagram = new QPushButton( this );
    mp_btn_template_data->setText( "复制线温度数据" );
    mp_btn_diagram->setText( "复制线曲线图" );
    connect( mp_btn_template_data, SIGNAL( clicked() ), this, SLOT( slot_copy_templdate_data() ) );
    connect( mp_btn_diagram, SIGNAL( clicked() ), this, SLOT( slot_copy_diagram() ) );
}

void WndPropertyBrowser::slot_copy_templdate_data()
{

}

void WndPropertyBrowser::slot_copy_diagram()
{

}
