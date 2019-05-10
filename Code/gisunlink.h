/*
* _COPYRIGHT_
*
* File Name:gisunlink.h
* System Environment: JOHAN-PC
* Created Time:2018-12-25
* Author: johan
* QQ: 4493953
* E-mail: johaness@qq.com
* Description: 
*
*/

#ifndef __GISUNLINK_H__
#define __GISUNLINK_H__

#include <string.h>
#include "N76E003.h"
#include "SFR_Macro.h"
#include "Function_define.h"
#include "Common.h"
#include "gisunlink_type.h"

//系统资源定义
#define GISUNLINK_BEEP						P05	//蜂鸣器		PWM2_P05_OUTPUT_ENABLE
#define GISUNLINK_SCREEN_COM4				P07	//屏幕段选
#define GISUNLINK_SCREEN_COM3				P06	//屏幕段选	状态显示
#define GISUNLINK_SCREEN_COM2				P03 //屏幕段选	主要显示滤芯状态
#define GISUNLINK_SCREEN_COM1				P04	//屏幕段选	显示制水文字以及制水图标
#define GISUNLINK_SCREEN_BITH				P02 //屏幕位选 复用 滤芯
#define GISUNLINK_SCREEN_BITG				P01 //屏幕位选 复用 RO膜
#define GISUNLINK_SCREEN_BITF				P00 //屏幕位选 复用 电源
#define GISUNLINK_SCREEN_BITE				P10 //屏幕位选 复用 强冲
#define GISUNLINK_SCREEN_BITD				P11 //屏幕位选 复用 低压
#define GISUNLINK_SCREEN_BITC				P12 //屏幕位选 复用 高压
#define GISUNLINK_SCREEN_BITB				P13 //屏幕位选
#define GISUNLINK_SCREEN_BITA				P14 //屏幕位选

#define GISUNLINK_HIGH_TENSION				GISUNLINK_SCREEN_BITC	//低压
#define GISUNLINK_LOW_TENSION				GISUNLINK_SCREEN_BITD	//高压
#define GISUNLINK_FLUSH						P30	//冲洗阀
#define GISUNLINK_INLET						P17	//进水阀

#define GISUNLINK_WASH_KEY					GISUNLINK_SCREEN_BITE	//强冲按键
#define GISUNLINK_POWER_KEY					GISUNLINK_SCREEN_BITF	//电源按键
#define GISUNLINK_ROFILM_KEY				GISUNLINK_SCREEN_BITG	//RO膜按键
#define GISUNLINK_FILTER_KEY				GISUNLINK_SCREEN_BITH	//滤芯按键

//系统事件或者状态定义
//屏幕状态定义
#define GISUNLINK_SCREEN_SEL				0xff
#define GISUNLINK_SCREEN_DISSEL				0x00
#define GISUNLINK_SCREEN_SHOW				0x00
#define GISUNLINK_SCREEN_DISSHOW			0xff
#define GISUNLINK_WORKING					0xff
#define GISUNLINK_DISWORKING				0x00

//滤芯状态定义
#define ENABLE_FILTER						0	//是否启动滤芯实际状态显示 1 启动 0 不启动
#define ENABLE_DEV_EXPIRATION				0	//是否启动设备过期验证 1 启动 0 不启动
#define MAX_DEV_EXPIRATION_TIME				2000 //滤芯最大使用时间

#define MAX_FILTER_LEVEL4					500	 //滤芯状态使用值
#define MIN_FILTER_LEVEL4					0
#define MAX_FILTER_LEVEL3					1000
#define MIN_FILTER_LEVEL3					500
#define MAX_FILTER_LEVEL2					1500
#define MIN_FILTER_LEVEL2					1000
#define MAX_FILTER_LEVEL1					2000
#define MIN_FILTER_LEVEL1					1500

#define GISUNLINK_FILTER_LEVEL4				4
#define GISUNLINK_FILTER_LEVEL3				3
#define GISUNLINK_FILTER_LEVEL2				2
#define GISUNLINK_FILTER_LEVEL1				1
#define GISUNLINK_FILTER_LEVEL0				0

