#ifndef NETCOMMON_H
#define NETCOMMON_H

#define VERSION         1
#define VERSION_SUB     0

#include <list>
#include <QtCore>
#include <QList>
using namespace std;


typedef enum
{
    IR_MONITOR_VER_NEGO_REQ = 0x0001, /* 版本协商请求 */
    IR_MONITOR_LINK_AUTH_REQ = 0x0002, /* 鉴权请求 */
    IR_MONITOR_LINK_HEART_REQ = 0x0003, /* 心跳检测请求 */
    IR_MONITOR_LINK_REL_REQ = 0x0004, /* 连接释放请求 */
    IR_MONITOR_XML_DATA_REQ = 0x0005, /* XML文件请求 */
    IR_MONITOR_VID_DATA_PLAY_REQ = 0x0006, /* 视频数据播放请求 */
    IR_MONITOR_VID_DATA_STOP_REQ = 0x0007, /* 视频数据停止请求 */
//    IR_MONITOR_REPLAY_DATA_REQ = 0x0008, /* 回放数据控制请求 */
//    IR_MONITOR_PLATFORM_CTRL_REQ = 0x0010, /* 云台控制请求 */
    IR_MONITOR_VID_KEY_DATA_PLAY_REQ   = 0x0008, /* 关键视频帧数据播放请求 */
    IR_MONITOR_VID_KEY_DATA_STOP_REQ   = 0x0009, /* 关键视频帧数据停止请求 */
    IR_MONITOR_VID_SUB_STREAM_PLAY_REQ = 0x000A, /* 子码流视频数据播放请求 */
    IR_MONITOR_VID_SUB_STREAM_STOP_REQ = 0x000B, /* 子码流视频数据停止请求 */

    IR_MONITOR_REPLAY_SETUP_REQ = 0x0010,  /*回放建立请求*/          //2014.11.14 blm add
    IR_MONITOR_REPLAY_PLAY_REQ	= 0x0011, /* 回放播放请求 */         //2014.11.14 blm add
    IR_MONITOR_REPLAY_PAUSE_REQ = 0x0012,        /*回放暂停请求*/          //2014.11.14 blm add
    IR_MONITOR_REPLAY_RESUME_REQ = 0x0013,      /*回放恢复请求*/          //2014.11.14 blm add
    IR_MONITOR_REPLAY_STOP_REQ = 0x0014,      /*回放停止请求*/          //2014.11.14 blm add
    IR_MONITOR_REPLAY_FRAME_REQ = 0x0015,     /*回放帧请求*/            //2014.11.14 blm add
    IR_MONITOR_REPLAY_HEART_REQ		= 0x0016, /* 回放心跳请求 */        //2014.11.17 blm add
    IR_MONITOR_REPLAY_PRESET_LIST_REQ = 0x0017, /*预置点播放信息请求 */   //2014.11.14 blm add
    IR_MONITOR_SET_REPLAY_NUM_REQ = 0x0018, /*回放最大支持个数设置请求*/   //2014.11.12 blm add
    IR_MONITOR_SET_RESERVER_CAPACITY_REQ = 0x0019, /* 设置剩余磁盘容量请求 */  //2014.11.23 blm add
    IR_MONITOR_REPLAY_PLAY_INIT_REQ	= 0x001A, /* 回放单帧播放初始化请求 */ //2014.11.23 blm add

    IR_MONITOR_GROUP_CFG_ADD_REQ = 0x0020, /* 配置信息新增请求 */
    IR_MONITOR_GROUP_CFG_DEL_REQ = 0x0021, /* 配置信息删除请求 */
    IR_MONITOR_GROUP_CFG_CHG_REQ = 0x0022, /* 配置信息变更请求 */
    IR_MONITOR_IR_ANALYSE_ADD_REQ = 0x0030, /* 红外分析新增请求 */
    IR_MONITOR_IR_ANALYSE_DEL_REQ	= 0x0031, /* 红外分析删除请求 */
    IR_MONITOR_IR_FOCUS_OUT_REQ		= 0x0032, /* 红外焦距调远请求 */
    IR_MONITOR_IR_FOCUS_IN_REQ			= 0x0033, /* 红外焦距调近请求 */
    IR_MONITOR_IR_TEMP_CHK_REQ			= 0x0034, /* 红外温度校准请求 */
    IR_MONITOR_VISI_FOCUS_INC_REQ		= 0x0035, /* 可见光正向调焦请求 */
    IR_MONITOR_VISI_FOCUS_INC_STOP_REQ	= 0x0036, /* 可见光停止正向调焦请求 */
    IR_MONITOR_VISI_FOCUS_DEC_REQ		= 0x0037, /* 可见光反向调焦请求 */
    IR_MONITOR_VISI_FOCUS_DEC_STOP_REQ	= 0x0038, /* 可见光停止反向调焦请求 */
    IR_MONITOR_IR_ANALYSE_LOAD_REQ = 0x0039, /* 红外分析重载请求 */

    IR_MONITOR_RAIL_CTRL_SET_REQ             	= 0x0040, /* 导轨设置预置点请求 */
    IR_MONITOR_RAIL_CTRL_CALL_BACK_REQ		= 0x0041, /* 导轨召回原点请求 */
    IR_MONITOR_RAIL_CTRL_DEL_PRESET_REQ		= 0x0042, /* 导轨设置巡航请求 */
    IR_MONITOR_RAIL_CTRL_STEP_LEFT_REQ		= 0x0043, /* 导轨单步向左请求 */
    IR_MONITOR_RAIL_CTRL_STEP_RIGHT_REQ	= 0x0044, /* 导轨单步向右请求 */
    IR_MONITOR_RAIL_CTRL_LEFT_REQ			= 0x0045, /* 导轨持续向左请求 */
    IR_MONITOR_RAIL_CTRL_RIGHT_REQ			= 0x0046, /* 导轨持续向右请求 */
    IR_MONITOR_RAIL_CTRL_STOP_REQ			= 0x0047, /* 导轨停止请求 */
    IR_MONITOR_RAIL_CTRL_TO_SET_POINT_REQ	= 0x0048, /* 导轨运行到设置点请求 */
    IR_MONITOR_RAIL_CTRL_TO_DISTANCE_REQ	= 0x0049, /* 导轨运行到指定位置请求 */
    IR_MONITOR_RAIL_CTRL_SET_SPEED_REQ		= 0x004a, /* 设置导轨速度请求 */
    IR_MONITOR_RAIL_CTRL_CHK_INFO_REQ		= 0x004b, /* 导轨校准请求 */
    IR_MONITOR_RAIL_CTRL_CUR_POS_REQ		= 0x004c, /* 获取导轨当前位置及速度请求 */

    IR_MONITOR_RAIL_CTRL_INFO_TRANS_REQ	= 0x004f, /* 导轨传输信息请求 */

    IR_MONITOR_GET_LIVE_COLOR_IDX_REQ		= 0x0050, /* 实时中获取当前调试板类型 */
    IR_MONITOR_SET_LIVE_COLOR_IDX_REQ		= 0x0051, /* 实时中设置调色板类型 */
    IR_MONITOR_SET_LIVE_ADJUST_RANGE_REQ	= 0x0052, /* 实时中手动设置色标温度范围 */
    IR_MONITOR_SET_LIVE_ADJUST_TYPE_REQ	= 0x0053, /* 实时中设置色标类型手动还是自动 */

    IR_MONITOR_GET_REPLAY_COLOR_IDX_REQ	= 0x0054, /* 回放中获取当前调试板类型 */
    IR_MONITOR_SET_REPLAY_COLOR_IDX_REQ	= 0x0055, /* 回放中设置调色板类型 */
    IR_MONITOR_SET_REPLAY_ADJUST_RANGE_REQ	= 0x0056, /* 回放中手动设置色标温度范围 */
    IR_MONITOR_SET_REPLAY_ADJUST_TYPE_REQ	= 0x0057, /* 回放中设置色标类型手动还是自动 */
    IR_MONITOR_REPLAY_ANALYSE_ADD_REQ	 = 0x0058, /* 回放中设置分析 */
    IR_MONITOR_REPLAY_ANALYSE_DEL_REQ	 = 0x0059, /* 回放中设置分析 */
    IR_MONITOR_REPLAY_GET_JPG_ON_TIME_REQ	= 0x005a, /* 回放中获取指定时间图片请求 */

    IR_MONITOR_PLATFORM_LEFT_REQ        		= 0x0060, /* 云台向左请求 */
    IR_MONITOR_PLATFORM_RIGHT_REQ       		= 0x0061, /* 云台向右请求 */
    IR_MONITOR_PLATFORM_UP_REQ        		= 0x0062, /* 云台向上请求 */
    IR_MONITOR_PLATFORM_DOWN_REQ        		= 0x0063, /* 云台向下请求 */
    IR_MONITOR_PLATFORM_LEFT_UP_REQ   		= 0x0064, /* 云台向左上请求 */
    IR_MONITOR_PLATFORM_RIGHT_UP_REQ  		= 0x0065, /* 云台向右上请求 */
    IR_MONITOR_PLATFORM_LEFT_DOWN_REQ  		= 0x0066, /* 云台向左下请求 */
    IR_MONITOR_PLATFORM_RIGHT_DAWN_REQ		= 0x0067, /* 云台向右下请求 */
    IR_MONITOR_PLATFORM_STOP_REQ        		= 0x0068, /* 云台停止请求 */
    IR_MONITOR_PLATFORM_SET_PRESET_REQ 		= 0x0069, /* 云台设置预置位请求 */
    IR_MONITOR_PLATFORM_SET_SPEED_REQ  		= 0x006a, /* 云台设置速度请求 */
    IR_MONITOR_PLATFORM_TO_PRESET_REQ  		= 0x006b, /* 云台移动到预置点请求 */
    IR_MONITOR_PLATFORM_DEL_PRESET_REQ 		= 0x006c, /* 云台删除预置点请求 */
    IR_MONITOR_PLATFORM_CRUISE_REQ     		= 0x006d, /* 云台巡航请求 */

    IR_MONITOR_GET_CRUISE_INFO_REQ     		= 0x0070, /* 获取巡航信息请求 */
    IR_MONITOR_GET_PRESET_INFO_REQ     		= 0x0071, /* 获取组中预置点信息请求 */
    IR_MONITOR_CHANGE_CRUISE_INFO_REQ     	= 0x0072, /* 新增或修改巡航方案请求 */
    IR_MONITOR_DEL_CRUISE_INFO_REQ     		= 0x0073, /* 删除巡航方案请求 */
    IR_MONITOR_SET_JPG_INFO_REQ     		= 0x0074, /* 设置图片请求 */
    IR_MONITOR_GET_JPG_INFO_REQ     		= 0x0075, /* 获取图片请求 */
    IR_MONITOR_DEL_JPG_INFO_REQ     		= 0x0076, /* 删除图片请求 */

    IR_MONITOR_START_CRUISE_REQ     		= 0x0077, /* 开始巡航请求 */
    IR_MONITOR_STOP_CRUISE_REQ      		= 0x0078, /* 停止巡航请求 */

    IR_MONITOR_SET_PRESET_REQ      			= 0x0079, /* 设置预置点请求 */
    IR_MONITOR_DEL_PRESET_REQ      			= 0x007a, /* 删除预置点请求 */
    IR_MONITOR_TO_PRESET_REQ      			= 0x007b, /* 移动到预置点请求 */
    IR_MONITOR_PRESET_ANA_ADD_REQ      		= 0x007c, /* 增加预置点分析请求 */
    IR_MONITOR_PRESET_ANA_MOD_REQ 			= 0x007d, /* 修改预置点分析请求 */
    IR_MONITOR_PRESET_ANA_DEL_REQ			= 0x007e, /* 删除预置点分析请求 */
    IR_MONITOR_MOD_PRESET_REQ   			= 0x007f, /* 修改预置位请求 */

    IR_MONITOR_ADD_USER_REQ			= 0x0080, /* 新增用户 */
    IR_MONITOR_DEL_USER_REQ			= 0x0081, /* 删除用户 */
    IR_MONITOR_CHG_USER_PWD_REQ		= 0x0082, /* 修改用户密码 */
    IR_MONITOR_CHG_USER_LEVEL_REQ		= 0x0083, /* 修改用户权限 */
    IR_MONITOR_QUERY_ALL_USER_REQ		= 0x0084, /* 查询所有用户请求 */
    IR_MONITOR_RECORD_CONDITION_GET_REQ= 0x0095, /* 设置红外录制条件获取请求 */
    IR_MONITOR_DEL_ALARM_CONDITION_REQ 	= 0x0096, /* 报警条件删除请求 */

    /* 报警条件 */
    IR_MONITOR_SET_ALARM_CONDITION_REQ	= 0x0090, /* 报警条件设置请求 */
    IR_MONITOR_GET_ALARM_CONDITION_REQ	= 0x0091, /* 报警条件获取请求 */
    IR_MONITOR_ALARM_PIC_LIST_REQ		= 0x0092, /* 报警条件图片列表请求 */
    IR_MONITOR_ALARM_PIC_GET_REQ		= 0x0093, /* 报警条件图片获取请求 */
    IR_MONITOR_RECORD_CONDITION_REQ	= 0x0094, /* 设置红外录制条件请求 */

    IR_MONITOR_TIMER_PIC_GET_REQ        = 0x0097, /* 时间条件图片获取请求 */

    IR_MONITOR_GET_CUR_CRUISE_INFO_REQ = 0x0100, /* 获取当前巡航信息请求 */

    IR_MONITOR_FRAME_ANA_ADD_REQ 		= 0x0110, /* 回放暂停时分析增加请求 */
    IR_MONITOR_FRAME_ANA_DEL_REQ 		= 0x0111, /* 回放暂停时分析删除请求 */
    IR_MONITOR_FRAME_CLR_SET_REQ 		= 0x0112, /* 回放暂停时调色板请求 */
    IR_MONITOR_FRAME_ADJUST_RANGE_REQ 	= 0x0113, /* 回放暂停时色标范围请求 */
    IR_MONITOR_FRAME_ADJUST_TYPE_REQ 	= 0x0114, /* 回放暂停时色标类型请求 */

    IR_MONITOR_SET_JPG_REQ          = 0x0120, /* 设置巡航图片请求 */

    IR_MONITOR_CLI_LOGOUT_NOTICE      	= 0x5000, /* 客户端退出通知 */
    IR_MONITOR_VID_ERR_NOTICE           	= 0x5001, /* 视频数据异常结束通知 */

    IR_MONITOR_ARRIVAL_PRESET_NOTICE   	= 0x5010, /* 到达预置点通知 */
    IR_MONITOR_LEAVE_PRESET_NOTICE     	= 0x5011, /* 离开预置点通知 */
    IR_MONITOR_CRUISE_STOP_NOTICE     	= 0x5012, /* 巡航停止通知 */
    IR_MONITOR_CRUISE_START_NOTICE     	= 0x5013, /* 巡航开始通知 */


    IR_MONITOR_VER_NEGO_RESP = IR_MONITOR_VER_NEGO_REQ + 0x8000,
    IR_MONITOR_LINK_AUTH_RESP = IR_MONITOR_LINK_AUTH_REQ + 0x8000,
    IR_MONITOR_LINK_HEART_RESP = IR_MONITOR_LINK_HEART_REQ + 0x8000,
    IR_MONITOR_LINK_REL_RESP = IR_MONITOR_LINK_REL_REQ + 0x8000,
    IR_MONITOR_XML_DATA_RESP = IR_MONITOR_XML_DATA_REQ + 0x8000,
    IR_MONITOR_VID_DATA_PLAY_RESP = IR_MONITOR_VID_DATA_PLAY_REQ + 0x8000,
    IR_MONITOR_VID_DATA_STOP_RESP = IR_MONITOR_VID_DATA_STOP_REQ + 0x8000,

 //   IR_MONITOR_REPLAY_DATA_RESP          = IR_MONITOR_REPLAY_DATA_REQ + 0x8000,
//    IR_MONITOR_PLATFORM_CTRL_RESP = IR_MONITOR_PLATFORM_CTRL_REQ + 0x8000,
    IR_MONITOR_VID_KEY_DATA_PLAY_RESP  = IR_MONITOR_VID_KEY_DATA_PLAY_REQ + 0x8000,  //2014.11.17
    IR_MONITOR_VID_KEY_DATA_STOP_RESP  = IR_MONITOR_VID_KEY_DATA_STOP_REQ + 0x8000,  //2014.11.17

    IR_MONITOR_VID_SUB_STREAM_PLAY_RESP = IR_MONITOR_VID_SUB_STREAM_PLAY_REQ + 0x8000,
    IR_MONITOR_VID_SUB_STREAM_STOP_RESP = IR_MONITOR_VID_SUB_STREAM_STOP_REQ + 0x8000,

    IR_MONITOR_REPLAY_SETUP_RESP = IR_MONITOR_REPLAY_SETUP_REQ + 0x8000,  //2014.11.14 blm add
    IR_MONITOR_REPLAY_PLAY_RESP  = IR_MONITOR_REPLAY_PLAY_REQ + 0x8000,  //2014.11.14 blm add
    IR_MONITOR_REPLAY_PAUSE_RESP = IR_MONITOR_REPLAY_PAUSE_REQ + 0x8000, //2014.11.14 blm add
    IR_MONITOR_REPLAY_RESUME_RESP = IR_MONITOR_REPLAY_RESUME_REQ + 0x8000,      /*回放恢复回复*/    //2014.11.14 blm add
    IR_MONITOR_REPLAY_STOP_RESP = IR_MONITOR_REPLAY_STOP_REQ + 0x8000,      /*回放停止回复*/          //2014.11.14 blm add
    IR_MONITOR_REPLAY_FRAME_RESP = IR_MONITOR_REPLAY_FRAME_REQ + 0x8000,     /*回放帧回复*/            //2014.11.14 blm add
    IR_MONITOR_REPLAY_HEART_RESP = IR_MONITOR_REPLAY_HEART_REQ + 0x8000,
    IR_MONITOR_REPLAY_PRESET_LIST_RESP = IR_MONITOR_REPLAY_PRESET_LIST_REQ + 0x8000, /*预置点播放信息回复 */   //2014.11.14 blm add
    IR_MONITOR_SET_REPLAY_NUM_RESP = IR_MONITOR_SET_REPLAY_NUM_REQ +0x8000, /*回放最大支持个数设置*/   //2014.11.12 blm add
    IR_MONITOR_SET_RESERVER_CAPACITY_RESP=IR_MONITOR_SET_RESERVER_CAPACITY_REQ + 0x8000, /* 设置剩余磁盘容量回复 */  //2014.11.23 blm add
    IR_MONITOR_REPLAY_PLAY_INIT_RESP	= IR_MONITOR_REPLAY_PLAY_INIT_REQ + 0x8000,/* 回放单帧播放初始化回复 */ //2014.11.23 blm add

    IR_MONITOR_GROUP_CFG_ADD_RESP = IR_MONITOR_GROUP_CFG_ADD_REQ + 0x8000,
    IR_MONITOR_GROUP_CFG_DEL_RESP = IR_MONITOR_GROUP_CFG_DEL_REQ + 0x8000,
    IR_MONITOR_GROUP_CFG_CHG_RESP = IR_MONITOR_GROUP_CFG_CHG_REQ + 0x8000,

    IR_MONITOR_IR_ANALYSE_ADD_RESP		= IR_MONITOR_IR_ANALYSE_ADD_REQ + 0x8000,
    IR_MONITOR_IR_ANALYSE_DEL_RESP		= IR_MONITOR_IR_ANALYSE_DEL_REQ + 0x8000,
    IR_MONITOR_IR_FOCUS_OUT_RESP		= IR_MONITOR_IR_FOCUS_OUT_REQ + 0x8000,
    IR_MONITOR_IR_FOCUS_IN_RESP		= IR_MONITOR_IR_FOCUS_IN_REQ + 0x8000,
    IR_MONITOR_IR_TEMP_CHK_RESP		= IR_MONITOR_IR_TEMP_CHK_REQ + 0x8000,
    IR_MONITOR_VISI_FOCUS_INC_RESP		= IR_MONITOR_VISI_FOCUS_INC_REQ + 0x8000,
    IR_MONITOR_VISI_FOCUS_DEC_RESP		= IR_MONITOR_VISI_FOCUS_DEC_REQ + 0x8000,
    IR_MONITOR_VISI_FOCUS_INC_STOP_RESP= IR_MONITOR_VISI_FOCUS_INC_STOP_REQ + 0x8000,
    IR_MONITOR_VISI_FOCUS_DEC_STOP_RESP= IR_MONITOR_VISI_FOCUS_DEC_STOP_REQ + 0x8000,
    IR_MONITOR_IR_ANALYSE_LOAD_RESP	= IR_MONITOR_IR_ANALYSE_LOAD_REQ + 0x8000,

    IR_MONITOR_GET_LIVE_COLOR_IDX_RESP		=IR_MONITOR_GET_LIVE_COLOR_IDX_REQ+0x8000,
    IR_MONITOR_SET_LIVE_COLOR_IDX_RESP		=IR_MONITOR_SET_LIVE_COLOR_IDX_REQ+0x8000,
    IR_MONITOR_SET_LIVE_ADJUST_RANGE_RESP=IR_MONITOR_SET_LIVE_ADJUST_RANGE_REQ+0x8000,
    IR_MONITOR_SET_LIVE_ADJUST_TYPE_RESP = IR_MONITOR_SET_LIVE_ADJUST_TYPE_REQ+0x8000,

    IR_MONITOR_GET_REPLAY_COLOR_IDX_RESP = IR_MONITOR_GET_REPLAY_COLOR_IDX_REQ+0x8000,
    IR_MONITOR_SET_REPLAY_COLOR_IDX_RESP = IR_MONITOR_SET_REPLAY_COLOR_IDX_REQ+0x8000,
    IR_MONITOR_SET_REPLAY_ADJUST_RANGE_RESP	 = IR_MONITOR_SET_REPLAY_ADJUST_RANGE_REQ + 0x8000,
    IR_MONITOR_SET_REPLAY_ADJUST_TYPE_RESP	= IR_MONITOR_SET_REPLAY_ADJUST_TYPE_REQ + 0x8000,
    IR_MONITOR_REPLAY_ANALYSE_ADD_RESP	= IR_MONITOR_REPLAY_ANALYSE_ADD_REQ + 0x8000,
    IR_MONITOR_REPLAY_ANALYSE_DEL_RESP	= IR_MONITOR_REPLAY_ANALYSE_DEL_REQ + 0x8000,
    IR_MONITOR_REPLAY_GET_JPG_ON_TIME_RESP = IR_MONITOR_REPLAY_GET_JPG_ON_TIME_REQ+0x8000,

    IR_MONITOR_RAIL_CTRL_SET_RESP        = IR_MONITOR_RAIL_CTRL_SET_REQ + 0x8000,
    IR_MONITOR_RAIL_CTRL_CALL_BACK_RESP = IR_MONITOR_RAIL_CTRL_CALL_BACK_REQ + 0x8000,
    IR_MONITOR_RAIL_CTRL_DEL_PRESET_RESP	= IR_MONITOR_RAIL_CTRL_DEL_PRESET_REQ + 0x8000,
    IR_MONITOR_RAIL_CTRL_STEP_LEFT_RESP = IR_MONITOR_RAIL_CTRL_STEP_LEFT_REQ + 0x8000,
    IR_MONITOR_RAIL_CTRL_STEP_RIGHT_RESP = IR_MONITOR_RAIL_CTRL_STEP_RIGHT_REQ + 0x8000,
    IR_MONITOR_RAIL_CTRL_LEFT_RESP		  = IR_MONITOR_RAIL_CTRL_LEFT_REQ + 0x8000,
    IR_MONITOR_RAIL_CTRL_RIGHT_RESP       = IR_MONITOR_RAIL_CTRL_RIGHT_REQ + 0x8000,
    IR_MONITOR_RAIL_CTRL_STOP_RESP		  = IR_MONITOR_RAIL_CTRL_STOP_REQ + 0x8000,
    IR_MONITOR_RAIL_CTRL_TO_SET_POINT_RESP	= IR_MONITOR_RAIL_CTRL_TO_SET_POINT_REQ + 0x8000,
    IR_MONITOR_RAIL_CTRL_TO_DISTANCE_RESP	= IR_MONITOR_RAIL_CTRL_TO_DISTANCE_REQ + 0x8000,
    IR_MONITOR_RAIL_CTRL_SET_SPEED_RESP		= IR_MONITOR_RAIL_CTRL_SET_SPEED_REQ + 0x8000,
    IR_MONITOR_RAIL_CTRL_CHK_INFO_RESP		= IR_MONITOR_RAIL_CTRL_CHK_INFO_REQ + 0x8000,
    IR_MONITOR_RAIL_CTRL_CUR_POS_RESP		= IR_MONITOR_RAIL_CTRL_CUR_POS_REQ + 0x8000,
    IR_MONITOR_RAIL_CTRL_INFO_TRANS_RESP	= IR_MONITOR_RAIL_CTRL_INFO_TRANS_REQ + 0x8000,

    IR_MONITOR_PLATFORM_LEFT_RESP       	=IR_MONITOR_PLATFORM_LEFT_REQ + 0x8000,
    IR_MONITOR_PLATFORM_RIGHT_RESP     	=IR_MONITOR_PLATFORM_RIGHT_REQ + 0x8000,
    IR_MONITOR_PLATFORM_UP_RESP      	= IR_MONITOR_PLATFORM_UP_REQ + 0x8000,
    IR_MONITOR_PLATFORM_DOWN_RESP       	= IR_MONITOR_PLATFORM_DOWN_REQ + 0x8000,
    IR_MONITOR_PLATFORM_LEFT_UP_RESP  	=IR_MONITOR_PLATFORM_LEFT_UP_REQ + 0x8000,
    IR_MONITOR_PLATFORM_RIGHT_UP_RESP  	=IR_MONITOR_PLATFORM_RIGHT_UP_REQ + 0x8000,
    IR_MONITOR_PLATFORM_LEFT_DOWN_RESP 	=IR_MONITOR_PLATFORM_LEFT_DOWN_REQ + 0x8000,
    IR_MONITOR_PLATFORM_RIGHT_DAWN_RESP = IR_MONITOR_PLATFORM_RIGHT_DAWN_REQ + 0x8000,
    IR_MONITOR_PLATFORM_STOP_RESP       	 =IR_MONITOR_PLATFORM_STOP_REQ + 0x8000,
    IR_MONITOR_PLATFORM_SET_PRESET_RESP = IR_MONITOR_PLATFORM_SET_PRESET_REQ + 0x8000,
    IR_MONITOR_PLATFORM_SET_SPEED_RESP 	 = IR_MONITOR_PLATFORM_SET_SPEED_REQ + 0x8000,
    IR_MONITOR_PLATFORM_TO_PRESET_RESP  = IR_MONITOR_PLATFORM_TO_PRESET_REQ + 0x8000,
    IR_MONITOR_PLATFORM_DEL_PRESET_RESP = IR_MONITOR_PLATFORM_DEL_PRESET_REQ + 0x8000,
    IR_MONITOR_PLATFORM_CRUISE_RESP    	 = IR_MONITOR_PLATFORM_CRUISE_REQ + 0x8000,

    IR_MONITOR_GET_CRUISE_INFO_RESP    	= IR_MONITOR_GET_CRUISE_INFO_REQ + 0x8000,
    IR_MONITOR_GET_PRESET_INFO_RESP    	= IR_MONITOR_GET_PRESET_INFO_REQ + 0x8000,
    IR_MONITOR_CHANGE_CRUISE_INFO_RESP = IR_MONITOR_CHANGE_CRUISE_INFO_REQ + 0x8000,
    IR_MONITOR_DEL_CRUISE_INFO_RESP  	= IR_MONITOR_DEL_CRUISE_INFO_REQ + 0x8000,
    IR_MONITOR_SET_JPG_INFO_RESP  		= IR_MONITOR_SET_JPG_INFO_REQ + 0x8000,
    IR_MONITOR_GET_JPG_INFO_RESP  		= IR_MONITOR_GET_JPG_INFO_REQ + 0x8000,
    IR_MONITOR_DEL_JPG_INFO_RESP  	 	= IR_MONITOR_DEL_JPG_INFO_REQ + 0x8000,

    IR_MONITOR_START_CRUISE_RESP  	 	= IR_MONITOR_START_CRUISE_REQ + 0x8000,
    IR_MONITOR_STOP_CRUISE_RESP  	 	= IR_MONITOR_STOP_CRUISE_REQ + 0x8000,

    IR_MONITOR_SET_PRESET_RESP  	 	= IR_MONITOR_SET_PRESET_REQ + 0x8000,
    IR_MONITOR_DEL_PRESET_RESP  	 	= IR_MONITOR_DEL_PRESET_REQ + 0x8000,
    IR_MONITOR_TO_PRESET_RESP  	 	= IR_MONITOR_TO_PRESET_REQ + 0x8000,
    IR_MONITOR_PRESET_ANA_ADD_RESP  	= IR_MONITOR_PRESET_ANA_ADD_REQ + 0x8000,
    IR_MONITOR_PRESET_ANA_MOD_RESP  	= IR_MONITOR_PRESET_ANA_MOD_REQ + 0x8000,
    IR_MONITOR_PRESET_ANA_DEL_RESP  	= IR_MONITOR_PRESET_ANA_DEL_REQ + 0x8000,
    IR_MONITOR_MOD_PRESET_RESP			= IR_MONITOR_MOD_PRESET_REQ + 0x8000,
    /* 用户密码及权限修改 */
    IR_MONITOR_ADD_USER_RESP			= IR_MONITOR_ADD_USER_REQ + 0x8000,
    IR_MONITOR_DEL_USER_RESP			= IR_MONITOR_DEL_USER_REQ + 0x8000,
    IR_MONITOR_CHG_USER_PWD_RESP		= IR_MONITOR_CHG_USER_PWD_REQ + 0x8000,
    IR_MONITOR_CHG_USER_LEVEL_RESP		= IR_MONITOR_CHG_USER_PWD_REQ + 0x8000,
    IR_MONITOR_QUERY_ALL_USER_RESP		= IR_MONITOR_QUERY_ALL_USER_REQ + 0x8000,
    /* 报警条件 */
    IR_MONITOR_SET_ALARM_CONDITION_RESP = IR_MONITOR_SET_ALARM_CONDITION_REQ + 0x8000,
    IR_MONITOR_GET_ALARM_CONDITION_RESP = IR_MONITOR_GET_ALARM_CONDITION_REQ + 0x8000,
    IR_MONITOR_ALARM_PIC_LIST_RESP       = IR_MONITOR_ALARM_PIC_LIST_REQ + 0x8000,
    IR_MONITOR_ALARM_PIC_GET_RESP 		 = IR_MONITOR_ALARM_PIC_GET_REQ + 0x8000,
    IR_MONITOR_RECORD_CONDITION_RESP	 = IR_MONITOR_RECORD_CONDITION_REQ + 0x8000,

    IR_MONITOR_TIMER_PIC_GET_RESP       = IR_MONITOR_TIMER_PIC_GET_REQ + 0x8000,

    IR_MONITOR_RECORD_CONDITION_GET_RESP= IR_MONITOR_RECORD_CONDITION_GET_REQ + 0x8000,
    IR_MONITOR_DEL_ALARM_CONDITION_RESP = IR_MONITOR_DEL_ALARM_CONDITION_REQ + 0x8000,

    IR_MONITOR_GET_CUR_CRUISE_INFO_RESP = IR_MONITOR_GET_CUR_CRUISE_INFO_REQ + 0x8000,

    IR_MONITOR_FRAME_ANA_ADD_RESP 		= IR_MONITOR_FRAME_ANA_ADD_REQ + 0x8000,
    IR_MONITOR_FRAME_ANA_DEL_RESP 		= IR_MONITOR_FRAME_ANA_DEL_REQ + 0x8000,
    IR_MONITOR_FRAME_CLR_SET_RESP 		= IR_MONITOR_FRAME_CLR_SET_REQ + 0x8000,
    IR_MONITOR_FRAME_ADJUST_RANGE_RESP 	= IR_MONITOR_FRAME_ADJUST_RANGE_REQ + 0x8000,
    IR_MONITOR_FRAME_ADJUST_TYPE_RESP 	= IR_MONITOR_FRAME_ADJUST_TYPE_REQ + 0x8000,

    IR_MONITOR_SET_JPG_RESP         = IR_MONITOR_SET_JPG_REQ + 0x8000,

    IR_MONITOR_MSG_TYPE_BUTT = 0xFFFF
}IR_MONITOR_MSG_TYPE_E;

