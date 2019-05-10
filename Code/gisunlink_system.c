/*
* _COPYRIGHT_
*
* File Name: gisunlink_system.c
* System Environment: JOHAN-PC
* Created Time:2019-01-09
* Author: johan
* QQ: 4493953
* E-mail: johaness@qq.com
* Description: 
*
*/

#include "gisunlink_task.h"
#include "gisunlink_beep.h"
#include "gisunlink_timer.h"
#include "gisunlink_screen.h"
#include "gisunlink_system.h"
#include "gisunlink_eeprom.h"
#include "gisunlink_peripheral.h"

static uint32 CurVsTiem = 0;				//当前电压侦测时间
//static bool Voltage_Sensing = false;		//电压侦测
static bool IsWashExec = false;				//冲洗执行标志
static bool IsWaterExec = false;			//制水执行标志
static uint32 CMakeWaterTime = 0;			//连续制水统计
static uint32 CMakeWashTime = 0;			//连续冲洗统计
static uint32 WashTime = WASHIIME_1;		//冲洗时间
static uint8 systemRunState = SYSTEMPOWER;
static uint8 systemRunTask = SYSTEM_TASK_IDLE;
static gisunlink_sys_time *SystemTime = NULL;
static gisunlink *GSystem = NULL;

static void gisunlink_system_change_beep_mode(uint8 mode) {
	gisunlink_flashing_task *task = gisunlink_task_get_param(GISUNLINK_BEEP_EVENT);	
	switch(mode) {
		case RAPID_BEEP_MODE://急促警报
			task->ontime = 500;
			task->offtime = 500;
			break;
		case NORMAL_BEEP_MODE://正常警报
			task->ontime = 1000;
			task->offtime = 500;			
			break;
	}
	gisunlink_task_set_param(GISUNLINK_BEEP_EVENT,task);
}

static void gisunlink_system_enter_water(void) {
	if(GISUNLINK_TASK_IDLE == gisunlink_task_get_state(GISUNLINK_WATER_EVENT)) {
		gisunlink_peripheral_water();
		gisunlink_task_inactive(GISUNLINK_BEEP_EVENT);
		gisunlink_system_change_beep_mode(RAPID_BEEP_MODE);		
		gisunlink_task_set_exe_cycles(GISUNLINK_BEEP_EVENT,1); //响1声
		gisunlink_task_active(GISUNLINK_WATER_EVENT);
		gisunlink_task_active(GISUNLINK_BEEP_EVENT);
	} 
}

static void gisunlink_system_exit_water(void) {
	gisunlink_peripheral_disworking();
	gisunlink_task_inactive(GISUNLINK_WATER_EVENT);
	gisunlink_task_inactive(GISUNLINK_BEEP_EVENT);
}

static void gisunlink_system_enter_lack(void) {
	if(GISUNLINK_TASK_IDLE == gisunlink_task_get_state(GISUNLINK_LACK_EVENT)) {
		gisunlink_task_inactive(GISUNLINK_BEEP_EVENT);
		gisunlink_system_change_beep_mode(NORMAL_BEEP_MODE);
		gisunlink_task_set_exe_cycles(GISUNLINK_BEEP_EVENT,10); //响10声
		gisunlink_task_active(GISUNLINK_LACK_EVENT);
		gisunlink_task_active(GISUNLINK_BEEP_EVENT);
	} 
}

static void gisunlink_system_exit_lack(void) {
	gisunlink_task_inactive(GISUNLINK_LACK_EVENT);
	gisunlink_task_inactive(GISUNLINK_BEEP_EVENT);
}

static void gisunlink_system_enter_full(void) {
	if(GISUNLINK_TASK_IDLE == gisunlink_task_get_state(GISUNLINK_FULL_EVENT)) {
		gisunlink_task_inactive(GISUNLINK_BEEP_EVENT);
		gisunlink_system_change_beep_mode(RAPID_BEEP_MODE);		
		gisunlink_task_set_exe_cycles(GISUNLINK_BEEP_EVENT,1); //响1声
		gisunlink_task_active(GISUNLINK_FULL_EVENT);
		gisunlink_task_active(GISUNLINK_BEEP_EVENT);
	} 
}

static void gisunlink_system_exit_full(void) {
	gisunlink_task_inactive(GISUNLINK_FULL_EVENT);
	gisunlink_task_inactive(GISUNLINK_BEEP_EVENT);
}

static void gisunlink_system_enter_wash(void) {
	if(GISUNLINK_TASK_IDLE == gisunlink_task_get_state(GISUNLINK_WASH_EVENT)) {
		gisunlink_peripheral_wash();
		gisunlink_task_active(GISUNLINK_WASH_EVENT);
	} 
}

