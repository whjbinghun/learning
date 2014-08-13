#ifndef CFGFILEINFO_H
#define CFGFILEINFO_H

#include <QMap>
#include <QString>

class CfgFileInfo
{
    //Q_OBJECT
public:
    explicit CfgFileInfo();
public:
    bool is_valid_cfg_line( const char *src );
    bool parser_cfg( const char *str, char *&key, char *&value );
    int load_cfg( QString &str_file_name );
    bool get_config_info( QString str_key, QString &str_value );
    bool set_config_info( QString str_key, QString str_value );
    void save_cofig_file( QString &str_file_name );
    void test_map();
private:
    QMap<QString, QString> m_map_config;
};

#endif // CFGFILEINFO_H