typedef enum
{
    IR_MONITOR_PLATFORM_CTRL_CMD_SET = 1,
    IR_MONITOR_PLATFORM_CTRL_CMD_CALL_BACK = 2,
    IR_MONITOR_PLATFORM_CTRL_CMD_CRUISE = 3,
    IR_MONITOR_PLATFORM_CTRL_CMD_UP = 4,
    IR_MONITOR_PLATFORM_CTRL_CMD_DOWN = 5,
    IR_MONITOR_PLATFORM_CTRL_CMD_LEFT = 6,
    IR_MONITOR_PLATFORM_CTRL_CMD_RIGHT = 7,
    IR_MONITOR_PLATFORM_CTRL_CMD_BUTT = 0xFFFFFFFF
}IR_MONITOR_PLATFORM_CTRL_CMD_E;

///.协议头标志位
typedef enum
{
    IR_MONITOR_MSG_MAGIC_CMD = 0xA2A22A2A,             //.命令标志位
    IR_MONITOR_MSG_MAGIC_REALTIME = 0xA0A05A5A,    //.实时标志位
    IR_MONITOR_MSG_MAGIC_REPLAYER = 0xA1A11A1A     //.回放标志位
} IR_MONITOR_MSG_MAGIC_E;

///.视频数据类型
typedef enum
{
    IR_MONITOR_VIDEO_DATA_FRAME_NORMAL = 0x00,      //.普通帧数据
    IR_MONITOR_VIDEO_DATA_FRAME_STANDARD = 0x01,   //.标准帧数据 I帧
    IR_MONITOR_VIDEO_DATA_FRAME_H264 = 0x02,             //.H264帧数据
    IR_MONITOR_VIDEO_DATA_FRAME_PS = 0x03,             //.H264帧数据
    IR_MONITOR_VIDEO_DATA_FRAME_JPG = 0x05,                //.单帧JPG帧数据（回放帧跳转使用）
    IR_MONITOR_VIDEO_DATA_FRAME_JPG_REPLAY = 0x06,                //.播放JPG帧数据（回放帧跳转使用）
    IR_MONITOR_VIDEO_DATA_TEMP = 0x10,                           //.温度数据（分析温度）
    IR_MONITOR_VIDEO_DATA_TEMP_END = 0x1F,
    IR_MONITOR_VIDEO_DATA_PALLETE = 0x20,                        //.调色板数据（levelspan）
    IR_MONITOR_VIDEO_DATA_TIME = 0x21                        //.时间数据（只实时使用）
} IR_MONITOR_VIDEO_DATA_TYPE_E;

