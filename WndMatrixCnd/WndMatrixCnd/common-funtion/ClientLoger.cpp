#include "ClientLoger.h"
#include <vector>
#ifdef WIN32
#include <time.h>
#include <Windows.h>
#include <direct.h>
#include <share.h>
#else
#include <string.h>
#include <stdarg.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <sys/time.h>
#include <time.h>

#endif



unsigned int get_process_name( std::string &str_process_name )
{
#ifdef WIN32
    char ac_szModule[512] = {0};
    char ac_szAppName[64] = {0};
    ::GetModuleFileNameA ( NULL, ac_szModule, sizeof(ac_szModule) );
    /*
    for ( int i = 0; i<(int)strlen(szModule); ++i)
        szModule[i] = (char)tolower(szModule[i]);*/

    int nLen = (int)strlen(ac_szModule);
    for ( int i = nLen; i >= 0; --i ){
        if ( ac_szModule[i] == '.')
            ac_szModule[i] = 0;
        if ( ac_szModule[i] == '\\' ){
            ac_szModule[i] = 0;
            strcpy_s( ac_szAppName, sizeof(ac_szAppName), &ac_szModule[i] );
            break;
        }
    }
    str_process_name = ac_szModule;
    return 0;
#else
    char sysfile[15] = "/proc/self/exe";
    char szFile[512] = { 0 };
    readlink( sysfile, szFile, 512 );
    str_process_name = szFile;
    std::size_t npos = str_process_name.rfind('/');
    if( npos > 0 && npos < str_process_name.size() ){
        str_process_name = str_process_name.substr( npos + 1, str_process_name.size() - npos -1 );
        return 0;
    }
#endif
    str_process_name = "";

    return -1;
}



