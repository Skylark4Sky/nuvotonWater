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

/*! @brief �ߵ�ѹ����ʼ��
 * @param void 
 * @return void
 */
void gisunlink_tension_init(void);

/*! @brief �ߵ�ѹIO��λ
 * @param void 
 * @return void
 */
void gisunlink_tension_reset_io(void);

/*! @brief �ߵ�ѹ״̬ɨ��
 * @param gisunlink_peripheral 
 * @return void
 */
void gisunlink_tension_scan(gisunlink_peripheral *peripheral);

#endif //__GISUNLINK_BEEP_H__
