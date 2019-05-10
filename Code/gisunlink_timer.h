/*
* _COPYRIGHT_
*
* File Name:gisunlink_timer.h
* System Environment: JOHAN-PC
* Created Time:2018-12-25
* Author: johan
* QQ: 4493953
* E-mail: johaness@qq.com
* Description: 
*
*/

#ifndef __GISUNLINK_TIMER_H__
#define __GISUNLINK_TIMER_H__

#include "gisunlink.h"

/*! @brief 初始化时间结构 
 * @param gisunlink_sys_time 
 * @return void
 */
void gisunlink_timer_time(gisunlink_sys_time *sys_time);

/*! @brief 时间处理 每60秒返回一次真
 * @param gisunlink_sys_time 
 * @return void
 */
bool gisunlink_timer_proc_time(gisunlink_sys_time *sys_time); 

/*! @brief 返回系统Tick时间 单位毫秒
 * @param void
 * @return uint32
 */
uint32 gisunlink_timer_get_tick_count(void);

/*! @brief 初始化定时器 定时器0用于统计时间，定时器1用于处理微任务， 定时器3用于屏幕和按键扫描 
 * @param gisunlink_sys_time 
 * @param Callback 每1秒回调一次
 * @param Callback 每500微秒回调一次
 * @param task 任务结构
 * @return void
 */
void gisunlink_timer_init(gisunlink_sys_time *sys_time,TimerCallback *SysTimeCallback, gisunlink_task *tasks);

#endif //__GISUNLINK_TIMER_H__
