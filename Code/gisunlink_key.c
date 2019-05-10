/*
* _COPYRIGHT_
*
* File Name: gisunlink_key.c
* System Environment: JOHAN-PC
* Created Time:2019-01-10
* Author: johan
* QQ: 4493953
* E-mail: johaness@qq.com
* Description: 
*
*/

#include "gisunlink_key.h"
#include "gisunlink_timer.h"

void gisunlink_key_reset_io(void) {
	P00_Input_Mode;
	P01_Input_Mode;
	P02_Input_Mode;
	P10_Input_Mode;	
}

static uint8 gisunlink_key_read(void) {
	if(!GISUNLINK_WASH_KEY) {
		return WASH_KEY;
	}

	if(!GISUNLINK_POWER_KEY) {
		return POWER_KEY;		
	}

	if(!GISUNLINK_ROFILM_KEY) {
		return ROFILM_KEY;		
	}

	if(!GISUNLINK_FILTER_KEY) {
		return FILTER_KEY;		
	}	
	return KEY_NONE;
}

void gisunlink_key_scan(gisunlink_peripheral *peripheral) {
	static uint32 LongPressTime = 0;
	static uint8 state = KEY_RELEASE;
	static uint8 LastPreeKey = 0, CurPressKey = 0;
	CurPressKey = gisunlink_key_read();
	switch(state)	{
		case KEY_RELEASE:
			if(CurPressKey != LastPreeKey)	
				state = KEY_PRESS; 					//有按键按下
			break;		
		case KEY_PRESS: 		
			if(CurPressKey == LastPreeKey)	
				state = KEY_PRESS_CHK; 	//开始检测按键
			else 
				state = KEY_RELEASE; 		//误触发
			break; 		
		case KEY_PRESS_CHK: 
			if(CurPressKey == LastPreeKey) {//如果还是按着的状态 可能是长按
				state = KEY_LONG_PRESS_CHK;
			} else {												//短按
				state = KEY_RELEASE; 								
				peripheral->hasKeyPress = LastPreeKey|KEY_SHORT;  
			}
			break;				
		case KEY_LONG_PRESS_CHK: 
			if(CurPressKey == LastPreeKey) {			    
				if(0 == LongPressTime) {
					LongPressTime = gisunlink_timer_get_tick_count();
				}
				if(gisunlink_timer_get_tick_count() >= (LongPressTime + 1500)) {//如果大于1.5秒 则为长按事件
					LongPressTime = 0; 					
					state = KEY_LONG_PRESS_RELEASE;					
					peripheral->hasKeyPress = LastPreeKey|KEY_LONG;
				} else {				
					state = KEY_RELEASE;				
					peripheral->hasKeyPress = LastPreeKey|KEY_SHORT;
				}
			}
			break;		
		case KEY_LONG_PRESS_RELEASE:
			if(CurPressKey != LastPreeKey)
				state = KEY_RELEASE;		
			break;
	}
	LastPreeKey = CurPressKey;
}
