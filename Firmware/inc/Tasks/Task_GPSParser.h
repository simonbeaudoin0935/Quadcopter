/*
 * Task_GPSParser.h
 *
 *  Created on: May 3, 2018
 *      Author: simon
 */

#ifndef TASKS_TASK_GPSPARSER_H_
#define TASKS_TASK_GPSPARSER_H_

#include "FreeRTOS.h"
#include "task.h"

TaskHandle_t vCreateTask_GPSParser(uint32_t stack_size);

#endif /* TASKS_TASK_GPSPARSER_H_ */
