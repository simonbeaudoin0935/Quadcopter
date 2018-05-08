#include "Tasks/Task_PMUReader.h"
#include "stm32f4xx.h"
#include "Mavlink/common/mavlink.h"
#include "semphr.h"
#include "COM/UART/UART1.h"

#include "PMU/PMU.h"

void vTask_PMUReader(void * pvParameters);

TaskHandle_t vCreateTask_PMUReader(uint32_t stack_size)
{
	BaseType_t xReturned;
	TaskHandle_t xHandle = NULL;

    /* Create the task, storing the handle. */
    xReturned = xTaskCreate(
    		        vTask_PMUReader,       /* Function that implements the task. */
                    "PMU Read",          /* Text name for the task. */
					stack_size,      /* Stack size in words, not bytes. */
                    NULL,    /* Parameter passed into the task. */
					tskIDLE_PRIORITY+1,/* Priority at which the task is created. */
                    &xHandle );      /* Used to pass out the created task's handle. */

    if( xReturned != pdPASS )
    {
        /* The task was created.  Use the task's handle to delete the task. */
        return NULL;
    }

    return xHandle;
}


float current = 0.0, voltage = 0.0, energy_drained = 0.0;

void vTask_PMUReader( void * pvParameters )
{

    while(1)
    {
    	PMU_read(&voltage, &current);
    	energy_drained += voltage * current * 0.1;

        vTaskDelay(100);
    }
}
