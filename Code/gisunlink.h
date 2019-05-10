/*
* _COPYRIGHT_
*
* File Name:gisunlink.h
* System Environment: JOHAN-PC
* Created Time:2018-12-25
* Author: johan
* QQ: 4493953
* E-mail: johaness@qq.com
* Description: 
*
*/

#ifndef __GISUNLINK_H__
#define __GISUNLINK_H__

#include <string.h>
#include "N76E003.h"
#include "SFR_Macro.h"
#include "Function_define.h"
#include "Common.h"
#include "gisunlink_type.h"

//ϵͳ��Դ����
#define GISUNLINK_BEEP						P05	//������		PWM2_P05_OUTPUT_ENABLE
#define GISUNLINK_SCREEN_COM4				P07	//��Ļ��ѡ
#define GISUNLINK_SCREEN_COM3				P06	//��Ļ��ѡ	״̬��ʾ
#define GISUNLINK_SCREEN_COM2				P03 //��Ļ��ѡ	��Ҫ��ʾ��о״̬
#define GISUNLINK_SCREEN_COM1				P04	//��Ļ��ѡ	��ʾ��ˮ�����Լ���ˮͼ��
#define GISUNLINK_SCREEN_BITH				P02 //��Ļλѡ ���� ��о
#define GISUNLINK_SCREEN_BITG				P01 //��Ļλѡ ���� ROĤ
#define GISUNLINK_SCREEN_BITF				P00 //��Ļλѡ ���� ��Դ
#define GISUNLINK_SCREEN_BITE				P10 //��Ļλѡ ���� ǿ��
#define GISUNLINK_SCREEN_BITD				P11 //��Ļλѡ ���� ��ѹ
#define GISUNLINK_SCREEN_BITC				P12 //��Ļλѡ ���� ��ѹ
#define GISUNLINK_SCREEN_BITB				P13 //��Ļλѡ
#define GISUNLINK_SCREEN_BITA				P14 //��Ļλѡ

#define GISUNLINK_HIGH_TENSION				GISUNLINK_SCREEN_BITC	//��ѹ
#define GISUNLINK_LOW_TENSION				GISUNLINK_SCREEN_BITD	//��ѹ
#define GISUNLINK_FLUSH						P30	//��ϴ��
#define GISUNLINK_INLET						P17	//��ˮ��

#define GISUNLINK_WASH_KEY					GISUNLINK_SCREEN_BITE	//ǿ�尴��
#define GISUNLINK_POWER_KEY					GISUNLINK_SCREEN_BITF	//��Դ����
#define GISUNLINK_ROFILM_KEY				GISUNLINK_SCREEN_BITG	//ROĤ����
#define GISUNLINK_FILTER_KEY				GISUNLINK_SCREEN_BITH	//��о����

//ϵͳ�¼�����״̬����
//��Ļ״̬����
#define GISUNLINK_SCREEN_SEL				0xff
#define GISUNLINK_SCREEN_DISSEL				0x00
#define GISUNLINK_SCREEN_SHOW				0x00
#define GISUNLINK_SCREEN_DISSHOW			0xff
#define GISUNLINK_WORKING					0xff
#define GISUNLINK_DISWORKING				0x00

//��о״̬����
#define ENABLE_FILTER						0	//�Ƿ�������оʵ��״̬��ʾ 1 ���� 0 ������
#define ENABLE_DEV_EXPIRATION				0	//�Ƿ������豸������֤ 1 ���� 0 ������
#define MAX_DEV_EXPIRATION_TIME				2000 //��о���ʹ��ʱ��

#define MAX_FILTER_LEVEL4					500	 //��о״̬ʹ��ֵ
#define MIN_FILTER_LEVEL4					0
#define MAX_FILTER_LEVEL3					1000
#define MIN_FILTER_LEVEL3					500
#define MAX_FILTER_LEVEL2					1500
#define MIN_FILTER_LEVEL2					1000
#define MAX_FILTER_LEVEL1					2000
#define MIN_FILTER_LEVEL1					1500

#define GISUNLINK_FILTER_LEVEL4				4
#define GISUNLINK_FILTER_LEVEL3				3
#define GISUNLINK_FILTER_LEVEL2				2
#define GISUNLINK_FILTER_LEVEL1				1
#define GISUNLINK_FILTER_LEVEL0				0