static void gisunlink_system_exit_wash(void) {
	gisunlink_peripheral_disworking();
	gisunlink_task_inactive(GISUNLINK_WASH_EVENT);
}

static void gisunlink_system_chk_lack(uint8 tension, uint8 RunTask) {
	if(tension == TENSIONHIGH) {
		switch(RunTask) {
			case SYSTEM_TASK_WATER:
				IsWaterExec = false;
				gisunlink_system_exit_water();
				systemRunTask = SYSTEM_TASK_LACK;
				break;
			case SYSTEM_TASK_WASH:
				IsWashExec = false;
				gisunlink_system_exit_wash();
				systemRunTask = SYSTEM_TASK_LACK;
				break;
			case SYSTEM_TASK_FULL:
				gisunlink_system_exit_full();
				systemRunTask = SYSTEM_TASK_LACK;
				break;
		}
	}
}

static void gisunlink_system_disposee_key(gisunlink_peripheral *peripheral, uint8 RunTask) {
	switch(peripheral->hasKeyPress) {//仅仅处理冲洗按键的长按事件
		case WASH_KEY|KEY_LONG:
			if(RunTask == SYSTEM_TASK_WATER) {
				gisunlink_system_exit_water();
			} else if(RunTask == SYSTEM_TASK_FULL) {
				gisunlink_system_exit_full();	
			}
			WashTime = WASHIIME_1;						
			systemRunTask = SYSTEM_TASK_WASH; //退到冲洗模式					
			peripheral->hasKeyPress = KEY_NONE;
			break;
	}
}

static void gisunlink_system_schedule(gisunlink_peripheral *peripheral) {
	gisunlink_task *task = NULL;
	gisunlink_system_chk_lack(peripheral->low_tension,systemRunTask);
	gisunlink_system_disposee_key(peripheral,systemRunTask);

	switch(systemRunTask) {
		case SYSTEM_TASK_IDLE: //开机后第一个状态就是这个
			if(peripheral->low_tension == TENSIONHIGH) { //如果低压拉高了则报缺水
				systemRunTask = SYSTEM_TASK_LACK;
			} else {
				systemRunTask = SYSTEM_TASK_WASH; //否则进入冲洗程序
			}
			break;
		case SYSTEM_TASK_WASH:
			if(false == IsWashExec) { //开始进入冲洗模式
				IsWashExec = true;
				CMakeWashTime = gisunlink_timer_get_tick_count();
			} else {
				gisunlink_system_enter_wash();
				if(gisunlink_timer_get_tick_count() >= CMakeWashTime + WashTime) {
					IsWashExec = false;					
					CMakeWashTime = gisunlink_timer_get_tick_count();
					gisunlink_system_exit_wash();
					if(peripheral->high_tension == TENSIONLOW) {//检测到高压开关被拉低了，设置下个模式为 制水模式
						systemRunTask = SYSTEM_TASK_WATER; //退到制水模式
					} else {
						systemRunTask = SYSTEM_TASK_FULL; //退到水满模式
					}
				}
			}
			break;
		case SYSTEM_TASK_WATER:
			if(peripheral->high_tension == TENSIONHIGH) { //检测到高压开关被拉高了，退出制水模式 并 设置下个模式为 水满模式
				gisunlink_system_exit_water();
				IsWaterExec = false;
				CMakeWaterTime = 0;
				systemRunTask = SYSTEM_TASK_FULL; //退到水满模式
			} else { 
				if(false == IsWaterExec) { //检测是否已在制水
					IsWaterExec = true;
					CMakeWaterTime = gisunlink_timer_get_tick_count();//记录开始制水时间
				} else {
					gisunlink_system_enter_water();	//开始制水
					//如果距上一次冲洗时间大于1小时 ，则先退出制水，进入冲洗模式
					if(gisunlink_timer_get_tick_count() >= (CMakeWashTime + WATERTIME_0)) {
						gisunlink_system_exit_water();						
						WashTime = WASHTIME_0;						
						systemRunTask = SYSTEM_TASK_WASH; //退到冲洗模式						
					}
					//如果制水时间大于2小时则设置为系统故障
					if(gisunlink_timer_get_tick_count() >= (CMakeWaterTime + WATERTIME_1)) { 
						systemRunState = SYSTEMDOWN;
						systemRunTask = SYSTEM_TASK_IDLE;						
						gisunlink_system_exit_water();
						IsWaterExec = false;
						CMakeWaterTime = 0;
					}
				}				
			}
			break;
		case SYSTEM_TASK_LACK:
			if(peripheral->low_tension == TENSIONLOW) { //如果检测到低压开关被拉低了，退出缺水模式 并 设置下个模式为 冲洗模式
				gisunlink_system_exit_lack();
				WashTime = WASHIIME_1;
				systemRunTask = SYSTEM_TASK_WASH; //退到冲洗模式
			} else {
				gisunlink_system_enter_lack();
			}
			break;
		case SYSTEM_TASK_FULL:
			if(peripheral->high_tension == TENSIONLOW) { //检测到高压开关被拉低了，退出水满模式 并 设置下个模式为 制水模式
				gisunlink_system_exit_full();
				systemRunTask = SYSTEM_TASK_WATER; //退到制水模式
			} else {
				gisunlink_system_enter_full();
			}
			break;
	}
}

