#ifndef ALARMJUDGEAPI_H
#define ALARMJUDGEAPI_H


//#ifdef  ALARMJUDGE_LIBRARY
//#define ALARMJUDGE_SO_API  __declspec(dllexport)
//#else
//#define ALARMJUDGE_SO_API  __declspec(dllimport)
//#endif

#include <string>
#include <list>
#include <vector>
#include <map>

#include "comm_typedef.h"

//#define ALARM_CONDITION_DEFINE_
#ifndef ALARM_CONDITION_DEFINE_
typedef struct tagTHREE_MONITOR_TEMPERATURE_S{
    float ft_max;
    float ft_min;
    float ft_avg;
    tagTHREE_MONITOR_TEMPERATURE_S(){
        ft_max = 0.0f;
        ft_min = 0.0f;
        ft_avg = 0.0f;
    }
}THREE_MONITOR_TEMPERATURE_S;

typedef struct {
    UINT32      ui_alarm_no;
    std::string str_alias;
    std::string str_alarm_condition;
} AlarmConditionInfo_S;
#endif

//INT32 AlarmJudgeTest();

/********************************************************************
 * Func    Name: AlarmJudge_Init
 * Descriptions: init
 * Input   Para:
 * *****************************************************************/
void AlarmJudge_Init();

/********************************************************************
 * Func    Name: SetAlarmTempMap
 * Descriptions: set temperature list to internal memory
 * Input   Para: alarm_temp_map
 * Return Value: 0 means succ. -1 means failed.
 * *****************************************************************/
INT32 SetAlarmTempMap( unsigned long int ui_address, std::map<UINT32, std::list<THREE_MONITOR_TEMPERATURE_S> > &alarm_temp_map );


/********************************************************************
 * Func    Name: Judge_alarm
 * Descriptions: Judge alarm condition
 * Input   Para: st_alarm_condition
 * Return Value: 0 means alarm. -1 means not.
 *******************************************************************/
INT32 Judge_alarm( unsigned long int ui_address, AlarmConditionInfo_S &st_alarm_condition );

/********************************************************************
 * Func    Name: Delete_TempMap
 * Descriptions: delete temperature list from internal memory
 * Input   Para: temperature list address
 * Return Value: 0 means succ. -1 means failed.
 *******************************************************************/
INT32 Delete_TempMap( unsigned long int ui_address );

/********************************************************************
 * Func    Name: AlarmJudge_UnInit
 * Descriptions: init
 * Input   Para:
 * *****************************************************************/
void AlarmJudge_UnInit();
#endif // ALARMJUDGESO_H
