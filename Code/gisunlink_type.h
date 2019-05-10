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
	uint32 sys_tick;										//���� ��λ:1ms
	uint8 sec;												//��
	uint8 min;												//��
	uint16 hour;											//ʱ ���65535��Сʱ
}gisunlink_sys_time, *Pgisunlink_sys_time;

typedef struct _gisunlink_flashing_task {
	uint8 cur_state;
	uint16 ontime;											 //����ʱ��1
	uint16 offtime;											 //����ʱ��2
	void (*TaskRun)(uint8 taskid,uint8 cur_state,void *attr);//�������ж���
	void (*TaskStop)(uint8 taskid);							 //����ֹͣ����
}gisunlink_flashing_task, *Pgisunlink_flashing_task;

typedef struct _gisunlink_normal_task {
	uint16 time;											//����ʱ��
	void (*TaskRun)(uint8 taskid, void *attr);				//�������ж���
	void (*TaskStop)(uint8 taskid);							//����ֹͣ����
}gisunlink_normal_task, *Pgisunlink_normal_task;

typedef struct _gisunlink_task {
	uint8 state;											//��ʾ����״̬
	uint8 type;												//0��ʾ��ʱ������  1��ʾ�л�����
	void *attr;												//�������
	void *param;											//���в���
	uint16 exe_cycles;										//ִ��������	��˸����ʱ��ֵ���ֻ��Ϊ32767
}gisunlink_task, *Pgisunlink_task;

typedef struct _gisunlink_screen {
	uint8 comBit;											//λѡ��
}gisunlink_screen, *Pgisunlink_screen;

typedef struct _gisunlink_peripheral {
	uint8 low_tension;										//��ѹ״̬ //0xFF��ʼ״̬ 0 �� 1��
	uint8 high_tension;										//��ѹ״̬ //0xFF��ʼ״̬ 0 �� 1��
	uint8 hasKeyPress;										//�а�������
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
