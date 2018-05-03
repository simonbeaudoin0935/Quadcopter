/*
 * Task_Idle.c
 *
 *  Created on: Apr 26, 2018
 *      Author: simon
 */
#include "Tasks/Task_RPIReception.h"
#include "COM/UART/UART1.h"
#include "Mavlink/common/mavlink.h"

void vTask_RPIReception(void * pvParameters);

TaskHandle_t vCreateTask_RPIReception(uint32_t stack_size)
{
	BaseType_t xReturned;
	TaskHandle_t xHandle = NULL;

	/* Create the task, storing the handle. */
    xReturned = xTaskCreate(vTask_RPIReception,       /* Function that implements the task. */
	                        "RPI_Rec",          /* Text name for the task. */
							stack_size,      /* Stack size in words, not bytes. */
	                        NULL,    /* Parameter passed into the task. */
	                        tskIDLE_PRIORITY+2,/* Priority at which the task is created. */
	                        &xHandle );      /* Used to pass out the created task's handle. */

	if( xReturned != pdPASS ) return NULL;

	return xHandle;

}

void vTask_RPIReception(void * pvParameters)
{
	UART1_init(115200);

	uint32_t ulNotifiedValue;

	mavlink_message_t rcv_msg;
	mavlink_status_t rcv_status;

	while(1){

		ulNotifiedValue = ulTaskNotifyTake( pdTRUE, portMAX_DELAY );

		if( ulNotifiedValue == 0 )
		{
		    //not supposed to get there
		}
		else
		{

		    while( ulNotifiedValue > 0 )
		    {
		        char c = UART1_read();

		        if(mavlink_parse_char(MAVLINK_COMM_0, c, &rcv_msg, &rcv_status))
		        {
		        	//handleIncomingMessage(&rcv_msg,&rcv_status);
		        }

		        ulNotifiedValue--;
		    }
		}
	}
}
