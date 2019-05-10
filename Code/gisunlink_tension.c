/*
* _COPYRIGHT_
*
* File Name: gisunlink_tension.c
* System Environment: JOHAN-PC
* Created Time:2019-01-10
* Author: johan
* QQ: 4493953
* E-mail: johaness@qq.com
* Description: 
*
*/

#include "gisunlink_tension.h"

typedef struct _gisunlink_tension {
	uint8 ReadCount;
	uint16 LTensionLow;			
	uint16 LTensionHigh;	
	uint16 HTensionLow;  
	uint16 HTensionHigh;	
} gisunlink_tension, *Pgisunlink_tension;

static gisunlink_tension GlobalTension = {0};
static gisunlink_tension *tension = NULL;

void gisunlink_tension_init(void) {
	tension = &GlobalTension;
	memset(tension,0x00,sizeof(gisunlink_tension));
}

void gisunlink_tension_reset_io(void) {
	P11_Input_Mode;
	P12_Input_Mode;
}

void gisunlink_tension_scan(gisunlink_peripheral *peripheral) {
	if(tension) {
		if(GISUNLINK_LOW_TENSION) {
			tension->LTensionHigh++;
		} else {
			tension->LTensionLow++;
		}

		if(GISUNLINK_HIGH_TENSION) {
			tension->HTensionHigh++;
		} else {
			tension->HTensionLow++;
		}	

		tension->ReadCount++;

		if(tension->ReadCount >= TENSIONMAXREADCOUNT) {
			if(tension->LTensionHigh > tension->LTensionLow) {
				peripheral->low_tension = TENSIONHIGH;
			} else {
				peripheral->low_tension = TENSIONLOW;		
			}

			if(tension->HTensionHigh > tension->HTensionLow){
				peripheral->high_tension = TENSIONHIGH;
			} else {
				peripheral->high_tension = TENSIONLOW;
			}	
			memset(tension,0x00,sizeof(gisunlink_tension));
		}
	}
}
