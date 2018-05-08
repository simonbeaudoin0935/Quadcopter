#include "Tasks/Task_GPSParser.h"

#include "stm32f4xx.h"


static void vTask_GPSParser(void * pvParameters);

TaskHandle_t vCreateTask_GPSParser(uint32_t stack_size, uint32_t priority)
{
	BaseType_t xReturned;
	TaskHandle_t xHandle = NULL;

	/* Create the task, storing the handle. */
    xReturned = xTaskCreate(vTask_GPSParser,       /* Function that implements the task. */
	                        "GPS_Parser",          /* Text name for the task. */
							stack_size,      /* Stack size in words, not bytes. */
	                        NULL,    /* Parameter passed into the task. */
	                        priority,/* Priority at which the task is created. */
	                        &xHandle );      /* Used to pass out the created task's handle. */

	if( xReturned != pdPASS ) return NULL;

	return xHandle;

}

void vTask_GPSParser(void * pvParameters)
{

	while(1){
		vTaskDelay(1000);
	}
}
