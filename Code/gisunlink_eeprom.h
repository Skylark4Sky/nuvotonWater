/*
* _COPYRIGHT_
*
* File Name:gisunlink_eeprom.h
* System Environment: JOHAN-PC
* Created Time:2018-12-25
* Author: johan
* QQ: 4493953
* E-mail: johaness@qq.com
* Description: 
*
*/

#ifndef __GISUNLINK_EEPROM_H__
#define __GISUNLINK_EEPROM_H__

#include "gisunlink.h"

/*! @brief 储存区初始化
 * @param gisunlink_sys_time *	
 * @return void
 */
void gisunlink_eeprom_init(gisunlink_sys_time *SystemTime);

/*! @brief 读运行时间
 * @param u16_addr 数据地址 						
 * @return void
 */
void gisunlink_eeprom_read_runtime(gisunlink_sys_time *time);

/*! @brief 读运行时间
 * @param u16_addr 数据地址 						
 * @param u16_addr 数据地址 						
 * @return void
 */
void gisunlink_eeprom_write_runtime(uint8 sec, uint8 min, uint16 hour);

/*! @brief 处理最大写记录状态
 * @param void 						
 * @return true 最大写记录状态 false 不是
 */
bool gisunlink_eeprom_max_write_runtime_record_chk(void);

/*! @brief 检查是否烧录后系统第一次运行，如果为第一次启动的话，返回真，并设置第一次运行状态位，下次再调用则返回假
 * @param void											
 * @return bool 返回true 为第一次启动 false 为
 */
bool gisunlink_eeprom_check_first_run(void);

#endif //__GISUNLINK_EEPROM_H__
