#include "wndtempana.h"
#include "wndchart.h"
//#include "wndpropertybrowser.h"
#include "chartpoints.h"
#include <QMessageBox>
#include "../common-funtion/msclientlog.h"

WndTempAna::WndTempAna(QWidget *parent,bool b_show_property) :
    QWidget(parent)
    ,mp_splitter_main( NULL )
    ,mp_wnd_chart( NULL )
//    ,mp_wnd_property( NULL )
    ,mb_preset_ana( false )
{
    init_splitter( b_show_property );
}

void WndTempAna::resizeEvent( QResizeEvent *event )
{
    if( 0 != mp_splitter_main ) mp_splitter_main->resize( size() );
    else mp_wnd_chart->resize( size() );
}

void WndTempAna::init_splitter(bool b_show_property)
{
    //主splitter，以下填充text和子splitter
    if( b_show_property )
    {
        mp_splitter_main = new QSplitter( Qt::Horizontal, this );
        mp_wnd_chart = new WndChartTrend( mp_splitter_main );
        mp_wnd_chart->setMouseTracking( true );

        mp_splitter_main->setStretchFactor ( 0, 3 );

//        mp_wnd_property = new WndPropertyBrowser( mp_wnd_chart, mp_splitter_main );
//        mp_wnd_property->resize( 80, height() );//设置编辑框的宽度
//        mp_splitter_main->setStretchFactor ( 1, 1 );

        //设置主splitter
        mp_splitter_main->setWindowTitle( QObject::tr( "Splitter" ) );
        mp_splitter_main->show();
    }
    else
    {
       mp_wnd_chart = new WndChartTrend( this );
       mp_wnd_chart->setMouseTracking( true );
    }
}
int  WndTempAna::get_index()
{
    if ( mp_wnd_chart ) {
        return mp_wnd_chart->get_index();
    }
    return -1;
}

bool WndTempAna::get_is_preset_ana()
{
    return mb_preset_ana;
}

void WndTempAna::set_curr_preset( const int n_curr_preset_no )
{
    mp_wnd_chart->set_preset( n_curr_preset_no );
}


void WndTempAna::set_preset_str_name(QString &str_preset_name )
{
    mp_wnd_chart->set_preset_name( str_preset_name );
}

//WndPropertyBrowser *WndTempAna::get_property_browser()
//{
//    return mp_wnd_property;
//}

void WndTempAna::init_wnd_ctrl(int n_num_group, bool b_realtime , int n_index, bool b_preset_ana )
{
    if ( mp_wnd_chart ) {
        mp_wnd_chart->set_group_num( n_num_group, n_index );
        mp_wnd_chart->set_index( n_index );
        mp_wnd_chart->set_type( b_realtime );
    }

//    if ( mp_wnd_property ) {
//        DeviceGroup dev_group;
//        CDataMode::Instance()->get_group_info( n_num_group, dev_group );

//        QString str_sn = dev_group.dev_ir.str_sn+"_real";
//        mp_wnd_property->init_wnd_ctrl( n_num_group, str_sn, n_index );
//    }\

    mb_preset_ana = b_preset_ana;
}
