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

/*! @brief ��ʼ��������(�ο�ʱ��Ϊ1M)
 * @param void 
 * @return void
 */
void gisunlink_beep_init(void);

/*! @brief ����������
 * @param void 
 * @return void
 */
void gisunlink_beep_work_start(uint16 freq,uint8 duty);

/*! @brief ������ֹͣ����
 * @param void 
 * @return void
 */
void gisunlink_beep_work_stop(void);


#endif //__GISUNLINK_BEEP_H__
