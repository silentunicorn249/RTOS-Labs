#include <stdint.h>
#include <stdio.h>
#include <FreeRTOS.h>
#include <task.h>
#include "tm4c123gh6pm.h"
#include "DIO.h"
#include "semphr.h"

xSemaphoreHandle xBinarySemaphore;



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

void psh_btn_init()
{
        SYSCTL_RCGCGPIO_R |= 0x20; // Enable clock for PORTF
        GPIO_PORTF_LOCK_R = 0x4C4F434B;
        GPIO_PORTF_CR_R = 0x11;
    GPIO_PORTF_DIR_R &= ~0x11; // PF0 as input
    GPIO_PORTF_DIR_R |= 0x0E; // PF1 as output
    GPIO_PORTF_DEN_R |= 0x1F; // Enable digital I/O for PF0 and PF1
        GPIO_PORTF_PUR_R = 0x11;
}


void UART0_init(void)
{
    SYSCTL_RCGCUART_R |= 0x01; /* Enable clock to UART0 */
    SYSCTL_RCGCGPIO_R |= 0x01; /* Enable clock to Port A for PA0/Rx and PA1/Tx */

    /* UART0 initialization */
    UART0_CTL_R = 0;          /* UART0 module disable */
    UART0_IBRD_R = 104;       /* Integer part of the baud rate divisor for 9600 baud rate */
    UART0_FBRD_R = 11;        /* Fractional part of the baud rate divisor for 9600 baud rate */
    UART0_CC_R = 0;           /* Select system clock as the UART clock source */
    UART0_LCRH_R = 0x60;      /* Data length 8-bit, no parity bit, no FIFO */
    UART0_CTL_R = 0x301;      /* Enable UART0 module, Rx and Tx */

    /* UART0 TX0 and RX0 use PA1 and PA0. Configure them digital and enable alternate function */
    GPIO_PORTA_DEN_R |= 0x03; /* Set PA0 and PA1 as digital */
    GPIO_PORTA_AFSEL_R |= 0x03; /* Use PA0, PA1 alternate function */
    GPIO_PORTA_AMSEL_R = 0;   /* Turn off analog function */
    GPIO_PORTA_PCTL_R = 0x00000011; /* Configure PA0 and PA1 for UART0 */
}

void Init()
{
				PortF_Init();
        psh_btn_init();
        UART0_init();
}


void UART_Write(unsigned char data)
{
  while ((UART0_FR_R & (1 << 5)) != 0)
    ;                /* wait until Tx buffer not full */
  UART0_DR_R = data; /* before giving it another byte */
}

void UART_Write_String(char *str)
{
  while (*str)
  {
    UART_Write(*(str++));
  }
}

void GPIOF_Handler(void)
{
	UART_Write_String( "Interrupt Handler.\n" ); 
	uint32_t i;
	portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;

	xSemaphoreGiveFromISR( xBinarySemaphore, &xHigherPriorityTaskWoken );

	GPIO_PORTF_ICR_R = 0x11;
	i = GPIO_PORTF_ICR_R;
	portEND_SWITCHING_ISR( xHigherPriorityTaskWoken );
	
}

static void vHandlerTask( void *pvParameters ) 
{ 
 /* As per most tasks, this task is implemented within an infinite loop. 
 
 Take the semaphore once to start with so the semaphore is empty before the 
 infinite loop is entered. The semaphore was created before the scheduler 
 was started so before this task ran for the first time.*/ 
 xSemaphoreTake( xBinarySemaphore, 0 ); 
 
 for( ;; ) 
 { 
 /* Use the semaphore to wait for the event. The task blocks 
 indefinitely meaning this function call will only return once the 
 semaphore has been successfully obtained - so there is no need to check 
 the returned value. */ 
 xSemaphoreTake( xBinarySemaphore, portMAX_DELAY ); 
 /* To get here the event must have occurred. Process the event (in this 
 case we just print out a message). */ 
 UART_Write_String( "Handler task - Processing event.\n" ); 
 } 
} 




static void vPeriodicTask( void *pvParameters ) 
{ 
 /* As per most tasks, this task is implemented within an infinite loop. */ 
 for( ;; ) 
 { 
 /* This task is just used to 'simulate' an interrupt. This is done by 
 periodically generating a software interrupt. */ 
 vTaskDelay( 500 / portTICK_RATE_MS ); 
 /* Generate the interrupt, printing a message both before hand and 
 afterwards so the sequence of execution is evident from the output. */ 
// vPrintString( "Periodic task - About to generate an interrupt.\n" ); 
 UART_Write_String( "Periodic task - About to generate an interrupt.\n");
// vPrintString( "Periodic task - Interrupt generated.\n\n" ); 
 }

}
 

int main( void ) 
{ 
 /* Configure both the hardware and the debug interface. */ 
	Init();
 /* Before a semaphore is used it must be explicitly created. In this example 
 a binary semaphore is created. */ 
 vSemaphoreCreateBinary( xBinarySemaphore ); 
 /* Check the semaphore was created successfully. */ 
 if( xBinarySemaphore != NULL ) 
 { 
 /* Create the 'handler' task. This is the task that will be synchronized 
 with the interrupt. The handler task is created with a high priority to 
 ensure it runs immediately after the interrupt exits. In this case a 
 priority of 3 is chosen. */ 
 xTaskCreate( vHandlerTask, "Handler", 240, NULL, 3, NULL ); 
 /* Create the task that will periodically generate a software interrupt. 
 This is created with a priority below the handler task to ensure it will 
 get preempted each time the handler task exits the Blocked state. */ 
 xTaskCreate( vPeriodicTask, "Periodic", 240, NULL, 1, NULL ); 
 /* Start the scheduler so the created tasks start executing. */ 
 vTaskStartScheduler(); 
 } 
 
 /* If all is well we will never reach here as the scheduler will now be 
 running the tasks. If we do reach here then it is likely that there was 
 insufficient heap memory available for a resource to be created. */ 
 for( ;; ); 
}