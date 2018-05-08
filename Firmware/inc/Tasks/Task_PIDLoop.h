#ifndef TASKS_TASK_PIDLOOP_H_
#define TASKS_TASK_PIDLOOP_H_

#include "FreeRTOS.h"
#include "task.h"

TaskHandle_t vCreateTask_PIDLoop(uint32_t stack_size, uint32_t priority);

#endif /* TASKS_TASK_PIDLOOP_H_ */
