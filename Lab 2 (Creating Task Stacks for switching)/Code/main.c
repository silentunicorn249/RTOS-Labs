#include "tm4c123gh6pm.h"
#include <stdint.h>
#define LED_RED  0x2
#define LED_BLUE  0x4

void main_blinky1(void);
void main_blinky2(void);


uint32_t stack_blinky1[40];
uint32_t stack_blinky2[40];
uint32_t *sp_blinky1 = &stack_blinky1[40];
uint32_t *sp_blinky2 = &stack_blinky2[40];

static uint32_t volatile l_tickCtr;
uint32_t start;
uint32_t ticks=1;

uint32_t BSP_tickCtr(void) {
    uint32_t tickCtr;

   __asm("CPSID  I");
    tickCtr = l_tickCtr;
   __asm("CPSIE  I");

    return tickCtr;
}

void main_blinky1() {
    while (1)  {
        GPIO_PORTF_DATA_R = LED_RED;
        __asm("CPSID  I");
        start = l_tickCtr;
        __asm("CPSIE  I");
        while ((l_tickCtr - start) < ticks) {
        }
        GPIO_PORTF_DATA_R &= ~LED_RED;
        __asm("CPSID  I");
        start = l_tickCtr;
        __asm("CPSIE  I");
        while ((l_tickCtr - start) < ticks) {
        }
    }
}

void main_blinky2() {
    while (1)  {
        GPIO_PORTF_DATA_R = LED_BLUE;
        __asm("CPSID  I");
        start = l_tickCtr;
        __asm("CPSIE  I");
        while ((l_tickCtr - start) < ticks) {
        }
        GPIO_PORTF_DATA_R &= ~LED_BLUE;
        __asm("CPSID  I");
        start = l_tickCtr;
        __asm("CPSIE  I");
        while ((l_tickCtr - start) < ticks) {
        }
    }
}

void SysTick_Handler(void) {
    ++l_tickCtr;
}

int main() {
    SYSCTL_RCGCGPIO_R = 0x20U;
    GPIO_PORTF_DIR_R = 0x0EU;
    GPIO_PORTF_DEN_R = 0x0EU;
    NVIC_ST_RELOAD_R = 0xFFFFFF; 
    NVIC_ST_CTRL_R = 7; 
    __asm("CPSIE  I");	
    *(--sp_blinky1) = (1U << 24);  /* xPSR, bit 24 of thumb 2*/
    *(--sp_blinky1) = (uint32_t)&main_blinky1; /* PC don't forget to type cast*/
    *(--sp_blinky1) = 0x00000111U; /* LR  */
    *(--sp_blinky1) = 0x0000000CU; /* R12 */
    *(--sp_blinky1) = 0x00000003U; /* R3  */
    *(--sp_blinky1) = 0x00000002U; /* R2  */
    *(--sp_blinky1) = 0x00000001U; /* R1  */
    *(--sp_blinky1) = 0x00000000U; /* R0  */
	
    *(--sp_blinky2) = (1U << 24);
    *(--sp_blinky2) = (uint32_t)&main_blinky2;
    *(--sp_blinky2) = 0x00000111U; 
    *(--sp_blinky2) = 0x000000CCU;
    *(--sp_blinky2) = 0x00000333U;
    *(--sp_blinky2) = 0x00000222U;
    *(--sp_blinky2) = 0x00000111U; 
    *(--sp_blinky2) = 0x00000000U;

        while (1) {
    }
}    
