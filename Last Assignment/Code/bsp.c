/* Board Support Package (BSP) for the EK-TM4C123GXL board */
#include <stdint.h>  /* Standard integers. WG14/N843 C99 Standard */

#include "bsp.h"
#include "miros.h"
#include "tm4c123gh6pm.h"

/* on-board LEDs */
#define LED_RED   (1U << 1)
#define LED_BLUE  (1U << 2)
#define LED_GREEN (1U << 3)

static uint32_t volatile l_tickCtr;

void SysTick_Handler(void) {
    ++l_tickCtr;

    __asm("CPSID I");   //disable 
    OS_sched();
  __asm("CPSIE I");  //enable

}

void BSP_init(void) {
	SYSCTL_RCGCGPIO_R = 0x20U;
	GPIO_PORTF_DIR_R = 0x0EU;
	GPIO_PORTF_DEN_R = 0x0EU;
	NVIC_ST_RELOAD_R = 0xFFFFFF; // reload reg. with max value
	NVIC_ST_CTRL_R = 7;					// enable it, enable interrupt, use system clock 
	
   /* set the SysTick interrupt priority (highest) */
	NVIC_SYS_PRI3_R &= ~(0xFFU << 24);

	//Enable the global interrupt to the processor 
	__asm("CPSIE I");
}
uint32_t BSP_tickCtr(void) {
    uint32_t tickCtr;

  __asm("CPSID I");   //disable 
   tickCtr = l_tickCtr;
  __asm("CPSIE I");  //enable  

    return tickCtr;
}

void BSP_delay(uint32_t ticks) {
    uint32_t start = BSP_tickCtr();
    while ((BSP_tickCtr() - start) < ticks) {
    }
}


void BSP_ledRedOn(void) {
   GPIO_PORTF_DATA_R |= LED_RED;
}

void BSP_ledRedOff(void) {
    GPIO_PORTF_DATA_R &= ~LED_RED;
}

void BSP_ledBlueOn(void) {
    GPIO_PORTF_DATA_R |= LED_BLUE;
}

void BSP_ledBlueOff(void) {
    GPIO_PORTF_DATA_R &= ~LED_BLUE;
}

void BSP_ledGreenOn(void) {
    GPIO_PORTF_DATA_R |= LED_GREEN;
}

void BSP_ledGreenOff(void) {
    GPIO_PORTF_DATA_R &= ~LED_GREEN;
}