#include "wndpropertybrowser.h"
#include "chartpoints.h"
#include "wndchart.h"
#include "datachartmode.h"
#include <QClipboard>
#include <QApplication>
#include <QSettings>

WndPropertyBrowser::WndPropertyBrowser(WndChart *p_wnd_chart, QWidget *parent) :
    QWidget(parent)
//    ,mp_btn_show_chart( NULL )
//    ,mp_btn_copy_temp( NULL )
//    ,mp_btn_copy_chart( NULL )
    ,mp_range_max( NULL )
    ,mp_range_min( NULL )
    ,mp_item_setting( NULL )
    ,mp_parent( parent )
    ,mp_wnd_chart( p_wnd_chart )
    ,mp_int_manager( NULL )
    ,p_enum_manager(NULL)
    ,mstr_sn( "" )
    ,mn_index( -1 )
    ,mn_group_number( 0 )
    ,mn_temp_update_mode( 0 )
    ,mf_max( 80.0 )
    ,mf_min( 0.0 )
    ,mn_storage_time_interval_index( 1 )
    ,mnu_clear_period( 43200 )
{
    init_button();

    init_property();
    create_property_setting();

    resize( 50, height() );
}

WndPropertyBrowser::~WndPropertyBrowser()
{
    set_ana_chart_setting();
//    QList< QtAbstractPropertyManager * >::iterator it_item =  m_list_property_manager.begin();
//    for ( ; it_item != m_list_property_manager.end(); ++it_item ) {
//        QtAbstractPropertyManager *p_item = *it_item;
//        if ( p_item ) { delete p_item; }
//    }
//    m_list_property_manager.clear();

//    QList< void * >::iterator it_factory = m_list_property_factory.begin();
//    for ( ; it_factory != m_list_property_factory.end(); ++it_factory ) {
//        void *p_item = *it_factory;
//        if ( p_item ) { delete p_item; }
//    }
//    m_list_property_factory.clear();

//    if ( mp_tree_property ) {
//        delete mp_tree_property;
//        mp_tree_property = 0;
//    }
}

void WndPropertyBrowser::resizeEvent( QResizeEvent *event )
{
    // int n_btn_height = 20;

    mp_tree_property->move( 0, 0 );
    mp_tree_property->resize( width(), height()/*-n_btn_height*3*/ );

//    mp_btn_copy_chart->move( 0, height()-n_btn_height );
//    mp_btn_copy_chart->resize( QSize( width(), n_btn_height ) );
//    mp_btn_copy_temp->move( 0, height()-n_btn_height*2 );
//    mp_btn_copy_temp->resize( QSize( width(), n_btn_height ) );
//    mp_btn_show_chart->move( 0, height()-n_btn_height*3 );
//    mp_btn_show_chart->resize( QSize( width(), n_btn_height ) );
}

void WndPropertyBrowser::init_button()
{
//    mp_btn_show_chart = new QPushButton( this );
//    mp_btn_copy_temp = new QPushButton( this );
//    mp_btn_copy_chart = new QPushButton( this );

//    mp_btn_show_chart->setText( "显示整体温度" );
//    mp_btn_copy_temp->setText( "复制趋势温度数据" );
//    mp_btn_copy_chart->setText( "复制趋势曲线图" );

//    connect( mp_btn_show_chart, SIGNAL( clicked() ), this, SLOT( slot_show_chart() ) );
//    connect( mp_btn_copy_temp, SIGNAL( clicked() ), this, SLOT( slot_copy_temp() ) );
//    connect( mp_btn_copy_chart, SIGNAL( clicked() ), this, SLOT( slot_copy_chart() ) );
}

void WndPropertyBrowser::init_property()
{
    mp_tree_property = new QtTreePropertyBrowser( this );

    mp_tree_property->setPropertiesWithoutValueMarked( true );
    mp_tree_property->setRootIsDecorated( false );
    mp_tree_property->show();
}

