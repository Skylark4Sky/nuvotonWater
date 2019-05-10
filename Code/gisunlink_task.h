/*
* _COPYRIGHT_
*
* File Name:gisunlink_task.h
* System Environment: JOHAN-PC
* Created Time:2019-01-09
* Author: johan
* QQ: 4493953
* E-mail: johaness@qq.com
* Description: 
*
*/

#ifndef __GISUNLINK_TASK_H__
#define __GISUNLINK_TASK_H__

#include "gisunlink.h"

/*! @brief 任务初始化 
 * @param gisunlink_task 任务列表 
 * @return void
 */
void gisunlink_task_init(gisunlink_task *taskList);

/*! @brief 任务检测 
 * @param void 
 * @return void
 */
void gisunlink_task_check(void);

/*! @brief 激活任务 
 * @param 任务id 
 * @return void
 */
bool gisunlink_task_active(uint8 taskid);

/*! @brief 闲置任务 
 * @param 任务id 
 * @return void
 */
bool gisunlink_task_inactive(uint8 taskid);

/*! @brief 获取任务信息 
 * @param 任务id 
 * @return void
 */
gisunlink_task *gisunlink_task_get(uint8 taskid);

/*! @brief 获取任务状态
 * @param 任务id 
 * @return void
 */
uint8 gisunlink_task_get_state(uint8 taskid);

/*! @brief 设置任务运行次数
 * @param 任务id 
 * @param 运行次数
 * @return void
 */
void gisunlink_task_set_exe_cycles(uint8 taskid, uint16_t exe_cycles);

/*! @brief 设置任务运行参数
 * @param 任务id 
 * @param 任务参数
 * @return void
 */
void gisunlink_task_set_param(uint8 taskid,void *param);

/*! @brief 获取任务运行参数
 * @param 任务id 
 * @return 任务参数
 */
void *gisunlink_task_get_param(uint8 taskid);


#endif //__GISUNLINK_TASK_H__
