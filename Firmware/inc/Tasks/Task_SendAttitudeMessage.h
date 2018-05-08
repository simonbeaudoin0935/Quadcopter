/*
 * Task_SendAttitudeMessage.h
 *
 *  Created on: May 2, 2018
 *      Author: simon
 */

#ifndef TASKS_TASK_SENDATTITUDEMESSAGE_H_
#define TASKS_TASK_SENDATTITUDEMESSAGE_H_

#include "FreeRTOS.h"
#include "task.h"

TaskHandle_t vCreateTask_SendAttitudeMessage(uint32_t stack_size, uint32_t priority);

#endif /* TASKS_TASK_SENDATTITUDEMESSAGE_H_ */