void WndPropertyBrowser::create_property_setting()
{
    if ( !mp_tree_property ) return;

    QtProperty *p_item = 0;

    QStringList list_string;
    list_string << "自动" << "手动";
    QtVariantPropertyManager *p_value_property = new QtVariantPropertyManager( this  );
    /*QtEnumPropertyManager */p_enum_manager = new QtEnumPropertyManager( this );
    QtIntPropertyManager *p_int_manager = new QtIntPropertyManager( this );

    QtVariantEditorFactory *p_variant_factory= new QtVariantEditorFactory( this );
    QtEnumEditorFactory *p_enum_factory = new QtEnumEditorFactory( this );
    QtSpinBoxFactory *p_spin_box_factory = new QtSpinBoxFactory( this );

    QtProperty *p_top_item = p_value_property->addProperty( QtVariantPropertyManager::groupTypeId(), "分析设置" );


    QtProperty * p_storage_item = p_value_property->addProperty( QtVariantPropertyManager::groupTypeId(), "更新时间" );

    QtProperty * p_display_time_item = p_value_property->addProperty( QtVariantPropertyManager::groupTypeId(), "曲线显示" );


    QStringList list_time_string;
    list_time_string << "无间隔"<<"1"<<"2"<<"5"<<"10";
    p_item = p_enum_manager->addProperty( "时间间隔(秒)" );
    p_enum_manager->setEnumNames( p_item, list_time_string);
    p_storage_item->addSubProperty( p_item );
    mp_storage_interval_item = p_item;

    p_item = p_enum_manager->addProperty("温度范围");
    p_enum_manager->setEnumNames( p_item, list_string );
    p_top_item->addSubProperty( p_item );
    mp_temp_update_mode_item = p_item;

    p_item = p_int_manager->addProperty( "最大温度" );
    p_int_manager->setRange( p_item, -100, 999999);
    p_top_item->addSubProperty( p_item );
    mp_range_max = p_item;

    p_item = p_int_manager->addProperty( "最小温度" );
    p_int_manager->setRange( p_item, -100, 999999);
    p_top_item->addSubProperty( p_item );
    mp_range_min = p_item;

    QStringList list_display_time_string;
    list_display_time_string << "1"<<"2"<<"5"<<"12"<<"24";
    p_item = p_enum_manager->addProperty( "总时间(小时)" );
    p_enum_manager->setEnumNames( p_item, list_display_time_string );
    p_display_time_item->addSubProperty( p_item );
    mp_display_time_item = p_item;


    mp_int_manager = p_int_manager;
    mp_item_setting = p_top_item;
    mp_item_setting->removeSubProperty( mp_range_max );
    mp_item_setting->removeSubProperty( mp_range_min );

    m_list_property_manager.push_back( p_enum_manager );
    m_list_property_manager.push_back( p_int_manager );
    m_list_property_manager.push_back( p_value_property );

    connect( p_enum_manager, SIGNAL( valueChanged( QtProperty*,int ) ), this, SLOT( slot_property_temp_change( QtProperty*,int ) ) );
    connect( p_int_manager, SIGNAL( valueChanged( QtProperty*,int ) ), this, SLOT( slot_property_value_change( QtProperty*,int ) ) );

    m_list_property_factory.push_back( ( void * )p_variant_factory );
    m_list_property_factory.push_back( ( void * )p_enum_factory );
    m_list_property_factory.push_back( ( void * )p_spin_box_factory );

    mp_tree_property->setFactoryForManager( p_value_property, p_variant_factory );
    mp_tree_property->setFactoryForManager( p_enum_manager, p_enum_factory );
    mp_tree_property->setFactoryForManager( p_int_manager, p_spin_box_factory );
    mp_tree_property->addProperty( p_top_item );
    mp_tree_property->addProperty( p_storage_item );
    mp_tree_property->addProperty( p_display_time_item );
}

void WndPropertyBrowser::create_property_ana( PropertyInfo &pro_info )
{
    QtVariantPropertyManager *p_value_property = new QtVariantPropertyManager( this  );
    QtColorPropertyManager *p_color_property = new QtColorPropertyManager( this );

    QtVariantEditorFactory *p_variant_factory= new QtVariantEditorFactory( this );
    QtColorEditorFactory *p_color_factory = new QtColorEditorFactory( this );

    QtProperty *p_top_item = p_value_property->addProperty( QtVariantPropertyManager::groupTypeId(), "分析" );

    QtProperty *p_clr_item = p_color_property->addProperty( "颜色" );
    p_color_property->setValue( p_clr_item, pro_info.p_chart->get_line_color() );
    p_top_item->addSubProperty( p_clr_item );

    m_list_property_manager.push_back( p_value_property );
    m_list_property_manager.push_back( p_color_property );

    connect( p_color_property, SIGNAL( valueChanged(QtProperty*,QColor)), this, SLOT(slot_property_value_change(QtProperty*,QColor) ) );

    mp_tree_property->setFactoryForManager( p_value_property, p_variant_factory );
    mp_tree_property->setFactoryForManager( p_color_property, p_color_factory );
    mp_tree_property->addProperty( p_top_item );

    pro_info.p_property_clr = p_clr_item;
    pro_info.p_property_group = p_top_item;
}

