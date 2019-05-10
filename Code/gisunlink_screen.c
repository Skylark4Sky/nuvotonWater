/*
* _COPYRIGHT_
*
* File Name: gisunlink_screen.c
* System Environment: JOHAN-PC
* Created Time:2018-12-25
* Author: johan
* QQ: 4493953
* E-mail: johaness@qq.com
* Description: 
*
*/

#include "gisunlink_common.h"
#include "gisunlink_screen.h"

#define SCREENCOM1DEFVAL	02
#define SCREENCOM2DEFVAL	00
#define SCREENCOM3DEFVAL	85

#define MAXSCREENCOM		3
#define MINSCREENCOM		1
#define MAXSCREENBIT		8

static uint8 screen_disp_row = MAXSCREENCOM;

static gisunlink_screen ScreenComScan[MAXSCREENCOM] = {
	{SCREENCOM1DEFVAL},//
	{SCREENCOM2DEFVAL},
	{SCREENCOM3DEFVAL}
};

static void gisunlink_screen_com_set(uint8 comID, uint8 val) {
	switch(comID) {
		case SCREENCOM3:
			GISUNLINK_SCREEN_COM3 = val;
			break;		
		case SCREENCOM2:
			GISUNLINK_SCREEN_COM2 = val;
			break;
		case SCREENCOM1:
			GISUNLINK_SCREEN_COM1 = val;
			break;
	}
}

static void gisunlink_screen_bit_set(uint8 bitID, uint8 val) {
	switch(bitID) {
		case SCREENBITA:
			GISUNLINK_SCREEN_BITA = val;
			break;
		case SCREENBITB:
			GISUNLINK_SCREEN_BITB = val;
			break;
		case SCREENBITC:
			GISUNLINK_SCREEN_BITC = val;
			break;
		case SCREENBITD:
			GISUNLINK_SCREEN_BITD = val;
			break;
		case SCREENBITE:
			GISUNLINK_SCREEN_BITE = val;
			break;
		case SCREENBITF:
			GISUNLINK_SCREEN_BITF = val;
			break;
		case SCREENBITG:
			GISUNLINK_SCREEN_BITG = val;
			break;
		case SCREENBITH:
			GISUNLINK_SCREEN_BITH = val;
			break;
	}
}

static void gisunlink_screen_clearAllCom(void) {
	//关闭全部段选位
	gisunlink_screen_com_set(SCREENCOM1,GISUNLINK_SCREEN_DISSEL);
	gisunlink_screen_com_set(SCREENCOM2,GISUNLINK_SCREEN_DISSEL);
	gisunlink_screen_com_set(SCREENCOM3,GISUNLINK_SCREEN_DISSEL);
	gisunlink_screen_com_set(SCREENCOM4,GISUNLINK_SCREEN_DISSEL);
}

static void gisunlink_screen_clearAllBit(void) {
	//关闭全部位选位
	gisunlink_screen_bit_set(SCREENBITA,GISUNLINK_SCREEN_DISSHOW);
	gisunlink_screen_bit_set(SCREENBITB,GISUNLINK_SCREEN_DISSHOW);
	gisunlink_screen_bit_set(SCREENBITC,GISUNLINK_SCREEN_DISSHOW);
	gisunlink_screen_bit_set(SCREENBITD,GISUNLINK_SCREEN_DISSHOW);
	gisunlink_screen_bit_set(SCREENBITE,GISUNLINK_SCREEN_DISSHOW);
	gisunlink_screen_bit_set(SCREENBITF,GISUNLINK_SCREEN_DISSHOW);
	gisunlink_screen_bit_set(SCREENBITG,GISUNLINK_SCREEN_DISSHOW);
	gisunlink_screen_bit_set(SCREENBITH,GISUNLINK_SCREEN_DISSHOW);
}

static void gisunlink_screen_reset_io(void) {
	//重设段
	P07_PushPull_Mode;
	P06_PushPull_Mode;
	P03_PushPull_Mode;
	P04_PushPull_Mode;

	//位重设
	P02_PushPull_Mode;
	P01_PushPull_Mode;
	P00_PushPull_Mode;
	P10_PushPull_Mode;
	P11_PushPull_Mode;
	P12_PushPull_Mode;
	P13_PushPull_Mode;
	P14_PushPull_Mode;

	gisunlink_screen_clearAllCom();
	gisunlink_screen_clearAllBit();
	return;
}

uint8 gisunlink_screen_get_vdata(uint8 comID) {
	uint8 vdata = 0;
	switch(comID) {
		case SCREENCOM3:
			vdata = ScreenComScan[2].comBit;
			break;		
		case SCREENCOM2:
			vdata = ScreenComScan[1].comBit;
			break;
		case SCREENCOM1:
			vdata = ScreenComScan[0].comBit;
			break;
	}
	return vdata;
}

void gisunlink_screen_set_vdata(uint8 comID, uint8 vdata) {
	switch(comID) {
		case SCREENCOM3:
			ScreenComScan[2].comBit = vdata;
			break;		
		case SCREENCOM2:
			ScreenComScan[1].comBit = vdata;
			break;
		case SCREENCOM1:
			ScreenComScan[0].comBit = vdata;
			break;
	}
}

static void gisunlink_screen_bitRefresh(uint8 bitData,uint8 bitLen) {
	uint8 bitpos = --bitLen;
	uint8 bitMark = 0;
	uint8 bitVal = 0;
	do {
		bitMark = (0x1 << bitpos);
		bitVal = GETSCREENBIT_VAL(bitData, bitMark);
		if(bitVal) {
			gisunlink_screen_bit_set(bitMark,GISUNLINK_SCREEN_SHOW);
		} else {
			gisunlink_screen_bit_set(bitMark,GISUNLINK_SCREEN_DISSHOW);
		}
	} while(bitpos--);
}

void gisunlink_screen_refresh(void) {
	uint8 comID = MAXSCREENCOM;
	gisunlink_screen_reset_io();
	gisunlink_screen_com_set(screen_disp_row,GISUNLINK_SCREEN_SEL);
	gisunlink_screen_bitRefresh(gisunlink_screen_get_vdata(screen_disp_row),MAXSCREENBIT);
	screen_disp_row--; 
	if(screen_disp_row < MINSCREENCOM) {
		screen_disp_row = MAXSCREENCOM;
	}
	gisunlink_delay(SCREEN_REFRESH_UNIT,SCREEN_REFRESH_DELAY);						//不做外围信号检测 延时6ms内
	gisunlink_screen_com_set(screen_disp_row,GISUNLINK_SCREEN_DISSEL);		//这里可要可不要
	gisunlink_screen_reset_io();
}

void gisunlink_screen_init(void) {
	gisunlink_screen_clearAllCom();
	gisunlink_screen_clearAllBit();
}

