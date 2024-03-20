#include <stdio.h>
#include "tm4c123gh6pm.h"

#define uint32_t unsigned int
// #define SYSCTL_RCGCGPIO_R (*((volatile uint32_t *)0x400FE608))
// #define GPIO_PORTF_DIR_R (*((volatile uint32_t *)0x40025400))
// #define GPIO_PORTF_DEN_R (*((volatile uint32_t *)0x4002551C))
// #define GPIO_PORTF_DATA_R (*((volatile uint32_t *)0x400253FC))

#define RED_LED (1 << 1)
#define BLUE_LED (1 << 2)

int x = 0;

void SysTick_Handler(void){
	x++;
}

void delay(int msecs)
{
	for (int i = 0; i < msecs * 1000; i++)
	{
		__asm("NOP");
	}
}

void blinky1()
{
	while (1)
	{
		delay(1000);
		GPIO_PORTF_DATA_R ^= RED_LED;
	}
}

void blinky2()
{
	while (1)
	{
		delay(500);
		GPIO_PORTF_DATA_R ^= BLUE_LED;
	}
}

int main()
{
	SYSCTL_RCGCGPIO_R |= 0x20;
	// Set Pin 1 of Port F as output
	GPIO_PORTF_DIR_R |= (RED_LED | BLUE_LED);

	// Enable digital functionality for Pin 1 of Port F
	GPIO_PORTF_DEN_R |= (RED_LED | BLUE_LED);

	NVIC_ST_CTRL_R = 0x7;
	NVIC_ST_RELOAD_R = 160000U;
	
	blinky1();

	return 0;
}