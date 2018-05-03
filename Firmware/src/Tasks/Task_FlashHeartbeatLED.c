#include "Tasks/Task_FlashHeartbeatLED.h"
#include "stm32f4xx.h"
#include "Mavlink/common/mavlink.h"
#include "semphr.h"
#include "COM/UART/UART1.h"
#include "COM/UART/UART6.h"

#include "Motor/Motor.h"

void vTask_FlashHeartbeatLED(void * pvParameters);

TaskHandle_t vCreateTask_FlashHeartbeatLED(uint32_t stack_size)
{
	BaseType_t xReturned;
	TaskHandle_t xHandle = NULL;

    /* Create the task, storing the handle. */
    xReturned = xTaskCreate(
    		        vTask_FlashHeartbeatLED,       /* Function that implements the task. */
                    "Flash LED",          /* Text name for the task. */
					stack_size,      /* Stack size in words, not bytes. */
                    NULL,    /* Parameter passed into the task. */
					tskIDLE_PRIORITY,/* Priority at which the task is created. */
                    &xHandle );      /* Used to pass out the created task's handle. */

    if( xReturned != pdPASS )
    {
        /* The task was created.  Use the task's handle to delete the task. */
        return NULL;
    }

    return xHandle;
}


extern SemaphoreHandle_t xUART1Semphr;

void vTask_FlashHeartbeatLED( void * pvParameters )
{
	uint8_t TXbuf[256];
	uint16_t len;
	mavlink_message_t msg;

    while(1)
    {
    	GPIO_ToggleBits(GPIOC, GPIO_Pin_13);

    	mavlink_msg_heartbeat_pack(1, 200, &msg, MAV_TYPE_QUADROTOR, MAV_AUTOPILOT_GENERIC, MAV_MODE_GUIDED_DISARMED, 0, MAV_STATE_ACTIVE);
    	len = mavlink_msg_to_send_buffer(TXbuf, &msg);

    	xSemaphoreTake(xUART1Semphr, portMAX_DELAY);

    		for(int i = 0; i != len; i++) UART1_write(TXbuf[i]);

    	xSemaphoreGive(xUART1Semphr);


        vTaskDelay(1000);
    }
}
