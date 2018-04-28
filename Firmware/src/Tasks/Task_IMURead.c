#include "Tasks/Task_IMURead.h"
#include "SPI/SPI1.h"
#include "MEMS/MPU9250.h"
#include "UART/UART6.h"

static void vTask_IMURead( void * pvParameters )
{
	SPI1_init();

	MPU_init();

	UART6_init(115200);

    while(1)
    {

    	UART6_write(MPU_ReadReg(0x00));
    	UART6_write(MPU_ReadReg(0x01));
    	UART6_write(MPU_ReadReg(0x02));

    	UART6_write(MPU_ReadReg(0x0D));
    	UART6_write(MPU_ReadReg(0x0E));
    	UART6_write(MPU_ReadReg(0x0F));


    	UART6_write(MPU_whoami());




  	   	GPIO_ToggleBits(GPIOC, GPIO_Pin_14);



        vTaskDelay(500);
    }
}

TaskHandle_t vCreateTask_IMURead(void)
{
	BaseType_t xReturned;
	TaskHandle_t xHandle = NULL;

    /* Create the task, storing the handle. */
    xReturned = xTaskCreate(
    		        vTask_IMURead,       /* Function that implements the task. */
                    "IMU Read",          /* Text name for the task. */
					512,      /* Stack size in words, not bytes. */
                    NULL,    /* Parameter passed into the task. */
					tskIDLE_PRIORITY+4,/* Priority at which the task is created. */
                    &xHandle );      /* Used to pass out the created task's handle. */

    if( xReturned != pdPASS )
    {
        /* The task was created.  Use the task's handle to delete the task. */
        return NULL;
    }

    return xHandle;
}




