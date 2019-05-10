/*
* _COPYRIGHT_
*
* File Name:gisunlink_type.h
* System Environment: JOHAN-PC
* Created Time:2018-12-25
* Author: johan
* QQ: 4493953
* E-mail: johaness@qq.com
* Description: 
*
*/

#ifndef __GISUNLINK_TYPE_H__
#define __GISUNLINK_TYPE_H__

typedef unsigned char   uint8;
typedef char            int8;
typedef unsigned short  uint16;
typedef short           int16;
typedef unsigned long   uint32;
typedef long            int32;

#ifndef bool
typedef uint8           bool;
#endif
#ifndef false
#define false           0
#endif
#ifndef true
#define true            1
#endif
#ifndef NULL
#define NULL            ((void *)0)
#endif

#define U8_MAX          0XFF
#define U16_MAX         0XFFFF
#define U32_MAX         0XFFFFFFFF
#define BIT0	        (1 << 00)
#define BIT1	        (1 << 01)
#define BIT2	        (1 << 02)
#define BIT3	        (1 << 03)
#define BIT4	        (1 << 04)
#define BIT5	        (1 << 05)
#define BIT6	        (1 << 06)
#define BIT7	        (1 << 07)
#define BIT8	        (1 << 08)
#define BIT9	        (1 << 09)
#define BIT10	        (1 << 10)
#define BIT11	        (1 << 11)
#define BIT12	        (1 << 12)
#define BIT13	        (1 << 13)
#define BIT14	        (1 << 14)
#define BIT15	        (1 << 15)

#define FLASHING_TASK		1
#define NORMAL_TASK			0	

#define FLASHING_TASK_ON	1
#define FLASHING_TASK_OFF	0

typedef struct _gisunlink_sys_time {
	uint32 sys_tick;										//毫秒 单位:1ms
	uint8 sec;												//秒
	uint8 min;												//分
	uint16 hour;											//时 最大65535个小时
}gisunlink_sys_time, *Pgisunlink_sys_time;

typedef struct _gisunlink_flashing_task {
	uint8 cur_state;
	uint16 ontime;											 //运行时间1
	uint16 offtime;											 //运行时间2
	void (*TaskRun)(uint8 taskid,uint8 cur_state,void *attr);//任务运行动作
	void (*TaskStop)(uint8 taskid);							 //任务停止动作
}gisunlink_flashing_task, *Pgisunlink_flashing_task;

typedef struct _gisunlink_normal_task {
	uint16 time;											//运行时间
	void (*TaskRun)(uint8 taskid, void *attr);				//任务运行动作
	void (*TaskStop)(uint8 taskid);							//任务停止动作
}gisunlink_normal_task, *Pgisunlink_normal_task;

typedef struct _gisunlink_task {
	uint8 state;											//表示任务状态
	uint8 type;												//0表示长时间运行  1表示切换运行
	void *attr;												//外带参数
	void *param;											//运行参数
	uint16 exe_cycles;										//执行周期数	闪烁任务时该值最大只能为32767
}gisunlink_task, *Pgisunlink_task;

typedef struct _gisunlink_screen {
	uint8 comBit;											//位选中
}gisunlink_screen, *Pgisunlink_screen;

typedef struct _gisunlink_peripheral {
	uint8 low_tension;										//低压状态 //0xFF初始状态 0 低 1高
	uint8 high_tension;										//高压状态 //0xFF初始状态 0 低 1高
	uint8 hasKeyPress;										//有按键按下
}gisunlink_peripheral, *Pgisunlink_peripheral;

typedef struct _gisunlink_beep {
	uint16 freq;
	uint8 duty;
}gisunlink_beep, *Pgisunlink_beep;

typedef struct _gisunlink {
	gisunlink_sys_time Time;
	gisunlink_peripheral Peripheral;
	uint8 FilterLevel;
	gisunlink_beep Beep;
}gisunlink, *Pgisunlink;

typedef void *TimerCallback(void);// uint8_t ms, uint8_t s, uint8_t m, uint16_t h reentrant;

#endif //__GISUNLINK_TYPE_H__
