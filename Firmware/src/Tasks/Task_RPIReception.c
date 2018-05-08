/*
 * Task_Idle.c
 *
 *  Created on: Apr 26, 2018
 *      Author: simon
 */
#include "Tasks/Task_RPIReception.h"
#include "COM/UART/UART1.h"
#include "Mavlink/common/mavlink.h"

static void handleIncomingMavlinkMessage(mavlink_message_t* msg, mavlink_status_t* status);
static void vTask_RPIReception(void * pvParameters);

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
				handle_heartbeat(msg);
				break;
			case(20):
				handle_param_request_read(msg);
				break;
			case(21):
				handle_param_request_list(msg);
				break;
			case(23):
				handle_param_set(msg);
				break;
			case(43):
				handle_mission_request_list(msg);
				break;
			case(47):
				handle_mission_ack(msg);
				break;
			case(69):
				handle_manual_control(msg);
				break;
			case(76):
				handle_command_long(msg);
				break;
			default:
				//printf("\nReceived packet: SYS: %d, COMP: %d, LEN: %d, MSG ID: %d\n", msg->sysid, msg->compid, msg->len, msg->msgid);
				break;
		}
}