//ϵͳ�¼�
#define GISUNLINK_WASH_EVENT				0 	//��ϴ�¼�
#define GISUNLINK_WATER_EVENT				1 	//��ˮ�¼�
#define GISUNLINK_FULL_EVENT				2 	//ˮ���¼�
#define GISUNLINK_LACK_EVENT				3 	//ȱˮ�¼�
#define GISUNLINK_FIXUP_EVENT				4 	//ά���¼�
#define GISUNLINK_FILTER_EVENT				5 	//��о״̬�¼�
#define GISUNLINK_BEEP_EVENT				6 	//�����¼�
#define GISUNLINK_SYSTEM_CHK_EVENT			7 	//ϵͳ״̬����Լ�����
#define GISUNLINK_MAX_EVENT_NUM				8	//�������������¼��������
#define GISUNLINK_MAX_ICON_EVENT_NUM		5 	//���ͼ���¼���

//ϵͳ����
#define GISUNLINK_TASK_ALWAYS_RUN			0xFFFF
#define GISUNLINK_RUN_FAULT					0xFF
#define GISUNLINK_RUN_NORMAL				0x00
#define GISUNLINK_TASK_IDLE					0x00
#define GISUNLINK_TASK_START				0x20
#define GISUNLINK_TASK_READY				0x30
#define GISUNLINK_TASK_RUN					0x40
#define GISUNLINK_TASK_RUNING				0x50
#define GISUNLINK_TASK_STOP					0x60
#define GISUNLINK_TASK_ERR					0xFF

#define SYSTEM_TASK_IDLE					0x1		//ϵͳ����
#define SYSTEM_TASK_WASH					0x2 	//��ϴ
#define SYSTEM_TASK_LACK					0x3 	//ȱˮ
#define SYSTEM_TASK_WATER					0x4 	//��ˮ
#define SYSTEM_TASK_FULL					0x5 	//ˮ��

#define WASHTIME_0							5000		//��ϴ5��
#define WASHIIME_1							30000		//��ϴ30��
#define WATERTIME_0 						3600000		//��ˮ1Сʱ
#define WATERTIME_1 						7200000		//��ˮ2Сʱ


//ϵͳ��ʱ
#define SYSTEM_DELAY_10US					TIMER_DIV4_VALUE_10us
#define SYSTEM_DELAY_100US					TIMER_DIV4_VALUE_100us
#define SYSTEM_DELAY_200US					TIMER_DIV4_VALUE_200us
#define SYSTEM_DELAY_500US					TIMER_DIV4_VALUE_500us
#define SYSTEM_DELAY_1MS 					TIMER_DIV4_VALUE_1ms
#define SCREEN_REFRESH_UNIT					SYSTEM_DELAY_500US
#define SCREEN_REFRESH_DELAY				7
#define PERIPHERA_REFRESH_UNIT				TIMER_DIV4_VALUE_10us
#define PERIPHERAL_REFRESH_DELAY			10

//��Χ���
#define TENSIONMAXREADCOUNT					100
#define TENSIONLOW							0x0F
#define TENSIONHIGH							0xF0
#define TENSIONUNKNOWN						0xFF

//ϵͳ����״̬
#define SYSTEMPOWER							0x00
#define SYSTEMON							0xF0
#define SYSTEMRUNING						0xFF
#define SYSTEMDOWN							0x0F

//��������
#define WASH_KEY							0x01
#define POWER_KEY							0x02
#define ROFILM_KEY							0x04
#define FILTER_KEY							0x08
#define KEY_SHORT							0x40
#define KEY_LONG							0x80
#define KEY_NONE							0x00

#define KEY_RELEASE							0
#define KEY_PRESS							1
#define KEY_PRESS_CHK						2
#define KEY_LONG_PRESS_CHK					3
#define KEY_LONG_PRESS_RELEASE				4

//����ģʽ
#define NORMAL_BEEP_MODE					0			//��������
#define RAPID_BEEP_MODE						1			//���پ���
#define BEEP_FREQ							2000		//������Ƶ�� 2000 = 2K
#define BEEP_DUTY							50			//������ռ�ձ� 50 = 50%

//����δʹ�þ���
#define UNUSED_VARIANT(x) 		if(x){}
															
#endif //__GISUNLINK_H__
