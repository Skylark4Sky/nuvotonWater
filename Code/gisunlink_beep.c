/*
* _COPYRIGHT_
*
* File Name: gisunlink_beep.c
* System Environment: JOHAN-PC
* Created Time:2018-12-25
* Author: johan
* QQ: 4493953
* E-mail: johaness@qq.com
* Description: 
*
*/

#include "gisunlink_beep.h"

#define MAXPWMFREQ 							8000000
#define MINHZ										1000
#define MAXHZ										10000
#define MAXUNIT 								100
#define MINUNIT									1

#define PWM_CHANNEL_ENABLE 			PWM2_P05_OUTPUT_ENABLE
#define PWM_CHANNEL_DISENABLE 	PWM2_P05_OUTPUT_DISABLE
#define PWM_CHANNEL_INVERSE 		PWM2_OUTPUT_INVERSE
#define PWM_CHANNEL_H 					PWM2H
#define PWM_CHANNEL_L 					PWM2L

void gisunlink_beep_init(void) {
	P05_Quasi_Mode;
	PWM_CHANNEL_ENABLE;
	PWM_IMDEPENDENT_MODE;
	PWM_CLOCK_DIV_2;
}

static void gisunlink_beep_work_freq(uint16 freq,uint8 duty) {
	if((freq >= MINHZ && freq <= MAXHZ) && (duty >= MINUNIT && duty <= MAXUNIT)) {
		uint16 PwmFreq = ((MAXPWMFREQ/freq ) - 1);
		uint16 PwmDuty = ((PwmFreq + 1) / MAXUNIT) * duty;
		PWMPH = ((PwmFreq >> 8) & 0xFF);
		PWMPL = (PwmFreq & 0xFF);
		PWM_CHANNEL_H = ((PwmDuty >> 8) & 0xFF);	
		PWM_CHANNEL_L = (PwmDuty & 0xFF);	
	}
}

static void gisunlink_beep_run(void) {
	set_LOAD;
	set_PWMRUN;	
}

void gisunlink_beep_work_start(uint16 freq,uint8 duty) {
	gisunlink_beep_work_freq(freq,duty);
	gisunlink_beep_run();
}

void gisunlink_beep_work_stop(void) {	
	clr_PWMRUN;
}