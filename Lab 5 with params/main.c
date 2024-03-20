#include <stdint.h>
#include <stdio.h>
#include <FreeRTOS.h>
#include <task.h>
#include "tm4c123gh6pm.h"
#include "DIO.h"

void blinkLED(void *pvParameters)
{
  int *LED = (int*)(pvParameters);
	for (;;)
	{
		GPIO_PORTF_DATA_R ^= 1 << *LED;
		vTaskDelay(1000 * (*LED));
	}
}


int main()
{
	LED_Init();
	static int LED_RED = 1;
	static int LED_BLUE = 2;
	static int LED_GREEN = 3;
	/*BaseType_t xTaskCreate( TaskFunction_t pxTaskCode,
							const char * const pcName,
							const configSTACK_DEPTH_TYPE usStackDepth,
							void * const pvParameters,
							UBaseType_t uxPriority,
							TaskHandle_t * const pxCreatedTask );*/
	xTaskCreate(blinkLED, "Toggle Red Task", 200, (void *)&LED_RED, 1, NULL);
	xTaskCreate(blinkLED, "Toggle Blue Task", 200, (void *)&LED_BLUE, 1, NULL);
	xTaskCreate(blinkLED, "Toggle Green Task", 200, (void *)&LED_GREEN, 1, NULL);
	// Startup of the FreeRTOS scheduler.  The program should block here.
	vTaskStartScheduler();

	// The following line should never be reached.
	// Failure to allocate enough memory from the heap could be a reason.
	for (;;)
		;
}
