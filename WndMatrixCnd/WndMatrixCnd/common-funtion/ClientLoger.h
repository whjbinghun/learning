#ifndef _H_MS_CLIENT_LOGER_
#define _H_MS_CLIENT_LOGER_

#include <stdarg.h>
#include  "msclientlog.h"
#include <fstream>
#include <iostream>
#include <map>
#include <list>

#ifndef WIN32
#include "./MS-Net/NetProtocol/log-service-packet.h"
#include "./MS-Net/NetProtocol/net-packet-serializer.h"
#include "./MS-Thread/task-thread.h"
#include "log-service-link.h"
#endif

const unsigned int UI_LOG_TIMER_ID = 1001;
const unsigned int UI_LOG_TIMER_ELAPSE = 3*1000;// 3S
const short    S_LOG_SERVER_PORT = 8890;

#define kHeaderTimestamp "[%02u:%02u:%02u.%03u]"
#define kHeaderThreadId "[Tid:%04x]"
#define kHeaderFileAndLine "[%s:%d->"
#define kHeaderFunction  "%s]"
#define kHeaderLogLevel  "[%s]"
#define kLogFilenameSuffix "%04u-%02u-%02u-%s.log"
#define kLogFilenameSuffixNULL "%04u-%02u-%02u.log"

#ifdef WIN32
#define kLogFilenameSuffixPath  "%04u\\%02u\\"
#else
#define kLogFilenameSuffixPath  "%04u/%02u/%02u/"
#endif

#define kLogFilenameSuffixSize   128

typedef struct tagLOG_SERVER_INFO_S
{
    tagLOG_SERVER_INFO_S(){
        str_server_ip = "";
        ui_server_port = 0;
    }
    std::string str_server_ip;
    unsigned short    ui_server_port;
}LOG_SERVER_INFO_S;

enum StorageType
{
    kScreen    = 0x01,  ///< 屏幕输出
    kDiskFile  = 0x02,  ///< 日志文件输出

    kStorageTypeMask = 0x0F,  ///< mask
};

/// 字段类型
enum FieldType
{
    kTimestamp = 0x01,  ///< 输出时间标签
    kThreadId  = 0x02,  ///< 输出线程Id标签
    kFileLine  = 0x04,  ///< 输出在源代码文件和行数标签
    kFunction  = 0x08,  ///< 输出在源代码的函数名称

    kFieldTypeMask = 0x0F,  // mask
};


FLogCallback g_log_callback = NULL;
void* g_user_data = NULL;

namespace detail
{

class Log
{
private:
    Log();
    ~Log();

public:
    static Log& instance();

public:
public:
    void set_directory( const char* path );

    void set_remote_flag( unsigned int ui_remote_flag, const std::string& str_ip, unsigned short ui_port );


    void set_level(unsigned int level);

    void set_storage_type( unsigned int storage_type );

    void set_field_type( unsigned int field_type );

    void set_reserve_days( int days );

    void print_v( unsigned int level, const char *file_name_suffix, const char *filename,
                  unsigned int line, const char *function, const char *format, va_list ap );
private:
    void create_remote();

    void write( const char *file_name_suffix, const char *message, int size, const SYSTEM_TIME_S& system_time );

    void wirte_to_file( const char *file_name_suffix, const char *message, int size, const SYSTEM_TIME_S& system_time );

    void wirte_to_remote_callback(  );

    void close_remote_files();

    void close_remote_files_callback(  );
    //清理的先不做
    void clear_files();

    std::string get_log_filename( const char* suffix );

private:
    //std::mutex mo_lock;
    pthread_mutex_t mo_log_lock;
    unsigned int mui_storage_type;// = kDiskFile;
    unsigned int mui_field_type;//   = kFieldTypeMask;
    unsigned int mui_level;// = LOG_LEVEL_MAX;
    unsigned int mui_remote_flag;// = LOG_Local;
    unsigned int mui_reserve_days; // = -1;// -1澶╂槸姘歌繙涓嶅垹闄?
    int mi_filename_depth; // = 1;
    std::string mstr_log_directory; // = "";
    std::string mstr_prefix_name; //= "";
    char mac_old_suffix[kLogFilenameSuffixSize]; //= {0};
    char mac_new_suffix[kLogFilenameSuffixSize]; //= {0};
    char mac_old_suffix_path[kLogFilenameSuffixSize]; // = {0};
    char mac_new_suffix_path[kLogFilenameSuffixSize]; // = {0};
    bool  mb_connect; // = false;
    bool  mb_create; // = false;
    std::map<std::string, FILE* > mmap_opertor_files;

#ifndef WIN32
    ConLogService        mo_LogService;
    MSThread::TaskThread mo_thread_pool;
    std::list<MSNet::MSProtocol::PacketMSLogPrintInfo_S> mlist_log_Stream;
#endif

    LOG_SERVER_INFO_S   mo_Log_Server_Info;

};

}  // namespace detail

/// @brief 设置日志存储类型
/// @param[in] storage_type
void set_storage_type( unsigned int storage_type );
/// @brief
/// @param[in] field_type
void set_field_type( unsigned int field_type );
/// @brief 设置日志保留天数
/// @param[in] days 日志保存天数
/// @note 日志保留天数，默认为-1天
void set_reserve_days( int days );


#endif
