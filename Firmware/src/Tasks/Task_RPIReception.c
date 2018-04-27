/*
 * Task_Idle.c
 *
 *  Created on: Apr 26, 2018
 *      Author: simon
 */
#include "Tasks/Task_RPIReception.h"


void vTask_RPIReception(void * pvParameters);

TaskHandle_t vCreateTask_RPIReception()
{
	BaseType_t xReturned;
	TaskHandle_t xHandle = NULL;

	/* Create the task, storing the handle. */
    xReturned = xTaskCreate(vTask_RPIReception,       /* Function that implements the task. */
	                        "RPI_Rec",          /* Text name for the task. */
						    configMINIMAL_STACK_SIZE,      /* Stack size in words, not bytes. */
	                        NULL,    /* Parameter passed into the task. */
	                        tskIDLE_PRIORITY+2,/* Priority at which the task is created. */
	                        &xHandle );      /* Used to pass out the created task's handle. */

	if( xReturned != pdPASS ) return NULL;

	return xHandle;

}

void vTask_RPIReception(void * pvParameters)
{

	while(1);
}
