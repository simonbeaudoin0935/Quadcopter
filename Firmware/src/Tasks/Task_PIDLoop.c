#include "Tasks/Task_PIDLoop.h"

void vTask_PIDLoop(void * pvParameters);

TaskHandle_t vCreateTask_PIDLoop(void)
{
	BaseType_t xReturned;
	TaskHandle_t xHandle = NULL;

	/* Create the task, storing the handle. */
    xReturned = xTaskCreate(vTask_PIDLoop,       /* Function that implements the task. */
	                        "PID Loop",          /* Text name for the task. */
						    configMINIMAL_STACK_SIZE,      /* Stack size in words, not bytes. */
	                        NULL,    /* Parameter passed into the task. */
	                        tskIDLE_PRIORITY+4,/* Priority at which the task is created. */
	                        &xHandle );      /* Used to pass out the created task's handle. */

	if( xReturned != pdPASS ) return NULL;

	return xHandle;
}

void vTask_PIDLoop(void * pvParameters)
{
	while(1){

		vTaskDelay(20); //execute at 50Hz
	}
}
