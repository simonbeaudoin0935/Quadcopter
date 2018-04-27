/*
 * Task_PIDLoop.h
 *
 *  Created on: Apr 27, 2018
 *      Author: simon
 */

#ifndef TASKS_TASK_PIDLOOP_H_
#define TASKS_TASK_PIDLOOP_H_

#include "FreeRTOS.h"
#include "task.h"

TaskHandle_t vCreateTask_PIDLoop(void);

#endif /* TASKS_TASK_PIDLOOP_H_ */
