/*
* _COPYRIGHT_
*
* File Name: gisunlink_peripheral.c
* System Environment: JOHAN-PC
* Created Time:2018-12-25
* Author: johan
* QQ: 4493953
* E-mail: johaness@qq.com
* Description: 
*
*/

#include "gisunlink_key.h"
#include "gisunlink_common.h"
#include "gisunlink_tension.h"
#include "gisunlink_peripheral.h"

static gisunlink_peripheral *peripheral = NULL;

static void gisunlink_peripheral_cfg_io(void) {
	P17_PushPull_Mode;
	P30_PushPull_Mode;
}

void gisunlink_peripheral_init(gisunlink_peripheral *SysPeripheral) {
	if(SysPeripheral) {
		memset(SysPeripheral,0x00,sizeof(gisunlink_peripheral));
		peripheral = SysPeripheral;
		peripheral->low_tension = TENSIONUNKNOWN;
		peripheral->high_tension = TENSIONUNKNOWN;
	}

	gisunlink_tension_init();
	gisunlink_peripheral_cfg_io();
	gisunlink_peripheral_disworking();
}

void gisunlink_peripheral_wash(void) {
	if(GISUNLINK_INLET != GISUNLINK_WORKING) {
		GISUNLINK_INLET = GISUNLINK_WORKING;
	}
	if(GISUNLINK_FLUSH != GISUNLINK_WORKING) {	
		GISUNLINK_FLUSH = GISUNLINK_WORKING;
	}
}

void gisunlink_peripheral_water(void) {
	if(GISUNLINK_INLET != GISUNLINK_WORKING) {	
		GISUNLINK_INLET = GISUNLINK_WORKING;
	}

	if(GISUNLINK_FLUSH != GISUNLINK_DISWORKING) {	
		GISUNLINK_FLUSH = GISUNLINK_DISWORKING;
	}	
}

void gisunlink_peripheral_disworking(void) {
	if(GISUNLINK_INLET != GISUNLINK_DISWORKING) {	
		GISUNLINK_INLET = GISUNLINK_DISWORKING;
	}		

	if(GISUNLINK_FLUSH != GISUNLINK_DISWORKING) {	
		GISUNLINK_FLUSH = GISUNLINK_DISWORKING;
	}	
}

void gisunlink_peripheral_check(void) {
	gisunlink_tension_reset_io();	
	gisunlink_key_reset_io();
	gisunlink_delay(PERIPHERA_REFRESH_UNIT,PERIPHERAL_REFRESH_DELAY);
	gisunlink_tension_scan(peripheral);	
	gisunlink_key_scan(peripheral);
	return;
}

