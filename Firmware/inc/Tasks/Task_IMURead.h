/*
 * Task_IMURead.h
 *
 *  Created on: Apr 27, 2018
 *      Author: simon
 */

#ifndef TASKS_TASK_IMUREAD_H_
#define TASKS_TASK_IMUREAD_H_

#include "FreeRTOS.h"
#include "task.h"

TaskHandle_t vCreateTask_IMURead(uint32_t stack_size, uint32_t priority);

#endif /* TASKS_TASK_IMUREAD_H_ */
