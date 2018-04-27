#include "Tasks/Task_FlashHeartbeatLED.h"
#include "stm32f4xx.h"

#include "UART/UART6.h"
//#include "UART/UART2.h"

void vTask_FlashHeartbeatLED(void * pvParameters);

TaskHandle_t vCreateTask_FlashHeartbeatLED(void)
{
	BaseType_t xReturned;
	TaskHandle_t xHandle = NULL;

    /* Create the task, storing the handle. */
    xReturned = xTaskCreate(
    		        vTask_FlashHeartbeatLED,       /* Function that implements the task. */
                    "Flash LED",          /* Text name for the task. */
					configMINIMAL_STACK_SIZE,      /* Stack size in words, not bytes. */
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


void vTask_FlashHeartbeatLED( void * pvParameters )
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOC, &GPIO_InitStruct);

	UART6_init(9600);

    while(1)
    {
    	GPIO_ToggleBits(GPIOC, GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);
    	UART6_print("ABC");
        vTaskDelay(1000);
    }
}