namespace detail
{


char get_system_slash()
{
#ifdef WIN32
    return '\\';
#else
    return '/';
#endif
}

const char* get_level_string( unsigned int level )
{
    switch (level){
    case LOG_LEVEL_INFO:
        return  "INF";
    case LOG_LEVEL_WARNNING:
        return "WAR";
    case LOG_LEVEL_ERROR:
        return "ERR";
    case LOG_LEVEL_DEBUG:
        return "DBG";
    case LOG_LEVEL_FATAL:
        return  "FAT";
    }
    return "Unknown";

}


tm to_tm( time_t t )
{
    struct tm _tm;
#ifdef WIN32
    localtime_s( &_tm, &t );
#else
    localtime_r( &t, &_tm );
#endif
    return _tm;
}

SYSTEM_TIME_S to_system_time( time_t t )
{
    tm _tm = to_tm(t);
    SYSTEM_TIME_S system_time;
    system_time.year    = _tm.tm_year + 1900;
    system_time.month   = _tm.tm_mon + 1;
    system_time.day     = _tm.tm_mday;
    system_time.hour    = _tm.tm_hour;
    system_time.minute  = _tm.tm_min;
    system_time.second  = _tm.tm_sec;
    system_time.millisecond = 0;

    return system_time;
}

#ifdef WIN32
SYSTEM_TIME_S get_system_time()
{
    /*
    std::chrono::system_clock::time_point now =
            std::chrono::system_clock::now();
    time_t t = std::chrono::system_clock::to_time_t(now);
    int milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(
                now.time_since_epoch()).count();
    SYSTEM_TIME_S system_time = to_system_time(t);
    system_time.millisecond = milliseconds;
    return system_time;*/
    SYSTEMTIME now;
    ::GetLocalTime(&now);

    SYSTEM_TIME_S system_time;
    system_time.year  =         now.wYear;
    system_time.month =         now.wMonth;
    system_time.day   =         now.wDay;
    system_time.hour  =         now.wHour;
    system_time.minute=         now.wMinute;
    system_time.second=         now.wSecond;
    system_time.millisecond =   now.wMilliseconds;

    return system_time;
}

#else
SYSTEM_TIME_S get_system_time()
{
    struct timeval tv_now;
    gettimeofday(&tv_now, NULL);

    struct tm tm_now;
    localtime_r(&tv_now.tv_sec, &tm_now);

    SYSTEM_TIME_S system_time;
    system_time.year  =      tm_now.tm_year + 1900;
    system_time.month =      tm_now.tm_mon + 1;
    system_time.day   =      tm_now.tm_mday;
    system_time.hour  =      tm_now.tm_hour;
    system_time.minute=      tm_now.tm_min;
    system_time.second=      tm_now.tm_sec;
    system_time.millisecond = (int)tv_now.tv_usec;

    return system_time;
}
#endif


#ifdef WIN32
bool create_directory( const char* pathName )
{
    char path[MAX_PATH];
    memset( path, 0x00, MAX_PATH);
    const char* pos = pathName;
    while ((pos = strchr(pos, '\\')) != NULL){
        memcpy( path, pathName, pos - pathName + 1 );
        pos++;
        if ( ::access( path, F_OK ) == 0 ){
            continue;
        }else{
            int ret = ::mkdir( path );
            if (ret != 0){
                return false;
            }
        }
    }
    return true;
}
#else

#endif  // #ifdef WIN32

unsigned int get_thread_id()
{
#ifdef WIN32
    return ::GetCurrentThreadId();
#else
    //return ::syscall( SYS_gettid );
#endif
}

int vsnprintf( char *buffer, size_t size, const char *format, va_list ap )
{
#ifdef WIN32
    int pos = ::vsnprintf_s( buffer, size, _TRUNCATE, format, ap );
    return pos == -1 ? ::_vscprintf(format, ap) : pos;
#else
    //return ::vsnprintf( buffer, size, format, ap );
#endif
}

int vsnprintf( wchar_t *buffer, size_t size, const wchar_t *format, va_list ap )
{
#ifdef WIN32
    int pos = ::_vsnwprintf_s( buffer, size, _TRUNCATE, format, ap );
    return pos == -1 ? ::_vscwprintf( format, ap ) : pos;
#else
    //return ::vswprintf( buffer, size, format, ap );
#endif
}

#ifdef WIN32
void strcpy( char* dest, size_t dest_size, const char* src )
{
    ::strcpy_s( dest, dest_size, src );
}

#else
void strcpy( char* dest, size_t dest_size, const char* src )
{
    ::strncpy( dest, src, dest_size );
}
#endif


int snprintf( char *buffer, size_t size, const char *format, ... )
{
    va_list ap;
    va_start( ap, format );
    int pos = vsnprintf( buffer, size, format, ap );
    va_end( ap );
    return pos;
}


int append_splitter( char* buffer, int pos )
{
    buffer[pos++] = '|';
    return 1;
}


int append_line_end( char* buffer, int pos )
{
#ifdef WIN32
    buffer[pos++] = '\r';
    buffer[pos++] = '\n';
    return 2;
#else
    //buffer[pos++] = '\n';
    //return 1;
#endif
}


void open_file( std::ofstream& file, const char* path )
{
    file.open( path, std::ios::binary | std::ios::app );
}

void print_to_screen( const wchar_t* message )
{
    std::wcout << message;
}

void print_to_screen( const char* message )
{
    std::cout << message;
}

#ifdef WIN32
bool delete_file( const char* filename )
{
    return FALSE != ::DeleteFileA( filename );
}

bool delete_file( const wchar_t* filename )
{
    return FALSE != ::DeleteFileW( filename );
}
#else
bool delete_file( const char* filename )
{
    return ( ::unlink( filename ) != -1 );
}
#endif


const char* get_last_path( const char* path, int slash_count )
{
    const char* p = path + strlen(path) - 1;
    while ( p-- != path ){
        if (*p == get_system_slash()) slash_count--;
        if (slash_count < 0) return ++p;
    }
    return path;
}


int sprintf_timestamp( char* buffer, int size, const SYSTEM_TIME_S& t )
{
    return snprintf( buffer, size, kHeaderTimestamp, t.hour, t.minute, t.second, t.millisecond );
}


int sprintf_thread_id( char* buffer, int size, unsigned int thread_id )
{
    return snprintf( buffer, size, kHeaderThreadId, thread_id );
}


int sprintf_file_and_line( char* buffer, int size, const char* filename, unsigned int line )
{
    return snprintf( buffer, size, kHeaderFileAndLine, get_last_path( filename, 1 ), line );
}

int sprintf_function( char* buffer, int size, const char* function )
{
    return snprintf( buffer, size, kHeaderFunction, function );
}

int sprintf_level( char* buffer, int size, unsigned int level )
{
    return snprintf( buffer, size, kHeaderLogLevel, get_level_string(level) );
}

int sprintf_filename_suffix( char* buffer, const char *file_name_suffix, int size, const SYSTEM_TIME_S& system_time )
{
    if( file_name_suffix && strlen(file_name_suffix) > 0 ){
        return snprintf( buffer, size, kLogFilenameSuffix, system_time.year, system_time.month, system_time.day, file_name_suffix );
    }
    return snprintf( buffer, size, kLogFilenameSuffixNULL, system_time.year, system_time.month, system_time.day );
}


int sprintf_filename_suffix_path( char* buffer, int size, const SYSTEM_TIME_S& system_time )
{
    return snprintf( buffer, size, kLogFilenameSuffixPath, system_time.year, system_time.month );
}


int format_line_head( unsigned int field_type, char *buffer, int size,
                      unsigned int level, const char *filename, unsigned int line, const char *function,
                      const SYSTEM_TIME_S& system_time )
{
    int pos = 0;


    pos += sprintf_level(buffer + pos, size - pos, level);
    if (pos >= size) return size;

    if ( field_type & kTimestamp ){
        pos += sprintf_timestamp(buffer + pos, size - pos, system_time);
    }

    if (pos >= size) return size;

    if ( field_type & kFileLine ){
        pos += sprintf_file_and_line(buffer + pos, size - pos, filename, line);
    }

    if (pos >= size) return size;

    if ( field_type & kFileLine ){
        pos += sprintf_function(buffer + pos, size - pos, function);
    }

    if (pos >= size) return size;

    if ( field_type & kThreadId ){
        pos += sprintf_thread_id(buffer + pos, size - pos, get_thread_id());
    }
    if (pos >= size) return size;

    pos += append_splitter(buffer, pos);
    return pos;
}



Log::Log()
{
    pthread_mutex_init( &mo_log_lock, NULL );
    mui_storage_type = kDiskFile;
    mui_field_type = kFieldTypeMask;
    mui_level = LOG_LEVEL_MAX;
    mui_remote_flag = LOG_Local;
    mui_reserve_days = -1;
    mi_filename_depth = 1;

    memset( mac_old_suffix, 0, kLogFilenameSuffixSize );
    memset( mac_new_suffix, 0, kLogFilenameSuffixSize );
    memset( mac_old_suffix_path, 0, kLogFilenameSuffixSize );
    memset( mac_new_suffix_path, 0, kLogFilenameSuffixSize );
//    mac_old_suffix[kLogFilenameSuffixSize] = {0};
//    mac_new_suffix[kLogFilenameSuffixSize] = {0};
//    mac_old_suffix_path[kLogFilenameSuffixSize] = {0};
//    mac_new_suffix_path[kLogFilenameSuffixSize] = {0};
    mb_connect = false;
    mb_create = false;
#ifndef WIN32
    mo_thread_pool.begin();
#endif


}

Log::~Log()
{
    for( std::map<std::string, FILE*>::iterator itfile = mmap_opertor_files.begin();
         itfile != mmap_opertor_files.end(); ++itfile ){
        FILE* pfile = itfile->second;
        fclose(pfile);
        pfile = NULL;
    }
    mmap_opertor_files.clear();

#ifndef WIN32
    mo_thread_pool.end();
#endif
    pthread_mutex_destroy( &mo_log_lock );
}

Log& Log::instance()
{
    static Log instance;
    return instance;
}

void Log::set_directory( const char* path )
{
    mstr_log_directory.assign(path);
#ifdef WIN32
    mstr_log_directory += "\\Log";
#else
    mstr_log_directory += "/Log";
#endif

    if ( !mstr_log_directory.empty() &&
         mstr_log_directory[mstr_log_directory.length()] != get_system_slash()
         /*mstr_log_directory.back() != get_system_slash()*/ ){
        mstr_log_directory.push_back( get_system_slash() );
    }
    create_directory( mstr_log_directory.c_str() );
}

void Log::set_remote_flag( unsigned int ui_remote_flag, const std::string& str_ip, unsigned short ui_port )
{
    mui_remote_flag = ui_remote_flag;
    mo_Log_Server_Info.str_server_ip = str_ip;
    mo_Log_Server_Info.ui_server_port = ui_port;
}


void Log::set_level(unsigned int level)
{
    mui_level = level;
}


void Log::set_storage_type( unsigned int storage_type )
{
    mui_storage_type = storage_type;
}

void Log::set_field_type( unsigned int field_type )
{
    mui_field_type = field_type;
}

void Log::set_reserve_days( int days )
{
    mui_reserve_days = days;
}

void Log::print_v( unsigned int level, const char *file_name_suffix, const char *filename,
                   unsigned int line, const char *function, const char *format, va_list ap )
{
    if( mstr_log_directory.empty() ){
        std::string str_dir = "";
        get_process_name( str_dir );
        set_directory( str_dir.c_str() );
    }

    //create_remote();

    //std::lock_guard<std::mutex> guard( mo_lock );
    pthread_mutex_lock( &mo_log_lock );
    SYSTEM_TIME_S system_time = get_system_time();

    const int kMaxBufferSize = 2048;
    char buffer[kMaxBufferSize + 16]= {0};
    int pos = format_line_head( mui_field_type, buffer, kMaxBufferSize, level, filename,line, function, system_time );

    int body_size = vsnprintf( buffer + pos, kMaxBufferSize - pos, format, ap );


    if ( body_size > kMaxBufferSize ){
        pos += snprintf( buffer + pos, kMaxBufferSize - pos, "LINE TOO LONG" );
    }else{
        pos += body_size;
    }

    pos += append_line_end(buffer, pos);
    buffer[pos++] = 0;


    if ( g_log_callback ){
        char log_buffer[ kMaxBufferSize + 16 ]= {0};
        vsnprintf( log_buffer, kMaxBufferSize, format, ap );
        g_log_callback( log_buffer, system_time, g_user_data );
    }


    write( file_name_suffix, buffer, pos - 1, system_time );
    pthread_mutex_unlock( &mo_log_lock );
}

void Log::create_remote()
{

}

void Log::write( const char *file_name_suffix, const char *message, int size, const SYSTEM_TIME_S& system_time )
{
    if ( mui_storage_type & kScreen )
        print_to_screen( message );
    if ( mui_storage_type & kDiskFile )
        wirte_to_file( file_name_suffix, message, size, system_time );
}

void Log::wirte_to_file( const char *file_name_suffix, const char *message, int size, const SYSTEM_TIME_S& system_time )
{
    //sprintf_filename_suffix_path( mac_new_suffix_path, std::extent<decltype(mac_new_suffix)>::value, system_time );
    sprintf_filename_suffix_path( mac_new_suffix_path, kLogFilenameSuffixSize, system_time );
    int i_compare_ret = ::strncmp( mac_old_suffix_path, mac_new_suffix_path, kLogFilenameSuffixSize );
    if (i_compare_ret  != 0 ){
        for( std::map<std::string, FILE* >::iterator itfile = mmap_opertor_files.begin() ; itfile != mmap_opertor_files.end(); ){
            int npos = itfile->first.find(mac_old_suffix_path);
            if( npos < 1){
                continue;
                ++itfile;
            }else{
                printf( "close file, file name = %s\n", itfile->first.c_str() );
                if( itfile->second ){
                    fclose(itfile->second);
                }
                itfile->second = NULL;
                //itfile = mmap_opertor_files.erase( itfile );
                mmap_opertor_files.erase( ++itfile );
            }
        }

        //strcpy( mac_old_suffix_path, std::extent<decltype(mac_old_suffix_path)>::value, mac_new_suffix_path );
        strcpy( mac_old_suffix_path, kLogFilenameSuffixSize, mac_new_suffix_path );
        std::string str_log_directory = mstr_log_directory;
        str_log_directory += mac_new_suffix_path;
        create_directory( str_log_directory.c_str() );
        //printf( "create dir = %s\n", str_log_directory.c_str() );
        close_remote_files();

    }

    //sprintf_filename_suffix( mac_new_suffix, file_name_suffix, std::extent<decltype(mac_new_suffix)>::value, system_time );
    sprintf_filename_suffix( mac_new_suffix, file_name_suffix, kLogFilenameSuffixSize, system_time );
    std::string path = get_log_filename( mac_new_suffix );
    int npos = path.rfind( get_system_slash() );
    std::string path_des = path.substr( 0, npos + 1 );
    path_des += mac_new_suffix_path;
    path_des += path.substr( npos + 1, path.size() - npos + 1 );
    std::map<std::string, FILE* >::iterator itfile = mmap_opertor_files.find(path_des);
    if (  itfile == mmap_opertor_files.end() ){
        //打本地日志
        if( (mui_remote_flag & LOG_Local) ){
            FILE *pfile = NULL;

            if( (pfile = fopen( path_des.c_str(), "at" )) != NULL){
                mmap_opertor_files[path_des] = pfile;
                //printf( "open file = %s\n", path_des.c_str() );
                fwrite( message, 1, size,  pfile );
                fflush( pfile );
            }else{
                //printf( "open file failed = %s, errno=%s\n", path_des.c_str(), strerror(errno) );
            }
        }

    }else if( (mui_remote_flag & LOG_Local) ){
        FILE *pfile = itfile->second;
        if ( pfile){
            fwrite( message, 1, size,  pfile );
            fflush( pfile );
            // pfile->flush();
        }
    }


}


void Log::wirte_to_remote_callback(  )
{

}

void Log::close_remote_files()
{

}

void Log::close_remote_files_callback(  )
{

}

//清理的先不做
void Log::clear_files()
{/*
        time_t seconds = time(NULL) - mui_reserve_days * 86400;
        for ( seconds -= 24 * 3600;; seconds -= 24 * 3600 ){
            std::basic_string<char> filename = get_log_filename(seconds);
            if ( !delete_file( filename.c_str() ) ) break;
        }*/
}

std::string Log::get_log_filename( const char* suffix )
{
    return mstr_log_directory + mstr_prefix_name + suffix;
}


}  // namespace detail


void print( unsigned int level, const char *file_name_suffix, const char *filename,
            unsigned int line, const char *function, const char *format, ... )
{
    va_list ap;
    va_start( ap, format );
    detail::Log::instance().print_v( level, file_name_suffix, filename, line, function, format, ap );
    va_end( ap );
}



void set_directory( const char* path )
{
    detail::Log::instance().set_directory( path );
}

void set_remote_flag( unsigned int ui_remote_flag, const std::string& str_ip, unsigned short ui_port )
{
    detail::Log::instance().set_remote_flag(ui_remote_flag, str_ip, ui_port);
}


void set_log_callback( FLogCallback f_log_callback, void* user_data )
{
    g_log_callback = f_log_callback;
    g_user_data = user_data;
}

void set_level( unsigned int level )
{
    detail::Log::instance().set_level( level );
}


void set_storage_type( unsigned int storage_type )
{
    detail::Log::instance().set_storage_type( storage_type );
}

void set_field_type( unsigned int field_type )
{
    detail::Log::instance().set_field_type( field_type );
}

void set_reserve_days( int days )
{
    detail::Log::instance().set_reserve_days( days );
}

