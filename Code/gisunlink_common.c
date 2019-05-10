/*
* _COPYRIGHT_
*
* File Name: gisunlink_common.c
* System Environment: JOHAN-PC
* Created Time:2019-01-11
* Author: johan
* QQ: 4493953
* E-mail: johaness@qq.com
* Description: 
*
*/

#include "gisunlink_common.h"

bool gisunlink_chk_dev_expiration(uint16 hour) {
#if ENABLE_DEV_EXPIRATION
	if(hour >= MAX_DEV_EXPIRATION_TIME) {
		return true;
	}
	return false;
#else 
	UNUSED_VARIANT(hour);
	return false;
#endif
}

uint8 gisunlink_chk_filter_level(uint16 hour) {
#if ENABLE_FILTER
	if(hour < MAX_FILTER_LEVEL4 && hour >= MIN_FILTER_LEVEL4) {
		return GISUNLINK_FILTER_LEVEL4; 
	}
	if(hour < MAX_FILTER_LEVEL3 && hour >= MIN_FILTER_LEVEL3) {
		return GISUNLINK_FILTER_LEVEL3;
	}
	if(hour < MAX_FILTER_LEVEL2 && hour >= MIN_FILTER_LEVEL2) {
		return GISUNLINK_FILTER_LEVEL2;
	}
	if(hour < MAX_FILTER_LEVEL1 && hour >= MIN_FILTER_LEVEL1) {
		return GISUNLINK_FILTER_LEVEL1;
	}

	return GISUNLINK_FILTER_LEVEL0;
#else
	UNUSED_VARIANT(hour);
	return  GISUNLINK_FILTER_LEVEL4;
#endif
}

void gisunlink_delay(uint16 unit,uint32 u32CNT) {
	clr_T2DIV2;																	//Timer2 Clock = Fsys/4 
	clr_T2DIV1;
	set_T2DIV0;
	set_TR2;                                		//Start Timer2
	while (u32CNT != 0)
	{
		TL2 = LOBYTE(unit);		//Find  define in "Function_define.h" "TIMER VALUE"
		TH2 = HIBYTE(unit);
		while (TF2 != 1);                   		//Check Timer2 Time-Out Flag
		clr_TF2;
		u32CNT --;
	}
	clr_TR2;                                		//Stop Timer2
}

