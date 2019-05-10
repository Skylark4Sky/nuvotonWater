/*
* _COPYRIGHT_
*
* File Name:gisunlink_key.h
* System Environment: JOHAN-PC
* Created Time:2019-01-10
* Author: johan
* QQ: 4493953
* E-mail: johaness@qq.com
* Description: 
*
*/

#ifndef __GISUNLINK_KEY_H__
#define __GISUNLINK_KEY_H__

#include "gisunlink.h"

/*! @brief °´¼üIO¸´Î»
 * @param void 
 * @return void
 */
void gisunlink_key_reset_io(void);

/*! @brief °´¼ü×´Ì¬É¨Ãè
 * @param gisunlink_peripheral 
 * @return void
 */
void gisunlink_key_scan(gisunlink_peripheral *peripheral);

#endif //__GISUNLINK_BEEP_H__