//系统事件
#define GISUNLINK_WASH_EVENT				0 	//冲洗事件
#define GISUNLINK_WATER_EVENT				1 	//制水事件
#define GISUNLINK_FULL_EVENT				2 	//水满事件
#define GISUNLINK_LACK_EVENT				3 	//缺水事件
#define GISUNLINK_FIXUP_EVENT				4 	//维修事件
#define GISUNLINK_FILTER_EVENT				5 	//滤芯状态事件
#define GISUNLINK_BEEP_EVENT				6 	//警报事件
#define GISUNLINK_SYSTEM_CHK_EVENT			7 	//系统状态检测以及处理
#define GISUNLINK_MAX_EVENT_NUM				8	//数量根据上面事件定义决定
#define GISUNLINK_MAX_ICON_EVENT_NUM		5 	//最大图标事件数

//系统任务
#define GISUNLINK_TASK_ALWAYS_RUN			0xFFFF
#define GISUNLINK_RUN_FAULT					0xFF
#define GISUNLINK_RUN_NORMAL				0x00
#define GISUNLINK_TASK_IDLE					0x00
#define GISUNLINK_TASK_START				0x20
#define GISUNLINK_TASK_READY				0x30
#define GISUNLINK_TASK_RUN					0x40
#define GISUNLINK_TASK_RUNING				0x50
#define GISUNLINK_TASK_STOP					0x60
#define GISUNLINK_TASK_ERR					0xFF

#define SYSTEM_TASK_IDLE					0x1		//系统闲置
#define SYSTEM_TASK_WASH					0x2 	//冲洗
#define SYSTEM_TASK_LACK					0x3 	//缺水
#define SYSTEM_TASK_WATER					0x4 	//制水
#define SYSTEM_TASK_FULL					0x5 	//水满

#define WASHTIME_0							5000		//冲洗5秒
#define WASHIIME_1							30000		//冲洗30秒
#define WATERTIME_0 						3600000		//制水1小时
#define WATERTIME_1 						7200000		//制水2小时


//系统延时
#define SYSTEM_DELAY_10US					TIMER_DIV4_VALUE_10us
#define SYSTEM_DELAY_100US					TIMER_DIV4_VALUE_100us
#define SYSTEM_DELAY_200US					TIMER_DIV4_VALUE_200us
#define SYSTEM_DELAY_500US					TIMER_DIV4_VALUE_500us
#define SYSTEM_DELAY_1MS 					TIMER_DIV4_VALUE_1ms
#define SCREEN_REFRESH_UNIT					SYSTEM_DELAY_500US
#define SCREEN_REFRESH_DELAY				7
#define PERIPHERA_REFRESH_UNIT				TIMER_DIV4_VALUE_10us
#define PERIPHERAL_REFRESH_DELAY			10

//外围检测
#define TENSIONMAXREADCOUNT					100
#define TENSIONLOW							0x0F
#define TENSIONHIGH							0xF0
#define TENSIONUNKNOWN						0xFF

//系统运行状态
#define SYSTEMPOWER							0x00
#define SYSTEMON							0xF0
#define SYSTEMRUNING						0xFF
#define SYSTEMDOWN							0x0F

//按键定义
#define WASH_KEY							0x01
#define POWER_KEY							0x02
#define ROFILM_KEY							0x04
#define FILTER_KEY							0x08
#define KEY_SHORT							0x40
#define KEY_LONG							0x80
#define KEY_NONE							0x00

#define KEY_RELEASE							0
#define KEY_PRESS							1
#define KEY_PRESS_CHK						2
#define KEY_LONG_PRESS_CHK					3
#define KEY_LONG_PRESS_RELEASE				4

//警报模式
#define NORMAL_BEEP_MODE					0			//正常警报
#define RAPID_BEEP_MODE						1			//急促警报
#define BEEP_FREQ							2000		//蜂鸣器频率 2000 = 2K
#define BEEP_DUTY							50			//蜂鸣器占空比 50 = 50%

//消除未使用警告
#define UNUSED_VARIANT(x) 		if(x){}
															
#endif //__GISUNLINK_H__
