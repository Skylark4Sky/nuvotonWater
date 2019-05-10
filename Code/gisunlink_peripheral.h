/*
* _COPYRIGHT_
*
* File Name:gisunlink_peripheral.h
* System Environment: JOHAN-PC
* Created Time:2018-12-25
* Author: johan
* QQ: 4493953
* E-mail: johaness@qq.com
* Description: 
*
*/

#ifndef __GISUNLINK_PERIPHERAL_H__
#define __GISUNLINK_PERIPHERAL_H__

#include "gisunlink.h"

/*! @brief 初始化外围接口
 * @param void 
 * @return void
 */
void gisunlink_peripheral_init(gisunlink_peripheral *SysPeripheral);

/*! @brief 冲洗模式
 * @param void 
 * @return void
 */
void gisunlink_peripheral_wash(void);

/*! @brief 制水模式
 * @param void 
 * @return void
 */
void gisunlink_peripheral_water(void);

/*! @brief 停止工作 
 * @param void 
 * @return void
 */
void gisunlink_peripheral_disworking(void);

/*! @brief 外围信号检测
 * @param void 
 * @return void 
 */
void gisunlink_peripheral_check(void);

#endif //__GISUNLINK_PERIPHERAL_H__
