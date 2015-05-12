#ifndef _H_MSLOG_
#define _H_MSLOG_

//#include <mutex>
//#include <condition_variable>
#include <vector>
#include <signal.h>
//#include <assert.h>
#include <unistd.h>

#include <string>
#include <string.h>

#include <Windows.h>
#include <direct.h>
#include <share.h>

#ifdef _WIN32
#define WIN32
#endif

using namespace std;

//常规日志
enum
{
    LOG_LEVEL_INFO       = 1,     //1
    LOG_LEVEL_WARNNING   = 1<<1,  //2
    LOG_LEVEL_ERROR      = 1<<2,  //4
    LOG_LEVEL_DEBUG      = 1<<3,  //8
    LOG_LEVEL_FATAL      = 1<<4,  //16


    LOG_LEVEL_MAX        = LOG_LEVEL_INFO | LOG_LEVEL_WARNNING | LOG_LEVEL_ERROR | LOG_LEVEL_DEBUG | LOG_LEVEL_FATAL,

};

//日志打印到本地或者远程
enum
{
    LOG_Local             = 1<<0,  //1
    LOG_Remote            = 1<<1,  //2
};


typedef struct tagSYSTEM_TIME_S
{
    tagSYSTEM_TIME_S()
    {
        year    = 0;
        month   = 0;
        day     = 0;
        hour    = 0;
        minute  = 0;
        second  = 0;
        millisecond = 0;
    }

    int year;
    int month;
    int day;
    int hour;
    int minute;
    int second;
    int millisecond;
}SYSTEM_TIME_S;


typedef void (*FLogCallback)( const char* message, SYSTEM_TIME_S system_time, void* user_data );


/// @brief 设置日志路径
/// @param[in] path
void set_directory( const char* path );

void set_remote_flag( unsigned int ui_remote_flag, const std::string& str_ip = "127.0.0.1", unsigned short ui_port = 8890);
/// @brief 设置日志级别
/// @param[in] level 日志级别
void set_level( unsigned int level );

/// @brief 设置操作日志级别
/// @param[in] level 日志操作级别
void set_opertor_level( unsigned int opertor_level );


void set_log_callback( FLogCallback f_log_callback, void* user_data );

/// @brief
/// @param[in] level 日志级别
/// @param[in] filename 文件名
/// @param[in] file_name_suffex 文件名后缀
/// @param[in] line 行
/// @param[in] function 函数
/// @param[in] format 格式

void print( unsigned int level, const char *file_name_suffex, const char*  filename,
            unsigned int line, const char *function, const char*  format, ... );


#define LOGER_PRINT(level, file_name_suffix, ... ) \
    print( level, file_name_suffix, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__ )

#define MS_LOGER_INFO( ... ) \
    print( LOG_LEVEL_INFO, NULL, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__ )
#define MS_LOGER_INFO_EX( file_name_suffix, ... ) \
    print( LOG_LEVEL_INFO, file_name_suffix, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__ )

#define MS_LOGER_WARNING( ... ) \
    print( LOG_LEVEL_WARNNING, NULL, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__ )
#define MS_LOGER_WARNING_EX( file_name_suffix, ...) \
    print( LOG_LEVEL_WARNNING, file_name_suffix, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__ )

#define MS_LOGER_ERROR( ... ) \
    print( LOG_LEVEL_ERROR, NULL, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__ )
#define MS_LOGER_ERROR_EX( file_name_suffix, ... ) \
    print( LOG_LEVEL_ERROR, file_name_suffix, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__ )

#define MS_LOGER_DEBUG( ... ) \
    print( LOG_LEVEL_DEBUG, NULL, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__ )
#define MS_LOGER_DEBUG_EX( file_name_suffix, ... ) \
    print( LOG_LEVEL_DEBUG, file_name_suffix, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__ )

#define MS_LOGER_FATAL(...) \
    print( LOG_LEVEL_FATAL, NULL, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__ )
#define MS_LOGER_FATAL_EX( file_name_suffix, ... ) \
    print( LOG_LEVEL_FATAL, file_name_suffix, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__ )


#endif
