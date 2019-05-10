/*
* _COPYRIGHT_
*
* File Name: gisunlink_timer.c
* System Environment: JOHAN-PC
* Created Time:2018-12-25
* Author: johan
* QQ: 4493953
* E-mail: johaness@qq.com
* Description: 
*
*/

#include "gisunlink_timer.h"
#include "gisunlink_task.h"

static gisunlink_sys_time *time = NULL;
static gisunlink_task *TaskList = NULL;
static TimerCallback *SysTimeCb = NULL;
static TimerCallback *ScanCb = NULL;
static uint16 task_runtime[GISUNLINK_MAX_EVENT_NUM] = {0};
static uint16 task_exe_cycles[GISUNLINK_MAX_EVENT_NUM] = {0};
static uint8 u8TH0_Tmp,u8TL0_Tmp,u8TH1_Tmp,u8TL1_Tmp;

void gisunlink_timer_time(gisunlink_sys_time *sys_time) {
	memset(sys_time,0x00,sizeof(gisunlink_sys_time));
}

bool gisunlink_timer_proc_time(gisunlink_sys_time *sys_time) {
	if(sys_time->sec >= 60) { //处理秒
		sys_time->sec = 0;
		sys_time->min++;
		if(sys_time->min >= 60) { //处理分钟
			sys_time->min = 0;
			sys_time->hour++;
		}
		return true;
	}
	return false;
}

uint32 gisunlink_timer_get_tick_count(void) {
	return time->sys_tick;
}

/************************************************************************************************************
 *    TIMER 0 interrupt subroutine
 ************************************************************************************************************/
void Timer0_ISR (void) interrupt 1  //interrupt address is 0x000B
{
	clr_TF0;
	time->sys_tick++;	
	if(0 == (time->sys_tick % 1000)) {
		if(SysTimeCb != NULL) {
			SysTimeCb();
		}		
	}
	TH0 = u8TH0_Tmp;
	TL0 = u8TL0_Tmp;    
}

/************************************************************************************************************
 *    TIMER 1 interrupt subroutine
 ************************************************************************************************************/
void Timer1_ISR (void) interrupt 3  //interrupt address is 0x001B
{
	uint8 i = 0;
	uint16 *runtime = 0;
	uint16 *exe_cycles = 0;
	clr_TF1;
	TH1 = u8TH1_Tmp;
	TL1 = u8TL1_Tmp;

	if(TaskList == NULL) {
		return;
	}

	//遍历任务数组
	for (i = 0; i < GISUNLINK_MAX_EVENT_NUM; ++i) {
		gisunlink_task *Task = (gisunlink_task *)TaskList + i;
		runtime = task_runtime + i; 
		exe_cycles = task_exe_cycles + i; 

		if(Task->param == NULL) {
			continue;
		}

		switch (Task->state) {
			case GISUNLINK_TASK_READY: 
				{
					if(Task->type == NORMAL_TASK) {
						*runtime = ((gisunlink_normal_task *)Task->param)->time;
						*exe_cycles = Task->exe_cycles;
					} else {
						*runtime = ((gisunlink_flashing_task *)Task->param)->ontime;
						*exe_cycles = (Task->exe_cycles * 2);
						((gisunlink_flashing_task *)Task->param)->cur_state = FLASHING_TASK_ON;				//先执行ON动作
					}
					Task->state = GISUNLINK_TASK_RUN;						//改变任务状态，执行任务函数
				}
				break;
			case GISUNLINK_TASK_RUNING:										//如果任务在执行运行状态中则开始处理执行时间
				{
					--(*runtime);
					if (0 == (*runtime)) {									//执行时间到了
						if(Task->type == NORMAL_TASK) {
							*runtime = ((gisunlink_normal_task *)Task->param)->time;
						} else {
							if(((gisunlink_flashing_task *)Task->param)->cur_state == FLASHING_TASK_ON) {
								*runtime = ((gisunlink_flashing_task *)Task->param)->offtime;
								((gisunlink_flashing_task *)Task->param)->cur_state = FLASHING_TASK_OFF;
							} else {
								*runtime = ((gisunlink_flashing_task *)Task->param)->ontime;
								((gisunlink_flashing_task *)Task->param)->cur_state = FLASHING_TASK_ON;
							}
						}

						if(GISUNLINK_TASK_ALWAYS_RUN == (*exe_cycles)) {
							Task->state = GISUNLINK_TASK_RUN;				//改变任务状态，执行任务函数												
						} else {
							--(*exe_cycles);
							if(0 == (*exe_cycles)) {						//全部周期执行完了
								Task->state = GISUNLINK_TASK_STOP;			//改变任务状态为停止
							} else {
								Task->state = GISUNLINK_TASK_RUN;								
							}			
						}							
					}						
				}
				break;
		}
	}
}

/************************************************************************************************************
 *    TIMER 3 interrupt subroutine
 ************************************************************************************************************/
void Timer3_ISR (void) interrupt 16
{
	clr_TF3;
	if(ScanCb) {
		ScanCb();
	}
}

void gisunlink_timer_init(gisunlink_sys_time *sys_time,TimerCallback *SysTimeCallback, gisunlink_task *tasks) {
	time = sys_time;

	if(SysTimeCallback) {
		SysTimeCb = SysTimeCallback;
	}

	if(tasks) {
		TaskList = tasks;
		//初始化任务管理
		gisunlink_task_init(tasks);
	}

	TIMER0_MODE1_ENABLE;
	TIMER1_MODE1_ENABLE;

	clr_T0M; //12分频
	clr_T1M; //12分频
#if 0
	//Timer3 
	clr_T3PS0;
	clr_T3PS1;
	clr_T3PS2;
	set_T3PS1;
#endif

	u8TH0_Tmp = HIBYTE(TIMER_DIV12_VALUE_1ms);
	u8TL0_Tmp = LOBYTE(TIMER_DIV12_VALUE_1ms);   
	u8TH1_Tmp = HIBYTE(TIMER_DIV12_VALUE_1ms);
	u8TL1_Tmp = LOBYTE(TIMER_DIV12_VALUE_1ms);

	TH0 = u8TH0_Tmp;
	TL0 = u8TL0_Tmp;
	TH1 = u8TH1_Tmp;
	TL1 = u8TL1_Tmp;

	//	RL3 = LOBYTE(TIMER_DIV4_VALUE_500us);
	//	RH3 = HIBYTE(TIMER_DIV4_VALUE_500us);

	set_ET0;                                    //enable Timer0 interrupt
	set_ET1;                                    //enable Timer1 interrupt
	//	set_ET3;
	set_EA;                                     //enable interrupts
	set_TR0;                                    //Timer0 run
	set_TR1;                                    //Timer1 run
	//	set_TR3;
}
