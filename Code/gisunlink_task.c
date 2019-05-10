/*
* _COPYRIGHT_
*
* File Name: gisunlink_task.c
* System Environment: JOHAN-PC
* Created Time:2019-01-09
* Author: johan
* QQ: 4493953
* E-mail: johaness@qq.com
* Description: 
*
*/

#include "gisunlink_task.h"

static gisunlink_task *tasks = NULL;
static uint8 task_index = 0;

void gisunlink_task_init(gisunlink_task *taskList) {
	if(taskList != NULL) {
		tasks = taskList;
	}
}

#if 0
/*! @brief 停掉其它图标显示任务
 * @param taskId 当前准备要运行的任务
 * @return void 
 */
static void StopOtherIconEventTask(uint8_t taskId) {
	uint8_t i = 0;
	uint8_t cmp = 0;
		
	uint8_t TaskIdArr[GISUNLINK_MAX_ICON_EVENT_NUM] = {GISUNLINK_WATER_EVENT,GISUNLINK_WASH_EVENT,GISUNLINK_FULL_EVENT,
																										 GISUNLINK_LACK_EVENT,GISUNLINK_FIXUP_EVENT};	
	for(i = 0; i < GISUNLINK_MAX_ICON_EVENT_NUM; i++) {
		cmp = TaskIdArr[i];
		if(taskId != cmp) {
			if(tasks[cmp].state != GISUNLINK_TASK_IDLE) {
				gisunlink_task_inactive(cmp);
			}
		}
	}		
}

/*! @brief 检查是否存在其它运行中的图标任务
 * @param taskId 当前准备要运行的任务
 * @return void 
 */
static void CheckOtherIconEventTask(uint8_t taskId) {
	switch(taskId) {
		case GISUNLINK_WATER_EVENT:
		case GISUNLINK_WASH_EVENT:	
		case GISUNLINK_FULL_EVENT:
		case GISUNLINK_LACK_EVENT:
		case GISUNLINK_FIXUP_EVENT:
			StopOtherIconEventTask(taskId);
		break;
	}																		 
}
#endif

static void gisunlink_normal_task_dispose(gisunlink_task *task,gisunlink_normal_task *param) {
	switch (task->state) {	 
		case GISUNLINK_TASK_RUN:
			//改变状态为执行中
			task->state = GISUNLINK_TASK_RUNING;
			if(param->TaskRun) {
				param->TaskRun(task_index,task->attr);
			}
			break;
		case GISUNLINK_TASK_STOP:
			//改变状态为空闲
			task->state = GISUNLINK_TASK_IDLE;
			if(param->TaskStop) {
				param->TaskStop(task_index);
			}
			break;
	}
}

static void gisunlink_flashing_task_dispose(gisunlink_task *task,gisunlink_flashing_task *param) {
	switch (task->state) {		 
		case GISUNLINK_TASK_RUN:
			//改变状态为执行中
			task->state = GISUNLINK_TASK_RUNING;
			if(param->TaskRun) {
				param->TaskRun(task_index,param->cur_state,task->attr);
			}
			break;
		case GISUNLINK_TASK_STOP:
			//改变状态为空闲
			task->state = GISUNLINK_TASK_IDLE;
			if(param->TaskStop) {
				param->TaskStop(task_index);
			}
			break;
	}
}

void gisunlink_task_check(void) {
	gisunlink_task *task = (gisunlink_task *)tasks + task_index;	

	if(tasks == NULL || task->param == NULL) {
		return;
	}

	switch (task->state) {
		case GISUNLINK_TASK_START:
			//改变状态为准备中
			task->state = GISUNLINK_TASK_READY;
			//执行准备动作
			//CheckOtherIconEventTask(task_index);
			break;	
	}

	if(task->type == NORMAL_TASK) {
		gisunlink_normal_task_dispose(task,(gisunlink_normal_task *)task->param);
	} else {
		gisunlink_flashing_task_dispose(task,(gisunlink_flashing_task *)task->param);
	}

	task_index++;
	if(task_index >= GISUNLINK_MAX_EVENT_NUM) {
		task_index = 0;
	}
}

bool gisunlink_task_active(uint8 taskid) {
	gisunlink_task *task = (gisunlink_task *)tasks + taskid;	
	if(taskid >= GISUNLINK_MAX_EVENT_NUM) {
		return false;
	}
	if(task->state == GISUNLINK_TASK_IDLE) {
		task->state = GISUNLINK_TASK_START;
		return true;
	}
	return false;
}

bool gisunlink_task_inactive(uint8 taskid) {
	gisunlink_task *task = (gisunlink_task *)tasks + taskid;	
	if(taskid >= GISUNLINK_MAX_EVENT_NUM) {
		return false;
	}
	if(task->state != GISUNLINK_TASK_IDLE) {
		task->state = GISUNLINK_TASK_STOP;
		return true;
	}

	return false;
}

gisunlink_task *gisunlink_task_get(uint8 taskid) {
	if(taskid >= GISUNLINK_MAX_EVENT_NUM) {
		return NULL;
	}
	return (gisunlink_task *)tasks + taskid;
}

uint8 gisunlink_task_get_state(uint8 taskid) {
	gisunlink_task *task = (gisunlink_task *)tasks + taskid;	
	if(taskid >= GISUNLINK_MAX_EVENT_NUM) {
		return GISUNLINK_TASK_ERR;
	}
	return task->state;
}

void gisunlink_task_set_exe_cycles(uint8 taskid, uint16_t exe_cycles) {
	gisunlink_task *task = (gisunlink_task *)tasks + taskid;	
	if(taskid >= GISUNLINK_MAX_EVENT_NUM) {
		return;
	}
	task->exe_cycles = exe_cycles;
}

void gisunlink_task_set_param(uint8 taskid,void *param) {
	gisunlink_task *task = (gisunlink_task *)tasks + taskid;	
	if(taskid >= GISUNLINK_MAX_EVENT_NUM) {
		return;
	}
	task->param = param;
}

void *gisunlink_task_get_param(uint8 taskid) {
	gisunlink_task *task = (gisunlink_task *)tasks + taskid;	
	if(taskid >= GISUNLINK_MAX_EVENT_NUM) {
		return NULL;
	}
	return task->param;
}