typedef enum {
    DEVICE_VISIBLELIGHT = 0,
    DEVICE_INFRARED = 1
}DEVICE_CLASS;

typedef enum {
    /*************************** 回放 ****************************/

    /* 回放红外文件错误(数据库找到但是文件有问题) */
    VOD_REPLAY_INFRA_FILE_ERR = -19999,
    /* 回放可见光文件错误(数据库找到但是文件有问题) */
    VOD_REPLAY_VISI_FILE_ERR = -19998,

    /* 回放红外文件播放结束 */
    VOD_REPLAY_INFRA_FILE_END = -19997,
    /* 回放可见光文件播放结束 */
    VOD_REPLAY_VISI_FILE_END = -19996,

    /* 回放端口过多的错误码 */
    VOD_REPLAY_PORT_FULL = -10000,
    /*************************** 回放 END ****************************/
} REPLAY_CODE;

#pragma pack(push)
#pragma pack(1)

typedef struct {
    unsigned int ui_magic;  //2014.11.11 blm add  0xA2A22A2A
    unsigned int msg_type;
    unsigned int total_len;
    unsigned int seq_id;
}MsgHeader;

typedef struct {
    unsigned int client_id;
}XmlReq;

typedef MsgHeader HeartBeatReq;


typedef struct {
    char device_sn[128];
}VodPlayReq;

