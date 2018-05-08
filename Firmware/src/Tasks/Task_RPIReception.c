/*
 * Task_Idle.c
 *
 *  Created on: Apr 26, 2018
 *      Author: simon
 */
#include "Tasks/Task_RPIReception.h"
#include "COM/UART/UART1.h"
#include "Mavlink/common/mavlink.h"
#include "stm32f4xx.h"
#include "Tasks/mavlink_handlers.h"

static void handleIncomingMavlinkMessage(mavlink_message_t* msg, mavlink_status_t* status);




static void vTask_RPIReception(void * pvParameters)
{
	uint32_t ulNotifiedValue;

	mavlink_message_t rcv_msg;
	mavlink_status_t rcv_status;

	while(1){

		ulNotifiedValue = ulTaskNotifyTake( pdTRUE, portMAX_DELAY );

		if( ulNotifiedValue == 0 )
		{
		    while(1);
		}
		else
		{
		    while( ulNotifiedValue > 0 )
		    {
		        char c = UART1_read();

		        if(mavlink_parse_char(MAVLINK_COMM_0, c, &rcv_msg, &rcv_status))
		        {
		        	handleIncomingMavlinkMessage(&rcv_msg,&rcv_status);
		        }

		        ulNotifiedValue--;
		    }
		}
	}
}

void handleIncomingMavlinkMessage(mavlink_message_t* msg, mavlink_status_t* status){
	switch(msg->msgid)
		{
			case(0):
				handle_heartbeat(msg, status);
				break;
			case(20):
				handle_param_request_read(msg, status);
				break;
			case(21):
				handle_param_request_list(msg, status);
				break;
			case(23):
				handle_param_set(msg, status);
				break;
			case(43):
				handle_mission_request_list(msg, status);
				break;
			case(47):
				handle_mission_ack(msg, status);
				break;
			case(69):
				handle_manual_control(msg, status);
				break;
			case(76):
				handle_command_long(msg, status);
				break;
			default:
				//printf("\nReceived packet: SYS: %d, COMP: %d, LEN: %d, MSG ID: %d\n", msg->sysid, msg->compid, msg->len, msg->msgid);
				break;
		}
}

TaskHandle_t vCreateTask_RPIReception(uint32_t stack_size, uint32_t priority)
{
	BaseType_t xReturned;
	TaskHandle_t xHandle = NULL;

	/* Create the task, storing the handle. */
    xReturned = xTaskCreate(vTask_RPIReception,       /* Function that implements the task. */
	                        "RPI_Rec",          /* Text name for the task. */
							stack_size,      /* Stack size in words, not bytes. */
	                        NULL,    /* Parameter passed into the task. */
	                        priority,/* Priority at which the task is created. */
	                        &xHandle );      /* Used to pass out the created task's handle. */

	if( xReturned != pdPASS ) return NULL;

	return xHandle;

}
