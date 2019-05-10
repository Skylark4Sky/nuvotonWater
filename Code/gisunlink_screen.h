/*
* _COPYRIGHT_
*
* File Name:gisunlink_screen.h
* System Environment: JOHAN-PC
* Created Time:2018-12-25
* Author: johan
* QQ: 4493953
* E-mail: johaness@qq.com
* Description: 
*
*/

#ifndef __GISUNLINK_SCREEN_H__
#define __GISUNLINK_SCREEN_H__

#define SCREENCOM4					4
#define SCREENCOM3					3
#define SCREENCOM2					2
#define SCREENCOM1					1

#define SCREENBITA	        (0x1 << 07)
#define SCREENBITB	        (0x1 << 06)
#define SCREENBITC	        (0x1 << 05)
#define SCREENBITD	        (0x1 << 04)
#define SCREENBITE	        (0x1 << 03)
#define SCREENBITF	        (0x1 << 02)
#define SCREENBITG	        (0x1 << 01)
#define SCREENBITH	        (0x1 << 00)

#define GETSCREENBIT_VAL(Data,bitMark) ((Data & bitMark) == bitMark)

#include "gisunlink.h"

/*! @brief 显示滤芯寿命
 * @param level 滤芯寿命 4 - 0 (0等于耗尽,指示灯全灭)
 * @return void
 */
void gisunlink_screen_filter_icon_show(void);

/*! @brief 获取显存数据
 * @param 段ID
 * @return 数据
 */
uint8 gisunlink_screen_get_vdata(uint8 comID);

/*! @brief 设置显存数据
 * @param 段ID
 * @param 数据 
 * @return void
 */
void gisunlink_screen_set_vdata(uint8 comID, uint8 vdata);

/*! @brief 屏幕刷新
 * @param void					
 * @return void
 */
void gisunlink_screen_refresh(void);

/*! @brief 显示屏控制初始化
 * @param void					
 * @return void
 */
void gisunlink_screen_init(void);

#endif //__GISUNLINK_SCREEN_H__