typedef struct {
    char device_sn[128];
}VodStopReq;

typedef struct {
    unsigned int num;
    char members[1024];
}GroupCfgAddReq;

typedef struct {
    unsigned int id;
}GroupCfgDelReq;

typedef struct {
    unsigned int id;
    unsigned int num;
    char members[1024];
}GroupCfgChgReq;


typedef struct {
    unsigned int n_group_num;
    unsigned int n_ctrl_cmd;
    unsigned int n_session;
    unsigned int n_play_speed;
    char sz_time[28];
}PlaybackControlReq;


//2014.11.14 blm add
//回放请求
typedef struct _tagPLAYBACK_REQ_S{
    unsigned int ui_session;
    unsigned int ui_group_num;
    unsigned short us_rail_preset;
    unsigned short us_ptz_preset;
    unsigned int ui_play_speed;
    char start_time[14];
    char end_time[14];
/*    _tagPLAYBACK_REQ_S()
    {
        ui_session=0;
        ui_group_num=0;
        us_rail_preset=0;
        us_ptz_preset=0;
        ui_play_speed=0;
        memset(pc_time,0,sizeof(pc_time));
    }*/
}PLAYBACK_REQ_S;

Q_DECLARE_METATYPE(PLAYBACK_REQ_S)
//预置点播放信息请求
typedef struct _tagPLAY_PRESET_REQ_S{
    unsigned int ui_session;
    unsigned int ui_group_num;
    unsigned short us_rail_preset;  //导轨预置点
    unsigned short us_ptz_preset;   //云台预置点
    char start_time[14];
    char end_time[14];
    char tz_name[1024];  //云台预置点别名
}PLAY_PRESET_REQ_S;

