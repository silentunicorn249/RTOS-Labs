#include <stdint.h>
#include <stdio.h>
#include <FreeRTOS.h>
#include <task.h>
#include "tm4c123gh6pm.h"
#include "DIO.h"


void vApplicationIdleHook(void){
	GPIO_PORTF_DATA_R ^= 1<<2;
}



void vTask1(void *pvParameters)
{

	for(;;){
		GPIO_PORTF_DATA_R ^= 1<<1;
		//taskYIELD();
		
	}
	
}	
void vTask2(void *pvParameters)
{

	for(;;){
		GPIO_PORTF_DATA_R ^= 1<<3;
		//taskYIELD();
	}
	
}	
void vTask3(void *pvParameters)
{

	for(;;){
		GPIO_PORTF_DATA_R ^= 1<<2;
		 
	}
	
}	
int main()
  {
	DIO_Init(PORTF);
	DIO_DIR(PORTF,1,OUTPUT);
	DIO_DIR(PORTF,2,OUTPUT);
	DIO_DIR(PORTF,3,OUTPUT);
	/*BaseType_t xTaskCreate( TaskFunction_t pxTaskCode,
                            const char * const pcName,
                            const configSTACK_DEPTH_TYPE usStackDepth,
                            void * const pvParameters,
                            UBaseType_t uxPriority,
                            TaskHandle_t * const pxCreatedTask );*/
xTaskCreate(vTask1,"Toggle Red Task",200,NULL,1,NULL );
xTaskCreate(vTask2,"Toggle Green Task",200,NULL,1,NULL );
//xTaskCreate(vTask3,"Toggle Blue Task",200,NULL,1,NULL );
	// Startup of the FreeRTOS scheduler.  The program should block here.  
	vTaskStartScheduler();
	
	// The following line should never be reached.  
	//Failure to allocate enough memory from the heap could be a reason.
	for (;;);
	
}