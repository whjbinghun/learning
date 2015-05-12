#include "dataconfig.h"

#define YF_ANA_LINE_COLOR "ANA_LINE_COLOR"
#define YF_ANA_LINE_WIDTH "ANA_LINE_WIDTH"
#define YF_ANA_LINE_STYLE "ANA_LINE_STYLE"

#define YF_CRUISE_AREA_SIZE "CRUISE_AREA_SIZE"
#define YF_CRUISE_CLR_IN "CRUISE_CLR_IN"
#define YF_CRUISE_CLR_OUT "CRUISE_CLR_OUT"
#define YF_CRUISE_CLR_PRESET "CRUISE_CLR_PRESET"

#define YF_CSERVER_HOST_IP "SERVER_HOST_IP"
#define YF_REALTIME_SERVER_PORT "REALTIME_SERVER_PORT"
#define YF_REPLAY_SERVER_PORT "PLAYBACK_SERVER_PORT"
#define YF_MONITOR_USER_NAME "MONITOR_USER_NAME"
#define YF_MONITOR_USER_PASSWD "MONITOR_USER_PASSWD"


//SINGLETON_IMPLEMENT( DataConfig )

DataConfig* DataConfig::s_instance=0;
QMutex DataConfig::s_cs;

DataConfig *DataConfig::get_instance()
{
if ( s_instance == 0)
    {
       QMutexLocker lock( &s_cs );
       if ( s_instance == 0)
       {
          s_instance = new DataConfig();
       }
    }
    return s_instance;
}

DataConfig::DataConfig()
{

}

DataConfig::~DataConfig()
{

}

bool DataConfig::init_cfg( QString str_path )
{
    if ( 0 == m_file_cfg.load_cfg( str_path ) ) {
        m_str_path = str_path;
        return true;
    }

    return false;
}

bool DataConfig::save()
{
    m_file_cfg.save_cofig_file( m_str_path );
}

bool DataConfig::set_clr( int n_type, QColor &clr )
{
    QMutexLocker lc( &m_mutex );

    QString str_clr = QString::number( clr.rgb() ) ;
    switch( n_type )
    {
    case AnaLineClr: m_file_cfg.set_config_info( YF_ANA_LINE_COLOR, str_clr ); break;
    case CruiseClrIn: m_file_cfg.set_config_info( YF_CRUISE_CLR_IN, str_clr ); break;
    case CruiseClrOut: m_file_cfg.set_config_info( YF_CRUISE_CLR_OUT, str_clr ); break;
    case CruiseClrPreset: m_file_cfg.set_config_info( YF_CRUISE_CLR_PRESET, str_clr ); break;
    default: return false;
    }

    return true;
}

bool DataConfig::set_value( int n_type, int n_value )
{
    QMutexLocker lc( &m_mutex );

    QString str_value = QString::number( n_value );
    switch( n_type )
    {
    case AnaLineWidth: m_file_cfg.set_config_info( YF_ANA_LINE_WIDTH, str_value ); break;
    case AnaLineStyle: m_file_cfg.set_config_info( YF_ANA_LINE_STYLE, str_value ); break;
    case CruiseAreaSize: m_file_cfg.set_config_info( YF_CRUISE_AREA_SIZE, str_value ); break;
    default: return false;
    }

    return true;
}

bool DataConfig::get_ana_line_info( QColor &clr, int &n_line_width, int &n_line_style )
{
    QMutexLocker lc( &m_mutex );

    QString str_clr( QString::number( qRgb( 0,255,255 ) ) );
    QString str_width( "1" ), str_style( "1" );

    m_file_cfg.get_config_info( YF_ANA_LINE_COLOR, str_clr );
    m_file_cfg.get_config_info( YF_ANA_LINE_WIDTH, str_width );
    m_file_cfg.get_config_info( YF_ANA_LINE_STYLE, str_style );

    clr.setRgb( QRgb( str_clr.toUInt() ) );
    n_line_width = str_width.toInt();
    n_line_style = str_style.toInt();

    return true;
}

