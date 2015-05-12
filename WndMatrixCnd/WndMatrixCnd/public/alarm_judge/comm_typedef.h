#ifndef __COMM_TYPEDEF_H__
#define __COMM_TYPEDEF_H__



using namespace std;

/*********************** ºê¶¨ÒåÇø ***********************/
#define DEBUG      0
#define __32_BIT__ 1

#if __32_BIT__

//typedef char                INT8;
typedef short               INT16;
typedef int                 INT32;
typedef signed long long    INT64;
typedef unsigned char       UINT8;
typedef unsigned short      UINT16;
typedef unsigned int        UINT32;
typedef unsigned long long  UINT64;

#endif

#endif
//end of comm_typedef.h