void WndPropertyBrowser::remove_property( QtProperty *p_item )
{
    if ( p_item ) {
        mp_tree_property->removeProperty( p_item );
    }
}

void WndPropertyBrowser::add_property_ana(YHANDLE h_ana, int n_type, ChartPoints *p_chart)
{
    QMap< YHANDLE, QMap< int , PropertyInfo > >::iterator it_ana = m_map_ana_property.find( h_ana );
    if ( it_ana != m_map_ana_property.end() ) {
        QMap< int , PropertyInfo >::iterator it_pro = it_ana.value().find( n_type );
        if ( it_pro != it_ana.value().end() ) {
            it_pro.value().p_chart = p_chart;
            return;
        } else {
            PropertyInfo pro_info;
            pro_info.p_chart = p_chart;
            create_property_ana( pro_info );
            it_ana.value().insert( n_type, pro_info );
        }
    } else {
        QMap< int , PropertyInfo > map_pty_info;
        PropertyInfo pro_info;
        pro_info.p_chart = p_chart;
        create_property_ana( pro_info );

        map_pty_info.insert( n_type, pro_info );
        m_map_ana_property.insert( h_ana, map_pty_info );
    }
}

void WndPropertyBrowser::remove_clr_property( QtProperty *p_clr_item )
{
    QMap< YHANDLE, QMap< int, PropertyInfo> >::iterator it = m_map_ana_property.begin();
    for ( ; it != m_map_ana_property.end(); ++it ) {
        QMap< int, PropertyInfo>::iterator it_pro = it.value().begin();
        for ( ; it_pro != it.value().begin(); ++it_pro ) {
            if ( it_pro.value().p_property_clr == p_clr_item ) {
                remove_property( it_pro.value().p_property_group );
                it.value().erase( it_pro );
                return ;
            }
        }
    }
}

ChartPoints *WndPropertyBrowser::find_chart(QtProperty *p_clr_item)
{
    QMap< YHANDLE, QMap< int, PropertyInfo> >::iterator it = m_map_ana_property.begin();
    for ( ; it != m_map_ana_property.end(); ++it ) {
        QMap< int, PropertyInfo>::iterator it_pro = it.value().begin();
        for ( ; it_pro != it.value().end(); ++it_pro ) {
            if ( it_pro.value().p_property_clr == p_clr_item ) {
                return it_pro.value().p_chart;
            }
        }
    }

    return 0;
}

void WndPropertyBrowser::delete_property_ana( YHANDLE h_ana, int n_type )
{
    QMap< YHANDLE, QMap< int, PropertyInfo> >::iterator it = m_map_ana_property.find( h_ana );
    if ( it == m_map_ana_property.end() ) { return; }

    QMap< int, PropertyInfo>::iterator it_pro = it.value().find( n_type );
    if ( it_pro == it.value().end() ) { return; }

    QtProperty *p_property = it_pro.value().p_property_group;
    remove_property( p_property );

    it.value().erase( it_pro );

    if ( it.value().empty() ) {
        m_map_ana_property.erase( it );
    }
}

void WndPropertyBrowser::delete_property_ana( YHANDLE h_ana )
{
    QMap< YHANDLE, QMap< int, PropertyInfo> >::iterator it = m_map_ana_property.find( h_ana );
    if ( it == m_map_ana_property.end() ) { return; }

    QMap< int, PropertyInfo>::iterator it_pro = it.value().begin();
    for ( ; it_pro != it.value().end(); ++it_pro ) {
        QtProperty *p_property = it_pro.value().p_property_group;
        remove_property( p_property );
    }

    m_map_ana_property.erase( it );
}

void WndPropertyBrowser::set_storage_interval_time( int time )
{
    int n_val = 1;
    if ( time == 5){
        n_val = 3;
    }else if ( time == 10 ){
        n_val = 10;
    }else{
        n_val = time;
    }
    p_enum_manager->setValue( mp_storage_interval_item, n_val );
}