bool DataConfig::set_ana_line_info( QColor &clr, int &n_line_width, int &n_line_style )
{
    QMutexLocker lc( &m_mutex );

    QString str_clr = QString::number( clr.rgb() ) ;
    m_file_cfg.set_config_info( YF_ANA_LINE_COLOR, str_clr );
    m_file_cfg.set_config_info( YF_ANA_LINE_WIDTH, QString::number( n_line_width ) );
    m_file_cfg.set_config_info( YF_ANA_LINE_STYLE, QString::number( n_line_style ) );

    return true;
}

bool DataConfig::get_map_cruise_int( int &n_area, QColor &clr_cru, QColor &clr_no_cru, QColor &clr_preset )
{
    QMutexLocker lc( &m_mutex );

    QString str_area( "20" );
    QString str_cru( QString::number( qRgb( 255,255,255 ) ) );
    QString str_no_cru( QString::number( qRgb( 0,255,0 ) ) );
    QString str_preset( QString::number( qRgb( 0,255,255 ) ) );

    m_file_cfg.get_config_info( YF_CRUISE_AREA_SIZE, str_area );
    m_file_cfg.get_config_info( YF_CRUISE_CLR_IN, str_cru );
    m_file_cfg.get_config_info( YF_CRUISE_CLR_OUT, str_no_cru );
    m_file_cfg.get_config_info( YF_CRUISE_CLR_PRESET, str_preset );

    n_area = str_area.toInt();
    clr_cru.setRgb( QRgb( str_cru.toUInt() ) );
    clr_no_cru.setRgb( QRgb( str_no_cru.toUInt() ) );
    clr_preset.setRgb( QRgb( str_preset.toUInt() ) );

    return true;
}

bool DataConfig::set_map_cruise_int( int &n_area, QColor &clr_cru, QColor &clr_no_cru, QColor &clr_preset )
{
    QMutexLocker lc( &m_mutex );

    QString str_cru( QString::number( clr_cru.rgb() ) );
    QString str_no_cru( QString::number( clr_no_cru.rgb()  ) );
    QString str_preset( QString::number( clr_preset.rgb()  ) );

    m_file_cfg.set_config_info( YF_CRUISE_AREA_SIZE, QString::number( n_area ) );
    m_file_cfg.set_config_info( YF_CRUISE_CLR_IN, str_cru );
    m_file_cfg.set_config_info( YF_CRUISE_CLR_OUT, str_no_cru );
    m_file_cfg.set_config_info( YF_CRUISE_CLR_PRESET, str_preset );

    return true;
}

bool DataConfig::get_login_info( QString &str_ip, QString &str_port, QString &str_user, QString &str_password, bool b_realtime_status )
{
    QMutexLocker lc( &m_mutex );

    m_file_cfg.get_config_info( YF_CSERVER_HOST_IP, str_ip );
    if( b_realtime_status ) {
        m_file_cfg.get_config_info( YF_REALTIME_SERVER_PORT, str_port );
    } else {
        m_file_cfg.get_config_info( YF_REPLAY_SERVER_PORT, str_port );
    }
    m_file_cfg.get_config_info( YF_MONITOR_USER_NAME, str_user );
    m_file_cfg.get_config_info( YF_MONITOR_USER_PASSWD, str_password );

    return true;
}

bool DataConfig::set_login_info( const QString &str_ip, const QString &str_port, const QString &str_user, const QString &str_password, bool b_realtime_status )
{
    QMutexLocker lc( &m_mutex );

    m_file_cfg.set_config_info( YF_CSERVER_HOST_IP, str_ip );
    if( b_realtime_status ) {
        m_file_cfg.set_config_info( YF_REALTIME_SERVER_PORT, str_port );
    } else {
        m_file_cfg.set_config_info( YF_REPLAY_SERVER_PORT, str_port );
    }
    m_file_cfg.set_config_info( YF_MONITOR_USER_NAME, str_user );
    m_file_cfg.set_config_info( YF_MONITOR_USER_PASSWD, str_password );

    return true;
}
