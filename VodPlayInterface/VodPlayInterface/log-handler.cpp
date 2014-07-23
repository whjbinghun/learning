#include <stdio.h>
#include <string>
#include <sys/time.h>
#include "log-handler.h"

using namespace std;

FILE *g_debug_file = NULL;
FILE *g_warning_file = NULL;
FILE *g_critical_file = NULL;
FILE *g_fatal_file = NULL;
string g_debug_file_name;
string g_warning_file_name;
string g_critical_file_name;
string g_fatal_file_name;
time_t g_cur_day = 0;

void message_output( QtMsgType type, const QMessageLogContext &context, const QString &msg )
{
    struct timeval now;
    gettimeofday( &now, NULL );
    struct tm local_time;
    localtime_r( &now.tv_sec, &local_time );

    if( ( now.tv_sec - g_cur_day ) >= 24*60*60 ) {
        struct tm begin_of_day = local_time;
        begin_of_day.tm_hour = 0;
        begin_of_day.tm_min = 0;
        begin_of_day.tm_sec = 0;
        g_cur_day = mktime( &begin_of_day );

        if( g_debug_file ) {
            fclose( g_debug_file );
            g_debug_file = NULL;
        }
        if( g_warning_file ) {
            fclose( g_warning_file );
            g_warning_file = NULL;
        }
        if( g_critical_file ) {
            fclose( g_critical_file );
            g_critical_file = NULL;
        }
        if( g_fatal_file ) {
            fclose( g_fatal_file );
            g_fatal_file = NULL;
        }

        char name_buf[128];
        memset( name_buf, 0, sizeof( name_buf ) );
        sprintf( name_buf, "debug_%04d%02d%02d.log", local_time.tm_year + 1900, local_time.tm_mon + 1, local_time.tm_mday );
        g_debug_file_name = name_buf;
        g_debug_file = fopen( g_debug_file_name.c_str(), "a" );

        memset( name_buf, 0, sizeof( name_buf ) );
        sprintf( name_buf, "warning_%04d%02d%02d.log", local_time.tm_year + 1900, local_time.tm_mon + 1, local_time.tm_mday );
        g_warning_file_name = name_buf;
        g_warning_file = fopen( g_warning_file_name.c_str(), "a" );

        memset( name_buf, 0, sizeof( name_buf ) );
        sprintf( name_buf, "critical_%04d%02d%02d.log", local_time.tm_year + 1900, local_time.tm_mon + 1, local_time.tm_mday );
        g_critical_file_name = name_buf;
        g_critical_file = fopen( g_critical_file_name.c_str(), "a" );

        memset( name_buf, 0, sizeof( name_buf ) );
        sprintf( name_buf, "fatal_%04d%02d%02d.log", local_time.tm_year + 1900, local_time.tm_mon + 1, local_time.tm_mday );
        g_fatal_file_name = name_buf;
        g_fatal_file = fopen( g_fatal_file_name.c_str(), "a" );
    }

    FILE *log_file;
    //string log_file_name;
    QByteArray localMsg = msg.toLocal8Bit();
    switch (type) {
    case QtDebugMsg:
        log_file = g_debug_file;
        //log_file_name = g_debug_file_name.c_str();
        break;
    case QtWarningMsg:
        log_file = g_warning_file;
        //log_file_name = g_warning_file_name.c_str();
        break;
    case QtCriticalMsg:
        log_file = g_critical_file;
        //log_file_name = g_critical_file_name.c_str();
        break;
    case QtFatalMsg:
        log_file = g_fatal_file;
        //log_file_name = g_fatal_file_name.c_str();
    }

    string code_file = context.file;
    string code_function = context.function;

    size_t pos = code_file.find_last_of( '/' );
    if( pos != string::npos ) {
        code_file = code_file.substr( pos + 1 );
    }

    pos = code_function.find_first_of( '(' );
    if( pos != string::npos ) {
        code_function = code_function.substr( 0, pos );
        pos = code_function.find_last_of( "::" );
        if( pos != string::npos ) {
            code_function = code_function.substr( pos + 1 );
        } else {
            pos = code_function.find_first_of( ' ' );
            if( pos != string::npos ) {
                code_function = code_function.substr( pos + 1 );
            }
        }
    }

    int i_ret = fprintf( log_file, "[%02d:%02d:%02d:%06d][%s:%u->%s]\t%s\n", local_time.tm_hour, local_time.tm_min, local_time.tm_sec, now.tv_usec, code_file.c_str(), context.line, code_function.c_str(), localMsg.constData() );
    //fflush( log_file );

    if( QtFatalMsg == type ) {
        abort();
    }
}
