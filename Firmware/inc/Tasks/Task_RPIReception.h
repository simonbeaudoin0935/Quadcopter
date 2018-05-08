#ifndef TASKS_TASK_RPIRECEPTION_H_
#define TASKS_TASK_RPIRECEPTION_H_

#include "FreeRTOS.h"
#include "task.h"

TaskHandle_t vCreateTask_RPIReception(uint32_t stack_size, uint32_t priority);

#endif /* TASKS_TASK_IDLE_H_ */
