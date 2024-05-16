#include <stdint.h>
#include <stdio.h>
#include <FreeRTOS.h>
#include <task.h>
#include "tm4c123gh6pm.h"
#include "DIO.h"
#include "semphr.h"

void PortF_Init(void){ 
 SYSCTL_RCGCGPIO_R |= 0x00000020; // 1) F clock
 GPIO_PORTF_LOCK_R = 0x4C4F434B; // 2) unlock PortF PF0 
 GPIO_PORTF_CR_R = 0x1F; // allow changes to PF4-0 
 GPIO_PORTF_AMSEL_R= 0x00; // 3) disable analog function
 GPIO_PORTF_PCTL_R = 0x00000000; // 4) GPIO clear bit PCTL 
 GPIO_PORTF_DIR_R = 0x0E; // 5) PF4,PF0 input, PF3,PF2,PF1 output 
 GPIO_PORTF_AFSEL_R = 0x00; // 6) no alternate function
 GPIO_PORTF_AFSEL_R = 0x00; // 6) no alternate function
 GPIO_PORTF_PUR_R = 0x11; // enable pullup resistors on PF4,PF0 
 GPIO_PORTF_DEN_R = 0x1F; // 7) enable digital pins PF4-PF0
 GPIO_PORTF_DATA_R = 0x00;
// Setup the interrupt on PortF
 GPIO_PORTF_ICR_R = 0x11; // Clear any Previous Interrupt 
 GPIO_PORTF_IM_R |=0x11; // Unmask the interrupts for PF0 and PF4
 GPIO_PORTF_IS_R |= 0x11; // Make bits PF0 and PF4 level sensitive
 GPIO_PORTF_IEV_R &= ~0x11; // Sense on Low Level
 NVIC_EN0_R |= (1<<30); // Enable the Interrupt for PortF in NVIC
}
xSemaphoreHandle xBinarySemaphore;

static void vHandler( void *pvParameters )
{

    xSemaphoreTake( xBinarySemaphore, 0 );

    for( ;; )
    {
        xSemaphoreTake( xBinarySemaphore, portMAX_DELAY );
				GPIO_PORTF_DATA_R ^= 0x4;
    }
}
static void vPeriodic( void *pvParameters )
{
    for( ;; )
    {
			vTaskDelay( 1000 / portTICK_RATE_MS );
			GPIO_PORTF_DATA_R ^= 0x2;
    }
}

void GPIOF_Handler(void)
{
	uint32_t i;
	portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;

	xSemaphoreGiveFromISR( xBinarySemaphore, &xHigherPriorityTaskWoken );

	GPIO_PORTF_ICR_R = 0x11;
	i = GPIO_PORTF_ICR_R;
	portEND_SWITCHING_ISR( xHigherPriorityTaskWoken );
	
}
int main()
{
		
	PortF_Init();
	__asm("CPSIE I");
  vSemaphoreCreateBinary( xBinarySemaphore );

  /* Check the semaphore was created successfully. */
	if( xBinarySemaphore != NULL )
	{
		xTaskCreate( vHandler, "Handler", 240, NULL, 3, NULL );
		xTaskCreate( vPeriodic, "Periodic", 240, NULL, 1, NULL );
		vTaskStartScheduler();
	}
	for( ;; );
}











