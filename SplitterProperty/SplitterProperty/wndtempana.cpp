#include "wndtempana.h"
#include "wndchart.h"
#include "wndpropertybrowser.h"

WndTempAna::WndTempAna(QWidget *parent) :
    QWidget(parent)
    ,mp_splitter_main( NULL )
    ,mp_wnd_chart( NULL )
    ,mp_wnd_property( NULL )
{
    init_splitter();
}

void WndTempAna::resizeEvent( QResizeEvent *event )
{
    mp_splitter_main->resize( width(), height() );
}

void WndTempAna::init_splitter()
{
    int n_space = 50;
    //主splitter，以下填充text和子splitter
    mp_splitter_main = new QSplitter( Qt::Horizontal, this );
    mp_wnd_chart = new WndChart( mp_splitter_main );
    mp_wnd_chart->resize( width()-n_space, height() );
    mp_wnd_property = new WndPropertyBrowser( mp_splitter_main );
    mp_wnd_property->resize( n_space, height() );//设置编辑框的宽度
    //设置主splitter
    mp_splitter_main->setStretchFactor( 1, 1 );
    mp_splitter_main->setWindowTitle( QObject::tr( "Splitter" ) );
    mp_splitter_main->show();
}
