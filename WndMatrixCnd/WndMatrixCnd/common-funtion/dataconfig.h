#ifndef DATACONFIG_H
#define DATACONFIG_H

#include <QColor>
#include <QMutex>

#include "singleton.h"
#include "../common-ui/cfgfileinfo.h"

class DataConfig
{
    //SINGLETON_DECLARE( DataConfig )/*
public:
    static DataConfig *s_instance;
    static QMutex s_cs;
public:
    static DataConfig* get_instance();

public:
    DataConfig();
    ~DataConfig();

public:
    bool init_cfg( QString str_path );
    bool save();

    enum{
        AnaLineClr,
        AnaLineWidth,
        AnaLineStyle,
        CruiseAreaSize,
        CruiseClrIn,
        CruiseClrOut,
        CruiseClrPreset
    };

    bool set_clr( int n_type, QColor &clr );
    bool set_value( int n_type, int n_value );

    bool get_ana_line_info( QColor &clr, int &n_line_width, int &n_line_style );
    bool set_ana_line_info( QColor &clr, int &n_line_width, int &n_line_style );

    bool get_map_cruise_int( int &n_area, QColor &clr_cru, QColor &clr_no_cru, QColor &clr_preset );
    bool set_map_cruise_int( int &n_area, QColor &clr_cru, QColor &clr_no_cru, QColor &clr_preset );

    bool get_login_info( QString &str_ip, QString &str_port, QString &str_user, QString &str_password , bool b_realtime_status );
    bool set_login_info( const QString &str_ip, const QString &str_port, const QString &str_user, const QString &str_password, bool b_realtime_status );

private:
    QMutex m_mutex;
    CfgFileInfo m_file_cfg;
    QString m_str_path;
};

#endif // DATACONFIG_H
