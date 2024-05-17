#include <stdint.h>
#include "os.h"
#include "diot.h"

uint32_t stackA[40];
OSThread taskA;
void TaskA() {
    while (1) {
        BSP_ledGreenOn();
        BSP_delay(BSP_TICKS_PER_SEC / 2U);
        BSP_ledGreenOff();
        BSP_delay(BSP_TICKS_PER_SEC / 2U);
    }
}

uint32_t stackB[40];
OSThread taskB;
void TaskB() {
    while (1) {
        BSP_ledBlueOn();
        BSP_delay(BSP_TICKS_PER_SEC );
        BSP_ledBlueOff();
        BSP_delay(BSP_TICKS_PER_SEC );
    }
}

int main(void) {
    BSP_init();
    OS_init();

    /* fabricate Cortex-M ISR stack frame for blinky1 */
    OSThread_start(&taskA,
                   &TaskA,
                   stackA, sizeof(stackA));

    /* fabricate Cortex-M ISR stack frame for blinky2 */
    OSThread_start(&taskB,
                   &TaskB,
                   stackB, sizeof(stackB));

    //BSP_ledRedOn();
    while (1) {
    }

    //return 0;
}
