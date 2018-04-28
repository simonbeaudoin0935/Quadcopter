
#include "FreeRTOS.h"
#include "Tasks/Task_FlashHeartbeatLED.h"
#include "Tasks/Task_RPIReception.h"
#include "Tasks/Task_PIDLoop.h"
#include "Tasks/Task_IMURead.h"

#include "semphr.h"

#include "stm32f4xx.h"

struct{
	TaskHandle_t TaskHandle_FlashHeartbeatLED;
	TaskHandle_t TaskHandle_RPIReception;
	TaskHandle_t TaskHandle_PIDLoop;
	TaskHandle_t TaskHandle_IMURead;
}TaskHandles;

SemaphoreHandle_t xUART1Semphr;

int main(void)
{
	NVIC_SetPriorityGrouping( 0 );

	xUART1Semphr =  xSemaphoreCreateMutex();

	TaskHandles.TaskHandle_FlashHeartbeatLED = vCreateTask_FlashHeartbeatLED();
	TaskHandles.TaskHandle_RPIReception = vCreateTask_RPIReception();
	TaskHandles.TaskHandle_PIDLoop = vCreateTask_PIDLoop();
	TaskHandles.TaskHandle_IMURead = vCreateTask_IMURead();


	vTaskStartScheduler();

	while(1);
}


void vApplicationMallocFailedHook( void )
{
	/* vApplicationMallocFailedHook() will only be called if
	configUSE_MALLOC_FAILED_HOOK is set to 1 in FreeRTOSConfig.h.  It is a hook
	function that will get called if a call to pvPortMalloc() fails.
	pvPortMalloc() is called internally by the kernel whenever a task, queue,
	timer or semaphore is created.  It is also called by various parts of the
	demo application.  If heap_1.c or heap_2.c are used, then the size of the
	heap available to pvPortMalloc() is defined by configTOTAL_HEAP_SIZE in
	FreeRTOSConfig.h, and the xPortGetFreeHeapSize() API function can be used
	to query the size of free heap space that remains (although it does not
	provide information on how the remaining heap might be fragmented). */
	taskDISABLE_INTERRUPTS();
	for( ;; );
}
/*-----------------------------------------------------------*/

void vApplicationIdleHook( void )
{
	/* vApplicationIdleHook() will only be called if configUSE_IDLE_HOOK is set
	to 1 in FreeRTOSConfig.h.  It will be called on each iteration of the idle
	task.  It is essential that code added to this hook function never attempts
	to block in any way (for example, call xQueueReceive() with a block time
	specified, or call vTaskDelay()).  If the application makes use of the
	vTaskDelete() API function (as this demo application does) then it is also
	important that vApplicationIdleHook() is permitted to return to its calling
	function, because it is the responsibility of the idle task to clean up
	memory allocated by the kernel to any task that has since been deleted. */
}
/*-----------------------------------------------------------*/

void vApplicationStackOverflowHook( TaskHandle_t pxTask, char *pcTaskName )
{
	( void ) pcTaskName;
	( void ) pxTask;

	/* Run time stack overflow checking is performed if
	configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.  This hook
	function is called if a stack overflow is detected. */
	taskDISABLE_INTERRUPTS();


	for( ;; ){
		GPIO_ToggleBits(GPIOC, GPIO_Pin_13);
		for(int i = 0; i != 75000; i++);
	}
}