static void gisunlink_system_init_voltage_sensing(void) {
	clr_BOF;
	clr_BORF;
	clr_BORST;
	clr_BOV1;
	clr_BOV0;
	set_EBOD;
	set_EA;
	set_BODEN;	
}

static void gisunlink_system_voltage_sensing_disposee(void) {
	//1秒钟内过滤过多中断响应
	if(gisunlink_timer_get_tick_count() - CurVsTiem >= 1000) {
		CurVsTiem = gisunlink_timer_get_tick_count();
		gisunlink_eeprom_write_runtime(SystemTime->sec,SystemTime->min,SystemTime->hour);
	}
}

/************************************************************************************************************
 *    BOD_ISR 8 interrupt subroutine
 ************************************************************************************************************/
 void BOD_ISR(void) interrupt 8 {
	clr_EA;
	clr_BODEN;
	clr_BOF;
	gisunlink_system_voltage_sensing_disposee();
	set_BODEN;//重新打开检测位	 
	set_EA;
}

void gisunlink_system_init(gisunlink *GlobalSystem) {
	if(GlobalSystem) {
		GSystem = GlobalSystem;
		SystemTime = &GlobalSystem->Time;
		GSystem->Beep.freq = BEEP_FREQ;
		GSystem->Beep.duty = BEEP_DUTY;
		//检测是否烧录后第一次启动 不是 则读取保存的时间
		if(gisunlink_eeprom_check_first_run() == false) {
			//读取设备已运行时间
			gisunlink_eeprom_read_runtime(SystemTime);
		}
	}
	gisunlink_system_init_voltage_sensing();
	gisunlink_task_active(GISUNLINK_SYSTEM_CHK_EVENT);
}

void gisunlink_system_down(void) {
	systemRunState = SYSTEMDOWN;	
	if(systemRunTask != SYSTEM_TASK_IDLE) {
		switch(systemRunTask) {
			case SYSTEM_TASK_WATER:
				IsWaterExec = false;
				gisunlink_system_exit_water();
				break;
			case SYSTEM_TASK_WASH:
				IsWashExec = false;
				gisunlink_system_exit_wash();
				break;
			case SYSTEM_TASK_FULL:
				gisunlink_system_exit_full();
				break;
			case SYSTEM_TASK_LACK:
				gisunlink_system_exit_lack();
				break;
		}		
		systemRunTask = SYSTEM_TASK_IDLE;
	}
}

void gisunlink_system_poweron(void) {
	systemRunState = SYSTEMON;
	gisunlink_task_active(GISUNLINK_FILTER_EVENT);
}

bool gisunlink_system_isRuning(void) {//2秒后开始处理状态
	if(systemRunState == SYSTEMON && gisunlink_timer_get_tick_count() >= 2000) {
		return true;
	}
	return false;
}

uint8 gisunlink_system_RunState(void) {
	return systemRunState;
}

void gisunlink_system_check(gisunlink_peripheral *peripheral) {
	switch(systemRunState) {
		case SYSTEMON: //检测2秒开机
			if(gisunlink_system_isRuning()) {
				systemRunState = SYSTEMRUNING;
			}
			break;
		case SYSTEMDOWN: //系统故障
			peripheral->hasKeyPress = KEY_NONE; //忽略按键事件
			if(GISUNLINK_TASK_IDLE == gisunlink_task_get_state(GISUNLINK_FIXUP_EVENT)) {
				gisunlink_task_inactive(GISUNLINK_BEEP_EVENT);
				gisunlink_system_change_beep_mode(RAPID_BEEP_MODE);				
				gisunlink_task_set_exe_cycles(GISUNLINK_BEEP_EVENT,120); //1声为1秒 ，120则2分钟
				gisunlink_task_active(GISUNLINK_FIXUP_EVENT);
				gisunlink_task_active(GISUNLINK_BEEP_EVENT);				
			}				
			break;
		case SYSTEMRUNING://系统运行中
			gisunlink_system_schedule(peripheral);
			break;
	}
}