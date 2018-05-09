#include "Tasks/Task_GPSParser.h"
#include "string.h"
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

typedef enum{
	WAITING_FOR_DOLLAR_SIGN,
	WAITING_FOR_PREFIX_G_LETTER,
	WAITING_FOR_PREFIX_P_LETTER,
	WAITING_FOR_MESSAGE_DESCRIPTION_FIRST_LETTER,
	WAITING_FOR_MESSAGE_DESCRIPTION_SECOND_LETTER,
	WAITING_FOR_MESSAGE_DESCRIPTION_THIRD_LETTER,
	ROUTE

}NMEA_Global_State;

void NMEA_state_machine(char c){

	static NMEA_Global_State state= WAITING_FOR_DOLLAR_SIGN;
	static char description[3];


	switch(state){

	case WAITING_FOR_DOLLAR_SIGN:
		if(c == '$') state = WAITING_FOR_PREFIX_G_LETTER;
		else state = WAITING_FOR_DOLLAR_SIGN;
		break;

	case WAITING_FOR_PREFIX_G_LETTER:
		if(c == 'G') state = WAITING_FOR_PREFIX_P_LETTER;
		else state = WAITING_FOR_DOLLAR_SIGN;
		break;

	case WAITING_FOR_PREFIX_P_LETTER:
		if(c == 'P') state = WAITING_FOR_MESSAGE_DESCRIPTION_FIRST_LETTER;
		else state = WAITING_FOR_DOLLAR_SIGN;
		break;

	case WAITING_FOR_MESSAGE_DESCRIPTION_FIRST_LETTER:
		description[0] = c;
		state = WAITING_FOR_MESSAGE_DESCRIPTION_SECOND_LETTER;
		break;

	case WAITING_FOR_MESSAGE_DESCRIPTION_SECOND_LETTER:
		description[1] = c;
		state = WAITING_FOR_MESSAGE_DESCRIPTION_THIRD_LETTER;
		break;

	case WAITING_FOR_MESSAGE_DESCRIPTION_THIRD_LETTER:
		description[2] = c;
		//state =
	}

}
