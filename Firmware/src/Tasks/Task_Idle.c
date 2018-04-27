/*
 * Task_Idle.c
 *
 *  Created on: Apr 26, 2018
 *      Author: simon
 */
#include "Tasks/Task_Idle.h"


void vTask_Idle(void * pvParameters);

TaskHandle_t vCreateTask_Idle(void)
{
	BaseType_t xReturned;
	TaskHandle_t xHandle = NULL;

	/* Create the task, storing the handle. */
    xReturned = xTaskCreate(vTask_Idle,       /* Function that implements the task. */
	                        "Idle",          /* Text name for the task. */
						    configMINIMAL_STACK_SIZE,      /* Stack size in words, not bytes. */
	                        NULL,    /* Parameter passed into the task. */
	                        tskIDLE_PRIORITY,/* Priority at which the task is created. */
	                        &xHandle );      /* Used to pass out the created task's handle. */

	if( xReturned != pdPASS ) return NULL;

	return xHandle;

}

void vTask_Idle(void * pvParameters)
{

	while(1);
}
