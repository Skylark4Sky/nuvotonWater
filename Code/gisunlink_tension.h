/*
* _COPYRIGHT_
*
* File Name:gisunlink_tension.h
* System Environment: JOHAN-PC
* Created Time:2019-01-10
* Author: johan
* QQ: 4493953
* E-mail: johaness@qq.com
* Description: 
*
*/

#ifndef __GISUNLINK_TENSION_H__
#define __GISUNLINK_TENSION_H__

#include "gisunlink.h"

/*! @brief ¸ßµÍÑ¹¼ì²â³õÊ¼»¯
 * @param void 
 * @return void
 */
void gisunlink_tension_init(void);

/*! @brief ¸ßµÍÑ¹IO¸´Î»
 * @param void 
 * @return void
 */
void gisunlink_tension_reset_io(void);

/*! @brief ¸ßµÍÑ¹×´Ì¬É¨Ãè
 * @param gisunlink_peripheral 
 * @return void
 */
void gisunlink_tension_scan(gisunlink_peripheral *peripheral);

#endif //__GISUNLINK_BEEP_H__
