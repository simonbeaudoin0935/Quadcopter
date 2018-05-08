/*
 * Task_PMUReader.h
 *
 *  Created on: May 3, 2018
 *      Author: simon
 */

#ifndef TASKS_TASK_PMUREADER_H_
#define TASKS_TASK_PMUREADER_H_

#include "FreeRTOS.h"
#include "task.h"

TaskHandle_t vCreateTask_PMUReader(uint32_t stack_size);

#endif /* TASKS_TASK_PMUREADER_H_ */
