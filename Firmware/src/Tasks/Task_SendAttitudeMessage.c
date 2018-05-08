#include "Tasks/Task_SendAttitudeMessage.h"

#include "stm32f4xx.h"
#include "Mavlink/common/mavlink.h"
#include "semphr.h"
#include "COM/UART/UART1.h"
#include "COM/UART/UART6.h"

void vTask_SendAttitudeMessage(void * pvParameters);

TaskHandle_t vCreateTask_SendAttitudeMessage(uint32_t stack_size, uint32_t priority)
{
	BaseType_t xReturned;
	TaskHandle_t xHandle = NULL;

    /* Create the task, storing the handle. */
    xReturned = xTaskCreate(
    		        vTask_SendAttitudeMessage,       /* Function that implements the task. */
                    "Send Atti",          /* Text name for the task. */
					stack_size,      /* Stack size in words, not bytes. */
                    NULL,    /* Parameter passed into the task. */
					priority,/* Priority at which the task is created. */
                    &xHandle );      /* Used to pass out the created task's handle. */

    if( xReturned != pdPASS )
    {
        /* The task was created.  Use the task's handle to delete the task. */
        return NULL;
    }

    return xHandle;
}





#define DEG_2_RAD (3.14159265359 / 180.0)

void vTask_SendAttitudeMessage( void * pvParameters )
{
	uint8_t TXbuf[256];
	uint16_t len;
	mavlink_message_t msg;

    while(1)
    {

    	extern float roll,pitch,yaw;
    	extern float x_rate, y_rate, z_rate;

		mavlink_msg_attitude_pack(1,
								  200,
								  &msg,
								  xTaskGetTickCount(),
								  roll,
								  pitch,
								  yaw,
								  x_rate * DEG_2_RAD,
								  y_rate * DEG_2_RAD,
 								  z_rate * DEG_2_RAD);

		len = mavlink_msg_to_send_buffer(TXbuf, &msg);


		extern SemaphoreHandle_t xUART1Mutex;

    	xSemaphoreTake(xUART1Mutex, portMAX_DELAY);

    		for(int i = 0; i != len; i++) UART1_write(TXbuf[i]);

    	xSemaphoreGive(xUART1Mutex);

        vTaskDelay(20);
    }
}
