#ifndef PARSECONFILE_H
#define PARSECONFILE_H
#include <QString>

bool parser_cfg( const char* src, char*& key, char*& value );
int load_cfg();
bool isValidCfgLine( const char* src );

QString get_ver();
QString get_sub_ver();
QString get_user_name();
QString get_user_pwd();
QString get_host_ip();
QString get_server_port();
void set_ver( QString str_ver );
void set_sub_ver( QString str_sub_ver );
void set_user_name( QString str_user_name );
void set_user_pwd( QString str_user_pwd );
void set_host_ip( QString str_host_ip );
void set_server_port( QString str_server_port );
void save_login_info();
#endif // PARSECONFILE_H
