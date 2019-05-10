/*
* _COPYRIGHT_
*
* File Name:gisunlink_common.h
* System Environment: JOHAN-PC
* Created Time:2019-01-11
* Author: johan
* QQ: 4493953
* E-mail: johaness@qq.com
* Description: 
*
*/

#ifndef __GISUNLINK_COMMON_H__
#define __GISUNLINK_COMMON_H__

#include "gisunlink.h"

/*! @brief 验证设备是否过期
 * @param 使用了多少个小时 
 * @return bool 过期为true 否则为false 
 */
bool gisunlink_chk_dev_expiration(uint16 hour);

/*! @brief 计算滤芯寿命 
 * @param 使用了多少个小时 
 * @return 使用等级 
 */
uint8 gisunlink_chk_filter_level(uint16 hour); 

/*! @brief 系统延时
 * @param 单位 10us 100us 200us 500us 1ms
 * @param 时间
 * @return void
 */
void gisunlink_delay(uint16 unit,uint32 u32CNT);

#endif //__GISUNLINK_COMMON_H__
