/*
* _COPYRIGHT_
*
* File Name: main.c
* System Environment: JOHAN-PC
* Created Time:2018-12-25
* Author: johan
* QQ: 4493953
* E-mail: johaness@qq.com
* Description: 
*
*/

#include "gisunlink.h"
#include "gisunlink_task.h"
#include "gisunlink_beep.h"
#include "gisunlink_event.h"
#include "gisunlink_timer.h"
#include "gisunlink_eeprom.h"
#include "gisunlink_screen.h"
#include "gisunlink_system.h"
#include "gisunlink_common.h"
#include "gisunlink_peripheral.h"

bit BIT_TMP;

static gisunlink GlobalSystem = {0};
static gisunlink_sys_time *SystemTime = &GlobalSystem.Time;
static gisunlink_peripheral *SystemPeripheral = &GlobalSystem.Peripheral;

static gisunlink_flashing_task wash_icon = {0,500,500,gisunlink_icon_event_run,gisunlink_icon_event_stop};
static gisunlink_flashing_task water_icon = {0,150,150,gisunlink_icon_event_run,gisunlink_icon_event_stop};
static gisunlink_flashing_task full_icon = {0,500,500,gisunlink_icon_event_run,gisunlink_icon_event_stop};
static gisunlink_flashing_task lack_icon = {0,500,500,gisunlink_icon_event_run,gisunlink_icon_event_stop};
static gisunlink_flashing_task fixup_icon = {0,500,500,gisunlink_icon_event_run,gisunlink_icon_event_stop};
static gisunlink_flashing_task filter_icon = {0,500,500,gisunlink_icon_event_run,gisunlink_icon_event_stop};
static gisunlink_flashing_task beep_act = {0,1000,500,gisunlink_beep_event_run,gisunlink_beep_event_stop};
static gisunlink_normal_task system_chk = {50,gisunlink_system_event, NULL};

static gisunlink_task TaskList[GISUNLINK_MAX_EVENT_NUM] = {
	{GISUNLINK_TASK_IDLE,FLASHING_TASK,NULL,&wash_icon,0xFFFF},												//冲水图标
	{GISUNLINK_TASK_IDLE,FLASHING_TASK,NULL,&water_icon,0xFFFF},											//制水图标
	{GISUNLINK_TASK_IDLE,FLASHING_TASK,NULL,&full_icon,0xFFFF},												//水满图标
	{GISUNLINK_TASK_IDLE,FLASHING_TASK,NULL,&lack_icon,0xFFFF},												//缺水图标
	{GISUNLINK_TASK_IDLE,FLASHING_TASK,NULL,&fixup_icon,0xFFFF},											//检修图标
	{GISUNLINK_TASK_IDLE,FLASHING_TASK,&GlobalSystem.FilterLevel,&filter_icon,0xFFFF},						//滤芯图标	 
	{GISUNLINK_TASK_IDLE,FLASHING_TASK,&GlobalSystem.Beep,&beep_act,0xFFFF},								//蜂鸣器	
	{GISUNLINK_TASK_IDLE,NORMAL_TASK,&GlobalSystem.Peripheral,&system_chk,0xFFFF}							//系统运行状态检测
};

static void gisunlink_timer_callback(void) {
	SystemTime->sec++;
	if(gisunlink_timer_proc_time(SystemTime)) {
		//获取滤芯等级
		GlobalSystem.FilterLevel = gisunlink_chk_filter_level(SystemTime->hour);
		//验证机器使用寿命
		if(gisunlink_chk_dev_expiration(SystemTime->hour)) {
			if(gisunlink_system_RunState() != SYSTEMDOWN) {
				gisunlink_system_down();
			}
		}
	}
}

void main (void) {
	//设置所有IO为双向模式
	Set_All_GPIO_Quasi_Mode;

	//初始化时间结构
	gisunlink_timer_time(SystemTime);
	//系统初始化
	gisunlink_system_init(&GlobalSystem);
	//获取滤芯等级
	GlobalSystem.FilterLevel = gisunlink_chk_filter_level(SystemTime->hour);

	//初始化外围接口 主要为进水阀和冲洗阀
	gisunlink_peripheral_init(SystemPeripheral);	
	//初始化蜂鸣器接口
	gisunlink_beep_init();	
	//初始化屏幕
	gisunlink_screen_init();
	//初始化定时器
	gisunlink_timer_init(SystemTime, gisunlink_timer_callback,TaskList);

	//验证机器使用寿命
	if(gisunlink_chk_dev_expiration(SystemTime->hour)) {
		//系统异常开机 并 直接报警
		gisunlink_system_down();
	} else {
		//系统正常开机
		gisunlink_system_poweron();					
	}

	while(1) {
		//检测到电压正常才执行下列动作
	//	if(gisunlink_system_voltage_sensing()) {
			//屏幕刷新
			gisunlink_screen_refresh();
			//外围信号检测
			gisunlink_peripheral_check();			
			//处理微任务
			gisunlink_task_check();
	//	}
	}
}
