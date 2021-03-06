#include "Tasks/Task_FlashHeartbeatLED.h"
#include "stm32f4xx.h"
#include "Mavlink/common/mavlink.h"
#include "semphr.h"
#include "COM/UART/UART1.h"
#include "COM/UART/UART6.h"

static void vTask_FlashHeartbeatLED( void * pvParameters )
{
	extern SemaphoreHandle_t xUART1Mutex;
	uint8_t TXbuf[256];
	uint16_t len;
	mavlink_message_t msg;

    while(1)
    {
    	GPIO_ToggleBits(GPIOC, GPIO_Pin_13);

    	mavlink_msg_heartbeat_pack(1, 200, &msg, MAV_TYPE_QUADROTOR, MAV_AUTOPILOT_GENERIC, MAV_MODE_GUIDED_DISARMED, 0, MAV_STATE_ACTIVE);
    	len = mavlink_msg_to_send_buffer(TXbuf, &msg);

    	xSemaphoreTake(xUART1Mutex, portMAX_DELAY);

    		for(int i = 0; i != len; i++) UART1_write(TXbuf[i]);

    	xSemaphoreGive(xUART1Mutex);

    	extern float voltage, current, energy_drained;
    	mavlink_msg_sys_status_pack(1, 200, &msg,

				MAV_SYS_STATUS_SENSOR_3D_GYRO |
				MAV_SYS_STATUS_SENSOR_3D_ACCEL|
				MAV_SYS_STATUS_SENSOR_3D_MAG  |
				MAV_SYS_STATUS_SENSOR_GPS,

				MAV_SYS_STATUS_SENSOR_3D_GYRO |
				MAV_SYS_STATUS_SENSOR_3D_ACCEL|
				MAV_SYS_STATUS_SENSOR_3D_MAG,

				MAV_SYS_STATUS_SENSOR_3D_GYRO |
				MAV_SYS_STATUS_SENSOR_3D_ACCEL|
				MAV_SYS_STATUS_SENSOR_3D_MAG,
				100,
				(uint16_t)(voltage*1000.0),
				(uint16_t)(current*100.0),
				100-(uint8_t)(((energy_drained)/3100.0)*100.0),
				0,0,0,0,0,0);

    	len = mavlink_msg_to_send_buffer(TXbuf, &msg);

    	xSemaphoreTake(xUART1Mutex, portMAX_DELAY);

    		for(int i = 0; i != len; i++) UART1_write(TXbuf[i]);

    	xSemaphoreGive(xUART1Mutex);

        vTaskDelay(1000);
    }
}


TaskHandle_t vCreateTask_FlashHeartbeatLED(uint32_t stack_size, uint32_t priority)
{
	BaseType_t xReturned;
	TaskHandle_t xHandle = NULL;

    /* Create the task, storing the handle. */
    xReturned = xTaskCreate(
    		        vTask_FlashHeartbeatLED,       /* Function that implements the task. */
                    "Flash LED",          /* Text name for the task. */
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

