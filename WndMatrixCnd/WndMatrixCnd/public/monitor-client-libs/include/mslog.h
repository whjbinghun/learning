#ifndef _H_MSLOG_
#define _H_MSLOG_

#include "TraceStack.h"

#define    LOG_LEVEL_INFO       0
#define    LOG_LEVEL_WARNNING   1
#define    LOG_LEVEL_ERROR      2
#define    LOG_LEVEL_MEDIA      3
#define    LOG_LEVEL_SNMP       4
#define    LOG_LEVEL_NET		5
#define    LOG_LEVEL_MAX        6

// path format "c:\\att\\"

void SetLogPath(char* szPath);
void StartLog();
void StopLog();

void   Log(int nLevel,char* szFile,char* szTime,char* szFunction,int nLine, const char* szFormat,...);


#define  MS_LOGER_INFO(format,...)                                                                         \
            Log(LOG_LEVEL_INFO,(char*)__FILE__,(char*)__TIMESTAMP__,(char*)__FUNCTION__,__LINE__,format,__VA_ARGS__);


#define  MS_LOGER_WARNING(format,...)                                                                      \
            Log(LOG_LEVEL_WARNNING,(char*)__FILE__,(char*)__TIMESTAMP__,(char*)__FUNCTION__,__LINE__,format,__VA_ARGS__);

#define  MS_LOGER_ERROR(format,...)                                                                        \
            Log(LOG_LEVEL_ERROR,(char*)__FILE__,(char*)__TIMESTAMP__,(char*)__FUNCTION__,__LINE__,format,__VA_ARGS__);

#define  MS_LOGER_MEDIA(format,...)                                                                        \
            Log(LOG_LEVEL_MEDIA,(char*)__FILE__,(char*)__TIMESTAMP__,(char*)__FUNCTION__,__LINE__,format,__VA_ARGS__);

#define  MS_LOGER_SNMP(format,...)                                                                        \
            Log(LOG_LEVEL_SNMP,(char*)__FILE__,(char*)__TIMESTAMP__,(char*)__FUNCTION__,__LINE__,format,__VA_ARGS__);

#define  MS_LOGER_NET(format,...)                                                                        \
            Log(LOG_LEVEL_NET,(char*)__FILE__,(char*)__TIMESTAMP__,(char*)__FUNCTION__,__LINE__,format,__VA_ARGS__);

#define  MS_TRACE_STACK(id)                                                                                    \
            CTraceStack  traceStack((int)id,LOG_LEVEL_INFO,__FILE__,__TIMESTAMP__,__FUNCTION__,__LINE__);


#endif
