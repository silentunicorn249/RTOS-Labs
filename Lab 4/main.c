#include <stdint.h>
#include <stdio.h>
#include <FreeRTOS.h>
#include <task.h>
#include "tm4c123gh6pm.h"
#include "DIO.h"
void vPeriodicTask(void *pvParameters)
{

	for (;;)
	{

		GPIO_PORTF_DATA_R ^= 1 << 1;
		vTaskDelay(500);
	}
}

void vPeriodicTask2(void *pvParameters)
{

	for (;;)
	{

		GPIO_PORTF_DATA_R ^= 1 << 2;
		vTaskDelay(1000);
	}
}

int main()
{
	DIO_Init(PORTF);
	DIO_DIR(PORTF, 1, OUTPUT);
	DIO_DIR(PORTF, 2, OUTPUT);
	/*BaseType_t xTaskCreate( TaskFunction_t pxTaskCode,
							const char * const pcName,
							const configSTACK_DEPTH_TYPE usStackDepth,
							void * const pvParameters,
							UBaseType_t uxPriority,
							TaskHandle_t * const pxCreatedTask );*/
	xTaskCreate(vPeriodicTask, "Toggle Red Task", 200, NULL, 1, NULL);
	xTaskCreate(vPeriodicTask2, "Toggle Red Task", 200, NULL, 1, NULL);
	// Startup of the FreeRTOS scheduler.  The program should block here.
	vTaskStartScheduler();

	// The following line should never be reached.
	// Failure to allocate enough memory from the heap could be a reason.
	for (;;)
		;
}
