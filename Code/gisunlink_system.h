/*
* _COPYRIGHT_
*
* File Name:gisunlink_system.h
* System Environment: JOHAN-PC
* Created Time:2019-01-09
* Author: johan
* QQ: 4493953
* E-mail: johaness@qq.com
* Description: 
*
*/

#ifndef __GISUNLINK_SYSTEM_H__
#define __GISUNLINK_SYSTEM_H__

#include "gisunlink.h"

/*! @brief 系统初始化
 * @param  gisunlink 
 * @return void 
 */
void gisunlink_system_init(gisunlink *GlobalSystem);

/*! @brief 系统故障
 * @param  void 
 * @return void 
 */
void gisunlink_system_down(void);

/*! @brief 系统开机
 * @param  void 
 * @return void 
 */
void gisunlink_system_poweron(void);

/*! @brief 系统是否运行
 * @param  void 
 * @return 是 返回 runing 
 */
bool gisunlink_system_isRuning(void);

/*! @brief 系统状态
 * @param  void 
 * @return 状态
 */
uint8 gisunlink_system_RunState(void);

/*! @brief 系统状态检测 
 * @param  void 
 * @return void
 */
void gisunlink_system_check(gisunlink_peripheral *peripheral);

#endif //__GISUNLINK_SYSTEM_H__
