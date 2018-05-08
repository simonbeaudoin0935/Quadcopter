#ifndef TASK_FLASHHEARTBEATLED_H
#define TASK_FLASHHEARTBEATLED_H

#include "FreeRTOS.h"
#include "task.h"

TaskHandle_t vCreateTask_FlashHeartbeatLED(uint32_t stack_size, uint32_t priority);

#endif //TASK_FLASHHEARTBEATLED_H