Q_DECLARE_METATYPE(PLAY_PRESET_REQ_S)
typedef struct {
    unsigned int m_uType;
    unsigned int m_uSessionId;
    unsigned int m_uLen;
    char*        m_szSn;
    unsigned int m_uLenAna;
    char*        m_szAna;
}AnaCtrlReq;

typedef struct {
    unsigned int n_ver;
    unsigned int n_sub_ver;
}VerNegoReq;


typedef struct {
    char sz_login[32];
    char sz_digest[16];
    unsigned int n_time;
    int n_rand;
}LinkAuthReq;

typedef struct {
    int n_session;
}ReplayGetClrIdReq;


typedef struct {
    int n_clr_id;
    int n_session;
}ReplaySetClrIdReq;

//回放中手动设置色标温度范围
typedef struct _tagREPLAY_SET_ADJUSTRANGE_S{
    float f_max;
    float f_min;
    int n_session;
    unsigned int n_frame_no;  //帧号
}REPLAY_SET_ADJUSTRANGE_S;

Q_DECLARE_METATYPE(REPLAY_SET_ADJUSTRANGE_S)

typedef struct _tagAlarmPicListSearch {
    int n_group;
    int n_ptz;
    int n_rail;
    QVector< unsigned int > vct_alarm_id;
    char sz_start[14];
    char sz_end[14];
} AlarmPicListSearch;
Q_DECLARE_METATYPE(AlarmPicListSearch)