void WndPropertyBrowser::slot_property_temp_change( QtProperty *property , int n_val )
{
    if ( property == mp_storage_interval_item ){
        mn_storage_time_interval_index = n_val;
        if ( n_val == 3 ){
            CDataChartMode::Instance()->set_collect_temp_interval_time( mstr_sn, mn_index, 5 );
        }else if ( n_val == 4 ){
            CDataChartMode::Instance()->set_collect_temp_interval_time( mstr_sn, mn_index, 10 );
        }else{
            CDataChartMode::Instance()->set_collect_temp_interval_time( mstr_sn, mn_index, n_val );
        }
    }
    else if ( property == mp_temp_update_mode_item )
    {
        if ( n_val == 1 ) {
            float f_min, f_max;
            mp_wnd_chart->set_axis_x_auto_range( false );
            mp_wnd_chart->get_axis_x_range( f_min, f_max );

            mp_int_manager->setValue( mp_range_max, (int)(f_max + 0.5) );
            mp_int_manager->setValue( mp_range_min, (int)(f_min + 0.5) );
            mp_item_setting->addSubProperty( mp_range_max );
            mp_item_setting->addSubProperty( mp_range_min );


            mn_temp_update_mode = 1;
        } else {
            mp_wnd_chart->set_axis_x_auto_range( true );
            mp_item_setting->removeSubProperty( mp_range_max );
            mp_item_setting->removeSubProperty( mp_range_min );
            mn_temp_update_mode = 0;
        }
    }
    else if ( property == mp_display_time_item )
    {
        if ( n_val == 0 ){
            mnu_clear_period = 1 * 60 * 60;
        } else if ( n_val == 1 ){
            mnu_clear_period = 2 * 60 * 60;
        } else if ( n_val == 2 ){
            mnu_clear_period = 5 * 60 * 60;
        } else if ( n_val == 3 ){
            mnu_clear_period = 12 * 60 * 60;
        } else if ( n_val == 4 ){
            mnu_clear_period = 24 * 60 * 60;
        }
        CDataChartMode::Instance()->set_chart_clear_period( mstr_sn, mn_index, mnu_clear_period );
    }
    set_ana_chart_setting();

}

void WndPropertyBrowser::slot_property_value_change( QtProperty *property, QColor clr )
{
    if ( property ) {
        ChartPoints *p_chart = find_chart( property );
        if ( p_chart ) {
            p_chart->set_line_color( clr );
            mp_wnd_chart->update();
        }
    }
}

void WndPropertyBrowser::slot_property_value_change( QtProperty *property, int n_val )
{
    if ( property == mp_range_max ) {
        int n_min = mp_wnd_chart->get_axis_x_min() - 0.5;
        if ( n_val < n_min + 5 ) {
            mp_int_manager->setValue( property, n_min+5 );
        } else {
            mp_wnd_chart->set_axis_x_max( n_val );
            mp_wnd_chart->update_axis_x();
        }
        mf_max = n_val;
    } else if ( property == mp_range_min ) {
        int n_max = mp_wnd_chart->get_axis_x_max() + 0.5;
        if ( n_val > n_max - 5 ) {
            mp_int_manager->setValue( property, n_max - 5 );
        } else {
            mp_wnd_chart->set_axis_x_min( n_val );
            mp_wnd_chart->update_axis_x();
        }
        mf_min = n_val;
    }
    set_ana_chart_setting();
}

void WndPropertyBrowser::slot_property_value_change( QtProperty *, bool b_val )
{

}

void WndPropertyBrowser::get_ana_chart_setting()
{
    QString str_path = QApplication::applicationDirPath() + "/config/ana_chart.ini" ;
    QSettings settings( str_path , QSettings::IniFormat, 0 );
    mn_temp_update_mode = settings.value( QString( "/%1-%2/update_mode" ).arg( mn_group_number ).arg( mn_index ), 0 ).toInt();
    mf_max = settings.value( QString( "/%1-%2/temp_max" ).arg( mn_group_number ).arg( mn_index ), 80.0 ).toFloat();
    mf_min = settings.value( QString( "/%1-%2/temp_min" ).arg( mn_group_number ).arg( mn_index ), 0.0 ).toFloat();
    mn_storage_time_interval_index = settings.value( QString( "/%1-%2/time_interval" ).arg( mn_group_number ).arg( mn_index ), 1 ).toInt();
    int n_display_time = settings.value( QString( "/%1-%2/clear_period" ).arg( mn_group_number ).arg( mn_index ), 3 ).toUInt();

    if ( n_display_time == 0 ){
        mnu_clear_period = 1 * 60 * 60;
    } else if ( n_display_time == 1 ){
        mnu_clear_period = 2 * 60 * 60;
    } else if ( n_display_time == 2 ){
        mnu_clear_period = 5 * 60 * 60;
    } else if ( n_display_time == 3 ){
        mnu_clear_period = 12 * 60 * 60;
    } else if ( n_display_time == 4 ){
        mnu_clear_period = 24 * 60 * 60;
    }

    if ( !mn_temp_update_mode ){
        p_enum_manager->setValue( mp_temp_update_mode_item, mn_temp_update_mode );
        mp_wnd_chart->set_axis_x_auto_range( true );
    }else{
        mp_wnd_chart->set_axis_x_auto_range( false );
       // mp_wnd_chart->get_axis_x_range( mf_min, mf_max );

        mp_int_manager->setValue( mp_range_max, (int)( mf_max + 0.5 ) );
        mp_int_manager->setValue( mp_range_min, (int)( mf_min + 0.5 ) );
        mp_item_setting->addSubProperty( mp_range_max );
        mp_item_setting->addSubProperty( mp_range_min );

        p_enum_manager->setValue( mp_temp_update_mode_item, mn_temp_update_mode );
    }
    p_enum_manager->setValue( mp_storage_interval_item, mn_storage_time_interval_index );
    set_chart_interval_time ( mn_storage_time_interval_index );


    p_enum_manager->setValue( mp_display_time_item, n_display_time );
    CDataChartMode::Instance()->set_chart_clear_period( mstr_sn, mn_index, mnu_clear_period );

}

