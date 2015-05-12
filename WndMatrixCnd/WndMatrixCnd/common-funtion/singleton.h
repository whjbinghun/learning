#ifndef _YF_SINGLETON_H
#define _YF_SINGLETON_H

#include <memory>
#include <stdio.h>
#include <stdarg.h>
#include <QMutex>
using namespace std;

//在头文件类声明中加入下列宏
/**
    @def  SINGLETON_DECLARE(class_name)
    @param  class_name 类名
	*  单态模式声明宏,多线程安全的
*/
#define  SINGLETON_DECLARE(class_name) \
    public:\
    static class_name* s_instance;\
    static QMutex s_cs; \
	public:\
    static class_name* get_instance()\
    {\
    if (s_instance == 0)\
        {\
           QMutexLocker lock( &s_cs );\
           if (s_instance == 0)\
           {\
              s_instance = new class_name();\
           }\
        }\
        return s_instance;\
    }\
    public:\
    class_name();\
    ~class_name();\
	private:\
    class_name(const class_name& ); \
    class_name& operator=(const class_name&);\

//在实现文件中加入下列宏
/**
    @def   SINGLETON_IMPLEMENT(class_name)
    @param class_name 类名

	*  单态模式实现宏
*/
#define  SINGLETON_IMPLEMENT(class_name) \
    class_name* class_name::s_instance;\
    QMutex class_name::s_cs;
	

#endif
