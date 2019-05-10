/*
* _COPYRIGHT_
*
* File Name:gisunlink_event.h
* System Environment: JOHAN-PC
* Created Time:2019-01-11
* Author: johan
* QQ: 4493953
* E-mail: johaness@qq.com
* Description: 
*
*/

#ifndef __GISUNLINK_EVENT_H__
#define __GISUNLINK_EVENT_H__

#include "gisunlink.h"

void gisunlink_icon_event_run(uint8 taskid, uint8 cur_state, void *attr);
void gisunlink_icon_event_stop(uint8 taskid);

void gisunlink_beep_event_run(uint8 taskid, uint8 cur_state, void *attr);
void gisunlink_beep_event_stop(uint8 taskid);

void gisunlink_system_event(uint8 taskid, void *attr);

#endif //__GISUNLINK_EVENT_H__