typedef struct _tagAlarmPicInfoSearch {
    int n_group;
    int n_ptz;
    int n_rail;
    int n_id_alarm;
    unsigned int n_sec;
    unsigned int n_msec;
} AlarmPicInfoSearch;
Q_DECLARE_METATYPE(AlarmPicInfoSearch)

typedef struct {
    int n_adjust;
    int n_session;
}ReplaySetAdjustAutoReq;

typedef struct {
    char sz_sn[1024];
}RealtimeGetClrIdReq;


typedef struct {
    int n_clr_id;
    char sz_sn[1024];
}RealtimeSetClrIdReq;

typedef struct {
    float f_max;
    float f_min;
    char sz_sn[1024];
}RealtimeSetAdjustRangeReq;

typedef struct {
    int n_adjust;
    char sz_sn[1024];
}RealtimeSetAdjustAutoReq;

typedef struct {
    unsigned int n_num_group;
    unsigned int n_info;
    char sz_info[1024];
}RailCtrlReq, PtzCtrlReq;

typedef struct _tagNetPackageRev{
    unsigned char by_data[2048];
} NetPackageRev, *PNetPackageRev;

typedef struct _tagDeviceCruise{
    unsigned short n_rail_preset;
    unsigned short n_ptz_preset;
    unsigned int n_wait_time;
} DeviceCruise;

