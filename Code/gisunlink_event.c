/*
* _COPYRIGHT_
*
* File Name: gisunlink_event.c
* System Environment: JOHAN-PC
* Created Time:2019-01-11
* Author: johan
* QQ: 4493953
* E-mail: johaness@qq.com
* Description: 
*
*/

#include "gisunlink_event.h"
#include "gisunlink_beep.h"
#include "gisunlink_task.h"
#include "gisunlink_screen.h"
#include "gisunlink_eeprom.h"
#include "gisunlink_system.h"
#include "gisunlink_peripheral.h"

static uint8 FilterShowPos = 0;

static uint8 gisunlink_get_icon_event_screen_index(uint8 taskid) {
	uint8 bit_index = 0;
	switch(taskid) {
		case GISUNLINK_WASH_EVENT:
			bit_index = SCREENBITA;
			break;
		case GISUNLINK_FULL_EVENT:
			bit_index = SCREENBITC;
			break;
		case GISUNLINK_LACK_EVENT:
			bit_index = SCREENBITE;
			break;
		case GISUNLINK_FIXUP_EVENT:
			bit_index = SCREENBITG;
			break;
		case GISUNLINK_WATER_EVENT:
			bit_index = SCREENBITG;
			break;
	}
	return bit_index;
}

static void gisunlink_com3_icon_dispose(uint8 cur_state,uint8 bit_index) {
	uint8 val = gisunlink_screen_get_vdata(SCREENCOM3);
	if(FLASHING_TASK_ON == cur_state) {
		gisunlink_screen_set_vdata(SCREENCOM3,(val |= bit_index));		
	} else {
		gisunlink_screen_set_vdata(SCREENCOM3,(val &= ~bit_index));		
	}
}

static void gisunlink_filter_show_level(uint8 level) {
	switch(level) {
		case GISUNLINK_FILTER_LEVEL0:
			gisunlink_screen_set_vdata(SCREENCOM2,0);
			break;
		case GISUNLINK_FILTER_LEVEL1:
			gisunlink_screen_set_vdata(SCREENCOM2,12);// SCREENBITE | SCREENBITF
			break;	
		case GISUNLINK_FILTER_LEVEL2:
			gisunlink_screen_set_vdata(SCREENCOM2,204);// SCREENBITE | SCREENBITF | SCREENBITA | SCREENBITB
			break;	
		case GISUNLINK_FILTER_LEVEL3:
			gisunlink_screen_set_vdata(SCREENCOM2,252);// SCREENBITE | SCREENBITF | SCREENBITA | SCREENBITB | SCREENBITC | SCREENBITD
			break;	
		case GISUNLINK_FILTER_LEVEL4:
			gisunlink_screen_set_vdata(SCREENCOM2,255);// SCREENBITE | SCREENBITF | SCREENBITA | SCREENBITB | SCREENBITC | SCREENBITD | SCREENBITH | SCREENBITG
			break;			
	}
}

static uint8 LevelCushion = 0; //用于处理滤芯显示变化缓冲，当刷新坐标为0时才拷贝
static void gisunlink_filter_icon_dispose(uint8 FilterLevel) {
	switch(LevelCushion) {
		case GISUNLINK_FILTER_LEVEL4:
		case GISUNLINK_FILTER_LEVEL3:
		case GISUNLINK_FILTER_LEVEL2:
		case GISUNLINK_FILTER_LEVEL1:
			if(FilterShowPos > LevelCushion) {
				FilterShowPos = 0;
				LevelCushion = FilterLevel;
			}						
			gisunlink_filter_show_level(FilterShowPos);
			FilterShowPos++;	
			break;		
		case GISUNLINK_FILTER_LEVEL0:
			LevelCushion = FilterLevel;
			gisunlink_filter_show_level(LevelCushion);
			break;
	}
}

void gisunlink_icon_event_run(uint8 taskid,uint8 cur_state,void *attr) {
	uint8 bit_index = gisunlink_get_icon_event_screen_index(taskid);
	uint8 FilterLevel = 0;
	switch(taskid) {
		case GISUNLINK_WASH_EVENT:
		case GISUNLINK_FULL_EVENT:
		case GISUNLINK_LACK_EVENT:
		case GISUNLINK_FIXUP_EVENT:
			gisunlink_com3_icon_dispose(cur_state,bit_index);
			break;
		case GISUNLINK_WATER_EVENT:
			if(FLASHING_TASK_ON == cur_state) {
				gisunlink_screen_set_vdata(SCREENCOM1,0x54 | bit_index); //ABCDEF
			} else {
				gisunlink_screen_set_vdata(SCREENCOM1,0xA8 | bit_index); //ABCDE
			}
			break;
		case GISUNLINK_FILTER_EVENT:
			FilterLevel = *((uint8 *)attr);
			gisunlink_filter_icon_dispose(FilterLevel);
			break;
	}
}

void gisunlink_icon_event_stop(uint8 taskid) {
	uint8 bit_index = gisunlink_get_icon_event_screen_index(taskid);
	switch(taskid) {
		case GISUNLINK_WASH_EVENT:
		case GISUNLINK_FULL_EVENT:
		case GISUNLINK_LACK_EVENT:
		case GISUNLINK_FIXUP_EVENT:
			gisunlink_com3_icon_dispose(FLASHING_TASK_OFF,bit_index);
			break;
		case GISUNLINK_WATER_EVENT:
			gisunlink_screen_set_vdata(SCREENCOM1,SCREENBITG);
			break;
		case GISUNLINK_FILTER_EVENT:
			break;
	}

}

void gisunlink_beep_event_run(uint8 taskid, uint8 cur_state, void *attr) {
	gisunlink_beep *Beep = NULL;
	UNUSED_VARIANT(taskid);
	Beep = (gisunlink_beep *)attr;
	if(cur_state == FLASHING_TASK_ON) {
		gisunlink_beep_work_start(Beep->freq,Beep->duty);
	} else {
		gisunlink_beep_work_stop();
	}
}

void gisunlink_beep_event_stop(uint8 taskid) {
	UNUSED_VARIANT(taskid);
	gisunlink_beep_work_stop();
}

void gisunlink_system_event(uint8 taskid, void *attr) {
	UNUSED_VARIANT(taskid);
	gisunlink_system_check((gisunlink_peripheral *)attr);
}
