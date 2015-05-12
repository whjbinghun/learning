#include "msanacolorsetting.h"\


MSAnaColorSetting* MSAnaColorSetting::m_ana_color_setting = NULL;

MSAnaColorSetting::MSAnaColorSetting(  ):
    mstr_path( "" )
{

    mstr_path = QApplication::applicationDirPath() + "/config/ana_chart_color.ini" ;
    m_settings = new QSettings( mstr_path, QSettings::IniFormat, 0 );
}

MSAnaColorSetting::~MSAnaColorSetting()
{
    if ( m_settings ){
        delete m_settings;
        m_settings = 0;
    }

}

MSAnaColorSetting *MSAnaColorSetting::Instance()
{
    if ( NULL == m_ana_color_setting ){
        return new MSAnaColorSetting();
    }
    return m_ana_color_setting;
}

void MSAnaColorSetting::Release()
{
    if ( m_ana_color_setting ){
        delete m_ana_color_setting;
        m_ana_color_setting = NULL;
    }
}

QStringList MSAnaColorSetting::get_ana_color_keys( int n_group_number, int n_index )
{
    QStringList str_list;
    m_settings->beginGroup( QString( "%1-%2" ).arg( n_group_number ).arg( n_index) );
    str_list = m_settings->allKeys();
    m_settings->endGroup();

    return str_list;
}

void MSAnaColorSetting::clear_ana_colors(int n_group_number, int n_index)
{
    m_settings->beginGroup( QString( "%1-%2" ).arg( n_group_number ).arg( n_index) );
    m_settings->remove( "" );
    m_settings->endGroup();
}

void MSAnaColorSetting::set_ana_chart_color(int n_group_number, int n_index, QString str_name, QColor color )
{
    return m_settings->setValue( QString( "/%1-%2/%3" ).arg( n_group_number ).arg( n_index ).arg( str_name ), color );
}

QColor MSAnaColorSetting::get_ana_chart_color( int n_group_number, int n_index, QString str_name )
{
    return m_settings->value( QString( "/%1-%2/%3" ).arg( n_group_number ).arg( n_index ).arg( str_name ) ).value<QColor>();
}