typedef struct _tagPresetItem{
    unsigned short n_rail_preset;
    unsigned short n_ptz_preset;
    unsigned short n_x;
    unsigned short n_y;
} PresetItem;
Q_DECLARE_METATYPE( PresetItem )
Q_DECLARE_METATYPE( QList< PresetItem > )

//分析的别名 分析的字符串
typedef struct _tagAnaItem{
    int n_id_ana;
    QString str_name;
    QString str_ana;
} AnaItem;
//

//预置点信息
typedef struct _tagPresetPt
{
    unsigned short n_rail_preset;
    unsigned short n_ptz_preset;
    QString str_preset_name;

    QVector < AnaItem > vct_anas;
}PresetPt;

//blm add
typedef struct _tagURLITEM_S{
    char pc_url[1024];
    char pc_sn[1024];
}URLITEM_S;

typedef struct _tagTEMP_ANA{
    int n_x, n_y;
    float f_temp;
} TEMP_ANA_S;

//若ui_type等于0x10，表示是温度包， 其后的格式见下:
typedef struct tagMONITOR_TEMPERATURE_HEAD_S{
    unsigned int ui_num;
    unsigned int ui_analyse_str_Length;
}MONITOR_TEMPERATURE_HEAD_S;

typedef struct tagMONITOR_TEMPERATURE_PKT_S{
    //quint32 x:10;//x轴坐标（范围为0~1023，全为1表示无坐标的温度）
    //quint32 y:9;// y轴坐标（范围为0~511，全为1表示无坐标的温度）
    //quint32 t1:13;//温度的整数位 （0~8191）
    //quint8 t2;//温度的小数位  （0~255）
    unsigned char by_temp[5];
}MONITOR_TEMPERATURE_PKT_S;

//色标协议
typedef struct tagMONITOR_ADJUST_DATA_S{
    int    i_type;  //色标类型。
    float  f_max;  //最高温度
    float  f_min;  //最低温度
    float  f_range_max; //范围中的最高温度
    float  f_range_min; //范围中的最低温度
}MONITOR_ADJUST_DATA_S;

//回放数据帧包包含的时间信息， 其后的格式见下：
typedef struct tagMONITOR_TIME_HEAD_S{
    unsigned int ui_sec;  //  帧的UTC时间的秒值
    unsigned int ui_usec;  // 帧的微秒值
    unsigned int ui_seq;   // 帧号:上一帧下一帧用。0xFFFFFFFF为无效值,可见光此处填无效值
    unsigned int ui_Length;  // 数据长度，包括头长度和其携带的数据的长度。
} MONITOR_TIME_HEAD_S;

#pragma pack(pop)


#endif // NETCOMMON_H
