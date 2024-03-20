#include <stdint.h>
#include <stdio.h>
#include <FreeRTOS.h>
#include <task.h>
#include "tm4c123gh6pm.h"
#include "DIO.h"

void blinkRed(void *pvParameters)
{

	for (;;)
	{
		GPIO_PORTF_DATA_R ^= 1 << 1;
		vTaskDelay(1000);
	}
}

void blinkBlue(void *pvParameters)
{

	for (;;)
	{
		GPIO_PORTF_DATA_R ^= 1 << 2;
		vTaskDelay(2000);
	}
}

void blinkGreen(void *pvParameters)
{

	for (;;)
	{
		GPIO_PORTF_DATA_R ^= 1 << 3;
		vTaskDelay(3000);
	}
}

int main()
{
	LED_Init();
	/*BaseType_t xTaskCreate( TaskFunction_t pxTaskCode,
							const char * const pcName,
							const configSTACK_DEPTH_TYPE usStackDepth,
							void * const pvParameters,
							UBaseType_t uxPriority,
							TaskHandle_t * const pxCreatedTask );*/
	xTaskCreate(blinkRed, "Toggle Red Task", 200, NULL, 1, NULL);
	xTaskCreate(blinkBlue, "Toggle Blue Task", 200, NULL, 1, NULL);
	xTaskCreate(blinkGreen, "Toggle Green Task", 200, NULL, 1, NULL);
	// Startup of the FreeRTOS scheduler.  The program should block here.
	vTaskStartScheduler();

	// The following line should never be reached.
	// Failure to allocate enough memory from the heap could be a reason.
	for (;;)
		;
}
