/*
* _COPYRIGHT_
*
* File Name:gisunlink_beep.h
* System Environment: JOHAN-PC
* Created Time:2018-12-25
* Author: johan
* QQ: 4493953
* E-mail: johaness@qq.com
* Description: 
*
*/

#ifndef __GISUNLINK_BEEP_H__
#define __GISUNLINK_BEEP_H__

#include "gisunlink.h"

/*! @brief 初始化蜂鸣器(参考时钟为1M)
 * @param void 
 * @return void
 */
void gisunlink_beep_init(void);

/*! @brief 蜂鸣器工作
 * @param void 
 * @return void
 */
void gisunlink_beep_work_start(uint16 freq,uint8 duty);

/*! @brief 蜂鸣器停止工作
 * @param void 
 * @return void
 */
void gisunlink_beep_work_stop(void);


#endif //__GISUNLINK_BEEP_H__