void WndPropertyBrowser::set_ana_chart_setting()
{
    QString str_path = QApplication::applicationDirPath() + "/config/ana_chart.ini" ;

    QSettings settings( str_path , QSettings::IniFormat, 0 );
    settings.beginGroup( QString( "%1-%2" ).arg( mn_group_number ).arg( mn_index) );
    settings.remove( "" );
    settings.endGroup();
    settings.setValue( QString( "/%1-%2/update_mode" ).arg( mn_group_number ).arg( mn_index ), mn_temp_update_mode );
    settings.setValue( QString( "/%1-%2/time_interval" ).arg( mn_group_number ).arg( mn_index ) , mn_storage_time_interval_index );
    settings.setValue( QString( "/%1-%2/temp_max" ).arg( mn_group_number ).arg( mn_index ), QString( "%1" ).arg( mf_max ) );
    settings.setValue( QString( "/%1-%2/temp_min" ).arg( mn_group_number ).arg( mn_index ), QString( "%1" ).arg( mf_min ) );

    int n_display_time = 0;
    if ( mnu_clear_period == 1 * 60 * 60 ){
        n_display_time = 0;
    } else if ( mnu_clear_period == 2 * 60 * 60 ){
        n_display_time = 1;
    } else if ( mnu_clear_period == 5 * 60 * 60 ){
        n_display_time = 2;
    } else if ( mnu_clear_period == 12 * 60 * 60 ){
        n_display_time = 3;
    } else if ( mnu_clear_period == 24 * 60 * 60 ){
        n_display_time = 4;
    }
    settings.setValue( QString( "/%1-%2/clear_period" ).arg( mn_group_number ).arg( mn_index ), QString( "%1" ).arg( n_display_time ) );
}

void WndPropertyBrowser::set_chart_interval_time(int n_val)
{
    if ( n_val == 3 ){
        CDataChartMode::Instance()->set_collect_temp_interval_time( mstr_sn, mn_index, 5 );
    }else if ( n_val == 4 ){
        CDataChartMode::Instance()->set_collect_temp_interval_time( mstr_sn, mn_index, 10 );
    }else{
        CDataChartMode::Instance()->set_collect_temp_interval_time( mstr_sn, mn_index, n_val );
    }
}

void WndPropertyBrowser::slot_show_chart()
{
    mp_wnd_chart->set_axis_y_show_all();
    mp_wnd_chart->cal_axis_y_span();
    mp_wnd_chart->update_axis_y();
}

void WndPropertyBrowser::slot_copy_temp()
{
    QString str_copy;
    QMap< YHANDLE, QMap< int, PropertyInfo> >::iterator it = m_map_ana_property.begin();
    for ( ; it != m_map_ana_property.end(); ++it ) {
        QMap< int, PropertyInfo>::iterator it_pro = it.value().begin();
        for ( ; it_pro != it.value().end(); ++it_pro ) {
            if ( it_pro.value().p_chart ) {
                str_copy += it_pro.value().p_chart->get_all_temp_string() + "\r\n";
            }
        }
    }

    QClipboard *p_board = QApplication::clipboard();
    p_board->setText( str_copy );
}

void WndPropertyBrowser::slot_copy_chart()
{
    mp_wnd_chart->copy_chart();
}

void WndPropertyBrowser::init_wnd_ctrl(int n_group_number, QString str_sn, int n_index )
{
    mn_group_number  = n_group_number;
    mstr_sn         = str_sn;
    mn_index        = n_index;

    get_ana_chart_setting();
    set_ana_chart_setting();
}
